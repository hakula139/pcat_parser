#!/bin/sh

# Install bison 3.8.2

cd lib
tar -xvzf bison-3.8.2.tar.gz
cd bison-3.8.2
./configure && make && sudo make install
cd ../..
