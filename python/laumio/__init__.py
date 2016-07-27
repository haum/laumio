#/usr/bin/env python3

import socket
import urllib.request

class Laumio:
    def __init__(self, ip):
        self.__ip = ip
        self.__url = 'http://'+str(ip)+'/api/'

    def fillColor(self, r, g, b):
        payload = bytearray([ 255, r, g, b ])
        response = self.send(payload)
        print(response)

    def setPixelColor(self, pixel, r, g, b):
        payload = bytearray([ pixel, r, g, b ])
        response = self.send(payload)
        print(response)

    def status(self):
        """ Get the JSON Laumio status """
        return urllib.request.urlopen(self.__url).read().decode()


    def send(self, payload):
        sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
        sock.sendto(payload, (self.__ip, 6969))
        return "XXX"
