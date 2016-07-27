Laumio's anatomy and HEX commands
=================================

The Laumio can be controlled using the bare-UDP API using simple HEX commands sent on the
**port 6969**. It can alternatively receive commands from a REST API.
Three different features can be set : the pixel, ring and the whole Laumio at once.
Additionnaly, some animations can be triggered.

The leds are disposed in 4 branches of three LED plus one at the top. They are numbered
starting from the bottom of one branch to its top, skipping the led at the center, going
down on the other side and then from the bottom of one of the other two branches to the
bottom of the other (with the top middle LED included).

The so-called rings correspond to a layer of LED. There's three of them, numbered from
bottom to top.

Bare UDP
--------

Pixel-wise control
******************

The command is ``0x00`` and the data to be sent is first the pixel ID and the three byte
of the (RGB) color::

  0x00 PixelID R G B

Ring-wise control
*****************

The command is ``0x01`` and one must specify first the ring (``0x00``, ``0x01`` or ``0x02``) and
then the three bytes of the color::

  0x01 RingID R G B

Full span control
*****************

The command is ``0xff`` and the only thing to specify is the RGB color::

  0xff R G B

Animations
**********

This is still a work in progress but 2 animations can be triggered on the Laumio.

Color Wipe
~~~~~~~~~~

The color wipe animation changes the color on the full Laumio one LED at a time. It takes
two parameters, the 3 bytes of color and the delay between two LED::

  0x0b R G B Delay

Rainbow
~~~~~~~

The rainbow animation takes no parameters, it's the one used when the Laumio manage to
connect to a Wifi network::

  0x0a


JSON API
--------

Status
******

The status of the Laumio can be retrieved using the ``/api/`` endpoint::

  GET http://<laumio's ip>/api/

The response is given in the form :

.. code-block:: javascript

  {"name":"laumio","version":"devel"}

Pixel-wise control
******************

The Laumio can be controlled through simple POST requests, specifying both the ``led`` and
``rgb`` key:

.. code-block:: javascript

  {
    'led': PixelID,
    'rgb': [R, G, B]
  }

Full span control
*****************

To control the full Laumio at once, just set the ``led`` parameter to 255 in the previous
JSON snippet:

.. code-block:: javascript

  {
    'led': 255,
    'rgb': [R, G, B]
  }
