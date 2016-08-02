#/usr/bin/env python3

import socket
import urllib.request

class Laumio:
    """ Laumio's abstraction layer

    ip -- Laumio's IP
    """

    def __init__(self, ip):
        self.__ip = ip
        self.__url = 'http://'+str(ip)+'/api/'

    def wipeOut(self):
        """ Shut down all the LED """
        return self.fillColor(0, 0, 0)

    def fillColor(self, r, g, b):
        """ Set the color of all the leds in the Laumio
        r -- red byte
        g -- green byte
        b -- blue byte
        """
        payload = bytearray([ 255, r, g, b ])
        return self._send(payload)

    def fillRing(self, ringid, r, g, b):
        """ Set the color of all the leds of ring ringid in the Laumio
        ringid -- Ring ID (0~2)
        r -- red byte
        g -- green byte
        b -- blue byte
        """
        payload = bytearray([ 1, ringid, r, g, b ])
        return self._send(payload)

    def fillColumn(self, columnid, r, g, b):
        """ Set the color of all the leds of column columnid in the Laumio
        columnid -- Column ID (0~3)
        r -- red byte
        g -- green byte
        b -- blue byte
        """
        payload = bytearray([ 2, columnid, r, g, b ])
        return self._send(payload)

    def setPixelColor(self, pixel, r, g, b):
        """ Set the color of all the leds pixel of the Laumio
        pixel -- LED ID (0~12)
        r -- red byte
        g -- green byte
        b -- blue byte
        """
        payload = bytearray([ 0, pixel, r, g, b ])
        return self._send(payload)

    def colorWipe(self, r, g, b, delay):
        """ Send colorWipe animation with the specified color and delay
        r -- red byte
        g -- green byte
        b -- blue byte
        delay -- delay to wait between LED
        """
        payload = bytearray([ 0x0b, r, g, b, delay ])
        return self._send(payload)

    def rainbow(self):
        """ Start Rainbow animation """
        payload = bytearray([ 0x0a ])
        return self._send(payload)

    def status(self):
        """ Get the JSON Laumio status """
        return urllib.request.urlopen(self.__url).read().decode()

    def _send(self, payload):
        """ Proxy to socket.socket.sendto w/ minimal error-handling """
        try:
            sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
            sock.sendto(payload, (self.__ip, 6969))
            return 0
        except socket.error:
            return 1

