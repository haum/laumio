Anatomie du Laumio's et and commandes en valeurs hexadécimales
==============================================================

Le laumio peut être commandé via une API UDP minimale utilisant des commandes
simples en hexadécimal envoyées sur le **port 6969**, via une interface REST ou
à travers MQTT.

Quatre types de sous-ensembles sont utilisables pour commander les LEDs du
Laumio : le pixel (une seule led), l'anneau, la colonne et le Laumio dans son
intégralité. Il est aussi possible de lancer quelques animations plus spécifiques.

Les LEDs sont disposées en 4 branches de trois LEDs, en plus d'une à son sommet.
Celles-ci sont toutes numérotées de 0 à 12 depuis le bas d'une des branches vers
sa cîme, évitant l'unique led du sommet, descendant ensuite de l'autre côté de
la structure, et enfin remontant du bas d'une des deux branches restantes pour
atteindre le bas de la dernière (la LED de sommet étant cette fois-ci reliée
entre les deux branches).

Les anneaux correspondant quant à eux à une ligne horizontale de LEDs. Il y en a
trois numérotées de 0  à 2, de bas en haut.

Enfin, dans le cas des colonnes, celles-ci se rapportent aux quatre branches de
LEDs, bien entendu sans la LED du sommet.

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


API REST
--------

L'API REST peut être appelée en envoyant des données sur le chemin ``/api/``.

Statut
******

Le statut du Laumio peut être obtenu en utilisant un endpoint de l'``/api/``::

  GET http://<laumio_ip>/api/

La réponse reçue et de la forme suivante :

.. code-block:: javascript

  {"hostname":"laumio","version":"devel"}

Commandes
*********

Le Laumio peut être controllé à travers un simple requête POST avec une donnée
JSON jointe. Voir l'API JSON pour les détails du contenu de ces JSON.

En cas de succès :

.. code-block:: javascript

  {"hostname":"laumio","status":"Success"}

En cas d'erreur :

.. code-block:: javascript

  {"hostname":"laumio","status":"Invalid Request","massage":"Unable to parse JSON"}


API MQTT
--------

Si le Laumio a pu se connecter au broker, il peut être contrôlé à travers MQTT.

Status de connection
********************

A la connecion le laumio publie sur le topic ``laumio/<Laumio_Name>/status`` son status de connecion,
Si le laumio se trouve hors ligne ce topic publiera un status offline

Annonce
*******

À la connecion, il publie son nom sur le topic ``laumio/status/advertise``.

Ce même message est envoyé quand il reçoit une commande ``discover``.

Commandes
*********

La commande est choisie en fonction du topic : ``laumio/all/<cmd>`` ou ``laumio/<name>/<cmd>``
selon que vous voulez l'envoyer à tous les clients connectés ou à un en particulier.

set_pixel
~~~~~~~~~

Change la couleur d'une led.

Les 4 octets du message sont le numéro de la led suivi des composantes rouge, vert, bleu de la couleur (0 à 255)

set_ring
~~~~~~~~~

Change la couleur d'un anneau.

Les 4 octets du message sont le numéro de l'anneau suivi des composantes rouge, vert, bleu de la couleur (0 à 255)

set_column
~~~~~~~~~~

Change la couleur d'une colonne.

Les 4 octets du message sont le numéro de la colonne suivi des composantes rouge, vert, bleu de la couleur (0 à 255)

color_wipe
~~~~~~~~~~

Démarre l'animation de remplissage progressif avec une couleur et une durée.

Les 4 octets du message sont les composantes rouge, vert, bleu de la couleur (0 à 255) suivies de la durée.

animate_rainbow
~~~~~~~~~~~~~~~

Démarre l'animation arc-en-ciel.

Le contenu du message est ignoré.

fill
~~~~

Change la couleur de toutes les leds.

Les 3 octets du message sont les composantes rouge, vert, bleu de la couleur (0 à 255)

json
~~~~

Envoie des commandes JSON via l'API JSON.

discover
~~~~~~~~

Renvoie un message sur le topic ``laumio/status/advertise`` contenant son nom.


API JSON
--------

Cette API ne peut pas être utilisée seule. Elle est accédée soit par la commande
``json`` de l'interface MQTT ou par l'API REST.

Commandes
*********

set_pixel
~~~~~~~~~

Change la couleur d'une led.

.. code-block:: javascript

  {
    'command': 'set_pixel',
    'led': PixelID,
    'rgb': [R, G, B]
  }

set_ring
~~~~~~~~~

Change la couleur d'un anneau.

.. code-block:: javascript

  {
    'command': 'set_ring',
    'ring': RingID,
    'rgb': [R, G, B]
  }

set_column
~~~~~~~~~~

Change la couleur d'une colonne.

.. code-block:: javascript

  {
    'command': 'set_column',
    'column': ColumnID,
    'rgb': [R, G, B]
  }

color_wipe
~~~~~~~~~~

Démarre l'animation de remplissage progressif avec une couleur et une durée.

.. code-block:: javascript

  {
    'command': 'color_wipe',
    'duration': Duration,
    'rgb': [R, G, B]
  }

animate_rainbow
~~~~~~~~~~~~~~~

Démarre l'animation arc-en-ciel.

.. code-block:: javascript

  {
    'command': 'animate_rainbow',
  }

fill
~~~~

Change la couleur de toutes les leds.

.. code-block:: javascript

  {
    'command': 'fill',
    'rgb': [R, G, B]
  }

Commandes multiples
*******************

Quelques commandes peuvent être chaînées dans un même appel lorsqu'elles
sont regroupées dans un tableau nommé ``commands``, mais notez que la
taille du JSON est quelque peu limitée.

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


Commande pixel par pixel (ancienne API)
***************************************

.. code-block:: javascript

  {
    'led': PixelID,
    'rgb': [R, G, B]
  }

Remplissage intégral (ancienne API)
***********************************

.. code-block:: javascript

  {
    'led': 255,
    'rgb': [R, G, B]
  }

