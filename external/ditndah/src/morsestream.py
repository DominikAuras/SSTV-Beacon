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
## File:	morsestream.py
## Author:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
## 
## TODO:
## 
## BUGS:
## 
## UPDATE INFO:
## Updated on:	06/22/08 23:26:47
## Updated by:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
##------==//

class MorseStream:
    """
    This is basically equivalent to a "template" in other languages. This class
    MUST be extended and the audioData method overridden to receive an audio
    stream.
    """
    def audioData(self, data, text):
        """
        Method used to receive audio data. This must be overridden.

        @param data: The raw audio data. The format will be as specified in
                     the constructor of the MorseGen object.
        @type data: Varies
        @param text: This is the text corresponding to the audio data. This
                     will in most cases be a single character.
        @type text: String
        """
        pass
