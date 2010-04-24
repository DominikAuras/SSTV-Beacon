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
## File:	__init__.py
## Author:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
## 
## TODO:
## 
## BUGS:
## 
## UPDATE INFO:
## Updated on:	06/11/08 15:22:00
## Updated by:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
##------==//

"""
This is the Dit 'n Dah Python Morse code library.  The name
comes from the sound of Morse code characters.

Simple library usage::

  import ditndah
  m = ditndah.MorseGen()
  data = m.textToAudioData("Hello World")
  <send data to sound card>

Morse complex example::
  import ditndah
  m = ditndah.MorseGen(pitch=700,
                       sampleRate=8000,
                       wpm=5,
                       fwpm=13,
                       format=ditndah.FORMAT_S16LE,
                       numChannels=2)
  m.textToWavFile("Four score and seven years ago...",
                  "/some/directory/output.wav")

Note that all the important classes, methods, and constants are imported
into the base package for you.  So, you don't need to explicitly import
ditndah.constants for example.

@author: Isaac E. Wagner
@copyright: This library is released under the GPL version 3. A copy of the
            license can be found in the source distribution.
"""

from exceptions import *
from constants import MORSE_TABLE, FORMAT_S8, FORMAT_U8, FORMAT_S16BE, FORMAT_S16LE, FORMAT_U16BE, FORMAT_U16LE
from morselib import MorseGen
from morsestream import MorseStream
