#!/bin/bash

# Consts
ANIM_TIME=0.05
PAUSE_TIME=0.3

# Utils
fill() {
	echo -e "\xff\x$1\x$2\x$3"
}

led() {
	echo -e "\x00\x$1\x$2\x$3\x$4"
}

ring() {
	echo -e "\x01\x$1\x$2\x$3\x$4"
}

# Program
(
fill 00 00 00

while true; do
	ring 0 00 ff 00
	sleep $ANIM_TIME
	ring 1 00 ff 00
	sleep $ANIM_TIME
	ring 2 00 ff 00
	sleep $ANIM_TIME
	led 9 00 ff 00
	sleep $PAUSE_TIME
	ring 0 00 00 00
	sleep $ANIM_TIME
	ring 1 00 00 00
	sleep $ANIM_TIME
	ring 2 00 00 00
	sleep $ANIM_TIME
	led 9 00 00 00
	sleep 1
done
) | socat - udp-sendto:Laumio_104B2E.local:6969
