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
from PIL import Image
import urllib

import gnuradio.gr.gr_threading as _threading

#vws_ip = "192.168.1.24"
vws_ip = "192.168.1.200"
imgurl = "http://"+vws_ip+"/Jpeg/CamImg.jpg"

tmp_filename = "/tmp/tmp_downloaded.jpg"

# http://192.168.1.24/SetChannel.cgi?Channel=0
# http://192.168.1.24/ChangeResolution.cgi?ResType=0  0=small,3=high res
# http://192.168.1.24/ChangeCompressRatio.cgi?Ratio=0 0=common,1=high

class image_loader:
  def __init__( self, imgsize ):
    self.retriever = None
    self.imgsize = imgsize
    self.imgurl = imgurl

  def start_get_image( self ):
    self.retriever = image_retriever( self.imgurl )

  def get_image( self ):
    if self.retriever is None:
      #error
      return None

    self.retriever.wait()

    if not self.successful():
      return None

    data = self.retriever.get_data()
    del self.retriever
    self.retriever = None

    print repr(data)

    if data is None:
      #error
      return None

    print "get from file"
    img = self.get_from_file( data[0], self.imgsize ) # may throw
    print "returned"
    if img is None:
      print "img is None"
    else:
      print "img is not None"

    del data

    return img

  def successful( self ):
    if self.retriever is None:
      return False

    return self.retriever.successful()

  def done( self ):
    if self.retriever is None:
      return False

    return self.retriever.ready()


  def get_from_file( self, filename, imgsize ):
    try:
      img = Image.open(filename)

      if not imgsize == img.size:
        img = img.resize(imgsize, Image.ANTIALIAS)

      return img
    except:
      raise


class image_retriever(_threading.Thread):
  def __init__(self,url):
    _threading.Thread.__init__(self)
    self.setDaemon(1)
    self.event = _threading.Event()
    self.url = url
    self.start()
    self.data = None

  def run(self):
    self.data = None
    # generates tmpfile, returns filename and headers
    try:
      self.data = urllib.urlretrieve( self.url, tmp_filename )
    except Exception,ex:
      # e.g. download interrupted
      self.data = None
      print repr(ex)
    self.event.set()

  def wait(self):
    try:
      while not self.event.isSet():
        self.event.wait(0.100)
    except KeyboardInterrupt:
      self.wait()

  def ready(self):
    return self.event.isSet()

  def get_data(self):
    return self.data

  def successful(self):
    if self.ready():
      return ( self.data is not None )
    return False

