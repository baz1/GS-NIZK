SHELL=/bin/sh

all: miracllib doc

miracl/:
	mkdir miracl

miracllib: miracl/miracl.a

miracl/miracl.a: miracl/miracl.h
	cp "install_files/linux64_cpp.sh" miracl/
	cd miracl/; bash linux64_cpp.sh
	echo "Test: Compiling BLS test with BN pairings"
	cd miracl/; g++ -m64 -O2 bls.cpp bn_pair.cpp zzn12a.cpp ecn2.cpp zzn4.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.a -o bls

miracl/miracl.h: install_files/master.zip
	unzip -j -aa -L "install_files/master.zip" -d miracl
	#Just to make sure it is not rerun:
	sleep 1
	touch miracl/miracl.h

install_files/master.zip:
	wget "https://github.com/miracl/MIRACL/archive/master.zip" -O install_files/master.zip

doc:

