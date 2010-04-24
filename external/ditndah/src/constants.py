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
## File:	constants.py
## Author:	Isaac E. Wagner <isaac@wagnerfam.com>
##
##
## TODO:
##
## BUGS:
##
## UPDATE INFO:
## Updated on:	06/10/08 12:49:00
## Updated by:	Isaac E. Wagner <isaac@wagnerfam.com>
##
##------==//

## 24/04/10 Dominik Auras: changed _MAX_PITCH

# Constants
_MIN_PITCH = 500
_MAX_PITCH = 3000
_MIN_WPM = 5
_MAX_WPM = 60
_MIN_SRATE = 8000
_MAX_SRATE = 48000
_DEFAULT_WPM = 5
_DEFAULT_FWPM = 13
_DEFAULT_PITCH = 700
_DEFAULT_SRATE = 8000

_PARIS_LENGTH = 50
_SECS_PER_MIN = 60

_VOLUME_ENVELOPE = 0.7
_RAMP_SECS = 0.005 # seconds to reach full volume

MORSE_TABLE = {'a': ".-",
               'b': "-...",
               'c': "-.-.",
               'd': "-..",
               'e': ".",
               'f': "..-.",
               'g': "--.",
               'h': "....",
               'i': "..",
               'j': ".---",
               'k': "-.-",
               'l': ".-..",
               'm': "--",
               'n': "-.",
               'o': "---",
               'p': ".--.",
               'q': "--.-",
               'r': ".-.",
               's': "...",
               't': "-",
               'u': "..-",
               'v': "...-",
               'w': ".--",
               'x': "-..-",
               'y': "-.--",
               'z': "--..",
               '0': "-----",
               '1': ".----",
               '2': "..---",
               '3': "...--",
               '4': "....-",
               '5': ".....",
               '6': "-....",
               '7': "--...",
               '8': "---..",
               '9': "----.",
               '.': ".-.-.-",
               ',': "--..--",
               '?': "..--..",
               ':': "---...",
               ';': "-.-.-.",
               '-': "-....-",
               '/': "-..-.",
               '"': ".-..-.",
               '+': ".-.-.",
               '|': ".-...",
               '>': "-.--.",
               '~': "...-.-",
               '=': "-...-",
               '@': ".--.-."
               }

FORMAT_S8    = 0x01
FORMAT_U8    = 0x02
FORMAT_S16BE = 0x04
FORMAT_S16LE = 0x08
FORMAT_U16BE = 0x10
FORMAT_U16LE = 0x20
