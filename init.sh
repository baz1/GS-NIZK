#!/bin/bash

wget "https://github.com/miracl/MIRACL/archive/master.zip"
mkdir miracl
unzip -j -aa -L master.zip -d miracl
rm master.zip
cp linux64_cpp.sh miracl/
cd miracl/
bash linux64_cpp.sh
echo "Compiling BLS test with BN pairings"
g++ -m64 -O2 bls.cpp bn_pair.cpp zzn12a.cpp ecn2.cpp zzn4.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.a -o bls
cd ..
echo "Done."

