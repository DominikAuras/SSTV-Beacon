#!/usr/bin/env python

#
# Copyright 2009 Dominik Auras
#
# This file is part of sstv-beacon
#
# SSTV-Beacon is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# SSTV-Beacon is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest
from sstv_swig import *
from modulator import offline_sstv_modulator

from math import pi
from numpy import concatenate

from random import random

from gnuradio import audio

from beacon import *



class qa_sstv (gr_unittest.TestCase):

  def setUp (self):
    self.tb = gr.top_block ()

  def tearDown (self):
    self.tb = None

#  def test_001(self):
#    sample_rate = 48000
#    uut = offline_sstv_modulator( sample_rate )
#    try:
#      self.assertRaises( ValueError, uut.set_data, []  )
#    except:
#      pass
#
#  def test_002(self):
#    sample_rate = 48000
#    uut = offline_sstv_modulator( sample_rate )
#    try:
#      self.assertRaises( TypeError, uut.set_data, ['a']  )
#    except:
#      pass
#
#  def test_003(self):
#    sample_rate = 48000
#    uut = offline_sstv_modulator( sample_rate )
#    try:
#      self.assertRaises( TypeError, uut.set_data, [1.0]  )
#    except:
#      pass
#
#  def test_004(self):
#    sample_rate = 48000
#    uut = offline_sstv_modulator( sample_rate )
#    try:
#      self.assertRaises( ValueError, uut.start  )
#    except:
#      pass

  def test_005(self):

    uut = beacon()

#  def test_100_compute_sstv_signal (self):
#    sample_rate = 44100
#    uut = offline_sstv_modulator( sample_rate )
#
#    a = [int( i % 160 ) for i in range(0,256*320*3)]
#
#    try:
#      print "set data"
#      self.assert_( uut.set_data(a) )
#
#      print "start"
#      self.assert_( uut.start() )
#
#      print "join"
#      uut.wait()
#      print "return"
#    except Exception, ex:
#      print "ERROR caught exception"
#      print repr(ex)
#
#    self.assert_( uut.done() )
#    self.assert_( uut.successful() )
#
#    signal = uut.get_signal()
#
#    print "Samples: %d" % ( len(signal) )
#
#    signal = concatenate([[0]*10000,signal])
#
#    src = gr.vector_source_f( signal )
#    sink = audio.sink( sample_rate )
#
#    self.tb.connect( src, sink )
#
#    r = gr.enable_realtime_scheduling()
#    if r == gr.RT_OK:
#      print "Enabled realtime scheduling"
#    else:
#      print "Failed to enable realtime scheduling"
#
#    try:
#      self.tb.run()
#    except Exception, ex:
#      print "WOW gnuradio failed"
#      print repr(ex)
#



if __name__ == '__main__':
    gr_unittest.main ()
