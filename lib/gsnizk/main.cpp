#include <iostream>
#include <cstdlib>

#include "pairings.h"

using namespace std;
using namespace pairings;

#define ASSERT(X) if (!(X)) cerr << "Error: Assert of " << #X << " at line " << __LINE__ << " failed!" << endl

#define TRANSFER_TESTS 100
#define PAIRING_TESTS 100
#define PAIRING_COUNT_MAX 10

int main() {
    int len;
    char data[512];
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
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        v1 = Fp::getRand();
        v1.getData(data);
        v2 = Fp::getValue(data, len);
        ASSERT(v1 == v2);
    }
    v1 = Fp::getRand();
    v3 = (v1 / Fp(42)) * Fp(42);
    ASSERT(v1 == v3);

    /* -------------------- Tests for G1 -------------------- */
    G1 g1 = G1::getRand(), g2, g3, g4;
    ASSERT(g1 != g2); // Note: Just highly unlikely if the randomness is fine
    len = g1.getDataLen();
    cout << "Len for random G1: " << len << endl;
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        g1 = G1::getRand();
        g1.getData(data);
        g2 = G1::getValue(data, len);
        ASSERT(g1 == g2);
    }
    len = g1.getDataLen(true);
    cout << "Len for random G1 compressed: " << len << endl;
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        g1 = G1::getRand();
        g1.getData(data, true);
        g3 = G1::getValue(data, len, true);
        ASSERT(g1 == g3);
    }
    g1 = G1::getRand();
    g3 = g1;
    g3 += g3;
    ASSERT((Fp(3) * g1) == (g1 + g3));
    ASSERT((-g1) == (Fp(-1) * g1));
    ASSERT((g1 - g1).isNull());

    /* -------------------- Tests for G2 -------------------- */
    G2 h1 = G2::getRand(), h2, h3, h4;
    ASSERT(h1 != h2); // Note: Just highly unlikely if the randomness is fine
    len = h1.getDataLen();
    cout << "Len for random G2: " << len << endl;
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        h1 = G2::getRand();
        h1.getData(data);
        h2 = G2::getValue(data, len);
        ASSERT(h1 == h2);
    }
    len = h1.getDataLen(true);
    cout << "Len for random G2 compressed: " << len << endl;
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        h1 = G2::getRand();
        h1.getData(data, true);
        h2 = G2::getValue(data, len, true);
        ASSERT(h1 == h2);
    }
    h1 = G2::getRand();
    h3 = h1;
    h3 += h3;
    ASSERT((Fp(3) * h1) == (h1 + h3));
    ASSERT((-h1) == (Fp(-1) * h1));
    ASSERT((h1 - h1).isNull());

    /* -------------------- Tests for GT -------------------- */
    GT t1 = GT::getRand(), t2, t3, t4;
    ASSERT(t1 != t2); // Note: Just highly unlikely if the randomness is fine
    t2 = GT::getRand();
    ASSERT(t1 != t2); // Note: Just highly unlikely if the randomness is fine
    len = t1.getDataLen();
    cout << "Len for random GT: " << len << endl;
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        t1 = GT::getRand();
        t1.getData(data);
        t2 = GT::getValue(data, len);
        ASSERT(t1 == t2);
    }
    t1 = GT::getRand();
    t3 = t1;
    t3 *= t3;
    ASSERT((t1 ^ Fp(3)) == (t1 * t3));
    ASSERT((GT() / t1) == (t1 ^ Fp(-1)));
    ASSERT((t1 / t1).isUnity());

    /* -------------------- Pairing tests -------------------- */
    g1 = G1::getRand();
    h1 = G2::getRand();
    cout << "Testing simple pairings..." << endl;
    for (int i = 0; i < PAIRING_TESTS; ++i) {
        v1 = Fp::getRand();
        v2 = Fp::getRand();
        ASSERT(GT::pairing(v1 * g1, v2 * h1) == (GT::pairing(g1, h1) ^ (v1 * v2)));
    }
    cout << "Testing multiple pairings..." << endl;
    for (int i = 0; i < PAIRING_TESTS; ++i) {
        std::vector< std::pair<G1,G2> > pairs;
        int n = rand() % PAIRING_COUNT_MAX;
        pairs.reserve(n);
        t1 = GT();
        while (n--) {
            g1 = G1::getRand();
            h1 = G2::getRand();
            t1 *= GT::pairing(g1, h1);
            pairs.push_back(std::pair<G1,G2>(g1, h1));
        }
        ASSERT(t1 == GT::pairing(pairs));
    }

    terminate_pairings();
    cout << "Done." << endl;
    return 0;
}
