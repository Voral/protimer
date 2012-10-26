#!/bin/sh

APP=binmac/Protimer.app

echo Compiling
cd src
qmake -spec macx-g++ protimer.pro
if [ $? -ne 0 ];then
    exit;
fi
#make clean
make
if [ $? -ne 0 ];then
    exit;
fi

lupdate -verbose protimer.pro
lrelease protimer.pro

cd ..

echo Copying Qt frameworks
macdeployqt ${APP} -no-plugins

echo Refining application bundle
cp deploy/Info.plist ${APP}/Contents
cp src/res/coelho/protimer.icns ${APP}/Contents/Resources
mkdir -p ${APP}/Contents/MacOS/lang
cp src/lang/*.qm ${APP}/Contents/MacOS/lang/

echo Strip
strip ${APP}/Contents/MacOS/protimer


echo "Done"
