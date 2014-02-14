#!/usr/bin/env python
'''
@file modulator_envelopgenerator.py
@brief Simple envelop generator - ADSD model
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

from modulators_common import EnvelopGeneratorInterface

class ADSD(EnvelopGeneratorInterface):
    '''
    Implements a attack-decay-sustain-decay envelop generator, mimicking
    the one of the Minimoog
    '''
    kAmplitudes = [1.0, 0.5, 0.5, 0.0]
    def __init__(self, sampling_rate):
        super(ADSD, self).__init__(sampling_rate)
        self._cursor = 0
        self._current_increment = 0.0
        self._current_value = 0.0
        self._time_settings = [0, 0, 0, 0]
        self._section = 0

    def SetParameters(self, attack, decay, sustain, release):
        self._time_settings[0] = attack
        self._time_settings[1] = decay + self._time_settings[0]
        self._time_settings[2] = sustain + self._time_settings[1]
        self._time_settings[3] = decay + self._time_settings[2]
        rise = self.kAmplitudes[0] - self._current_value
        self._current_increment = self._ComputeIncrement(rise, attack)
        self._section = 4

    def TriggerOn(self):
        self._cursor = 0
        self._section = 0

    def TriggerOff(self):
        self._section = 3

    def ProcessSample(self):
        if self._section == 0:
            self._current_value += self._current_increment
        elif self._section == 1:
            self._current_value += self._current_increment
        elif self._section == 2:
            # Sustain case
            pass
        elif self._section == 3:
            self._current_value += self._current_increment
        elif self._section == 4:
            # Out of the envelop case
            return 0.0
        else:
            raise Exception("This should not happen!")

        self._cursor += 1
        if (self._cursor >= self._time_settings[self._section]):
            self._section += 1
            if self._section == 4:
                return 0.0
            if self._section == 2:
                rise = 0.0
            else:
                rise = self.kAmplitudes[self._section] - self._current_value
            run = self._time_settings[self._section] - self._time_settings[self._section - 1]
            self._current_increment = self._ComputeIncrement(rise, run)

        return self._current_value

    def _ComputeIncrement(self, rise, run):
        '''
        Helper function for computing the increment at each increment,
        given the rise (vertical change) and run (horizontal change)
        '''
        return rise / run

if __name__ == "__main__":
    '''
    Various tests/sandbox
    '''
    import numpy
    import pylab

    sampling_freq = 48000

    attack = 100
    decay = 50
    sustain = 30
    release = 25

    TriggerOccurence = 384
    length = 512

    generator = ADSD(sampling_freq)
    generator.SetParameters(attack, decay, sustain, release)
    generated_data = numpy.zeros(length)
    for idx, _ in enumerate(generated_data):
        if (idx % TriggerOccurence) == 0:
            generator.TriggerOn()
        generated_data[idx] = generator.ProcessSample()

    pylab.plot(generated_data)
    pylab.show()
