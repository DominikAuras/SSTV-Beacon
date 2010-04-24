
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

from gnuradio import audio, gr
import numpy
from sstv_swig import *

import gnuradio.gr.gr_threading as _threading


class audio_server:
  def __init__( self, sample_rate, default_signal ):

    self.sample_rate = sample_rate

    try:
      self.sink = audio.sink( self.sample_rate )
    except Exception, ex:
      print repr(ex)
      raise RuntimeError,"Could not access audio device"

    self.sources = []
    self.lengths = []
    self.is_const = []

    default_signal = list( default_signal )
    self.default_src = gr.vector_source_f( default_signal, True )


    self.sources.append( self.default_src )
    self.lengths.append( len(default_signal) )
    self.is_const.append( True )

    self.mux = mux() # from sstvi namespace

    self.mux.set_default_signal_length( len(default_signal) )

    self.tb = gr.top_block()
    self.tb.connect( self.default_src, ( self.mux, 0 ) )
    self.tb.connect( self.mux, self.sink )


  def start(self):

#    r = gr.enable_realtime_scheduling()
#    if r == gr.RT_OK:
#      print "Enabled realtime scheduling"
#    else:
#      print "Failed to enable realtime scheduling"

    try:
      self.tb.start()
    except Exception, ex:
      print repr(ex)
      raise RuntimeError, "Starting GR topblock failed"


  def add_source( self, signal = None ):
    try:
      if signal is None:
        src = gr.message_source( gr.sizeof_float, 1 ) # queue depth 1, MUST BE 1
        self.sources.append( src )
        self.lengths.append( 0 )
        self.is_const.append( False )
      else:
        src = gr.vector_source_f( list( signal ), True )
        self.sources.append( src )
        self.lengths.append( len( signal ) )
        self.is_const.append( True )

      id = len(self.sources)-1
      self.tb.connect( src, ( self.mux, id ) )
    except Exception,ex:
      print repr(ex)
      raise RuntimeError,"Could not connect new source to mux"

    return id


  def src_id_valid(self,id):
    return ( id < len(self.sources) and id > 0 )


  def src_ready(self,id):
    if not self.src_id_valid(id):
      return False # TODO raise
    if self.is_const[id]:
      return True

    return self.sources[id].msgq().empty_p()


  def enqueue_signal(self,id,signal):
    if not self.src_id_valid(id):
      raise RuntimeError, "Src ID invalid"

    if not self.src_ready(id):
      return False

    if self.is_const[id]:
      raise RuntimeError,"Can't enqueue signal in constant source"

    try:
      signal = numpy.array(signal,dtype=numpy.float32)
      msg = gr.message_from_string( signal.tostring() )
      self.sources[id].msgq().insert_tail( msg )
      self.lengths[id] = len( signal )
      return True
    except Exception,ex:
      print repr(ex)
      raise RuntimeError, "FAILED to enqueue signal?"

    return self.src_ready(id)


  def scheduler_ready(self):
    return self.mux.ready()


  def schedule_src(self,id):
    if not self.src_id_valid(id):
      raise RuntimeError, "Src ID invalid"

#    if self.src_ready(id): # i.e. empty
#      return False

    if not self.scheduler_ready():
      print "not ready"
      return False

    if self.lengths[id] <= 0:
      print "shit"
      return False

    self.mux.activate_port( id, self.lengths[id] )

    if not self.is_const[id]:
      self.lengths[id] = 0

    return True


  def wait(self):
    _waiter(self).wait()


  def stop(self):
    try:
      for i in range( 1, len( self.sources ) ):
        if not self.is_const[i]:
          self.sources[i].msgq().flush()
          self.sources[i].msgq().insert_tail( gr.message(1) )
    except Exception, ex:
      print repr(ex)

    self.tb.stop()





class _waiter(_threading.Thread):
  def __init__(self,audio):
    _threading.Thread.__init__(self)
    self.setDaemon(1)
    self.event = _threading.Event()
    self.audio = audio
    self.start()


  def run(self):
    self.audio.tb.wait()
    self.event.set()

  def wait(self):
    try:
      while not self.event.isSet():
        self.event.wait(0.100)
    except KeyboardInterrupt:
      self.audio.stop()
      self.wait()











