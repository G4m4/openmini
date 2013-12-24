#!/usr/bin/env python
# coding=utf-8
'''
@file Interpolation.py
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
    Implements interpolation based on the given interpolation function
    '''
    def __init__(self, interpolation_func):
        self._func = interpolation_func
        self._cursor_pos = 0.0
        self._ratio = 1.0

    def SetRatio(self, ratio):
        '''
        Ratio setter
        @param    ratio        Actual ratio to set the interpolation to
                               > 1.0 -> faster, etc.
        '''
        self._ratio = ratio

    def SetData(self, input_array):
        '''
        Input setter
        @param    input_array        Actual input array to interpolate from
        '''
        self._input = input_array

    def Reset(self):
        '''
        Reset internal counter to zero 
        '''
        self._cursor_pos = 0

    def IsInputFinished(self):
        '''
        @return True if internal input data cursor is outside the input buffer
        '''
        return self._cursor_pos >= (len(self._input) - 1)

    def ProcessSample(self):
        '''
        Actual process function: gets the interpolated sample
        @return the next sample according interpolation ratio and input data
        '''
        #if self._cursor_pos < 1.0:
        #    left_idx = 0
        #    context = (self._input[0], self._input[0], self._input[0], self._input[1])
        #elif self._cursor_pos < 2.0:
        #    left_idx = int(math.floor(self._cursor_pos))
        #    context = (self._input[0], self._input[0], self._input[1], self._input[2])
        #else:
        left_idx = int(math.floor(self._cursor_pos))
        context = self._input[left_idx:left_idx + 2]

        sample = self._func(context,
                            self._cursor_pos - float(left_idx))

        self._cursor_pos += self._ratio

        return sample

if __name__ == "__main__":
    '''
    Various tests/sandbox
    '''
    import numpy
    import pylab
    import utilities

    sampling_freq = 48000.0
    length = 64
    # Normalized
    artifacts = 0.05
    ratio = 1.0
    out_length = length / ratio
    base_freq = 500

    # This method generates a zero at the beginning, that's why it is sliced
#    base_data = utilities.GenerateData(base_freq,
#                                       base_freq * 2,
#                                       length + 1,
#                                       sampling_freq)[0:length]
    base_data = numpy.random.rand(length)

    in_data = numpy.copy(base_data)
    # Random artifacts are now made
    for artifact in range(int(math.floor(artifacts * length))):
        in_data[numpy.random.randint(0, length)] = 0.0

    print(utilities.PrintMetadata(utilities.GetMetadata(base_data - in_data)))

    resampler = Interpolate(LinearInterpolation)
    resampler.SetData(in_data)
    resampler.SetRatio(ratio)

    out_data = numpy.zeros(out_length)
    idx = 0
    while not resampler.IsInputFinished():
        out_data[idx] = resampler.ProcessSample()
        idx += 1

    pylab.plot(in_data)
    pylab.plot(out_data)
    print(utilities.PrintMetadata(utilities.GetMetadata(base_data[0:out_length]
                                                         - out_data)))

    pylab.show()
