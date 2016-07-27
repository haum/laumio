Python Library
==============

The class ``Laumio`` is built as a wrapper around the primitives exposed in the `API
page`_.

.. py:class:: Laumio

  .. py:method:: __init__(ip)

    Constructor to control a Laumio responding on IP address *ip*.

  .. py:method:: wipeOut()

    Shuts down all the LED

    *Actually just an alias for :py:method:`fillColor`*.

  .. py:method:: fillColor(r, g, b)

    Fill the whole Laumio with a color (*r*, *g*, *b*).

  .. py:method:: fillRing(ringid, r, g, b)

    Fill a ring with a color. *ringid* can be 0~3 and *r*, *g*, *b* specify the color.
    The top LED is not addressed.

  .. py:method:: setPixelColor(pixel, r, g, b)

    Set a pixel *pixel*  (0~12) to a given color (*r*, *g*, *b*).

  .. py:method:: colorWipe(r, g, b, delay)

    Start a color wipe animation with color (*r*, *g*, *b*) and inter-LED delay *delay*.

  .. py:method:: rainbow()

    Start a rainbow animation.

  .. py:method:: status()

    Return the JSON **string** for the Laumio's status

  .. py:method:: _send(payload)

    Conventionnaly private, this method is used to transmit a raw ``bytearray`` payload to
    a Laumio. It can be used to trigger custom animations.

.. _API page: /api.html
