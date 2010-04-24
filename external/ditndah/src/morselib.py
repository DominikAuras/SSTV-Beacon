##------==//
## 
## Copyright (c) 2008 Isaac E. Wagner
## 
## This file is part of Dit 'n Dah
## 
## Dit 'n Dah is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## Dit 'n Dah is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Dit 'n Dah.  If not, see <http://www.gnu.org/licenses/>.
## 
##------==//

##------==//
## 
## Project:	Dit 'n Dah
## 
## File:	morselib.py
## Author:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
## 
## TODO:
## 
## BUGS:
## 
## UPDATE INFO:
## Updated on:	06/14/08 11:26:47
## Updated by:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
##------==//

# Imports
from constants import _DEFAULT_PITCH, _DEFAULT_SRATE, _DEFAULT_WPM, _DEFAULT_FWPM, _RAMP_SECS, FORMAT_U8, FORMAT_S8, FORMAT_U16BE, FORMAT_U16LE, FORMAT_S16BE, FORMAT_S16LE, _MIN_PITCH, _MAX_PITCH, _MIN_WPM, _MAX_WPM, _MIN_SRATE, _MAX_SRATE, _VOLUME_ENVELOPE, MORSE_TABLE
from exceptions import OutOfRangeException, InvalidValueException

import math
import wave
import struct

