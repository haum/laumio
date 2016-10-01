Anatomie du Laumio's et and commandes en valeurs hexadécimales
==============================================================

Le laumio peut être commandé via une API UDP minimale utilisant des commandes simples en hexadécimal envoyées sur le
**port 6969**. Celui-ci peut également recevoir via des commandes d'une API REST.
Quatre types de sous-ensembles sont utilisables pour commander les LEDs du Laumio  : le pixel (une seule led), l'anneau, la colonne et le Laumio dans son intégralité.
Il est aussi possible de lancer quelques animations plus spécifiques.

Les LEDs sont disposées en 4 branches de trois LEDs, en plus d'une à son sommet. Celles-ci sont toutes numérotées de 0 à 12 depuis le bas d'une des branches vers sa cîme, évitant l'unique led du sommet, descendant ensuite de l'autre côté de la structure, et enfin remontant du bas d'une des deux branches restantes pour atteindre le bas de la dernière (la LED de sommet étant cette fois-ci reliée entre les deux branches).

Les anneaux correspondant quant à eux à une ligne horizontale de LEDs. Il y en a trois numérotées de 0  à 2, de bas en haut.

Enfin, dans le cas des colonnes, celles-ci se rapportent aux quatre branches de LEDs, bien entendu sans la LED du sommet.

API UDP minimale
----------------

Commande pixel par pixel
************************

La commande est ``0x00``. Les données à envoyer sont d'abord l'ID du pixel considéré et les trois octets de la couleur (RGB) choisie::

  0x00 PixelID R G B

Commande anneau par anneau
**************************

La commande est ``0x01``. Il faut d'abord spécifier l'ID de l'anneau (``0x00``, ``0x01`` or ``0x02``) puis préciser les trois octets de couleur::

  0x01 RingID R G B

Commande colonne par colonne
****************************

La commande est ``0x02``. Ici, il faut indiquer l'ID de la colonne (``0x00``, ``0x01``, ``0x02`` or ``0x03``) puis les trois octets de couleur::

  0x02 ColumnID R G B

Remplissage intégral
********************

La commande est ``0xff``, il suffit ensuite de specifier la couleur RGB::

  0xff R G B

Animations
**********

Les animations prédéfinies sont encore en développement mais deux d'entre elles ont déjà implémentées dans le Laumio.

Remplissage progressif
~~~~~~~~~~~~~~~~~~~~~~

L'animation de Remplissage progressif ("color wipe") change passe le Laumio intégralement dans une seule couleur, mais LED après LED. Elle prend deux paramètres, que sont les trois octets de la nouvelle couleur et l'intervalle de temps entre de deux LEDs::

  0x0b R G B Delay

Arc-en-ciel
~~~~~~~~~~~

L'animation Arc-en-ciel ("rainbow") ne prend pas de paramètres. En l'occurence, c'est celle utilisée par le Laumio quand celui-ci réussit à se connecter à un réseau WiFi::

  0x0a


API JSON
--------

L'API JSON est toujours en développement et toutes les requêtes UDP actuellement possibles n'ont pas encore été réimplémentées .

Statut
******

Le statut du Laumio peut être obtenu en utilisant un endpoint de l'``/api/``::

  GET http://<laumio's ip>/api/

La réponse reçue et de la forme suivante :

.. code-block:: javascript

  {"name":"laumio","version":"devel"}

Commande pixel par pixel
************************

Le Laumio est contrôlable vie de simples requêtes POST, précisant à la fois les clefs ``led`` et
``rgb`` :

.. code-block:: javascript

  {
    'led': PixelID,
    'rgb': [R, G, B]
  }

Remplissage intégral
********************

Pour commande toutes les LEDs du Laumio en une seule fois, il suffit de passer le paramètre ``led`` à la valeur 255 dans la bribe de JSON précédente:

.. code-block:: javascript

  {
    'led': 255,
    'rgb': [R, G, B]
  }
