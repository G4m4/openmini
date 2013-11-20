#!/usr/bin/env python
'''
@file filter_secondorderraw.py
@brief Common definitions for all filters
@author gm
@copyright gm 2013

This file is part of OpenMini

OpenMini is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenMini is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMini.  If not, see <http://www.gnu.org/licenses/>.
'''

'''
Note that all of this mimics C++ code for testing/prototyping purpose.
Hence it may not really seems "pythonic" and not intended to be in any way.
'''

import math

import filters_common

class SecondOrderRaw(filters_common.FilterInterface):
    '''
    Implements a simple Direct Form 1 biquad filter
    '''
    def __init__(self):
        self._gain = 0.0
        self._coeffs = (0.0, 0.0, 0.0, 0.0)
        self._history = (0.0, 0.0, 0.0, 0.0)

    def SetParameters(self, frequency, resonance):
        '''
        Sets both frequency (normalized, < 0.5) and "resonance" (Q factor)
        '''
        kOmega = 2.0 * math.pi * frequency
        kSinOmega = math.sin(kOmega)
        kCosOmega = math.cos(kOmega)
        kAlpha = kSinOmega / (2.0 * resonance)

        b0 = (1.0 - kCosOmega) / 2.0;
        b1 = (1.0 - kCosOmega);
        b2 = (1.0 - kCosOmega) / 2.0;
        a0 = 1.0 + kAlpha;
        a1 = -2.0 * kCosOmega;
        a2 = 1.0 - kAlpha;

        self._gain = b0 / a0
        self._coeffs = [b2 / a0,
                        b1 / a0,
                        -a2 / a0,
                        -a1 / a0]

    def ProcessSample(self, sample):
        '''
        Actual process function
        '''
        out = self._gain * sample
        out += filters_common.VectorDotProduct(self._coeffs, self._history)

        self._history = filters_common.VectorRotateEachHalf(self._history,
                                                            sample,
                                                            out)

        return out

    def Process4Samples(self, vector):
        '''
        Same as above but vectorized for a 4 samples tuple
        '''
        # Vector = (x_{n}, x_{n + 1}, x_{n + 2}, x_{n + 3})
        # previous = (x_{n - 1}, x_{n}, x_{n + 1}, x_{n + 2})
        # last = (x_{n - 2}, x_{n - 1}, x_{n}, x_{n + 1})
        previous = filters_common.RotateOnRight(vector, self._history[1])
        last = filters_common.RotateOnRight(previous, self._history[0])
        current = filters_common.MulConst(vector, self._gain)
        previous = filters_common.MulConst(previous, self._coeffs[1])
        last = filters_common.MulConst(last, self._coeffs[0])
        tmp_sum = numpy.add(current, previous)
        tmp_sum = numpy.add(tmp_sum, last)
        oldest_out = tmp_sum[0] \
                    + self._history[2] * self._coeffs[2] \
                    + self._history[3] * self._coeffs[3]
        old_out = tmp_sum[1] \
                    + self._history[3] * self._coeffs[2] \
                    + oldest_out * self._coeffs[3]
        new_out = tmp_sum[2] \
                    + oldest_out * self._coeffs[2] \
                    + old_out * self._coeffs[3]
        newest_out = tmp_sum[3] \
                    + old_out * self._coeffs[2] \
                    + new_out * self._coeffs[3]

        out = (oldest_out, old_out, new_out, newest_out)
        self._history = filters_common.TakeEachRightHalf(vector, out)

        return out

if __name__ == "__main__":
    '''
    Various tests/sandbox
    '''
    import numpy
    import pylab
    import utilities

    freq = 1000.0
    sampling_freq = 48000.0
    length = 512
    resonance = 0.7

    in_data = utilities.GenerateData(200, 2000, length, sampling_freq)

    # Printing output data in C format
#     out_string = ""
#     for sample in in_data[:len(in_data) - 1]:
#         out_string += str(sample) + "f"
#         out_string += ","
#         out_string += "\n"
#     out_string += str(in_data[len(in_data) - 1]) + "f"
#     print(out_string)

    out_data = numpy.zeros(length)
    out_data_vectorized = numpy.zeros(length)

    lowpass = SecondOrderRaw()
    lowpass.SetParameters(0.2, resonance)

    for idx, _ in enumerate(in_data):
        out_data[idx] = lowpass.ProcessSample(in_data[idx])

    # Vectorized processing
    lowpass_v = SecondOrderRaw()
    lowpass_v.SetParameters(0.2, resonance)
    idx = 0
    while idx < len(in_data):
        current_vector = (in_data[idx],
                          in_data[idx + 1],
                          in_data[idx + 2],
                          in_data[idx + 3])
        (out_data_vectorized[idx],
         out_data_vectorized[idx + 1],
         out_data_vectorized[idx + 2],
         out_data_vectorized[idx + 3]) = lowpass_v.Process4Samples(current_vector)
        idx += 4

    pylab.plot(in_data)
    pylab.plot(out_data)
    pylab.plot(out_data_vectorized)
    pylab.show()
