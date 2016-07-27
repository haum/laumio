Designing the Laumio
====================

Laumio is based on a `Ikea FADO lamp`_ and a homemade system bearing 13 RGB LED.

The physical system itself is composed of the mechanical part supporting the lamps and
the electronic board and system.

Mechanical Part
---------------

.. tip::

  All the design files can be found `on the Git repository`_.


The mechanical part is composed of a central structure designed to stand on the lightbulb
socket. It's is machined using a in-house CNC mill and dry-assembled.

.. image:: /_static/images/mechanical/structure.png

Two different propositions were made to hold the LED themselves.

The first one is based on foldable arms with LED glued at the ends. The main issues are
both the fragility of the design and the effort needed to fit it in the glass.

The second one, still under study, is based on flexible plastic branches cruve from the to
to the bottom of the structure.

Electronic system
-----------------

The electronic system consist of a `ESP Wemos d1 Mini`_ and on 13 LED WS2812. To be able
to reuse the Wemos in other projects, it was decided to create a shield to dock it on. The
KiCAD design files for the shield itself can be `downloaded here`_.

The code running on the ESP is `given on the repo too`_, please follow the `README`_ to be
able to compile and flash it.

Once flashed and started, the Laumio will look for the specified Wifi network (see
``wifi-config.h``) and wait for UDP or HTTP instructions.

An animation engine is implemented but **not exposed yet**.

.. _Ikea FADO lamp: http://www.ikea.com/us/en/catalog/products/70096377/
.. _ESP Wemos d1 Mini: http://www.wemos.cc/Products/d1_mini.html
.. _on the Git repository: https://github.com/haum/laumio/tree/master/CAD
.. _downloaded here: https://github.com/haum/laumio/tree/master/kicad/wemos_d1mini_shield
.. _given on the repo too: https://github.com/haum/laumio/tree/master/laumio
.. _README: https://github.com/haum/laumio/blob/master/README.rst
