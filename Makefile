SHELL=/bin/sh

all: miracllib pbclib mainlib doc

miracllib: miracl/miracl.a

miracl/miracl.a: miracl/miracl.h
	cd miracl/; bash linux64_cpp
	echo "Test: Compiling BLS test with BN pairings"
	cd miracl/; g++ -m64 -O2 bls.cpp bn_pair.cpp zzn12a.cpp ecn2.cpp zzn4.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.a -o bls

miracl/miracl.h: install_files/miracl.zip
	unzip -j -aa -L "install_files/miracl.zip" -d miracl
	touch miracl/miracl.h

install_files/miracl.zip:
	mkdir -p install_files
	wget "https://github.com/miracl/MIRACL/archive/master.zip" -O install_files/miracl.zip

pbclib: pbc/pbc-master/libpbc.a

pbc/pbc-master/libpbc.a: pbc/pbc-master/setup
	cd pbc/pbc-master/; ./setup && ./configure && make && ar rc libpbc.a libpbc_*.o
	cd pbc/pbc-master/; cp libpbc.a include/ # (sorry for the mess)

pbc/pbc-master/setup: install_files/pbc.zip
	unzip -a "install_files/pbc.zip" -d pbc
	touch pbc/pbc-master/setup

install_files/pbc.zip:
	mkdir -p install_files
	wget "https://github.com/blynn/pbc/archive/master.zip" -O install_files/pbc.zip

mainlib: lib/gsnizk/bigendian_cfg_gen lib/gsnizk/bigendian_cfg.h

lib/gsnizk/bigendian_cfg_gen: lib/gsnizk/bigendian_cfg_gen.cpp
	cd lib/gsnizk/; g++ -m64 -std=c++11 -o bigendian_cfg_gen bigendian_cfg_gen.cpp

lib/gsnizk/bigendian_cfg.h: lib/gsnizk/bigendian_cfg_gen
	cd lib/gsnizk/; ./bigendian_cfg_gen

doc: lib/gsnizk/*.h lib/gsnizk/*.dox lib/gsnizk/Doxyfile
	cd lib/gsnizk/; doxygen Doxyfile

