#include <iostream>
#include <fstream>
#include <cstdlib>

#include "gsnizk.h"

using namespace std;
using namespace gsnizk;

static int n_err = 0;
#define ASSERT(X) if (!(X)) (cerr << "Error: Assert of " << #X << " at line " \
    << __LINE__ << " failed!" << endl, ++n_err)

#define TRANSFER_TESTS 10
#define PAIRING_TESTS 10
#define PAIRING_COUNT_MAX 10

#define DATA_SIZE 512

bool checkDataSize(int size, int line) {
    if (size > DATA_SIZE) {
        cerr << "Error: Please increase DATA_SIZE for the tests (size " << size
             << " required at line " << line << ")." << endl;
        pairings::terminate_pairings();
        return false;
    }
    return true;
}

#define CHECK_DATA_SIZE(s) if (!checkDataSize((s), __LINE__)) return

void testPairings() {
    cout << "########## PAIRING TESTS ##########" << endl;
    int len;
    char data[DATA_SIZE], *data2;

    /* -------------------- Hash prerequisite -------------------- */
    char *hash = new char[pairings::getHashLen()];
    pairings::getHash("hello", 5, hash);

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
    CHECK_DATA_SIZE(len);
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
    ASSERT(Fp::fromHash("hello", 5) != Fp::fromHash("hi", 2));
    ASSERT(Fp::fromHash("hello", 5) == Fp::fromHash(hash));

    /* -------------------- Tests for G1 -------------------- */
    G1 g1 = G1::getRand(), g2, g3, g4;
    ASSERT(g1 != g2); // Note: Just highly unlikely if the randomness is fine
    len = g1.getDataLen();
    cout << "Len for random G1: " << len << endl;
    CHECK_DATA_SIZE(len);
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        g1 = G1::getRand();
        g1.getData(data);
        g2 = G1::getValue(data, len);
        ASSERT(g1 == g2);
    }
    len = g1.getDataLen(true);
    cout << "Len for random G1 compressed: " << len << endl;
    CHECK_DATA_SIZE(len);
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
    ASSERT(G1::fromHash("hello", 5) != G1::fromHash("hi", 2));
    ASSERT(G1::fromHash("hello", 5) == G1::fromHash(hash));
    g1 = G1::getRand();
    v1 = Fp::getRand();
    g2 = v1 * g1;
    g3 = Fp::getUnit() * g1;
    ASSERT(g1 == g3);
    g1.precomputeForMult();
    g1.saveMultPrecomputations(data2);
    g3.loadMultPrecomputations(data2);
    ASSERT(g1 == g3);
    ASSERT(g2 == (v1 * g1));
    ASSERT(g2 == (v1 * g3));

    /* -------------------- Tests for G2 -------------------- */
    G2 h1 = G2::getRand(), h2, h3, h4;
    ASSERT(h1 != h2); // Note: Just highly unlikely if the randomness is fine
    len = h1.getDataLen();
    cout << "Len for random G2: " << len << endl;
    CHECK_DATA_SIZE(len);
    cout << "Testing transfers..." << endl;
    for (int i = 0; i < TRANSFER_TESTS; ++i) {
        h1 = G2::getRand();
        h1.getData(data);
        h2 = G2::getValue(data, len);
        ASSERT(h1 == h2);
    }
    len = h1.getDataLen(true);
    cout << "Len for random G2 compressed: " << len << endl;
    CHECK_DATA_SIZE(len);
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
    ASSERT(G2::fromHash("hello", 5) != G2::fromHash("hi", 2));
    ASSERT(G2::fromHash("hello", 5) == G2::fromHash(hash));
    h1 = G2::getRand();
    v1 = Fp::getRand();
    h2 = v1 * h1;
    h3 = Fp::getUnit() * h1;
    ASSERT(h1 == h3);
    h1.precomputeForMult();
    h1.saveMultPrecomputations(data2);
    h3.loadMultPrecomputations(data2);
    ASSERT(h2 == (v1 * h1));
    ASSERT(h2 == (v1 * h3));

    /* -------------------- Tests for GT -------------------- */
    GT t1 = GT::getRand(), t2, t3, t4;
    ASSERT(t1 != t2); // Note: Just highly unlikely if the randomness is fine
    t2 = GT::getRand();
    ASSERT(t1 != t2); // Note: Just highly unlikely if the randomness is fine
    len = t1.getDataLen();
    cout << "Len for random GT: " << len << endl;
    CHECK_DATA_SIZE(len);
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
    ASSERT((t1 / t1).isUnit());
    t1 = GT::getRand();
    v1 = Fp::getRand();
    t2 = t1 ^ v1;
    t3 = t1 ^ Fp::getUnit();
    ASSERT(t1 == t3);
    t1.precomputeForPower();
    t1.savePowerPrecomputations(data2);
    t3.loadPowerPrecomputations(data2);
    ASSERT(t2 == (t1 ^ v1));
    ASSERT(t2 == (t3 ^ v1));

    /* -------------------- Pairing tests -------------------- */
    g1 = G1::getRand();
    h1 = G2::getRand();
    cout << "Testing simple pairings..." << endl;
    for (int i = 0; i < PAIRING_TESTS; ++i) {
        v1 = Fp::getRand();
        v2 = Fp::getRand();
        ASSERT(GT::pairing(v1 * g1, v2 * h1) ==
               (GT::pairing(g1, h1) ^ (v1 * v2)));
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
    ASSERT((GT::pairing(g1, h1) * GT::pairing(-g1, h1)).isUnit());
    ASSERT((GT::pairing(g1, h1) * GT::pairing(g1, -h1)).isUnit());
    g1 = G1::getRand();
    h1 = G2::getRand();
    t1 = GT::pairing(g1, h1);
    h3 = Fp::getUnit() * h1;
    ASSERT(h1 == h3);
    h1.precomputeForPairing();
    h1.savePairingPrecomputations(data2);
    h3.loadPairingPrecomputations(data2);
    ASSERT(t1 == GT::pairing(g1, h1));
    ASSERT(t1 == GT::pairing(g1, h3));

    /* -------------------- iostream tests -------------------- */
    cout << "Testing iostream serialization..." << endl;
    v1 = Fp::getRand();
    v2 = Fp();
    g1 = G1::getRand();
    g2.clear();
    h1 = G2::getRand();
    h2.clear();
    t1 = GT::getRand();
    t2.clear();
    {
        ofstream out("test1.test");
        out << v1 << v2;
        out << g1 << g2;
        out << h1 << h2;
        out << t1 << t2;
        out.close();
    }
    {
        ifstream in("test1.test");
        in >> v3 >> v4;
        in >> g3 >> g4;
        in >> h3 >> h4;
        in >> t3 >> t4;
        in.close();
    }
    ASSERT(v1 == v3);
    ASSERT(v2 == v4);
    ASSERT(g1 == g3);
    ASSERT(g2 == g4);
    ASSERT(h1 == h3);
    ASSERT(h2 == h4);
    ASSERT(t1 == t3);
    ASSERT(t2 == t4);
}

void testProof(NIZKProof &proof, ProofData &d, const CRS &crs, CRS *verif = 0) {
    ASSERT(proof.verifySolution(d));
    {
        cout << "* Creating and writing proof..." << endl;
        ofstream out("test2.test");
        proof.writeProof(out, crs, d);
        out.close();
    }
    d.privFp.clear();
    d.privG1.clear();
    d.privG2.clear();
    {
        cout << "* Reading and checking proof..." << endl;
        ifstream in("test2.test");
        if (verif) {
            ASSERT(proof.checkProof(in, *verif, d));
        } else {
            ASSERT(proof.checkProof(in, crs, d));
        }
        in.close();
    }
    if (!(proof.isZeroKnowledge() && crs.isSimulationReady()))
        return;
    {
        cout << "* Creating and writing simulated proof..." << endl;
        ofstream out("test3.test");
        proof.simulateProof(out, crs, d);
        out.close();
    }
    {
        cout << "* Reading and checking simulated proof..." << endl;
        ifstream in("test3.test");
        ASSERT(proof.checkProof(in, crs, d));
        in.close();
    }
}

void testProofs() {
    cout << "########## PROOF TESTS ##########" << endl;
    {
        cout << "Instantiation 1: discrete log in G1" << endl;
        cout << "* Creating the equation system..." << endl;

        G1 a = G1::getRand();
        Fp k = Fp::getRand();
        G1 b = k * a;

        CRS crs(false);
        NIZKProof proof;
        proof.addEquation(FpVar(0) * G1Const(0), FpUnit() * G1Const(1));
        ASSERT(proof.endEquations());

        ProofData d;
        d.privFp.push_back(k);
        d.pubG1.push_back(a);
        d.pubG1.push_back(b);

        testProof(proof, d, crs);
    }
    {
        cout << "Instantiation 2: discrete log in G1 with private CRS" << endl;
        cout << "* Creating the equation system..." << endl;

        G1 a = G1::getRand();
        Fp k = Fp::getRand();
        G1 b = k * a;

        CRS crsref(true), crspriv, crspub;
        crsref.makePublic();
        {
            ofstream out("tmp.test");
            crspriv = crsref.genPrivate(out);
            out.close();
        }
        crspub = crspriv;
        crspub.makePublic();

        NIZKProof proof, proofcp;
        proof.addEquation(FpVar(0) * G1Const(a), FpUnit() * G1Const(b));
        ASSERT(proof.endEquations());

        ProofData d;
        d.privFp.push_back(k);

        cout << "* Writing and reading back the equation system..." << endl;
        {
            ofstream out("test4.test");
            out << proof;
            out.close();
        }
        {
            ifstream in("test4.test");
            in >> proofcp;
            in.close();
        }

        testProof(proofcp, d, crspriv, &crspub);
    }
}

int main() {
    pairings::initialize_pairings(0, 0);

    //testPairings();
    testProofs();

    pairings::terminate_pairings();
    if (n_err) {
        cout << "Done; " << n_err << " error(s) have occured!" << endl;
    } else {
        cout << "Done; no errors have occured." << endl;
    }
    return 0;
}
