#include <iostream>

#define MR_PAIRING_BN // AES-128 or AES-192 security

#ifndef AES_SECURITY
#define AES_SECURITY 128
//#define AES_SECURITY 192
#endif /* Not AES_SECURITY */

#include "pairing_3.h"

using namespace std;

int main2() {
    PFC pfc(AES_SECURITY);
    G2 t1, t2, t3;
    pfc.random(t1);
    t2 = t1 + t1;
    //t2.g.norm();
    t3 = t1 + t2;
    return 0;
}
