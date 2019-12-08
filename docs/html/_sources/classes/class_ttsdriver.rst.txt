:github_url: hide

.. _class_TTSDriver:

TTSDriver
=========

**Category:** Core

Brief Description
-----------------

Native TTS driver.


Properties
----------

+--------+---------------------------------------------------------------+-----+
| String | :ref:`voice<class_TTSDriver_property_voice>`                  | ""  |
+--------+---------------------------------------------------------------+-----+
| int    | :ref:`volume<class_TTSDriver_property_volume>`                | 100 |
+--------+---------------------------------------------------------------+-----+
| int    | :ref:`rate<class_TTSDriver_property_rate>`                    | 0   |
+--------+---------------------------------------------------------------+-----+


Methods
-------

.. table::
   :class: longtable
   :widths: 7 20

   +-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | void  | :ref:`speak<class_TTSDriver_method_speak>` **(** String text, bool interrupt **)**                                                                            |
   +-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | void  | :ref:`stop<class_TTSDriver_method_stop>` **(** **)**                                                                                                          |
   +-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | bool  | :ref:`is_speaking<class_TTSDriver_method_is_speaking>` **(** **)** const                                                                                      |
   +-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | Array | :ref:`get_voices<class_TTSDriver_method_get_voices>` **(** **)** const                                                                                        |
   +-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+

Property Descriptions
---------------------

.. _class_TTSDriver_property_voice:

- String **voice**

.. table::
   :class: longtable
   :widths: 7 20

   +-----------+----------------------+
   | *Default* | ""                   |
   +-----------+----------------------+
   | *Setter*  | set_voice(value)     |
   +-----------+----------------------+
   | *Getter*  | get_voice()          |
   +-----------+----------------------+

Speech synthesizer’s current voice identifier. Not set by defualt. Should be one of the ids returend by :ref:`get_voices<class_TTSDriver_method_get_voices>`.

----

.. _class_TTSDriver_property_volume:

- int **volume**

.. table::
   :class: longtable
   :widths: 7 20

   +-----------+----------------------+
   | *Default* | ""                   |
   +-----------+----------------------+
   | *Setter*  | set_volume(value)    |
   +-----------+----------------------+
   | *Getter*  | get_volume()         |
   +-----------+----------------------+

Speech synthesizer’s volume. The granularity of the volume is engine dependent. Values may be truncated. Ranges from 0 to 100.

----

.. _class_TTSDriver_property_rate:

- int **rate**

.. table::
   :class: longtable
   :widths: 7 20

   +-----------+----------------------+
   | *Default* | ""                   |
   +-----------+----------------------+
   | *Setter*  | set_rate(value)      |
   +-----------+----------------------+
   | *Getter*  | get_rate()           |
   +-----------+----------------------+

Speech synthesizer’s rendering rate adjustment. The granularity of the rate is engine dependent. Values may be truncated. Ranges from -100 to 100.

----

Method Descriptions
-------------------

.. _class_TTSDriver_method_speak:

- void **speak** **(** String text, bool interrupt **)**

Begins speaking synthesized text or adds text to the query. If ``interrupt`` is true, stops current synthesis in progress and clears query.

----

.. _class_TTSDriver_method_stop:

- void **stop** **(** **)**

Stops synthesis in progress and clears query.

----

.. _class_TTSDriver_method_is_speaking:

- bool **is_speaking** **(** **)** const

Returns ``true`` if the synthesizer is generating speech, ``false`` otherwise.

----

.. _class_TTSDriver_method_get_voices:

- Array **get_voices** **(** **)** const

Returns an ``Array`` of voice information ``Dictionaries``.

.. table::
   :class: longtable
   :widths: 7 20

   +----------+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | *String* | *name*     | voice identifier                                                                                                                                                                                        |
   +----------+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | *String* | *language* | ``lang_variant`` value. Where `lang`` part is 2 or 3-letter code based on the ISO-639 standard, in lowercase and ``variant`` part is engine dependent string describing country, region or/and dialect. |
   +----------+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
