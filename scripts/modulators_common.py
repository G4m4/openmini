#!/usr/bin/env python
'''
@file modulators_common.py
@brief Common stuff for all envelop generators
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

class EnvelopGeneratorInterface(object):
    '''
    Common interface for all envelop generators - defines functions to be
    implemented by all subclasses
    (calling one of this "pure virtual" function will throw)
    '''
    def __init__(self, sampling_rate):
        '''
        The only difference with actual C++ code:
        here in Python we are initializing generators with a sampling rate,
        instead of using a static constant
        '''
        self._sampling_rate = sampling_rate

    def SetParameters(self, attack, decay, sustain, release):
        raise Exception("'Pure virtual' function called!")

    def TriggerOn(self):
        raise Exception("'Pure virtual' function called!")

    def TriggerOff(self):
        raise Exception("'Pure virtual' function called!")

    def ProcessSample(self):
        raise Exception("'Pure virtual' function called!")
