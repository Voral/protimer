#!/bin/sh
cd src
qmake
make clean
make 

lupdate -verbose protimer.pro
lrelease protimer.pro

cd ..

mkdir -p bin64/lang
cp src/lang/*.qm bin64/lang/

echo Strip
strip bin64/protimer
