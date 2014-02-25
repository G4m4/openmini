#!/usr/bin/env python
# coding=utf-8
'''
@file interpolation.py
@brief Interpolation method sandbox
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

def LinearInterpolation(context, ratio):
    '''
    Linear interpolation between two points
    '''
    left = context[0]
    right = context[1]
    return (right - left) * ratio + left;

def LinearInterpolation4(context, ratio):
    '''
    Linear interpolation between two points
    '''
    previous = context[0]
    left = context[1]
    right = context[2]
    next = context[3]
    coeff0 = (1.0 - ratio) / 4.0
    coeff1 = (1.0 - ratio) / 1.3333
    coeff2 = ratio / 1.3333
    coeff3 = ratio / 4.0
    return (previous * coeff0
            + left * coeff1
            + right * coeff2
            + next * coeff3)

class Interpolate():
    '''
    Implements interpolation of (possibly non-contiguous) blocks based on the given
    interpolation function
    '''
    def __init__(self, interpolation_func):
        self._func = interpolation_func
        self._history = 0.0
        self._cursor_pos = 0.0
        self._ratio = 1.0

    def SetRatio(self, ratio):
        '''
        Ratio setter
        @param    ratio        Actual ratio to set the interpolation to
                               > 1.0 -> faster, etc.
        '''
        self._ratio = ratio

    def Reset(self):
        '''
        Reset internal counter to zero
        '''
        self._cursor_pos = 0

    def Process(self, in_signal, out_length):
        '''
        Actual process function
        @return the interpolated signal according interpolation ratio and input
        '''
        in_length = len(in_signal)
        current_out_idx = 0
        out_signal = numpy.zeros(out_length)
        temp_cursor = self._cursor_pos
        while current_out_idx < out_length:
            left_idx = int(math.floor(temp_cursor))
            cursor = temp_cursor - left_idx
            context = in_signal[left_idx:left_idx + 2]
            if temp_cursor < 0.0:
                left_idx = 0
                cursor = 1.0 + temp_cursor
                context = [self._history, in_signal[left_idx]]
                if temp_cursor < -1.0:
                    raise Exception("WTF?")
            if left_idx == in_length - 1.0:
                context = [in_signal[left_idx], in_signal[left_idx]]
            if left_idx >= in_length:
                raise Exception("WTF?")

            out_signal[current_out_idx] = self._ProcessSample(context,
                                                              cursor)

            current_out_idx += 1
            temp_cursor += self._ratio
        self._history = context[-1]
        self._cursor_pos += current_out_idx * self._ratio
        self._cursor_pos -= in_length

        return out_signal

    def _ProcessSample(self, context, cursor):
        '''
        Process function for one sample
        @return the next sample according interpolation ratio and input data
        '''
        return self._func(context, cursor)

def ExpectedOutLength(in_length, ratio):
    '''
    Compute the expected output signal length for a given input length and ratio
    '''
    return int(math.ceil(in_length / ratio))

def RequiredInLength(out_length, ratio, cursor):
    '''
    Compute the required input data length in order to retrieve out_length elements,
    given the ratio
    '''
    # What we actually want is at least 1 sample of advance, except of course
    # if out_length * ratio is an integer
    return int(math.ceil(out_length * ratio + cursor))

if __name__ == "__main__":
    '''
    Various tests/sandbox
    '''
    import numpy
    import pylab
    import utilities

    sampling_freq = 96000.0
    length = 2048
    # Normalized
    artifacts = 0.05
    ratio = 96000.0 / 44100.0
    out_length = ExpectedOutLength(length, ratio)
    base_freq = 2000

    # This method generates a zero at the beginning, that's why it is sliced
    base_data = utilities.GenerateData(base_freq,
                                       base_freq * 2,
                                       length + 1,
                                       sampling_freq)[0:length]
#     base_data = numpy.random.rand(length)

    in_data = numpy.copy(base_data)
    # Random artifacts are now made
#     for artifact in range(int(math.floor(artifacts * length))):
#         in_data[numpy.random.randint(0, length)] = 0.0

    print(utilities.PrintMetadata(utilities.GetMetadata(base_data - in_data)))

    resampler = Interpolate(LinearInterpolation)
    resampler.SetRatio(ratio)

    # Input data is split in blocks of random size each
    input_idx = 0
    output_idx = 0
    min_block_size = 2
    out_data = numpy.zeros(out_length)
    block_size = 64
    while input_idx < length - math.ceil(ratio * block_size):
#         block_size = numpy.random.randint(min_block_size, out_length - output_idx)
        input_length = RequiredInLength(block_size, ratio, resampler._cursor_pos)
        out_data[output_idx:output_idx + block_size] = resampler.Process(in_data[input_idx:input_idx + input_length],
                                                                         block_size)
        output_idx += block_size
        input_idx += input_length

    # Process in the other way
    resampler.Reset()
    resampler.SetRatio(1.0 / ratio)
    out_data_resampled = resampler.Process(out_data, ExpectedOutLength(out_length,
                                                                       1.0 / ratio))
    pylab.plot(out_data_resampled, label='resampled')

    pylab.plot(in_data, label='in')
    pylab.plot(out_data, label='out')

    pylab.legend()
    pylab.show()
