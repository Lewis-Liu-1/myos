#!/bin/bash

find . -maxdepth 4 -name '*.bin' -delete
find . -maxdepth 4 -name '*.img' -delete
find . -maxdepth 4 -name '*.sys' -delete
find . -maxdepth 4 -name '*.o' -delete
find . -maxdepth 4 -name '*.out' -delete
find . -maxdepth 4 -name '*.iso' -delete
find . -maxdepth 4 -name '*.d' -delete
find . -maxdepth 5 -name 'Makefile' -exec make clean \;
	
