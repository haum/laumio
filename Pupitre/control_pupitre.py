#!/usr/bin/env python2

import socket, struct

serv = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
choice = 0
to = ("localhost", 50000)

while choice != 'q':

	print "1) Set Position"
	print "2) Force color"
	print "3) Release color"
	print "4) Set IP"
	print "q) Quit"
	choice = raw_input('> ')
	print ""

	if choice == "1":
		nb = int(raw_input("nb = "))
		x = float(raw_input("x = "))
		y = float(raw_input("y = "))
		print ""
		serv.sendto(struct.pack('=BBff', ord('P'), nb, x, y), to)

	elif choice == "2":
		nb = int(raw_input("nb = "))
		r = int(raw_input("r = "))
		g = int(raw_input("g = "))
		b = int(raw_input("b = "))
		print ""
		serv.sendto(struct.pack('=BBBBBB', ord('F'), nb, r, g, b, 1), to)

	elif choice == "3":
		nb = int(raw_input("nb = "))
		print ""
		serv.sendto(struct.pack('=BBBBBB', ord('F'), nb, 0, 0, 0, 0), to)

	elif choice == "4":
		nb = int(raw_input("nb = "))
		ip = raw_input("ip = ")
		print ""
		serv.sendto(struct.pack('=BB16s', ord('I'), nb, ip), to)
