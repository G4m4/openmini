#!/usr/bin/env python
'''
@file extract_test_data.py
@brief Utilities for retrieving generated test data 
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

import numpy
import os

def ExtractSignal(filename):
    '''
    Open a file and extract a signal from it

    the file is supposed to be organized as follows:
    sample,\n
    sample,\n
    ...
    sample,\n

    Return a numpy signal
    '''
    if not os.path.exists(filename):
        raise Exception("No such file: "  + str(filename))
    return numpy.fromfile(filename, sep='\n')

if __name__ == "__main__":
    import pylab
    import utilities
    import os.path
    import scipy.io.wavfile

    files = (#'../build/out.dat',
             '../build/in.dat',
             '../build/intermediate.dat'
             )

    view_beginning = 16000
    view_length = 384

    signals = []
    for filename in files:
        label = os.path.basename(filename)
        print ("File '" + label + "': " + str(filename))
        signal = ExtractSignal(filename)
        metas = utilities.GetMetadata(signal)
        print(utilities.PrintMetadata(metas))
        pylab.plot(signal[view_beginning:view_beginning + view_length:1], label=label)
        signals.append(signal)
        utilities.WriteWav(signal, filename, 96000.0)

    pylab.legend()
    pylab.show()
