!/bin/bash
#sudo apt-get install automake autoconf

#git clone https://github.com/hyperrealm/libconfig.git
#cd libconfig && ./configure && touch configure.ac aclocal.m4 configure Makefile.am Makefile.in && make && sudo make install

wget http://mirrors.kernel.org/ubuntu/pool/main/libc/libconfig/libconfig++-dev_1.4.9-2_amd64.deb
sudo dpkg --install libconfig++9_1.4.9-2_amd64.deb
