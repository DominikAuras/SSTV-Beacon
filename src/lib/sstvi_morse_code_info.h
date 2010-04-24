//
// Copyright 2009 Dominik Auras
//
// This file is part of sstv-beacon
//
// SSTV-Beacon is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// SSTV-Beacon is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Radio; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street,
// Boston, MA 02110-1301, USA.
//

#ifndef SSTVI_MORSE_CODE_INFO_H_
#define SSTVI_MORSE_CODE_INFO_H_

#include "common_data_types.h"

typedef std::pair< bool, time_duration > m_symbol_t;

struct morse_code_info
{
  freq_t        base_freq;

  time_duration unit;

  time_duration t_dih;
  time_duration t_dah;
  time_duration t_intra_character_gap;    // between dih/dahs
  time_duration t_short_gap;              // between letters
  time_duration t_long_gap;               // between words

  m_symbol_t    dih;
  m_symbol_t    dah;
  m_symbol_t    intra_character_gap;
  m_symbol_t    short_gap;
  m_symbol_t    long_gap;
};

// Next table is taken from
/*
 *      cwbeacon.c
 *
 *      cwbeacon is part of the beaconProject, ea4rj@amsat.org
 *      and it is based on the original:
 *
 *      GW4PTS Morse tutor for Linux, by Alan Cox (morse.c 1.00, 21/09/93)
 *
 *      This software is placed under the GNU software license
 */


typedef struct
{
        char code;
        enum
        {
                NIL,
                DIH,
                DAH,
        } data[7];
} Morse;

static Morse MorseTable[]=
{
        ' ',NIL,NIL,NIL,NIL,NIL,NIL,NIL,
        'A',DIH,DAH,NIL,NIL,NIL,NIL,NIL,
        'B',DAH,DIH,DIH,DIH,NIL,NIL,NIL,
        'C',DAH,DIH,DAH,DIH,NIL,NIL,NIL,
        'D',DAH,DIH,DIH,NIL,NIL,NIL,NIL,
        'E',DIH,NIL,NIL,NIL,NIL,NIL,NIL,
        'F',DIH,DIH,DAH,DIH,NIL,NIL,NIL,
        'G',DAH,DAH,DIH,NIL,NIL,NIL,NIL,
        'H',DIH,DIH,DIH,DIH,NIL,NIL,NIL,
        'I',DIH,DIH,NIL,NIL,NIL,NIL,NIL,
        'J',DIH,DAH,DAH,DAH,NIL,NIL,NIL,
        'K',DAH,DIH,DAH,NIL,NIL,NIL,NIL,
        'L',DIH,DAH,DIH,DIH,NIL,NIL,NIL,
        'M',DAH,DAH,NIL,NIL,NIL,NIL,NIL,
        'N',DAH,DIH,NIL,NIL,NIL,NIL,NIL,
        'O',DAH,DAH,DAH,NIL,NIL,NIL,NIL,
        'P',DIH,DAH,DAH,DIH,NIL,NIL,NIL,
        'Q',DAH,DAH,DIH,DAH,NIL,NIL,NIL,
        'R',DIH,DAH,DIH,NIL,NIL,NIL,NIL,
        'S',DIH,DIH,DIH,NIL,NIL,NIL,NIL,
        'T',DAH,NIL,NIL,NIL,NIL,NIL,NIL,
        'U',DIH,DIH,DAH,NIL,NIL,NIL,NIL,
        'V',DIH,DIH,DIH,DAH,NIL,NIL,NIL,
        'W',DIH,DAH,DAH,NIL,NIL,NIL,NIL,
        'X',DAH,DIH,DIH,DAH,NIL,NIL,NIL,
        'Y',DAH,DIH,DAH,DAH,NIL,NIL,NIL,
        'Z',DAH,DAH,DIH,DIH,NIL,NIL,NIL,
        '1',DIH,DAH,DAH,DAH,DAH,NIL,NIL,
        '2',DIH,DIH,DAH,DAH,DAH,NIL,NIL,
        '3',DIH,DIH,DIH,DAH,DAH,NIL,NIL,
        '4',DIH,DIH,DIH,DIH,DAH,NIL,NIL,
        '5',DIH,DIH,DIH,DIH,DIH,NIL,NIL,
        '6',DAH,DIH,DIH,DIH,DIH,NIL,NIL,
        '7',DAH,DAH,DIH,DIH,DIH,NIL,NIL,
        '8',DAH,DAH,DAH,DIH,DIH,NIL,NIL,
        '9',DAH,DAH,DAH,DAH,DIH,NIL,NIL,
        '0',DAH,DAH,DAH,DAH,DAH,NIL,NIL,
        '/',DAH,DIH,DIH,DAH,DIH,NIL,NIL,
        '*',DIH,DAH,DIH,DAH,DIH,NIL,NIL,        /* 'AR', end of message */
        '.',DIH,DAH,DIH,DAH,DIH,DAH,NIL,
        '-',DAH,DIH,DIH,DIH,DIH,DAH,NIL,
        ',',DAH,DAH,DIH,DIH,DAH,DAH,NIL,
        '?',DIH,DIH,DAH,DAH,DIH,DIH,NIL,
        0,  NIL,NIL,NIL,NIL,NIL,NIL,NIL         /* END MARKER */
};


#endif /* SSTVI_MORSE_CODE_INFO_H_ */
