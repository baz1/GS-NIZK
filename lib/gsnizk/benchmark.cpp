/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "benchmark.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "gsnizk.h"

#define SAMPLES 2

#define HASH_SAMPLE_SIZE        1000000
#define PAIRING_SAMPLE_SIZE     500

using namespace std;
using namespace gsnizk;

static ofstream *b_out;
static clock_t timer_value;
static int current;

void update_progress(int p) {
    if (p <= current) return;
    if (current < 10) {
        cout << "\b\b" << p << "%";
        cout.flush();
    } else {
        cout << "\b\b\b" << p << "%";
        cout.flush();
    }
    current = p;
}

#define BEGIN_TASK(TASK_NAME,SAMPLE_SIZE,RANDOMIZATION_STEP) \
    *b_out << TASK_NAME << " " << SAMPLE_SIZE; \
    cout << "0%"; \
    cout.flush(); \
    current = 0; \
    for (int sample = 0; sample < SAMPLES; ++sample) { \
        update_progress((sample * 100) / SAMPLES); \
        RANDOMIZATION_STEP \
        timer_value = clock(); \
        int sample_iter = SAMPLE_SIZE; \
        do { \
            if (!(sample_iter & 0xFF)) { \
                /* Randomization step every 256 iterations */ \
                timer_value -= clock(); \
                update_progress(((SAMPLE_SIZE - sample_iter + \
                    sample * SAMPLE_SIZE) * 100) / \
                    (SAMPLES * SAMPLE_SIZE)); \
                RANDOMIZATION_STEP \
                timer_value += clock(); \
            }
#define END_TASK() \
        } while (--sample_iter); \
        timer_value = clock() - timer_value; \
        *b_out << " "; \
        *b_out << (((double) timer_value) / CLOCKS_PER_SEC); \
    } \
    *b_out << endl; \
    update_progress(100); \
    cout << endl;

void benchmarkHashes() {
    cout << "########## HASH BENCHMARK ##########" << endl;
    srand(42);
    char *hash = new char[pairings::getHashLen()], *data = new char[256];
    BEGIN_TASK("Hashes", HASH_SAMPLE_SIZE,
               for (int j = 256; j-- > 0;)
                   data[j] = static_cast<char>(rand() & 0xFF);)
    pairings::getHash(data, rand() % 257, hash);
    END_TASK()
}

