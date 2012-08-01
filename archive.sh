#!/bin/sh
NAME=protimer_1.0
git archive --format=tar --prefix=$NAME/ HEAD | gzip >$NAME.tar.gz
