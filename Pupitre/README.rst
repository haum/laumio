The Laumio desk
###############

Presentation
============

The Laumio desk is a stand bearing a window on which one could move some colored disks. These window is filmed by a webcam so that the disks can be recognized by the associatd script using OpenCV. Once these disks have been seen, the script locates them onto the window. As the script knows Laumios are connected and where they are on the piece via a vitual map, it sends them color according to relative position between the color points and the Laumios, with decreasing profile for color. Mixing colors is allowed, and strongly recommended! Will you find how to make yellow without yellow colored disk?

FYI, "desk" stands here for  "pupitre" in french.


Setup
=====

You need Python 2, opencv for python and a webcam.

To launch the script, just type the following line in a terminal:

.. code-block: bash

  python pupitre.py <webcam id>

This script launches a server which communicates with Laumios via UDP sockets.
If 2 or more webcams are linkd to the computer, one can choose wich one to use by specifiying the webcam id (an int between 0 and n-1, n being the number of webcams). By default and if unspecified, the value is 0.


Configuring
===========

When launching the first script, Laumios are place randomly and without IP on the virtual map. In order to modify this, one configuration script has been developed. Using this, one can change position, color, and IPs.

To launch this script, type this command in the terminal:

.. code-block: bash

  python control_pupitre.py

An infinite loop is launched, asking you for configurations to set.

 -   Type "1" to set position for the Laumio. The coordinates x and y are floats between 0 and 1, they represent the relative position of the Laumio on the webcam screen. the origin is set on the upper left corner.
 -   Type "2" to set RGB color to a Laumio object
 -   Type "3" to release color to a Laumio object
 -   Type "4" to set IP to link a Laumio object with the real matching one
 -   Type "q" to escape from the script
 
For the lazy ones, configuration files can be used. For instance, the following one sets the second Laumio in the center of the virtuel map and forces him to green color.

.. code-block: bash
  1
  1
  0.5
  0.5
  2
  1
  0
  255
  0
  q

To send this file, just type the following command in your terminal:

.. code-block: bash

  python control_pupitre.py < <your configuration file>
