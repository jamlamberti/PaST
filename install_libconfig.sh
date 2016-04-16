#!/bin/bash
#sudo apt-get install automake

#git clone https://github.com/hyperrealm/libconfig.git
#cd libconfig && ./configure && make

wget http://mirrors.kernel.org/ubuntu/pool/main/libc/libconfig/libconfig++9_1.4.9-2_amd64.deb
sudo dpkg --install libconfig++9_1.4.9-2_amd64.deb
