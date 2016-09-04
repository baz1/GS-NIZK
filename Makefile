SHELL=/bin/sh


all: miracllib pbclib mainlibprepare doc

miracl-build: miracllib mainlibprepare

pbc-build: pbclib mainlibprepare


miracllib: miracl/miracl.a miracl/mr_bn.a

miracl/miracl.a: miracl/miracl.h
	cd miracl/; bash linux64_cpp

miracl/miracl.h: install_files/miracl.zip
	unzip -j -aa -L "install_files/miracl.zip" -d miracl
	touch miracl/miracl.h

install_files/miracl.zip:
	mkdir -p install_files
	wget "https://github.com/miracl/MIRACL/archive/master.zip" -O install_files/miracl.zip

miracl/mr_bn.a: miracl/miracl.a
	cd miracl/; g++ -c -m64 -O2 bn_pair.cpp zzn12a.cpp ecn2.cpp zzn4.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp
	cd miracl/; ar rc mr_bn.a bn_pair.o zzn12a.o ecn2.o zzn4.o zzn2.o big.o zzn.o ecn.o
	echo "Test: Compiling BLS test with BN pairings"
	cd miracl/; g++ -m64 -O2 bls.cpp mr_bn.a miracl.a -o bls

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

mainlibprepare: lib/gsnizk/bigendian_cfg_gen lib/gsnizk/bigendian_cfg.h

lib/gsnizk/bigendian_cfg_gen: lib/gsnizk/bigendian_cfg_gen.cpp
	cd lib/gsnizk/; g++ -m64 -std=c++11 -o bigendian_cfg_gen bigendian_cfg_gen.cpp

lib/gsnizk/bigendian_cfg.h: lib/gsnizk/bigendian_cfg_gen
	cd lib/gsnizk/; ./bigendian_cfg_gen

doc: lib/gsnizk/*.h lib/gsnizk/*.dox lib/gsnizk/Doxyfile
	cd lib/gsnizk/; doxygen Doxyfile

