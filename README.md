# Blieng

Game engine/backend library for C++

Aim is to create helper library instead of
full blown game engine for C++ based games.

## Features

 - Cross platform: Windows, Linux, Android.
 - Modern: C++11 and Boost libraries.
 - General base object with built-in (unoptimal) key-value map
 - Support for "Any" objects and conversions.
 - JSON as main data, definition and configuration file format
 - Items with producing, consumption and aging support. Base items defined as JSON
 - Points (X, Y) with conversion to geo (longitude, latitude) support
 - Paths, collections of Points. Area calculation with crossing lines and complex shapes support.
 - Basic character (player, enemy, etc.) with bag (items/inventory)
 - Maps with paths, points and image. Stored as JSON.
 - Town (or points of interest), with Character and Items bags. Can be located to Map.
 - Complex numbers (partial support)
 - Random number and UUID support
 - Serialize and deserialize to/from data file
 - Configuration files
 - General JSON data backend to store objects and their key-value mappings
 - Data file archive with obfuscation support
 - Compression (FastLZ)
 - Encryption (AES)
 - General event log
 - Object log, logging events on object
 - Wall clock to control time in universe. Experimental item producing and consuming support.


## Dependencies

 - Boost
 - libcppunit
 - googlemock
 - googletest


## Building

You need [meson](http://mesonbuild.com/) and [ninja](https://ninja-build.org/).

    mkdir build
    cd build
    meson.py ..
    ninja

You may also use build.sh script on top level:

    ./build.sh "$PWD"


## License

Copyright(c) 2013-2017 Jouni Roivas <jroivas@iki.fi>

MIT, see [LICENSE](LICENSE)


## 3rd party components

All third party components are verified to have compatible license.

Some components are modified when included into this engine.
These modifications inherit original license.

### auto_vector.h
  - Any use, commercial or noncommercial of this code
    is hereby granted, under the condition
    that this copyright notice be not removed.
  - Reliable Software (c) 2003, http://www.relisoft.com
  - http://relisoft.com/resource/source/autovector.zip

### Optimised ANSI C code for the Rijndael cipher
  - Public Domain / BSD alike
  - Original authors:
    * Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
    * Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
    * Paulo Barreto <paulo.barreto@terra.com.br>
  - http://www.cs.ucdavis.edu/~rogaway/ocb/ocb-ref/

### json.h/json.c
  - BSD alike
  - Copyright (C) 2012, 2013 James McLaughlin et al.
  - https://github.com/udp/json-parser

### fastlz.h/fastlz.c
  - MIT
  - Original copyrights:
    * Copyright (C) 2007 Ariya Hidayat (ariya@kde.org)
    * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
    * Copyright (C) 2005 Ariya Hidayat (ariya@kde.org)
  - http://fastlz.org
