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
from sstv_swig import *
import gnuradio.gr.gr_threading as _threading
import sstv_swig
# try to catch exceptions only if we can provide additional information
# or if we can recover
# else pass them up

class offline_sstv_modulator:
  def __init__( self, sample_rate ):
    self.sample_rate = sample_rate
    self.__create_impl( sample_rate )

  def set_data( self, data ):
    try:
      return self._impl.set_data( data )
    except TypeError, ex:
      print "WARNING caught exception on setting data"
      print "Python type doesn't match expected data type of modulator"
      print repr(ex)
      raise ex

    return False

  def start( self ):
    return self._impl.start()

  def wait( self ):
    try:
      _waiter( self._impl ).wait()
      return True
    except Exception, ex:
      print "ERROR caught exception on wait"
      print repr(ex)
      raise

    return False

  def done( self ):
    return self._impl.done()

  def recreate_impl( self ):
    """
    One chance to recover
    """
    try:
      del self._impl
      self.__create_impl( self.sample_rate )
    except Exception, ex:
      print "FAILED to recreate implementation"
      print repr(ex)

    return True

  def ready( self ):
    try:
      return self._impl.done()
    except Exception,ex:
      print "ERROR caught exception in ready"
      print repr(ex)

    return False

  def __create_impl( self, sample_rate ):
    try:
      sm =  martin_m1_state_machine()
      sm = sm.to_baseptr( sm )
      self._impl = offline_compute_sstv_signal( sample_rate, sm )
    except Exception,ex:
      print "FAILED to instantiate offline modulator implementation"
      raise ex

  def successful( self ):
    try:
      return self._impl.successful()
    except Exception, ex:
      print "ERROR caught exception in successful"
      print repr(ex)

    return False

  def get_signal( self ):
    try:
      return list(self._impl.get_signal())
    except Exception, ex:
      # segfault
      # _impl doesn't exist
      # etc.
      print "Caugh exception on get_signal"
      print repr(ex)

    return list()


class _waiter(_threading.Thread):
  def __init__(self, block):
    _threading.Thread.__init__(self)
    self.setDaemon(1)
    self.block = block
    self.event = _threading.Event()
    self.start()

  def run(self):
    join_unlocked(self.block)
    self.event.set()

  def wait(self):
    try:
      while not self.event.isSet():
        self.event.wait(0.100)
    except KeyboardInterrupt:
      self.block.cancel()
      self.wait()