void benchmarkPairings() {
    cout << "########## PAIRING BENCHMARK ##########" << endl;
    char *data;
    Fp fp_1, fp_2, fp_3;
    G1 g1_1, g1_2, g1_3;
    G2 g2_1, g2_2, g2_3;
    GT gt_1, gt_2, gt_3;
    std::vector< std::pair<G1,G2> > p4;

    /* -------------------- Benchmark for Fp -------------------- */
    cout << "Fp:" << endl << " * 1/3: ";
    BEGIN_TASK("Fp-Random-Add", 10000000,
               fp_1 = Fp::getRand(); fp_2 = Fp::getRand();)
    fp_3 = fp_1 + fp_2;
    END_TASK()
    cout << " * 2/3: ";
    BEGIN_TASK("Fp-Random-Mult", 2000000,
               fp_1 = Fp::getRand(); fp_2 = Fp::getRand();)
    fp_3 = fp_1 * fp_2;
    END_TASK()
    cout << " * 3/3: ";
    BEGIN_TASK("Fp-Random-Div", 300000,
               fp_1 = Fp::getRand(); fp_2 = Fp::getRand();)
    fp_3 = fp_1 / fp_2;
    END_TASK()

    /* -------------------- Benchmark for G1 -------------------- */
    cout << "G1:" << endl << " * 1/5: ";
    BEGIN_TASK("G1-Random-Add", 1000000,
               g1_1 = G1::getRand(); g1_2 = G1::getRand();)
    g1_3 = g1_1 + g1_2;
    END_TASK()
    cout << " * 2/5: ";
    BEGIN_TASK("G1-Random-Scalar", 6000,
               fp_1 = Fp::getRand(); g1_2 = G1::getRand();)
    g1_3 = fp_1 * g1_2;
    END_TASK()
    cout << " * 3/5: ";
    BEGIN_TASK("G1-Random-Scalar-Precomp", 16000,
               fp_1 = Fp::getRand(); g1_2 = G1::getRand();
               g1_2.precomputeForMult();)
    g1_3 = fp_1 * g1_2;
    END_TASK()
    cout << " * 4/5: ";
    data = new char[G1::getDataLen(false)];
    BEGIN_TASK("G1-IO-Uncompressed", 800000,
               g1_1 = G1::getRand();)
    g1_1.getData(data, false);
    g1_2.getValue(data,  false);
    END_TASK()
    delete[] data;
    cout << " * 5/5: ";
    data = new char[G1::getDataLen(true)];
    BEGIN_TASK("G1-IO-Compressed", 40000,
               g1_1 = G1::getRand();)
    g1_1.getData(data, true);
    g1_2.getValue(data,  true);
    END_TASK()
    delete[] data;

    /* -------------------- Tests for G2 -------------------- */
    cout << "G2:" << endl << " * 1/5: ";
    BEGIN_TASK("G2-Random-Add", 300000,
               g2_1 = G2::getRand(); g2_2 = G2::getRand();)
    g2_3 = g2_1 + g2_2;
    END_TASK()
    cout << " * 2/5: ";
    BEGIN_TASK("G2-Random-Scalar", 3000,
               fp_1 = Fp::getRand(); g2_2 = G2::getRand();)
    g2_3 = fp_1 * g2_2;
    END_TASK()
    cout << " * 3/5: ";
    BEGIN_TASK("G2-Random-Scalar-Precomp", 6000,
               fp_1 = Fp::getRand(); g2_2 = G2::getRand();
               g2_2.precomputeForMult();)
    g2_3 = fp_1 * g2_2;
    END_TASK()
    cout << " * 4/5: ";
    data = new char[G2::getDataLen(false)];
    BEGIN_TASK("G2-IO-Uncompressed", 300000,
               g2_1 = G2::getRand();)
    g2_1.getData(data, false);
    g2_2.getValue(data,  false);
    END_TASK()
    delete[] data;
    cout << " * 5/5: ";
    data = new char[G2::getDataLen(true)];
    BEGIN_TASK("G2-IO-Compressed", 20000,
               g2_1 = G2::getRand();)
    g2_1.getData(data, true);
    g2_2.getValue(data,  true);
    END_TASK()
    delete[] data;

    /* -------------------- Tests for GT -------------------- */
    cout << "GT:" << endl << " * 1/3: ";
    BEGIN_TASK("GT-Random-Mult", 100000,
               gt_1 = GT::getRand(); gt_2 = GT::getRand();)
    gt_3 = gt_1 * gt_2;
    END_TASK()
    cout << " * 2/3: ";
    BEGIN_TASK("GT-Random-Power", 1000,
               fp_1 = Fp::getRand(); gt_2 = GT::getRand();)
    gt_3 = gt_2 ^ fp_1;
    END_TASK()
    cout << " * 3/3: ";
    BEGIN_TASK("GT-Random-Power-Precomp", 3000,
               fp_1 = Fp::getRand(); gt_2 = GT::getRand();
               gt_2.precomputeForPower();)
    gt_3 = gt_2 ^ fp_1;
    END_TASK()

    /* -------------------- Pairing tests -------------------- */
    cout << "Pairings:" << endl << " * 1/3: ";
    BEGIN_TASK("Pairing-Simple", PAIRING_SAMPLE_SIZE,
               g1_1 = G1::getRand(); g2_1 = G2::getRand();)
    gt_1 = GT::pairing(g1_1, g2_1);
    END_TASK()
    cout << " * 2/3: ";
    BEGIN_TASK("Pairing-Simple-Precomp", PAIRING_SAMPLE_SIZE,
               g1_1 = G1::getRand(); g2_1 = G2::getRand();
               g2_1.precomputeForPairing();)
    gt_1 = GT::pairing(g1_1, g2_1);
    END_TASK()
    cout << " * 3/3: ";
    p4.resize(4);
    BEGIN_TASK("Pairing-Batch", PAIRING_SAMPLE_SIZE / 4,
               p4[0].first = G1::getRand(); p4[0].second = G2::getRand();
               p4[1].first = G1::getRand(); p4[1].second = G2::getRand();
               p4[2].first = G1::getRand(); p4[2].second = G2::getRand();
               p4[3].first = G1::getRand(); p4[3].second = G2::getRand();)
    gt_1 = GT::pairing(p4);
    END_TASK()
}

void benchmarkProof(string name, int smult, NIZKProof &proof, ProofData &d,
                    const CRS &crs, CRS *verif = NULL) {
    cout << name << ":" << endl << " * 1/4: ";
    BEGIN_TASK((name + "-construction"), 50000 * smult,)
    NIZKProof cpy = proof;
    if (!cpy.endEquations()) {
        cerr << "Error: construction failed for " << name << endl;
        return;
    }
    END_TASK()
    proof.endEquations();
    cout << " * 2/4: ";
    BEGIN_TASK((name + "-verification"), 200 * smult,)
    if (!proof.verifySolution(d, crs)) {
        cerr << "Error: verification (1) failed for " << name << endl;
        return;
    }
    END_TASK()
    {
        cout << " * 3/4: ";
        ofstream out("proof.benchmark");
        BEGIN_TASK((name + "-creation"), 100 * smult,)
        proof.writeProof(out, crs, d);
        END_TASK()
        out.close();
    }
    d.privFp.clear();
    d.privG1.clear();
    d.privG2.clear();
    {
        cout << " * 4/4: ";
        ifstream in("proof.benchmark");
        BEGIN_TASK((name + "-check"), 5 * smult,)
        if (verif) {
            if (!proof.checkProof(in, *verif, d)) {
                cerr << "Error: verification (2) failed for " << name << endl;
                return;
            }
        } else {
            if (!proof.checkProof(in, crs, d)) {
                cerr << "Error: verification (2) failed for " << name << endl;
                return;
            }
        }
        END_TASK()
        in.close();
    }
}

