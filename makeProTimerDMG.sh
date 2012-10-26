#!/bin/sh
cp forpackages/mac/protimerbg.png binmac/
cd binmac
../makeDMG.sh -V -b protimerbg.png -i ../src/res/coelho/protimer.icns -s "800:500" -c 535:345:253:345 "Protimer.app"

cd ..
rm binmac/protimerbg.png