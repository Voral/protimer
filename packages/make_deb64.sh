#!/bin/sh
#gzip -9
VERSION="1.0"
VERSIONPKG="1"
PLATFORM="amd64"
ARCH=64
PKGNAME=protimer

SRCDIR=../bin$ARCH
DEBNAME=${PKGNAME}_${VERSION}-${VERSIONPKG}_$PLATFORM.deb

SIZE=`du $PKGNAME --exclude=DEBIAN -s | sed -rn "s/^([0-9]+).*/\1/gp"`

rm $PKGNAME/usr/bin/*
cp $SRCDIR/* $PKGNAME/usr/bin/
echo "Version: $VERSION" > $PKGNAME/DEBIAN/control
echo "Architecture: $PLATFORM" >> $PKGNAME/DEBIAN/control
echo "Installed-Size: $SIZE" >> $PKGNAME/DEBIAN/control
cat control >> $PKGNAME/DEBIAN/control
cd $PKGNAME
md5deep -r usr -l > DEBIAN/md5sums
cd ..
fakeroot dpkg-deb --build $PKGNAME
mv $PKGNAME.deb $DEBNAME
lintian $DEBNAME
alien --to-rpm --scripts ./$DEBNAME
alien --to-tgz --scripts ./$DEBNAME
gzip -c -9 install/* >> $PKGNAME-$VERSION.tgz
mv $PKGNAME-$VERSION.tgz $PKGNAME-$VERSION-$PLATFORM-$VERSIONPKG.tgz
