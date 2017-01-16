# Blieng(tm)

Game engine/backend library.

Copyright(c) 2013-2017 Jouni Roivas <jroivas@iki.fi>


## Dependencies

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

MIT, see LICENSE


## 3rd party components

auto_vector.h
  Any use, commercial or noncommercial of this code
  is hereby granted, under the condition
  that this copyright notice be not removed.

  Reliable Software (c) 2003, http://www.relisoft.com
  http://relisoft.com/resource/source/autovector.zip

Optimised ANSI C code for the Rijndael cipher
  Public Domain / BSD alike

  Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
  Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
  Paulo Barreto <paulo.barreto@terra.com.br>

json.h/json.c
  BSD alike

  Copyright (C) 2012, 2013 James McLaughlin et al.
  https://github.com/udp/json-parser

fastlz.h/fastlz.c
   MIT

   Copyright (C) 2007 Ariya Hidayat (ariya@kde.org)
   Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
   Copyright (C) 2005 Ariya Hidayat (ariya@kde.org)
   http://fastlz.org
