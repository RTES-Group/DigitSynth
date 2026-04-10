#!/usr/bin/env bash

PACKAGENAME=digitsynth-aarch64

cd build/
mkdir -p $PACKAGENAME/DEBIAN/
mkdir -p $PACKAGENAME/usr/bin/ 
cp control.in $PACKAGENAME/DEBIAN/control
cp digitsynth $PACKAGENAME/usr/bin/
dpkg-deb --root-owner-group --build $PACKAGENAME

