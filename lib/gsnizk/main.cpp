#include <iostream>

#include "pairings.h"

using namespace std;
using namespace pairings;

#define ASSERT(X) if (!(X)) cerr << "Error: Assert of " << #X << " at line " << __LINE__ << " failed!" << endl

int main() {
    int len;
    char data[256];
    initialize_pairings(0, 0);

    /* -------------------- Tests for Fp -------------------- */
    Fp v1, v2(0), v3(42), v4(1764);
    ASSERT(v1 == v2);
    v3 *= v3;
    ASSERT(v3 == v4);
    v2 += v3;
    ASSERT(v2 == v4);
    v2 -= v3;
    ASSERT(v1 == v2);
    v1 = Fp::getRand();
    ASSERT(v1 != v2); // Note: Just highly unlikely if the randomness is fine
    len = v1.getDataLen();
    cout << "Len for random Fp: " << len << endl;
    v1.getData(data);
    v2 = Fp::getValue(data, len);
    ASSERT(v1 == v2);
    v3 = (v1 / Fp(42)) * Fp(42);
    ASSERT(v1 == v3);

    /* -------------------- Tests for G1 -------------------- */
    G1 g1 = G1::getRand(), g2, g3, g4;
    ASSERT(g1 != g2); // Note: Just highly unlikely if the randomness is fine
    len = g1.getDataLen();
    cout << "Len for random G1: " << len << endl;
    g1.getData(data);
    g2 = G1::getValue(data, len);
    ASSERT(g1 == g2);
    len = g1.getDataLen(true);
    cout << "Len for random G1 compressed: " << len << endl;
    g1.getData(data, true);
    g3 = G1::getValue(data, len, true);
    ASSERT(g1 == g3);
    g3 += g3;
    ASSERT((Fp(3) * g1) == (g1 + g3));
    ASSERT((-g1) == (Fp(-1) * g1));
    ASSERT((g1 - g1).isNull());
    terminate_pairings();
    return 0;
}
