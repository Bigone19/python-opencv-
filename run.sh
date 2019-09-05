#! /bin/bash

g++ `pkg-config opencv --cflags` -o LineTracker LineTracker.cpp `pkg-config opencv --libs`

./LineTracker

