#!/bin/bash

if [ ! -f "miracl/miracl.h" ]; then
    if [ ! -f "master.zip" ]; then
        echo "Downloading the MIRACL library..."
        wget "https://github.com/miracl/MIRACL/archive/master.zip"
    fi
    mkdir miracl
    unzip -j -aa -L master.zip -d miracl
    rm master.zip
fi
if [ ! -f miracl/miracl.a ]; then
    cp linux64_cpp.sh miracl/
    cp bn_pair.cpp miracl/
    cd miracl/
    echo "Compiling the MIRACL library..."
    bash linux64_cpp.sh
    echo "Test: Compiling BLS test with BN pairings"
    g++ -m64 -O2 bls.cpp bn_pair.cpp zzn12a.cpp ecn2.cpp zzn4.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.a -o bls
    cd ..
fi
echo "End of the script."

