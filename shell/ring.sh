#!/bin/bash

# Consts
ANIM_TIME=0.05
PAUSE_TIME=0.3

# Utils
fill() {
	echo -en "\xff\x$1\x$2\x$3"
}

led() {
	echo -en "\x00\x$1\x$2\x$3\x$4"
}

ring() {
	echo -en "\x01\x$1\x$2\x$3\x$4"
}

column() {
	echo -en "\x02\x$1\x$2\x$3\x$4"
}

# Program
(
fill 00 00 00
sleep $ANIM_TIME

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

	sleep $PAUSE_TIME

	fill 00 00 00
	sleep $ANIM_TIME

	column 0 ff 00 00
	sleep $ANIM_TIME
	column 3 00 00 00
	sleep $PAUSE_TIME
	column 1 ff 00 00
	sleep $ANIM_TIME
	column 0 00 00 00
	sleep $PAUSE_TIME
	column 2 ff 00 00
	sleep $ANIM_TIME
	column 1 00 00 00
	sleep $PAUSE_TIME
	column 3 ff 00 00
	sleep $ANIM_TIME
	column 2 00 00 00
	sleep $PAUSE_TIME

done



) | socat - udp-sendto:Laumio_104B2E.local:6969
