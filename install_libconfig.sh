#!/bin/bash
sudo apt-get install automake

git clone https://github.com/hyperrealm/libconfig.git
cd libconfig && ./configure && make
