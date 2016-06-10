#!/bin/sh

# sh bench_france_flag.sh | socat - udp-sendto:Laumio_10508F.local:6969


fill() {
	echo -e "\xff\x$1\x$2\x$3"
}

led() {
	echo -e "\x$1\x$2\x$3\x$4"
}

branche() {
	if [ "$1" -eq 0 ]; then
		for i in $(seq 0 2); do
			sleep 0.05
			led $((i)) $2 $3 $4
		done
	elif [ "$1" -eq 2 ]; then
		for i in $(seq 3 5); do
			sleep 0.05
			led $((i)) $2 $3 $4
		done
	elif [ "$1" -eq 1 ]; then
		for i in $(seq 6 8); do
			sleep 0.05
			led $((i)) $2 $3 $4
		done
	elif [ "$1" -eq 3 ]; then
		for i in $(seq 9  12); do
			sleep 0.05
			led $((i)) $2 $3 $4
		done
	fi
}

while true; do
	sleep 1

	fill 00 00 00
	branche 0 00 00 ff
	branche 1 ff ff ff
	branche 2 ff 00 00
	led 9 00 00 00

	sleep 1

	fill ff 00 00
	branche 0 00 00 00
	branche 1 00 00 ff
	branche 2 ff ff ff
	led 9 00 00 00
	sleep 1

	fill ff ff ff
	branche 0 ff 00 00
	branche 1 00 00 00
	branche 2 00 00 ff
	led 9 00 00 00
	sleep 1

	fill 00 00 ff
	branche 0 ff ff ff
	branche 1 ff 00 00
	branche 2 00 00 00
	led 9 00 00 00
done
