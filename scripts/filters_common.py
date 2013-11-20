#!/usr/bin/env python
'''
@file filters_common.py
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
import numpy

def VectorDotProduct(left, right):
    '''
    Explicit vectorized dot product between two vectors:
    given left = (x0, x1, x2, x3) and right = (y0, y1, y2, y3)
    it will return x0 * y0 + x1 * y1 + x2 * y2 + x3 * y3

    @param    left     4 samples tuple
    @param    right    4 samples tuple
    @return    One scalar value
    '''
    return left[0] * right[0] \
           + left[1] * right[1] \
           + left[2] * right[2] \
           + left[3] * right[3]

def VectorRotateEachHalf(vector, left, right):
    '''
    Rotate each half of the given vector, introducing given values.
    E.g. given vector = (x0, x1, x2, x3)
    it will return (x1, left, x3, right)

    @param    vector    4 samples tuple
    @param    left    value to shift in the left half
    @param    right    value to shift in the right half
    @return    4 samples tuple
    '''
    return (vector[1],
            left,
            vector[3],
            right)

def RotateOnLeft(vector, value):
    '''
    Rotate the given vector, introducing given value.
    E.g. given vector = (x0, x1, x2, x3)
    it will return (x1, x2, x3, value)

    @param    vector    4 samples tuple
    @param    value    value to shift in
    @return    4 samples tuple
    '''
    return (vector[1],
            vector[2],
            vector[3],
            value)

def RotateOnRight(vector, value):
    '''
    Rotate the given vector, introducing given value.
    E.g. given vector = (x0, x1, x2, x3)
    it will return (value, x0, x1, x2)

    @param    vector    4 samples tuple
    @param    value    value to shift in
    @return    4 samples tuple
    '''
    return (value,
            vector[0],
            vector[1],
            vector[2])

def TakeEachRightHalf(left, right):
    '''
    Get each right half of the two given vectors:
    given left = (x0, x1, x2, x3) and right = (y0, y1, y2, y3)
    it will return (x2, x3, y2, y3)

    @param    left     4 samples tuple
    @param    right    4 samples tuple
    @return    4 samples tuple
    '''
    return (left[2],
            left[3],
            right[2],
            right[3])

def Fill(value):
    '''
    Fill a 4 samples tuple with the given value

    @param    value    Value to fill with
    @return    4 samples tuple
    '''
    return (value,
            value,
            value,
            value)

def MulConst(vector, value):
    '''
    Multiplies each element of the given vector by "value"

    @param    vector    Vector to operate on
    @param    value    Value to fill with
    @return    4 samples tuple
    '''
    return numpy.multiply(value, vector)

class FilterInterface(object):
    '''
    Common interface for all filters - defines functions to be implemented
    by all subclasses (calling one of this "pure virtual" function will throw)
    '''
    def __init__(self):
        pass

    def SetParameters(self, frequency, resonance):
        raise Exception("'Pure virtual' function called!")

    def ProcessSample(self):
        raise Exception("'Pure virtual' function called!")
