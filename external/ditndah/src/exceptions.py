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
## File:	exceptions.py
## Author:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
## 
## TODO:
## 
## BUGS:
## 
## UPDATE INFO:
## Updated on:	06/10/08 12:48:31
## Updated by:	Isaac E. Wagner <isaac@wagnerfam.com>
## 
##------==//

class MorseLibException(Exception):
    """
    This is the base exception for all exceptions in this library.
    """
    def __init__(self, value):
        self._value = value

    def __str__(self):
        return repr(self._value)

class OutOfRangeException(MorseLibException):
    """
    This exception is thrown when an input parameter is out of range.
    """
    pass

class InvalidValueException(MorseLibException):
    """
    This exception is thrown when an input parameter is invalid.
    """
    pass
