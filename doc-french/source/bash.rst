Interface bash minimale
=======================

Le Laumio peut être commandé très simplement en utilisant `socat`_ ou n'importe quel autre outil réseau bien conçu.

Nous vous proposons le code suivant en guise d'exemple et de base de travail :

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

En outre, il est également possible d'envoyer plusieurs commandes en une seule requête, comme dans l'exemple suivant:

.. code-block::bash

  echo -en "\xff\xFF\x00\x00\x00\x09\xff\xff\xff" | socat - udp-sendto:$IP:6969

Ici, le Laumio est d'abord intégralement passé en rouge, puis la LED du sommet est passée en blanc.


.. _socat: http://www.dest-unreach.org/socat/
