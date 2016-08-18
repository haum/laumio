Simple Bash Interface
=====================

The Laumio can be controlled simply using socat_ or any other well-designed network tool.

One can use the following boilerplate :

.. code-block:: bash

  #!/bin/bash

  # Consts
  IP=             # Laumio's IP
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

    # write your animation here...

  done
  ) | socat - udp-sendto:$IP:6969


.. _socat: http://www.dest-unreach.org/socat/
