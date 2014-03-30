#!/usr/bin/env python
'''
@file filter_contour.py
@brief Sandbox for "filter contour" feature
@author gm
@copyright gm 2014

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

class VCF(object):
    '''
    Implements a "VCF", e.g. a wrapper around a filter giving it the
    "filter contour" feature - ADSD-controlled cutoff frequency
    '''
    def __init__(self, wrapped_filter, modulator, sampling_rate):
        self._dry_filter = wrapped_filter()
        self._wet_filter = wrapped_filter()
        self._modulator = modulator(sampling_rate)
        self._amount = 0.0
        self._resonance = 0.0
        self._frequency = 0.0

    def SetParameters(self, frequency, resonance,
                      attack, decay, sustain_level, release,
                      amount):
        '''
        Sets all parameters:
        - filter parameters (frequency, resonance)
        - modulator parameters (attack, decay, sustain_level, release)
        - VCF parameter (amount)
        '''
        # These stays unchanged until this function next call
        self._resonance = resonance
        self._frequency = frequency
        self._dry_filter.SetParameters(frequency, resonance)
        self._wet_filter.SetParameters(frequency, resonance)
        self._modulator.SetParameters(attack, decay, sustain_level, release)
        self._amount = amount

    def TriggerOn(self):
        self._modulator.TriggerOn()

    def TriggerOff(self):
        self._modulator.TriggerOff()

    def ProcessSample(self, sample):
        '''
        Actual process function
        '''
        kMAXFREQ = 20000 / self._modulator._sampling_rate
        # First, process the input through the dry filter
        self._dry = self._dry_filter.ProcessSample(sample)
        # Then through the wet (contour'd) filter
        modulation = self._modulator.ProcessSample()
        self._contour = modulation * (kMAXFREQ - self._frequency) + self._frequency
        self._wet_filter.SetParameters(self._contour, self._resonance)
#         if self._modulator._section == 2:
#             print "toto"
        self._wet = self._wet_filter.ProcessSample(sample)
        # And mix
        out = (1.0 - self._amount) * self._dry + self._amount * self._wet 

        return out

    def Process4Samples(self, vector):
        '''
        Actual process function - vectorized version
        The frequency change is actually decimated by a factor 4 here
        '''
        kMAXFREQ = 20000 / self._modulator._sampling_rate
        dry = [0.0, 0.0, 0.0, 0.0]
        wet = [0.0, 0.0, 0.0, 0.0]
        out = [0.0, 0.0, 0.0, 0.0]
        self._modulator.ProcessSample()
        self._modulator.ProcessSample()
        self._modulator.ProcessSample()
        modulation = self._modulator.ProcessSample()
        contour = modulation * (kMAXFREQ - self._frequency) + self._frequency
        self._wet_filter.SetParameters(contour, self._resonance)
        for idx, sample in enumerate(vector):
            # First, process the input through the dry filter
            dry[idx] = self._dry_filter.ProcessSample(sample)
            # Then through the wet (contour'd) filter
    #         if self._modulator._section == 2:
    #             print "toto"
            wet[idx] = self._wet_filter.ProcessSample(sample)
            # And mix
            out[idx] = (1.0 - self._amount) * dry[idx] + self._amount * wet[idx]

        return out

if __name__ == "__main__":
    '''
    Various tests/sandbox
    '''
    import numpy
    import pylab

    # SoundTailor imports!
    import generator_sawtoothdpw
    import filter_secondorderraw
    import modulator_adsd

    import utilities

    freq = 100.0

    sampling_freq = 48000.0
    length = sampling_freq

    filter_freq = 100.0 / sampling_freq
    resonance = 1.0
    attack = 5000
    decay = 1000
    sustain_level = 0.1
    amount = 0.5

    view_beginning = 0
    view_length = 4096

    in_data = numpy.zeros(length)
    contour_data = numpy.zeros(length)
    dry_data = numpy.zeros(length)
    wet_data = numpy.zeros(length)
    generator = generator_sawtoothdpw.SawtoothDPW(sampling_freq)
    generator.SetFrequency(freq)
    for idx, _ in enumerate(in_data):
        in_data[idx] = generator.ProcessSample()
    # Printing output data in C format
#     out_string = ""
#     for sample in in_data[:len(in_data) - 1]:
#         out_string += str(sample) + "f"
#         out_string += ","
#         out_string += "\n"
#     out_string += str(in_data[len(in_data) - 1]) + "f"
#     print(out_string)

    out_data = numpy.zeros(length)
    out_vec_data = numpy.zeros(length)

    vcf = VCF(filter_secondorderraw.SecondOrderRaw,
              modulator_adsd.ADSD,
              sampling_freq)
    vcf.SetParameters(filter_freq, resonance,
                      attack, decay, sustain_level, decay,
                      amount)

    vcf.TriggerOn()
    for idx, _ in enumerate(in_data):
        out_data[idx] = vcf.ProcessSample(in_data[idx])
#         contour_data[idx] = vcf._contour
#         dry_data[idx] = vcf._dry
#         wet_data[idx] = vcf._wet

    vcf_vec = VCF(filter_secondorderraw.SecondOrderRaw,
                  modulator_adsd.ADSD,
                  sampling_freq)
    vcf_vec.SetParameters(filter_freq, resonance,
                          attack, decay, sustain_level, decay,
                          amount)

    vcf_vec.TriggerOn()
    idx = 0
    while idx < len(in_data) - 1:
        in_vec = [in_data[idx],
                  in_data[idx + 1],
                  in_data[idx + 2],
                  in_data[idx + 3]]
        (out_vec_data[idx],
        out_vec_data[idx + 1],
        out_vec_data[idx + 2],
        out_vec_data[idx + 3]) = vcf_vec.Process4Samples(in_vec)
        idx += 4
#         contour_data[idx] = vcf._co ntour
#         dry_data[idx] = vcf._dry
#         wet_data[idx] = vcf._wet

    utilities.WriteWav(out_data, "contour_filter", sampling_freq)
    utilities.WriteWav(out_vec_data, "contour_filter_vec", sampling_freq)

    print(utilities.PrintMetadata(utilities.GetMetadata(out_vec_data - out_data)))

#     pylab.plot(in_data[view_beginning:view_beginning + view_length], label="in")
#     pylab.plot(out_data[view_beginning:view_beginning + view_length], label="out")
#     pylab.plot(out_vec_data[view_beginning:view_beginning + view_length], label="out_vec")
#     pylab.plot(out_data[view_beginning:view_beginning + view_length], label="out")
    pylab.plot((out_vec_data - out_data)[view_beginning:view_beginning + view_length], label="out_vec")
#     pylab.plot(contour_data[view_beginning:view_beginning + view_length], label="contour")
#     pylab.plot(dry_data[view_beginning:view_beginning + view_length], label="dry")
#     pylab.plot(wet_data[view_beginning:view_beginning + view_length], label="wet")
    pylab.legend()
    pylab.show()