class MorseGen:
    def __init__(self,
                 pitch=_DEFAULT_PITCH,
                 sampleRate=_DEFAULT_SRATE,
                 wpm=_DEFAULT_WPM,
                 fwpm=_DEFAULT_FWPM,
                 format=FORMAT_U8,
                 numChannels=1):
        """
        Morse Library Constructor.

        See L{setParams} for details about parameters.
        """
        self.setParams(pitch, sampleRate, wpm, fwpm, format, numChannels)

    def setParams(self,
                  pitch,
                  sampleRate,
                  wpm,
                  fwpm,
                  format,
                  numChannels):
        """
        Set all the Morse parameters at once.

        @param pitch: The frequency (in Hz) of the Morse output.
        @type  pitch: Integer
        @param sampleRate: The sample rate of the output waveform in Hz.
                           The allowable rates are 8000 - 48000Hz.
        @type  sampleRate: Integer
        @param wpm: Morse code rate in words per minute. Valid range
                    is 5-60wpm.
        @type  wpm: Integer
        @param fwpm: Farnsworth rate in words per minute. Valid range
                     is 5-60wpm.
        @type  fwpm: Integer
        @param format: The audio format of the output waveform. Valid
                       formats are: signed 8-bit (FORMAT_S8),
                       unsigned 8-bit (FORMAT_U8), signed 16-bit little
                       endian (FORMAT_S16LE), unsigned 16-bit little
                       endian (FORMAT_U16LE), signed 16-bit big endian
                       (FORMAT_S16BE), unsigned 16-bit big endian
                       (FORMAT_U16BE)
        @type  format: Format Type
        @param numChannels: The number of audio channels in the output
                            data. This can be 1 or 2 channels.
        @type  numChannels: Integer
        """
        self._pitch = pitch
        self._sampleRate = sampleRate
        self._wpm = wpm
        self._fwpm = fwpm
        self._format = format
        self._numChannels = numChannels
        self._validateInputs()
        self._generateData()

    def setPitch(self, pitch):
        """
        Set the pitch.

        @param pitch: The pitch in Hertz
        @type pitch: Integer
        """
        self._pitch = pitch
        self._validateInputs()
        self._generateData()

    def setSampleRate(self, sampleRate):
        """
        Set the sample rate.

        @param sampleRate: The sample rate of the output waveform in Hz.
                           The allowable rates are 8000 - 48000Hz.
        @type  sampleRate: Integer
        """
        self._sampleRate = sampleRate
        self._validateInputs()
        self._generateData()

    def setWPM(self, wpm):
        """
        Set the words per minute.

        @param wpm: Morse code rate in words per minute. Valid range
                    is 5-60wpm.
        @type  wpm: Integer
        """
        self._wpm = wpm
        self._validateInputs()
        self._generateData()

    def setFWPM(self, fwpm):
        """
        Set the Farnsworth words per minute.

        @param fwpm: Morse code rate in words per minute. Valid range
                     is 5-60wpm.
        @type  fwpm: Integer
        """
        self._fwpm = fwpm
        self._validateInputs()
        self._generateData()

    def _validateInputs(self):
        """
        Validate the constructor inputs to the class.
        """
        if self._pitch < _MIN_PITCH or self._pitch > _MAX_PITCH:
            raise OutOfRangeException("Pitch must be between %d-%dHz" %
                                      (_MIN_PITCH, _MAX_PITCH))
        if self._wpm < _MIN_WPM or self._wpm > _MAX_WPM:
            raise OutOfRangeException("WPM must be between %d-%dHz" %
                                      (_MIN_WPM, _MAX_WPM))
        if self._fwpm < _MIN_WPM or self._fwpm > _MAX_WPM:
            raise OutOfRangeException("FWPM must be between %d-%dHz" %
                                      (_MIN_WPM, _MAX_WPM))
        if self._fwpm < self._wpm:
            raise OutOfRangeException("Farnsworth speed cannot be lower than word speed")
        if self._sampleRate < _MIN_SRATE or self._sampleRate > _MAX_SRATE:
            raise OutOfRangeException("Valid sample rate is %d-%dHz" %
                                      (_MIN_SRATE, _MAX_SRATE))
        if self._numChannels < 1 or self._numChannels > 2:
            raise OutOfRangeException("Valid number of channels is 1-2")

        if self._format == FORMAT_S8:
            endianStr = ''
            formatCode = 'b'
        elif self._format == FORMAT_U8:
            endianStr = ''
            formatCode = 'B'
        elif self._format == FORMAT_S16LE:
            endianStr = '<'
            formatCode = 'h'
        elif self._format == FORMAT_S16BE:
            endianStr = '>'
            formatCode = 'h'
        elif self._format == FORMAT_U16LE:
            endianStr = '<'
            formatCode = 'H'
        elif self._format == FORMAT_U16BE:
            endianStr = '>'
            formatCode = 'H'
        else:
            raise InvalidValueException("Invalid format specified")
        self._endianStr = endianStr
        self._formatCode = formatCode

    def _generateData(self):
        # To avoid clicks and pops we need to ramp up the waveform.
        # This is done over a predefined time period.
        numRampSamples = self._sampleRate * _RAMP_SECS
        
        # how long are the dit's and dah's
        # To calculate the lengths we rely on the "Paris" length
        # which is 50 "dit" lengths.  Based on this a dit length, in
        # seconds is 1.2/wpm.  We need this length in samples,
        # so we'll convert it.
        len_dit = int((1.2 / self._fwpm) * self._sampleRate)
        len_dah = len_dit * 3

        # how long are the spaces between elements, characters, and words?
        farndit = len_dit
        if self._fwpm > self._wpm:
            ta = 60.0 / float(self._wpm) - 37.2 / float(self._fwpm)
            farndit = int((float(self._sampleRate) * ta) / 19.0)
        len_ies = len_dit        # inter-element spacing
        len_ics = farndit * 3   # inter-character spacing
        len_iws = farndit * 7   # inter-word spacing

        # construct arrays 
        dit = []
        for index in range(0,len_dit):
            for c in range(0,self._numChannels):
                val = _VOLUME_ENVELOPE * math.sin(float(self._pitch) * 
                      math.pi * 2.0 * float(index) / float(self._sampleRate))
                if index < numRampSamples:
                    val *= float(index) / float(numRampSamples)
                if index > (len_dit - numRampSamples):
                    val *= (float(len_dit) - float(index)) / float(numRampSamples)
                dit.append(val)
        dah = []
        for index in range(0,len_dah):
            for c in range(0,self._numChannels):
                val = _VOLUME_ENVELOPE * math.sin(float(self._pitch) * 
                      math.pi * 2.0 * float(index) / float(self._sampleRate))
                if index < numRampSamples:
                    val *= float(index) / float(numRampSamples)
                if index > (len_dah - numRampSamples):
                    val *= (float(len_dah) - float(index)) / float(numRampSamples)
                dah.append(val)
        # ies is same length as a dit 
        ies = [0] * self._numChannels * len_ies
   
        # ics is three "farndits" in length 
        ics = [0] * self._numChannels * len_ics

        # iws is seven "farndits" in length 
        iws = [0] * self._numChannels * len_iws

        # Save these arrays into the current object
        self._dit = dit
        self._dah = dah
        self._ies = ies
        self._ics = ics
        self._iws = iws

    def _applyFormat(self, val):
        """
        This method scales/shifts the input value to the appropriate
        range for the selected format.

        @param val: The individual value to shift and scale. This should
                    be in the range of [-1.0, 1.0]
        @type val: float
        @return: The scaled/shifted value
        @rtype: int
        """
        if self._format == FORMAT_S8:
            val = val * 127
        if self._format == FORMAT_U8:
            val = (val + 1.0) / 2.0 * 255
        if self._format == FORMAT_S16LE or self._format == FORMAT_S16BE:
            val = val * 32767
        if self._format == FORMAT_U16LE or self._format == FORMAT_U16BE:
            val = (val + 1.0) / 2.0 * 65535
        return int(round(val))

    def textToAudioData(self, text):
        """
        This method takes an input string and converts it into raw audio
        data.  This audio data can be written into a wav file, after adding
        the correct header information.  Or, it be sent directly to an audio
        device, such as a sound card.

        @note: This routine stores all the audio data in memory, so
               long input strings will most likely result in an out of
               memory error.  In cases where you need a long message converted
               to Morse audio it is best to use the method taking a callback
               and using the audio data as it is streamed to your callback.

        @param text: The input text to convert to audio
        @type text: String
        @return: A compacted string representing the audio data
        @rtype: String
        """
        text = text.lower()
        data = []

        spaceflag = True
        for c in text:
            # only write iws if whitespace follows non-whitespace 
            if c.isspace() and spaceflag is False:
                data += self._iws
                spaceflag = True
            else:
                # check if the character is in the list of Morse chars
                if c in MORSE_TABLE:
                    if spaceflag is False:
                        data += self._ics
                    else:
                        spaceflag = False
                    firstPass = True
                    for mc in MORSE_TABLE[c]:
                        if not firstPass:
                            data += self._ies
                        if mc == '.':
                            data += self._dit
                        else:
                            data += self._dah
                        firstPass = False
        # At this point the numerical values in the array are in the
        # range [-1.0, 1.0] and need to be converted to the appropriate
        # values.
        data = map(lambda x: self._applyFormat(x), data)

        # Finally, pack the data into a string representation
        structFmt = "%s%d%s" % (self._endianStr, len(data), self._formatCode)
        return struct.pack(structFmt, *data)

    def textToAudioStream(self, text, callback):
        """
        This method accepts a callback parameter and passes chunks of audio
        data to the callback.  This effectively forms an audio stream that
        can be passed to an audio device or written to a wav file. Using this
        "streaming" method is preferred because it is much more memory
        efficient; though it does take more effort to use.

        @param text: The input text to convert to Morse audio
        @type text: String
        @param callback: Callback method to received audio data. More
                         accurately, this parameter is actually a class instance
                         of the MorseStream class.
        @type callback: MorseStream
        """
        pass

    def textToWaveFile(self, text, fname):
        """
        Convert text to a Morse wave file.  For example::
          import ditndah
          m = ditndah.MorseGen(format=ditndah.FORMAT_U16)
          m.textToWaveFile("hello world", "/path/to/output.wav")

        @param text: The text to convert to Morse audio.
        @type text: String
        @param fname: The output file name.
        @type fname: String
        """
        data = self.textToRawData(text)
        fp = wave.open(fname, 'wb')
        fp.setnchannels(self._numChannels)
        fp.setsampwidth(1)
        fp.setframerate(self._sampleRate)
        fp.setnframes(len(data))
        fp.writeframes(data)
        fp.close()
