Librairie Python
================

La classe ``Laumio`` est construite comme une encapsulation des commandes minimales présentées dans la `page de l'API`_.

.. py:class:: Laumio

  .. py:method:: __init__(ip)

    Constructeur pour contrôler un Laumio répondant à l'adresse IP *ip*.

  .. py:method:: wipeOut()

    Eteint toutes les LEDs.

    *En réalité juste un alias pour :py:method:`fillColor`*.

  .. py:method:: fillColor(r, g, b)

    Remplit toutes les LEDs du Laumio avec la couleur (*r*, *g*, *b*).

  .. py:method:: fillRing(ringid, r, g, b)

    Remplit un anneau d'une seule couleur. *ringid* est dans l'intervalle 0~3 et *r*, *g*, *b* précisent la couleur.
    La LED du sommet n'est pas commandable ici.

  .. py:method:: fillColumn(colmunid, r, g, b)

    Remplit une colonne d'une seule couleur. *columnid* est dans l'intervalle 0~4 et *r*, *g*, *b* précisentla couleur.
    La LED du sommet n'est pas incluse dans les colonnes.

  .. py:method:: setPixelColor(pixel, r, g, b)

    Passe un pixel *pixel* (0~12) à la couleur (*r*, *g*, *b*) demandée.

  .. py:method:: colorWipe(r, g, b, delay)

    Lance une animation de remplissage progressif avec la color (*r*, *g*, *b*) et un intervalle de temps *delay* entre deux LEDs.

  .. py:method:: rainbow()

    Lance une animation arc-en-ciel.

  .. py:method:: status()

    Return the JSON **string** for the Laumio's status

  .. py:method:: _send(payload)

    Conventionnaly private, this method is used to transmit a raw ``bytearray`` payload to
    a Laumio. It can be used to trigger custom animations.

.. _page de l'API: /api.html
