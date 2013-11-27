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

def GetMetadata(signal):
    '''
    Get signal basic metadata such as sum, mean etc.

    Return a tuple organized as follows:
    ((text, value)
     ...
     (text, value))
    '''
    return (("Length: ", len(signal)),
            ("Sum: ", numpy.sum(signal)),
            ("Max: ", numpy.max(signal)),
            ("Mean: ", numpy.mean(signal)),
            ("Var: ", numpy.var(signal)))

def PrintMetadata(metadatas):
    '''
    Print signal metadata extracted before

    Return a string organized as follows:
    text value ... text value
    '''
    output_str = ""
    for metadata in metadatas:
        output_str += metadata[0] + str(metadata[1])
        output_str += " "
    return output_str

if __name__ == "__main__":
    import pylab

    files = ('../msvc/openmini/tests/out.dat',
#              '../msvc/openmini/tests/in.dat'
             )

    view_length = 32768

    for filename in files:
        print ("File: " + str(filename))
        signal = ExtractSignal(filename)
        metas = GetMetadata(signal)
        print(PrintMetadata(metas))
        pylab.plot(signal[0:view_length:1])

    pylab.show()
