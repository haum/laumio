#!/usr/bin/env python2

import cv2
import sys, random, socket, struct
import numpy as np
from math import sqrt, sin, cos, pi, atan2, fmod

#################
# Choose camera #
#################

camera = ' '.join(sys.argv[1:])
try:
	v = int(camera)
	camera = v
except ValueError:
	pass
if camera == '':
	camera = 0

######################
# Open video capture #
######################
 
VIDEO_W = 640
VIDEO_H = 480
cap = cv2.VideoCapture(camera)
cap.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, VIDEO_W)
cap.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, VIDEO_H)

##################
# Control server #
##################

class ControlServer:
	def __init__(self):
		self.serv = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.serv.bind(("", 50000))
		self.serv.settimeout(0.0)

	def process(self):
		try:
			data, addr = self.serv.recvfrom(1024)
			if ord(data[0]) == ord('P'):
				cmd, idx, x, y = struct.unpack('=BBff', data)
				if len(lhaumpions) > idx:
					lhaumpions[idx].x = x * VIDEO_W
					lhaumpions[idx].y = y * VIDEO_H
			elif ord(data[0]) == ord('F'):
				cmd, idx, r, g, b, f = struct.unpack('=BBBBBB', data)
				if len(lhaumpions) > idx:
					lhaumpions[idx].force_color(r, g, b, f)
			elif ord(data[0]) == ord('I'):
				cmd, idx, ip = struct.unpack('=BB16s', data)
				if len(lhaumpions) > idx:
					lhaumpions[idx].ip = ip.rstrip(' \t\r\n\0')
		except socket.error:
			pass

ctrlserv = ControlServer()

##############
# LHAUMpions #
##############

class LHAUMpion:
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	def __init__(self, x, y):
		self.x = x * VIDEO_W
		self.y = y * VIDEO_H
		self.hx = 0
		self.hy = 0
		self.v = 0
		self.bgr = (0,0,0)
		self.forced = False
		self.ip = False
		self.reset()

	def reset(self):
		self.hue = fmod(atan2(self.hy, self.hx) / 2 / pi + 1, 1)
		norm = (self.hx*self.hx + self.hy*self.hy) * self.v
		if norm > 1: norm = 1
		if norm < 0: norm = 0
		self.value = norm
		if not self.forced:
			bgr = cv2.cvtColor(np.uint8([[[int(self.hue * 180), 255, self.value*255]]]), cv2.COLOR_HSV2BGR)[0][0]
			self.bgr = (int(bgr[0]), int(bgr[1]), int(bgr[2]))

		self.hx = 0
		self.hy = 0
		self.v = 0

	def merge(self, obj):
		dist = sqrt((obj.pt[0] - self.x)**2/VIDEO_W**2 + (obj.pt[1] - self.y)**2/VIDEO_H**2)/sqrt(2)
		weight = obj.size/dist**2 / 500
		self.v += weight
		if weight > 1: weight = 1
		if weight < 0: weight = 0
		self.hx += weight*cos(pi*2*obj.hue)
		self.hy += weight*sin(pi*2*obj.hue)

	def force_color(self, r, g, b, forced):
		self.forced = forced
		self.bgr = (b, g, r)

	def send(self):
		if self.ip:
			msg = struct.pack('=BBBB', 0xff, self.bgr[2], self.bgr[1], self.bgr[0])
			self.sock.sendto(msg, (self.ip, 6969))

lhaumpions = []
for i in range(10):
	lhaumpions.append(LHAUMpion(random.random(), random.random()))

#########################
# Circle detection init #
#########################

# Parameters instance
params = cv2.SimpleBlobDetector_Params()

# Change thresholds
params.minThreshold = 100
params.maxThreshold = 240

# Filter by Area
params.filterByArea = True
params.minArea = 100
params.maxArea = 1000000

# Filter by Circularity
params.filterByCircularity = True
params.minCircularity = 0.80
 
# Filter by Convexity
params.filterByConvexity = True
params.minConvexity = 0.90
params.maxConvexity = 1
 
# Create a detector with the parameters
detector = None
ver = (cv2.__version__).split('.')
if int(ver[0]) < 3 :
    detector = cv2.SimpleBlobDetector(params)
else : 
    detector = cv2.SimpleBlobDetector_create(params)

#######################
# Interactive objects #
#######################

class InteractiveObject:
	def __init__(self, pt, size, hue):
		self.update(pt, size, hue)

	def update(self, pt, size, hue):
		self.pt = pt
		self.size = size
		self.hue = hue
		self.alive = 1

	def areYou(self, pt, hue):
		return ((self.pt[0]-pt[0])**2 + (self.pt[1]-pt[1])**2 < (3*self.size)**2) and (abs(self.hue - hue) < 0.1)

	def age(self):
		self.alive -= 0.1
		if self.alive <= 0: self.alive = 0
alives = []

###########
# Process #
###########

display = True
while cv2.waitKey(10) == -1:
	# Read next image
	ret, im = cap.read()

	# Find circles
	keypoints = detector.detect(im)

	# Update alive objects
	for kp in keypoints:
		found = False
		hue = float(cv2.cvtColor(np.uint8([[im[kp.pt[1]][kp.pt[0]]]]), cv2.COLOR_BGR2HSV)[0][0][0])/180
		for a in alives:
			if a.areYou(kp.pt, hue):
				a.update(kp.pt, int(kp.size), hue)
				found = True
		if not found:
			alives.append(InteractiveObject(kp.pt, int(kp.size), hue))

	# Draw, age and kill-olds alives
	for a in alives:
		bgr = cv2.cvtColor(np.uint8([[[int(a.hue * 180), 255, 255]]]), cv2.COLOR_HSV2BGR)[0][0]
		bgr = (int(bgr[0]), int(bgr[1]), int(bgr[2]))
		if display:
			cv2.ellipse(
				im,
				(int(a.pt[0]), int(a.pt[1])),
				(a.size, a.size),
				-90,
				0, 360*a.alive,
				bgr,
				4
			)
		for l in lhaumpions:
			l.merge(a)
		a.age()
		if a.alive == 0:
			alives.remove(a)

	# Draw lhaumpions
	for l in lhaumpions:
		if display:
			cv2.circle(
				im,
				(int(l.x), int(l.y)),
				10,
				l.bgr,
				20
			)
		l.send()
		l.reset()

	# Get pupitre control
	ctrlserv.process()

	# Show results
	if display:
		cv2.imshow('Vision', im)
