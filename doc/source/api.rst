Laumio's anatomy and HEX commands
=================================

The Laumio can be controlled using the bare-UDP API using simple HEX commands sent on the
**port 6969**, using REST API or through MQTT.

Four different features can be set : the pixel, ring, column and the whole Laumio at once.
Additionnaly, some animations can be triggered.

The leds are disposed in 4 branches of three LED plus one at the top. They are numbered
starting from the bottom of one branch to its top, skipping the led at the center, going
down on the other side and then from the bottom of one of the other two branches to the
bottom of the other (with the top middle LED included).

The so-called rings correspond to a layer of LED. There's three of them, numbered from
bottom to top.

As for the columns, they correspond to the four branches of LEDs, of course without the top LED.

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

Column-wise control
*******************

The command is ``0x02`` and one must specify first the column (``0x00``, ``0x01``, ``0x02`` or ``0x03``) and
then the three bytes of the color::

  0x02 ColumnID R G B

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
two parameters, the 3 bytes of color and the delay (ms between each light-up) between two LED::

  0x0b R G B Delay

Rainbow
~~~~~~~

The rainbow animation takes no parameters, it's the one used when the Laumio manage to
connect to a Wifi network::

  0x0a


REST API
--------

The REST API can be called by sending JSON data to the ``/api/`` endpoint.

Status
******

The status of the Laumio can be retrieved using a simple ``GET`` request, no data requiered::

  GET http://<laumio_ip>/api/

The response is given in the form :

.. code-block:: javascript

  {"hostname":"laumio","version":"devel"}

Commands
********

The Laumio can be controlled through simple POST requests with JSON content. See
JSON API for details about JSON content to send.

In case of success:

.. code-block:: javascript

  {"hostname":"laumio","status":"Success"}

In case of failure:

.. code-block:: javascript

  {"hostname":"laumio","status":"Invalid Request","massage":"Unable to parse JSON"}


MQTT API
--------

If the laumio can connect to your broker, it can be controlled through MQTT.

Online status
*************

At connection laumio publishes to the topic ``laumio/<name>/status`` is online status,
if laumio will go offline this topic will send an offline status

Advertise
*********

At connection, it publishes its name to the topic ``laumio/status/advertise``.

The same message is sent when it receives a ``discover`` command.

Commands
********

The command is chosen with the topic: ``laumio/all/<cmd>`` or ``laumio/<name>/<cmd>``
whether you want to send it to all connected clients or to a specific one.

set_pixel
~~~~~~~~~

Change color of a led.

The 4 bytes payload is the led number followed by red, green, blue components of the color (0-255 range)

set_ring
~~~~~~~~~

Change color of a ring.

The 4 bytes payload is the ring number followed by red, green, blue components of the color (0-255 range)

set_column
~~~~~~~~~~

Change color of a column.

The 4 bytes payload is the column number followed by red, green, blue components of the color (0-255 range).

color_wipe
~~~~~~~~~~

Start color wipe animation with a given color and duration.

The 4 bytes payload is red, green, blue components of the color (0-255 range) followed by duration.

animate_rainbow
~~~~~~~~~~~~~~~

Start rainbow animation.

Payload is ignored.

fill
~~~~

Set the same color for all leds.

The 3 bytes payload is red, green, blue components of the color (0-255 range).

json
~~~~

Send JSON commands through the JSON API.

discover
~~~~~~~~

Send back a ``laumio/status/advertise`` message with its name as payload.


JSON API
--------

This API cannot be used alone. It is accessed either though MQTT ``json`` command or REST API.

Commands
********

set_pixel
~~~~~~~~~

Change color of a led.

.. code-block:: javascript

  {
    'command': 'set_pixel',
    'led': PixelID,
    'rgb': [R, G, B]
  }

set_ring
~~~~~~~~~

Change color of a ring.

.. code-block:: javascript

  {
    'command': 'set_ring',
    'ring': RingID,
    'rgb': [R, G, B]
  }

set_column
~~~~~~~~~~

Change color of a column.

.. code-block:: javascript

  {
    'command': 'set_column',
    'column': ColumnID,
    'rgb': [R, G, B]
  }

color_wipe
~~~~~~~~~~

Start color wipe animation with a given color and duration.

.. code-block:: javascript

  {
    'command': 'color_wipe',
    'duration': Duration,
    'rgb': [R, G, B]
  }

animate_rainbow
~~~~~~~~~~~~~~~

Start rainbow animation.

.. code-block:: javascript

  {
    'command': 'animate_rainbow',
  }

fill
~~~~

Set the same color for all leds.

.. code-block:: javascript

  {
    'command': 'fill',
    'rgb': [R, G, B]
  }

Multiple commands
*****************

A few commands can be chained in one call when set in the same array named
``commands``, but beware that the size of JSON is somewhat limited.


.. code-block:: javascript

  {
    'commands': [
      {
        'command': 'set_column',
        'column': 0,
        'rgb': [255, 0, 0]
      },
      {
        'command': 'set_column',
        'column': 2,
        'rgb': [0, 0, 255]
      }
    ]
  }


Pixel-wise control (legacy API)
*******************************

.. code-block:: javascript

  {
    'led': PixelID,
    'rgb': [R, G, B]
  }

Full span control (legacy API)
******************************

.. code-block:: javascript

  {
    'led': 255,
    'rgb': [R, G, B]
  }

