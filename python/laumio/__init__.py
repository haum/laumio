#/usr/bin/env python3

import urllib3

class Laumio:
    def __init__(self, ip):
        self.__ip = ip
        self.__url = 'http://'+str(ip)+'/api/'
        self.__http = urllib3.PoolManager()

    def fillColor(self, r, g, b):
        payload = '{led:255,rgb:['+str(r)+','+str(g)+','+str(b)+']}'
        response = self.send(payload)
        print(response)

    def status(self):
        return self.__http.urlopen('GET', self.__url)

    def send(self, payload):
        print(payload)
        return self.__http.urlopen('POST', self.__url, body=payload).read()
