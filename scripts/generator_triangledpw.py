#!/usr/bin/env python
'''
@file generator_triangledpw.py
@brief Triangle Signal generator based on DPW algorithm
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

from generators_common import GeneratorInterface, Differentiator, PhaseAccumulator

class TriangleDPW(GeneratorInterface):
    '''
    Implements a triangle signal generator based on DPW algorithm
    '''
    def __init__(self, sampling_rate):
        super(TriangleDPW, self).__init__(sampling_rate)
        self._sawtooth_gen = PhaseAccumulator(sampling_rate)
        self._differentiator = Differentiator()
        self._normalization_factor = 0.0
        self._frequency = 0.0
        self._update = False

    def SetPhase(self, phase):
        self._sawtooth_gen.SetPhase(phase);

    def SetFrequency(self, frequency):
        self._frequency = frequency
        self._sawtooth_gen.SetFrequency(frequency)
        self._update = True

    def ProcessSample(self):
        self._ProcessParameters()
        current = self._sawtooth_gen.ProcessSample()
        current -= current * abs(current)
        current = self._differentiator.ProcessSample(current)
        current *= self._normalization_factor

        return current

    def _ProcessParameters(self):
        if self._update:
            self._normalization_factor = self._sampling_rate \
                                            / (2.0 * self._frequency)
            self._update_ = False

if __name__ == "__main__":
    '''
    Various tests/sandbox
    '''
    import numpy
    import pylab

    freq = 352.0
    sampling_freq = 48000
    length = 512

    generator = TriangleDPW(sampling_freq)
    generator.SetFrequency(freq)
    generated_data = numpy.zeros(length)
    for idx, _ in enumerate(generated_data):
        generated_data[idx] = generator.ProcessSample()

    pylab.plot(generated_data)
    pylab.show()
    