Conception du Laumio
====================

Le Laumio est constitué d'une `lampe Ikea FADO`_ et d'une structure "maison" contrôlant 13 LEDs RGB.

Cette structure physique est elle-même constituée d'une partie mécanique supportant les lampes et du système électronique associé.

Partie mécanique
----------------

.. tip::

  Tous les fichiers de conception peuvent être récupérés `sur le dépôt Git`_.


La structure mécanique est composée d'un tronc central conçu pour se tenir sur la douille de l'ampoule. Celui-ci a été usiné grâce à une minifraiseuse et assemblé sans colle.

.. image:: /_static/images/mechanical/structure.png

Deux propositions ont ensuite été portées pour accueillir les LEDs en elles-mêmes.

La première proposition est basée sur des bras pliables, les LEDs étant collées à leurs extrémités. Les problèmes qui en découlent sont principalement la fragilité de la conception (notamment au niveau des soudures) et l'importante manipulation nécessaire pour insérer cet arbre dans le verre.

La seconde proposition est toujours à l'étude. Celle-ci utiliserait des bandes de plastique souple reliées en haut et en bas de la structure.

Système électronique
--------------------

Le système électronique est construit autour d'un `ESP Wemos d1 Mini`_ de de 13 LED WS2812. Afin de réutiliser le  Wemos dans d'autres projets, il a été décidé de créer un shield adapter pour y connecter le microcontrôleur.
Les fichiers de conception KiCAD design de ce shield peuvent être `téléchargés ici`_.

Le code utilisé dans l'ESP est aussi `présent sur le dépôt`_, merci de lire le fichier `README`_ pour être en mesure de compiler et de flasher le programme.

Une fois flashé et démarré, le Laumio va chercher à se connecter au réseau WiFi spécifié (voir
``wifi-config.h``) et attendre des instructions UDP ou HTTP.

Démarrage
---------

Au lancement, le Laumio clignote en violet puis démarre une animation de couleur rouge : il est en train de chercher le réseau WiFi spécifié et va tenter de s'y connecter.

Si la connexion est réussie, le Laumio lancera une animation Arc-en-ciel. Sinon, il s'illuminera en orage, signalant par-là que le mode *Access Point* est activé. Ce mode permettra à l'utilisateur de donner les informations nécessaires pour connecter l'appareil à un réseau WiFi valide.


.. _lampe Ikea FADO: http://www.ikea.com/us/en/catalog/products/70096377/
.. _ESP Wemos d1 Mini: http://www.wemos.cc/Products/d1_mini.html
.. _sur le dépôt Git: https://github.com/haum/laumio/tree/master/CAD
.. _téléchargés ici: https://github.com/haum/laumio/tree/master/kicad/wemos_d1mini_shield
.. _présent sur le dépôt: https://github.com/haum/laumio/tree/master/laumio
.. _README: https://github.com/haum/laumio/blob/master/README.rst