void benchmarkProofs() {
    cout << "########## PROOF BENCHMARK ##########" << endl;
    CRS crs(true), crspriv, crspub;
    vector<CRS> crsprivs;
    crs.makePublic();
    {
        ofstream out("priv-crs.benchmark");
        cout << "Private CRS:" << endl << " * 1/2: ";
        BEGIN_TASK("Private-CRS-Gen", 50,)
        crsprivs.push_back(crs.genPrivate(out));
        END_TASK()
        out.close();
    }
    {
        ifstream in("priv-crs.benchmark");
        cout << " * 2/2: ";
        int i = 0;
        BEGIN_TASK("Private-CRS-Check", 50,)
        if (!crs.checkPrivate(in, crsprivs[i++])) {
            cerr << "Error: verification of the private CRS failed" << endl;
            return;
        }
        END_TASK()
        in.close();
    }
    remove("priv-crs.benchmark");
    crspriv = crsprivs.front();
    crsprivs.clear();
    crspub = crspriv;
    crspub.makePublic();
    {
        G1 a = G1::getRand();
        Fp k = Fp::getRand();
        G1 b = k * a;

        NIZKProof proof;
        proof.addEquation(FpVar(0) * G1Const(0), FpUnit() * G1Const(1));

        ProofData d;
        d.privFp.push_back(k);
        d.pubG1.push_back(a);
        d.pubG1.push_back(b);

        benchmarkProof("DiscreteLog-Pub", 5, proof, d, crs);
        d.privFp.push_back(k);
        benchmarkProof("DiscreteLog-Priv", 5, proof, d, crspriv, &crspub);
    }
    {
        ProofData d;

        /* Certificate Authority's credentials */
        Fp sk_A = Fp::getRand();
        G1 pk_A = sk_A * crs.getG1Base();
        d.pubG1.push_back(pk_A);
        G1Element _pk_A = G1Const(0);

        /* User's credentials */
        Fp sk_C = Fp::getRand();
        G2 pk_C = sk_C * crs.getG2Base();
        d.privFp.push_back(sk_C);
        FpElement _sk_C = FpVar(0);
        d.privG2.push_back(pk_C);
        G2Element _pk_C = G2Var(0);

        /* User's certificate */
        G2 cert = sk_A * pk_C;
        d.privG2.push_back(cert);
        G2Element _cert = G2Var(1);

        /* Hash of the one-time public key */
        G1 HK = G1::getRand();
        d.pubG1.push_back(HK);
        G1Element _HK = G1Const(1);

        /* Signature of the one-time public key */
        G1 sign = sk_C * HK;
        d.pubG1.push_back(sign);
        G1Element _sign = G1Const(2);

        /* Service Provider ID (in G1 instead of G2 for efficiency) */
        G1 v_SP = G1::getRand();
        d.pubG1.push_back(v_SP);
        G1Element _v_SP = G1Const(3);

        /* Linkability value */
        G1 value = sk_C * v_SP;
        d.pubG1.push_back(value);
        G1Element _value = G1Const(4);

        NIZKProof proof;
        proof.addEquation(FpUnit() * _pk_C, _sk_C * G2Base());
        proof.addEquation(e(G1Base(), _cert), e(_pk_A, _pk_C));
        proof.addEquation(FpUnit() * _sign, _sk_C * _HK);
        proof.addEquation(FpUnit() * _value, _sk_C * _v_SP);

        benchmarkProof("UserTokens", 1, proof, d, crs);
    }
    {
        ProofData d;

        Fp k = Fp::getRand(), l = Fp::getRand();
        d.privFp.push_back(k);
        FpElement _k = FpVar(0);
        G1 v = (k * l) * crs.getG1Base();

        NIZKProof proof;
        proof.addEquation(e(G1Base(), (_k * FpConst(l)) * G2Base()),
                          e(G1Const(v), G2Base()));

        benchmarkProof("BigEquation", 1, proof, d, crs);
    }
    remove("proof.benchmark");
}

void benchmarkLibrary() {
    b_out = new ofstream("benchmark");
    benchmarkHashes();
    benchmarkPairings();
    benchmarkProofs();
    b_out->close();
    delete b_out;
}
