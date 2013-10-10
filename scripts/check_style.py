#!/usr/bin/env python
'''
@file check_style.py
@brief Various utility functions/classes for checking C++ code style
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

import os

import cpplint

def CheckRecursivelyCppFiles(root_path, file_exts, arguments):
    '''
    Recursively browse through all subfiles in "root_path" and applies cpplint
    to each one of them suffixed with one of the "file_exts" extension
    cpplint gets called with "arguments"
    '''
    absolute_root = os.path.abspath(root_path)
    concatenated_filenames = ""
    for sub_root,filenames in [(sub_root,filenames) for sub_root,_,filenames in os.walk(absolute_root)]:
        for filename in filenames:
            if os.path.splitext(filename)[1] in file_exts:
                concatenated_filenames += os.path.join(sub_root, filename) + ' '
    arguments.append(concatenated_filenames)

    filenames = cpplint.ParseArguments(arguments)
    cpplint._cpplint_state.ResetErrorCounts()
    for filename in filenames[0].split(' '):
        cpplint.ProcessFile(filename, cpplint._cpplint_state.verbose_level)
    cpplint._cpplint_state.PrintErrorCounts()

if __name__ == "__main__":
    root_path = os.path.join(os.path.join(os.path.realpath(__file__),
                                          os.pardir,
                                          os.pardir),
                             "openmini")
    file_exts = (".cc", ".h")
    arguments = ["--filter=-whitespace/comments,-build/header_guard",
                 "--counting=detailed",
                 "--output=vs7"]
    CheckRecursivelyCppFiles(root_path, file_exts, arguments)
