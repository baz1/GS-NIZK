/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "gsnizk.h"

#include <set>

#ifdef DEBUG
#define ASSERT(X) if (!(X)) { \
    std::cerr << "Error: Assert of " << #X << " at line " \
    << __LINE__ << " failed in gsnizk.cpp!" << endl; \
    throw "FATAL ERROR"; \
    }
#else
#define ASSERT(X) /* noop */
#endif

namespace gsnizk {

FpData::~FpData() {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        el.~Fp();
        return;
    case ELEMENT_PAIR:
    case ELEMENT_SCALAR:
        pair.~PairFp();
        return;
    default:
        return;
    }
}

G1Data::~G1Data() {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        el.~G1();
        return;
    case ELEMENT_PAIR:
        pair.~PairG1();
        return;
    case ELEMENT_SCALAR:
        scalar.~ScalarG1();
        return;
    default:
        return;
    }
}

G2Data::~G2Data() {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        el.~G2();
        return;
    case ELEMENT_PAIR:
        pair.~PairG2();
        return;
    case ELEMENT_SCALAR:
        scalar.~ScalarG2();
        return;
    default:
        return;
    }
}

GTData::~GTData() {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        el.~GT();
        return;
    case ELEMENT_PAIR:
        pair.~PairGT();
        return;
    case ELEMENT_PAIRING:
        pring.~PairingGT();
        return;
    default:
        return;
    }
}

FpElement &FpElement::operator+=(const FpElement &other) {
    FpData *d = new FpData(ELEMENT_PAIR);
    new (&d->pair) PairFp(data, other.data);
    data = std::shared_ptr<FpData>(d);
    return *this;
}

FpElement FpElement::operator+(const FpElement &other) const {
    FpData *d = new FpData(ELEMENT_PAIR);
    new (&d->pair) PairFp(data, other.data);
    return FpElement(std::shared_ptr<FpData>(d));
}

FpElement &FpElement::operator*=(const FpElement &other) {
    FpData *d = new FpData(ELEMENT_SCALAR);
    new (&d->pair) PairFp(data, other.data);
    data = std::shared_ptr<FpData>(d);
    return *this;
}

FpElement FpElement::operator*(const FpElement &other) const {
    FpData *d = new FpData(ELEMENT_SCALAR);
    new (&d->pair) PairFp(data, other.data);
    return FpElement(std::shared_ptr<FpData>(d));
}

FpElement FpVar(int index) {
    FpData *d = new FpData(ELEMENT_VARIABLE);
    d->index = index;
    return FpElement(std::shared_ptr<FpData>(d));
}

FpElement FpConst(int index) {
    FpData *d = new FpData(ELEMENT_CONST_INDEX);
    d->index = index;
    return FpElement(std::shared_ptr<FpData>(d));
}

FpElement FpConst(Fp value) {
    FpData *d = new FpData(ELEMENT_CONST_VALUE);
    new (&d->el) Fp(value);
    return FpElement(std::shared_ptr<FpData>(d));
}

FpElement FpUnit() {
    FpData *d = new FpData(ELEMENT_BASE);
    return FpElement(std::shared_ptr<FpData>(d));
}

G1Element &G1Element::operator+=(const G1Element &other) {
    G1Data *d = new G1Data(ELEMENT_PAIR);
    new (&d->pair) PairG1(data, other.data);
    data = std::shared_ptr<G1Data>(d);
    return *this;
}

G1Element G1Element::operator+(const G1Element &other) const {
    G1Data *d = new G1Data(ELEMENT_PAIR);
    new (&d->pair) PairG1(data, other.data);
    return G1Element(std::shared_ptr<G1Data>(d));
}

G1Element operator*(const FpElement &s, const G1Element &e) {
    G1Data *d = new G1Data(ELEMENT_SCALAR);
    new (&d->scalar) ScalarG1(s.data, e.data);
    return G1Element(std::shared_ptr<G1Data>(d));
}

G1Element G1Var(int index) {
    G1Data *d = new G1Data(ELEMENT_VARIABLE);
    d->index = index;
    return G1Element(std::shared_ptr<G1Data>(d));
}

G1Element G1Const(int index) {
    G1Data *d = new G1Data(ELEMENT_CONST_INDEX);
    d->index = index;
    return G1Element(std::shared_ptr<G1Data>(d));
}

G1Element G1Const(G1 value) {
    G1Data *d = new G1Data(ELEMENT_CONST_VALUE);
    new (&d->el) G1(value);
    return G1Element(std::shared_ptr<G1Data>(d));
}

G1Element G1Base() {
    G1Data *d = new G1Data(ELEMENT_BASE);
    return G1Element(std::shared_ptr<G1Data>(d));
}

G2Element &G2Element::operator+=(const G2Element &other) {
    G2Data *d = new G2Data(ELEMENT_PAIR);
    new (&d->pair) PairG2(data, other.data);
    data = std::shared_ptr<G2Data>(d);
    return *this;
}

G2Element G2Element::operator+(const G2Element &other) const {
    G2Data *d = new G2Data(ELEMENT_PAIR);
    new (&d->pair) PairG2(data, other.data);
    return G2Element(std::shared_ptr<G2Data>(d));
}

G2Element operator*(const FpElement &s, const G2Element &e) {
    G2Data *d = new G2Data(ELEMENT_SCALAR);
    new (&d->scalar) ScalarG2(s.data, e.data);
    return G2Element(std::shared_ptr<G2Data>(d));
}

G2Element G2Var(int index) {
    G2Data *d = new G2Data(ELEMENT_VARIABLE);
    d->index = index;
    return G2Element(std::shared_ptr<G2Data>(d));
}

G2Element G2Const(int index) {
    G2Data *d = new G2Data(ELEMENT_CONST_INDEX);
    d->index = index;
    return G2Element(std::shared_ptr<G2Data>(d));
}

G2Element G2Const(G2 value) {
    G2Data *d = new G2Data(ELEMENT_CONST_VALUE);
    new (&d->el) G2(value);
    return G2Element(std::shared_ptr<G2Data>(d));
}

G2Element G2Base() {
    G2Data *d = new G2Data(ELEMENT_BASE);
    return G2Element(std::shared_ptr<G2Data>(d));
}

GTElement &GTElement::operator*=(const GTElement &other) {
    GTData *d = new GTData(ELEMENT_PAIR);
    new (&d->pair) PairGT(data, other.data);
    data = std::shared_ptr<GTData>(d);
    return *this;
}

GTElement GTElement::operator*(const GTElement &other) const {
    GTData *d = new GTData(ELEMENT_PAIR);
    new (&d->pair) PairGT(data, other.data);
    return GTElement(std::shared_ptr<GTData>(d));
}

GTElement e(const G1Element &a, const G2Element &b) {
    GTData *d = new GTData(ELEMENT_PAIRING);
    new (&d->pring) PairingGT(a.data, b.data);
    return GTElement(std::shared_ptr<GTData>(d));
}

GTElement GTConst(int index) {
    GTData *d = new GTData(ELEMENT_CONST_INDEX);
    d->index = index;
    return GTElement(std::shared_ptr<GTData>(d));
}

GTElement GTConst(GT value) {
    GTData *d = new GTData(ELEMENT_CONST_VALUE);
    new (&d->el) GT(value);
    return GTElement(std::shared_ptr<GTData>(d));
}

void NIZKProof::addEquation(const FpElement &leftHandSide,
                            const FpElement &rightHandSide) {
    if (fixed) throw "Unexpected use of gsnizk::addEquation";
    eqsFp.push_back(PairFp(leftHandSide.data, rightHandSide.data));
}

void NIZKProof::addEquation(const G1Element &leftHandSide,
                            const G1Element &rightHandSide) {
    if (fixed) throw "Unexpected use of gsnizk::addEquation";
    eqsG1.push_back(PairG1(leftHandSide.data, rightHandSide.data));
}

void NIZKProof::addEquation(const G2Element &leftHandSide,
                            const G2Element &rightHandSide) {
    if (fixed) throw "Unexpected use of gsnizk::addEquation";
    eqsG2.push_back(PairG2(leftHandSide.data, rightHandSide.data));
}

void NIZKProof::addEquation(const GTElement &leftHandSide,
                            const GTElement &rightHandSide) {
    if (fixed) throw "Unexpected use of gsnizk::addEquation";
    eqsGT.push_back(PairGT(leftHandSide.data, rightHandSide.data));
}

enum SAT_NODE_TYPE {
    SAT_NODE_AND,
    SAT_NODE_OR,
    SAT_NODE_INDEX,
    SAT_NODE_TRUE,
    SAT_NODE_FALSE
};

#define INDEX_TYPE_G1   0
#define INDEX_TYPE_G2   1

struct SAT_NODE;

struct SAT_PTRPAIR {
    SAT_NODE *left, *right;
};

struct SAT_INDEX {
    int index_type, index;
};

struct SAT_NODE {
    SAT_NODE_TYPE type;
    union {
        SAT_PTRPAIR pair;
        SAT_INDEX idx;
    };
};

SAT_NODE *getSAT(const FpData &d) {
    SAT_NODE *node = new SAT_NODE;
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_BASE:
        node->type = SAT_NODE_TRUE;
        return node;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        node->type = SAT_NODE_FALSE;
        return node;
    case ELEMENT_PAIR:
        node->type = SAT_NODE_AND;
        break;
    case ELEMENT_SCALAR:
        node->type = SAT_NODE_OR;
        break;
    default:
        ASSERT(false /* Unexpected data type */);
        return node;
    }
    node->pair.left = getSAT(*d.pair.first);
    node->pair.right = getSAT(*d.pair.second);
    return node;
}

SAT_NODE *getSAT(const G1Data &d) {
    SAT_NODE *node = new SAT_NODE;
    switch (d.type) {
    case ELEMENT_VARIABLE:
        node->type = SAT_NODE_INDEX;
        node->idx.index_type = INDEX_TYPE_G1;
        node->idx.index = d.index;
        return node;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        node->type = SAT_NODE_FALSE;
        return node;
    case ELEMENT_PAIR:
        node->type = SAT_NODE_AND;
        node->pair.left = getSAT(*d.pair.first);
        node->pair.right = getSAT(*d.pair.second);
        return node;
    case ELEMENT_SCALAR:
        node->type = SAT_NODE_OR;
        node->pair.left = getSAT(*d.scalar.first);
        node->pair.right = getSAT(*d.scalar.second);
        return node;
    case ELEMENT_BASE:
        node->type = SAT_NODE_TRUE;
        return node;
    default:
        ASSERT(false /* Unexpected data type */);
        return node;
    }
}

SAT_NODE *getSAT(const G2Data &d) {
    SAT_NODE *node = new SAT_NODE;
    switch (d.type) {
    case ELEMENT_VARIABLE:
        node->type = SAT_NODE_INDEX;
        node->idx.index_type = INDEX_TYPE_G2;
        node->idx.index = d.index;
        return node;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        node->type = SAT_NODE_FALSE;
        return node;
    case ELEMENT_PAIR:
        node->type = SAT_NODE_AND;
        node->pair.left = getSAT(*d.pair.first);
        node->pair.right = getSAT(*d.pair.second);
        return node;
    case ELEMENT_SCALAR:
        node->type = SAT_NODE_OR;
        node->pair.left = getSAT(*d.scalar.first);
        node->pair.right = getSAT(*d.scalar.second);
        return node;
    case ELEMENT_BASE:
        node->type = SAT_NODE_TRUE;
        return node;
    default:
        ASSERT(false /* Unexpected data type */);
        return node;
    }
}

SAT_NODE *getSAT(const GTData &d) {
    SAT_NODE *node = new SAT_NODE;
    switch (d.type) {
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        node->type = SAT_NODE_FALSE;
        return node;
    case ELEMENT_PAIR:
        node->type = SAT_NODE_AND;
        node->pair.left = getSAT(*d.pair.first);
        node->pair.right = getSAT(*d.pair.second);
        return node;
    case ELEMENT_PAIRING:
        node->type = SAT_NODE_OR;
        node->pair.left = getSAT(*d.pring.first);
        node->pair.right = getSAT(*d.pring.second);
        return node;
    default:
        ASSERT(false /* Unexpected data type */);
        return node;
    }
}

template <typename T> bool checkIndexesSet(
        const std::vector< std::shared_ptr<T> > &v) {
    for (const std::shared_ptr<T> &el : v) {
        if (!el) return false;
    }
    return true;
}


void delNode(SAT_NODE *node) {
    switch (node->type) {
    case SAT_NODE_AND:
        delNode(node->pair.left);
        delNode(node->pair.right);
        break;
    case SAT_NODE_OR:
        delNode(node->pair.left);
        delNode(node->pair.right);
        break;
    default:
        break;
    }
    delete node;
}

void simplify(SAT_NODE *node) {
    switch (node->type) {
    case SAT_NODE_AND:
        simplify(node->pair.left);
        simplify(node->pair.right);
        switch (node->pair.left->type) {
        case SAT_NODE_FALSE:
            delete node->pair.left;
            delNode(node->pair.right);
            node->type = SAT_NODE_FALSE;
            return;
        case SAT_NODE_TRUE:
        {
            delete node->pair.left;
            SAT_NODE *tmp = node->pair.right;
            *node = *tmp;
            delete tmp;
            return;
        }
        default:
            break;
        }
        switch (node->pair.right->type) {
        case SAT_NODE_FALSE:
            delNode(node->pair.left);
            delete node->pair.right;
            node->type = SAT_NODE_FALSE;
            return;
        case SAT_NODE_TRUE:
        {
            delete node->pair.right;
            SAT_NODE *tmp = node->pair.left;
            *node = *tmp;
            delete tmp;
            return;
        }
        default:
            break;
        }
        return;
    case SAT_NODE_OR:
        simplify(node->pair.left);
        simplify(node->pair.right);
        return;
    default:
        return;
    }
}

void countIndexes(SAT_NODE *node, std::vector<int> cnt[2]) {
    switch (node->type) {
    case SAT_NODE_AND:
    case SAT_NODE_OR:
        countIndexes(node->pair.left, cnt);
        countIndexes(node->pair.right, cnt);
        return;
    case SAT_NODE_INDEX:
        ++cnt[node->idx.index_type][node->idx.index];
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

SAT_NODE *duplicateNode(const SAT_NODE *node) {
    SAT_NODE *result = new SAT_NODE;
    switch (node->type) {
    case SAT_NODE_AND:
    case SAT_NODE_OR:
        result->type = node->type;
        result->pair.left = duplicateNode(node->pair.left);
        result->pair.right = duplicateNode(node->pair.right);
        break;
    default:
        *result = *node;
    }
    return result;
}

void instanciateIndex(SAT_NODE *node, int i_type, int i_value,
                      SAT_NODE_TYPE value) {
    ASSERT((value == SAT_NODE_TRUE) || (value == SAT_NODE_FALSE));
    switch (node->type) {
    case SAT_NODE_AND:
    case SAT_NODE_OR:
        instanciateIndex(node->pair.left, i_type, i_value, value);
        instanciateIndex(node->pair.right, i_type, i_value, value);
        return;
    case SAT_NODE_INDEX:
        if ((node->idx.index_type == i_type) && (node->idx.index == i_value))
            node->type = value;
        return;
    default:
        return;
    }
}

#define SAT_VALUE_UNSET 0
#define SAT_VALUE_TRUE  1
#define SAT_VALUE_FALSE 2

int tryPermutation(SAT_NODE *root, std::vector<int> val[4],
        std::vector<int> cnt[4]) {
    simplify(root);
    cnt[0].clear();
    cnt[0].resize(val[0].size(), 0);
    cnt[1].clear();
    cnt[1].resize(val[1].size(), 0);
    countIndexes(root, cnt);
    int max = 0, mi, mj;
    for (int i = 2; i--;) {
        for (int j = val[i].size(); j--;) {
            if (val[i][j]) continue;
            if (!cnt[i][j]) {
                val[i][j] = SAT_VALUE_FALSE;
            } else if (cnt[i][j] > max) {
                max = cnt[i][j];
                mi = i;
                mj = j;
            }
        }
    }
    if (!max) {
        switch (root->type) {
        case SAT_NODE_FALSE:
            return -1;
        case SAT_NODE_TRUE:
            return 0;
        default:
            ASSERT(false /* Unexpected error */);
            return -1;
        }
    }
    std::vector<int> valcp[2];
    valcp[0] = val[0];
    valcp[1] = val[1];
    SAT_NODE *rootcp = duplicateNode(root);
    instanciateIndex(rootcp, mi, mj, SAT_NODE_FALSE);
    valcp[mi][mj] = SAT_VALUE_FALSE;
    int r1 = tryPermutation(rootcp, valcp, cnt);
    instanciateIndex(root, mi, mj, SAT_NODE_TRUE);
    val[mi][mj] = SAT_VALUE_TRUE;
    int r2 = tryPermutation(root, val, cnt);
    if ((r1 < 0) || (r2 <= r1)) return r2;
    val[0] = valcp[0];
    val[1] = valcp[1];
    return r1;
}

void joinSAT(SAT_NODE *&main, SAT_NODE *other) {
    SAT_NODE *tmp = new SAT_NODE;
    tmp->type = SAT_NODE_AND;
    tmp->pair.left = main;
    tmp->pair.right = other;
    main = tmp;
}

bool NIZKProof::endEquations() {
    /* Subsequent calls are ignored. */
    if (fixed) return true;
    {
        for (PairFp &p : eqsFp) {
            getIndexes(p.first);
            if (!p.second) continue;
            getIndexes(p.second);
        }
        for (PairG1 &p : eqsG1) {
            getIndexes(p.first);
            if (!p.second) continue;
            getIndexes(p.second);
        }
        for (PairG2 &p : eqsG2) {
            getIndexes(p.first);
            if (!p.second) continue;
            getIndexes(p.second);
        }
        for (PairGT &p : eqsGT) {
            getIndexes(p.first);
            if (!p.second) continue;
            getIndexes(p.second);
        }
        if (!(checkIndexesSet(varsFp) && checkIndexesSet(cstsFp) &&
              checkIndexesSet(varsG1) && checkIndexesSet(cstsG1) &&
              checkIndexesSet(varsG2) && checkIndexesSet(cstsG2) &&
              checkIndexesSet(cstsGT))) {
            varsFp.clear();
            cstsFp.clear();
            varsG1.clear();
            cstsG1.clear();
            varsG2.clear();
            cstsG2.clear();
            cstsGT.clear();
            return false;
        }
    }
    // TODO rewrite equations, etc.
    if (type == SelectedEncryption) {
        SAT_NODE *root = new SAT_NODE;
        root->type = SAT_NODE_TRUE;
        for (const PairFp &p : eqsFp) {
            joinSAT(root, getSAT(*p.first));
            if (!p.second) continue;
            joinSAT(root, getSAT(*p.second));
        }
        for (const PairG1 &p : eqsG1) {
            joinSAT(root, getSAT(*p.first));
            if (!p.second) continue;
            joinSAT(root, getSAT(*p.second));
        }
        for (const PairG2 &p : eqsG2) {
            joinSAT(root, getSAT(*p.first));
            if (!p.second) continue;
            joinSAT(root, getSAT(*p.second));
        }
        for (const PairGT &p : eqsGT) {
            joinSAT(root, getSAT(*p.first));
            if (!p.second) continue;
            joinSAT(root, getSAT(*p.second));
        }
        std::vector<int> cnt[2];
        sEnc[INDEX_TYPE_G1].resize(varsG1.size(), SAT_VALUE_UNSET);
        cnt[INDEX_TYPE_G1].reserve(varsG1.size());
        sEnc[INDEX_TYPE_G2].resize(varsG2.size(), SAT_VALUE_UNSET);
        cnt[INDEX_TYPE_G2].reserve(varsG2.size());
        if (tryPermutation(root, sEnc, cnt) < 0)
            throw "Cannot use ZK with the equations provided (in gsnizk)";
        /* Converting to 0/1 (boolean) values; 1 for encrypted */
        for (int i = 2; i--;) {
            for (int j = sEnc[i].size(); j--;)
                --sEnc[i][j];
        }
    }
    // TODO fill the equation proof types
    fixed = true;
    return true;
}


bool NIZKProof::verifySolution(const ProofData &instantiation,
                               const CRS &crs) const {
    if (!fixed)
        throw "Equations not fixed yet! (in NIZKProof::verifySolution)";
    if (!checkInstantiation(instantiation))
        return false;
    for (const AdditionalFp &aFp : additionalFp)
        aFp.value = real_eval(*aFp.formula, instantiation, crs);
    for (const AdditionalG1 &aG1 : additionalG1)
        aG1.value = real_eval(*aG1.formula, instantiation, crs);
    for (const AdditionalG2 &aG2 : additionalG2)
        aG2.value = real_eval(*aG2.formula, instantiation, crs);
    for (const PairFp &p : eqsFp) {
        if (real_eval(*p.first, instantiation, crs) !=
                real_eval(*p.second, instantiation, crs))
            return false;
    }
    for (const PairG1 &p : eqsG1) {
        if (real_eval(*p.first, instantiation, crs) !=
                real_eval(*p.second, instantiation, crs))
            return false;
    }
    for (const PairG2 &p : eqsG2) {
        if (real_eval(*p.first, instantiation, crs) !=
                real_eval(*p.second, instantiation, crs))
            return false;
    }
    for (const PairGT &p : eqsGT) {
        if (real_eval(*p.first, instantiation, crs) !=
                real_eval(*p.second, instantiation, crs))
            return false;
    }
    return true;
}

enum ValueType {
    VALUE_NULL = 0,
    VALUE_Fp = 1,
    VALUE_G = 2,
    VALUE_B = 3
};

struct PiG1 {
    ValueType type;
    Fp fpValue;
    B1 b1Value;
    inline PiG1() {}
    inline PiG1(ValueType type) : type(type) {}
    PiG1 &operator*=(const Fp &c);
};

PiG1 &PiG1::operator*=(const Fp &c) {
    switch (type) {
    case VALUE_NULL:
        break;
    case VALUE_Fp:
        fpValue *= c;
        break;
    case VALUE_G:
        b1Value._2 *= c;
        break;
    case VALUE_B:
        b1Value *= c;
        break;
    }
    return *this;
}

struct PiG2 {
    ValueType type;
    Fp fpValue;
    B2 b2Value;
    inline PiG2() {}
    inline PiG2(ValueType type) : type(type) {}
    PiG2 &operator*=(const Fp &c);
};

PiG2 &PiG2::operator*=(const Fp &c) {
    switch (type) {
    case VALUE_NULL:
        break;
    case VALUE_Fp:
        fpValue *= c;
        break;
    case VALUE_G:
        b2Value._2 *= c;
        break;
    case VALUE_B:
        b2Value *= c;
        break;
    }
    return *this;
}

enum CommitType {
    COMMIT_PUB,
    COMMIT_ENC,
    COMMIT_PRIV
};

struct G1Commit {
    CommitType type;
    Fp r, s;
    PiG1 c;
};

struct G2Commit {
    CommitType type;
    Fp r, s;
    PiG2 c;
};

struct ProofEls {
    PiG1 p1_v, p1_w;
    PiG2 p2_v, p2_w;
};

void addPiG1(const PiG1 &a, const PiG1 &b, PiG1 &result, const CRS &crs) {
    if ((a.type == VALUE_Fp) && (b.type == VALUE_Fp)) {
        result.type = VALUE_Fp;
        result.fpValue = a.fpValue + b.fpValue;
        return;
    }
    result.type = VALUE_B;
    if (a.type == VALUE_Fp)
        result.b1Value = B1(a.fpValue, crs);
    else
        result.b1Value = a.b1Value;
    if (b.type == VALUE_Fp)
        result.b1Value += B1(b.fpValue, crs);
    else
        result.b1Value += b.b1Value;
}

void addPiG2(const PiG2 &a, const PiG2 &b, PiG2 &result, const CRS &crs) {
    if ((a.type == VALUE_Fp) && (b.type == VALUE_Fp)) {
        result.type = VALUE_Fp;
        result.fpValue = a.fpValue + b.fpValue;
        return;
    }
    result.type = VALUE_B;
    if (a.type == VALUE_Fp)
        result.b2Value = B2(a.fpValue, crs);
    else
        result.b2Value = a.b2Value;
    if (b.type == VALUE_Fp)
        result.b2Value += B2(b.fpValue, crs);
    else
        result.b2Value += b.b2Value;
}

void subPiG1(const PiG1 &a, const PiG1 &b, PiG1 &result, const CRS &crs) {
    if ((a.type == VALUE_Fp) && (b.type == VALUE_Fp)) {
        result.type = VALUE_Fp;
        result.fpValue = a.fpValue - b.fpValue;
        return;
    }
    result.type = VALUE_B;
    if (a.type == VALUE_Fp)
        result.b1Value = B1(a.fpValue, crs);
    else
        result.b1Value = a.b1Value;
    if (b.type == VALUE_Fp)
        result.b1Value -= B1(b.fpValue, crs);
    else
        result.b1Value -= b.b1Value;
}

void subPiG2(const PiG2 &a, const PiG2 &b, PiG2 &result, const CRS &crs) {
    if ((a.type == VALUE_Fp) && (b.type == VALUE_Fp)) {
        result.type = VALUE_Fp;
        result.fpValue = a.fpValue - b.fpValue;
        return;
    }
    result.type = VALUE_B;
    if (a.type == VALUE_Fp)
        result.b2Value = B2(a.fpValue, crs);
    else
        result.b2Value = a.b2Value;
    if (b.type == VALUE_Fp)
        result.b2Value -= B2(b.fpValue, crs);
    else
        result.b2Value -= b.b2Value;
}

template <class T> void addCommitGX(const T &c1, const T &c2, T &cr) {
    switch (c1.type) {
    case COMMIT_PUB:
        switch (c2.type) {
        case COMMIT_PRIV:
            cr.s = c2.s;
        case COMMIT_ENC:
            cr.r = c2.r;
        case COMMIT_PUB:
            break;
        }
        break;
    case COMMIT_ENC:
        switch (c2.type) {
        case COMMIT_PRIV:
            cr.s = c2.s;
        case COMMIT_ENC:
            cr.r = c1.r + c2.r;
            break;
        case COMMIT_PUB:
            cr.r = c1.r;
            break;
        }
        break;
    case COMMIT_PRIV:
        switch (c2.type) {
        case COMMIT_PRIV:
            cr.r = c1.r + c2.r;
            cr.s = c1.s + c2.s;
            break;
        case COMMIT_ENC:
            cr.r = c1.r + c2.r;
            cr.s = c1.s;
            break;
        case COMMIT_PUB:
            cr.r = c1.r;
            cr.s = c1.s;
            break;
        }
        break;
    }
}

void addCommitG1(const G1Commit &c1, const G1Commit &c2, G1Commit &cr,
                 const CRS &crs) {
    addCommitGX(c1, c2, cr);
    if (c1.c.type == VALUE_Fp) {
        if (c2.c.type == VALUE_Fp) {
            cr.c.fpValue = c1.c.fpValue + c2.c.fpValue;
        } else {
            cr.c.b1Value = B1(c1.c.fpValue, crs) + c2.c.b1Value;
        }
    } else {
        if (c2.c.type == VALUE_Fp) {
            cr.c.b1Value = c1.c.b1Value + B1(c2.c.fpValue, crs);
        } else {
            cr.c.b1Value = c1.c.b1Value + c2.c.b1Value;
        }
    }
}

void addCommitG2(const G2Commit &c1, const G2Commit &c2, G2Commit &cr,
                 const CRS &crs) {
    addCommitGX(c1, c2, cr);
    if (c1.c.type == VALUE_Fp) {
        if (c2.c.type == VALUE_Fp) {
            cr.c.fpValue = c1.c.fpValue + c2.c.fpValue;
        } else {
            cr.c.b2Value = B2(c1.c.fpValue, crs) + c2.c.b2Value;
        }
    } else {
        if (c2.c.type == VALUE_Fp) {
            cr.c.b2Value = c1.c.b2Value + B2(c2.c.fpValue, crs);
        } else {
            cr.c.b2Value = c1.c.b2Value + c2.c.b2Value;
        }
    }
}

void removeProof(const FpData &d);
void removeProof(const G1Data &d);
void removeProof(const G2Data &d);
void removeProof(const GTData &d);
void removeLeft(const FpData &d);
void removeLeft(const G1Data &d);
void removeRight(const FpData &d);
void removeRight(const G2Data &d);

void writeEqProof(std::ostream &stream, const ProofEls &left,
                  const ProofEls &right, EqProofType expectedType,
                  const CRS &crs) {
    (void) expectedType; /* (prevent warning when no debugging) */
    ProofEls result;
    subPiG1(left.p1_v, right.p1_v, result.p1_v, crs);
    ASSERT((int) result.p1_v.type == (int) expectedType.tv1);
    // TODO check that it is theoretically true
    subPiG1(left.p1_w, right.p1_w, result.p1_w, crs);
    ASSERT((int) result.p1_w.type == (int) expectedType.tw1);
    // TODO check that it is theoretically true
    subPiG2(left.p2_v, right.p2_v, result.p2_v, crs);
    ASSERT((int) result.p2_v.type == (int) expectedType.tv2);
    // TODO check that it is theoretically true
    subPiG2(left.p2_w, right.p2_w, result.p2_w, crs);
    ASSERT((int) result.p2_w.type == (int) expectedType.tw2);
    // TODO check that it is theoretically true
    switch (result.p1_v.type) {
    case VALUE_Fp:
        stream << result.p1_v.fpValue;
        break;
    case VALUE_G:
        stream << result.p1_v.b1Value._2;
        break;
    case VALUE_B:
        stream << result.p1_v.b1Value;
        break;
    default:
        ASSERT(false /* Unexpected error */);
    }
    switch (result.p1_w.type) {
    case VALUE_Fp:
        stream << result.p1_w.fpValue;
        break;
    case VALUE_G:
        stream << result.p1_w.b1Value._2;
        break;
    case VALUE_B:
        stream << result.p1_w.b1Value;
        break;
    default:
        ASSERT(false /* Unexpected error */);
    }
    switch (result.p2_v.type) {
    case VALUE_Fp:
        stream << result.p2_v.fpValue;
        break;
    case VALUE_G:
        stream << result.p2_v.b2Value._2;
        break;
    case VALUE_B:
        stream << result.p2_v.b2Value;
        break;
    default:
        ASSERT(false /* Unexpected error */);
    }
    switch (result.p2_w.type) {
    case VALUE_Fp:
        stream << result.p2_w.fpValue;
        break;
    case VALUE_G:
        stream << result.p2_w.b2Value._2;
        break;
    case VALUE_B:
        stream << result.p2_w.b2Value;
        break;
    default:
        ASSERT(false /* Unexpected error */);
    }
}

void NIZKProof::writeProof(std::ostream &stream, const CRS &crs,
                           const ProofData &instantiation) const {
    if (!fixed)
        throw "Equations not fixed yet! (in NIZKProof::writeProof)";
    if (!checkInstantiation(instantiation))
        throw "Wrong instantiation in NIZKProof::writeProof!";
    for (const AdditionalFp &aFp : additionalFp)
        aFp.value = real_eval(*aFp.formula, instantiation, crs);
    for (const AdditionalG1 &aG1 : additionalG1)
        aG1.value = real_eval(*aG1.formula, instantiation, crs);
    for (const AdditionalG2 &aG2 : additionalG2)
        aG2.value = real_eval(*aG2.formula, instantiation, crs);
    ASSERT(varsFp.size() == varsFpInB1.size());
    ASSERT(cstsFp.size() == cstsFpInB1.size());
    G1Commit c1;
    G2Commit c2;
    int j = varsFp.size(), i = additionalFp.size();
    c1.type = COMMIT_ENC;
    c1.c.type = VALUE_Fp;
    c2.type = COMMIT_ENC;
    c2.c.type = VALUE_B;
    while ((--j, i--) > 0) {
        if (varsFpInB1[j]) {
            c1.r = Fp::getRand();
            c1.c.fpValue = additionalFp[i].value;
            varsFp[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
            stream << B1::commit(c1.c.fpValue, c1.r, crs);
        } else {
            c2.r = Fp::getRand();
            c2.c.b2Value = B2::commit(additionalFp[i].value, c2.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
            stream << c2.c.b2Value;
        }
    }
    while (j-- > 0) {
        if (varsFpInB1[j]) {
            c1.r = Fp::getRand();
            c1.c.fpValue = instantiation.privFp[j];
            varsFp[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
            stream << B1::commit(c1.c.fpValue, c1.r, crs);
        } else {
            c2.r = Fp::getRand();
            c2.c.b2Value = B2::commit(instantiation.privFp[j], c2.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
            stream << c2.c.b2Value;
        }
    }
    j = varsG1.size();
    i = additionalG1.size();
    c1.c.type = VALUE_G;
    while ((--j, i--) > 0) {
        c1.r = Fp::getRand();
        c1.c.b1Value._2 = additionalG1[i].value;
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[1][j])) {
            c1.type = COMMIT_ENC;
            stream << B1::commit(c1.c.b1Value, c1.r, crs);
        } else {
            c1.type = COMMIT_PRIV;
            c1.s = Fp::getRand();
            stream << B1::commit(c1.c.b1Value, c1.r, c1.s, crs);
        }
        varsG1[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
    }
    while (j-- > 0) {
        c1.r = Fp::getRand();
        c1.c.b1Value._2 = instantiation.privG1[j];
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[1][j])) {
            c1.type = COMMIT_ENC;
            stream << B1::commit(c1.c.b1Value, c1.r, crs);
        } else {
            c1.type = COMMIT_PRIV;
            c1.s = Fp::getRand();
            stream << B1::commit(c1.c.b1Value, c1.r, c1.s, crs);
        }
        varsG1[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
    }
    j = varsG2.size();
    i = additionalG2.size();
    c2.c.type = VALUE_B;
    while ((--j, i--) > 0) {
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[2][j])) {
            c2.type = COMMIT_ENC;
            c2.c.b2Value = B2::commit(additionalG2[i].value, c2.r, crs);
        } else {
            c2.type = COMMIT_PRIV;
            c2.s = Fp::getRand();
            c2.c.b2Value = B2::commit(additionalG2[i].value, c2.r, c2.s, crs);
        }
        stream << c2.c.b2Value;
        varsG2[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
    }
    while (j-- > 0) {
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[2][j])) {
            c2.type = COMMIT_ENC;
            c2.c.b2Value = B2::commit(instantiation.privG2[j],
                    c2.r, crs);
        } else {
            c2.type = COMMIT_PRIV;
            c2.s = Fp::getRand();
            c2.c.b2Value = B2::commit(instantiation.privG2[j],
                    c2.r, c2.s, crs);
        }
        stream << c2.c.b2Value;
        varsG2[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
    }
    c1.type = COMMIT_PUB;
    c1.c.type = VALUE_Fp;
    c2.type = COMMIT_PUB;
    c2.c.type = VALUE_Fp;
    for (j = cstsFp.size(); j-- > 0;) {
        if (cstsFpInB1[j]) {
            c1.c.fpValue = instantiation.pubFp[j];
            cstsFp[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
        } else {
            c2.c.fpValue = instantiation.pubFp[j];
            cstsFp[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
        }
    }
    c1.c.type = VALUE_G;
    for (j = cstsG1.size(); j-- > 0;) {
        c1.c.b1Value._2 = instantiation.pubG1[j];
        cstsG1[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
    }
    c2.c.type = VALUE_G;
    for (j = cstsG2.size(); j-- > 0;) {
        c2.c.b2Value._2 = instantiation.pubG2[j];
        cstsG2[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
    }
    for (j = cstsGT.size(); j-- > 0;) {
        ProofEls *elGT = new ProofEls;
        elGT->p1_v.type = VALUE_NULL;
        elGT->p1_w.type = VALUE_NULL;
        elGT->p2_v.type = VALUE_NULL;
        elGT->p2_w.type = VALUE_NULL;
        cstsGT[j]->d = reinterpret_cast<void*>(elGT);
    }
    for (i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, *reinterpret_cast<ProofEls*>(left.d),
                     *reinterpret_cast<ProofEls*>(right.d), tFp[i], crs);
    }
    for (i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, *reinterpret_cast<ProofEls*>(left.d),
                     *reinterpret_cast<ProofEls*>(right.d), tG1[i], crs);
    }
    for (i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, *reinterpret_cast<ProofEls*>(left.d),
                     *reinterpret_cast<ProofEls*>(right.d), tG2[i], crs);
    }
    for (i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, *reinterpret_cast<ProofEls*>(left.d),
                     *reinterpret_cast<ProofEls*>(right.d), tGT[i], crs);
    }
    for (i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        removeProof(left);
        removeProof(right);
    }
}

bool NIZKProof::checkInstantiation(const ProofData &instantiation) const {
    return (instantiation.pubFp.size() != cstsFp.size()) &&
        (instantiation.pubG1.size() != cstsG1.size()) &&
        (instantiation.pubG2.size() != cstsG2.size()) &&
        (instantiation.pubGT.size() != cstsGT.size()) &&
        (instantiation.privFp.size() + additionalFp.size() == varsFp.size()) &&
        (instantiation.privG1.size() + additionalG1.size() == varsG1.size()) &&
        (instantiation.privG2.size() + additionalG2.size() == varsG2.size());
}

void NIZKProof::getIndexes(std::shared_ptr<FpData> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
        if (varsFp.size() <= static_cast<size_t>(d->index)) {
            varsFp.resize(d->index + 1);
            varsFp[d->index] = d;
        } else if (varsFp[d->index]) {
            d = varsFp[d->index];
        } else {
            varsFp[d->index] = d;
        }
        return;
    case ELEMENT_CONST_INDEX:
        if (cstsFp.size() <= static_cast<size_t>(d->index)) {
            cstsFp.resize(d->index + 1);
            cstsFp[d->index] = d;
        } else if (cstsFp[d->index]) {
            d = cstsFp[d->index];
        } else {
            cstsFp[d->index] = d;
        }
        return;
    case ELEMENT_CONST_VALUE:
        return;
    case ELEMENT_PAIR:
    case ELEMENT_SCALAR:
        getIndexes(d->pair.first);
        getIndexes(d->pair.second);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

void NIZKProof::getIndexes(std::shared_ptr<G1Data> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
        if (varsG1.size() <= static_cast<size_t>(d->index)) {
            varsG1.resize(d->index + 1);
            varsG1[d->index] = d;
        } else if (varsG1[d->index]) {
            d = varsG1[d->index];
        } else {
            varsG1[d->index] = d;
        }
        return;
    case ELEMENT_CONST_INDEX:
        if (cstsG1.size() <= static_cast<size_t>(d->index)) {
            cstsG1.resize(d->index + 1);
            cstsG1[d->index] = d;
        } else if (cstsG1[d->index]) {
            d = cstsG1[d->index];
        } else {
            cstsG1[d->index] = d;
        }
        return;
    case ELEMENT_CONST_VALUE:
        return;
    case ELEMENT_PAIR:
        getIndexes(d->pair.first);
        getIndexes(d->pair.second);
        return;
    case ELEMENT_SCALAR:
        getIndexes(d->scalar.first);
        getIndexes(d->scalar.second);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

void NIZKProof::getIndexes(std::shared_ptr<G2Data> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
        if (varsG2.size() <= static_cast<size_t>(d->index)) {
            varsG2.resize(d->index + 1);
            varsG2[d->index] = d;
        } else if (varsG2[d->index]) {
            d = varsG2[d->index];
        } else {
            varsG2[d->index] = d;
        }
        return;
    case ELEMENT_CONST_INDEX:
        if (cstsG2.size() <= static_cast<size_t>(d->index)) {
            cstsG2.resize(d->index + 1);
            cstsG2[d->index] = d;
        } else if (cstsG2[d->index]) {
            d = cstsG2[d->index];
        } else {
            cstsG2[d->index] = d;
        }
        return;
    case ELEMENT_CONST_VALUE:
        return;
    case ELEMENT_PAIR:
        getIndexes(d->pair.first);
        getIndexes(d->pair.second);
        return;
    case ELEMENT_SCALAR:
        getIndexes(d->scalar.first);
        getIndexes(d->scalar.second);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

void NIZKProof::getIndexes(std::shared_ptr<GTData> &d) {
    switch (d->type) {
    case ELEMENT_CONST_INDEX:
        if (cstsGT.size() <= static_cast<size_t>(d->index)) {
            cstsGT.resize(d->index + 1);
            cstsGT[d->index] = d;
        } else if (cstsGT[d->index]) {
            d = cstsGT[d->index];
        } else {
            cstsGT[d->index] = d;
        }
        return;
    case ELEMENT_CONST_VALUE:
        return;
    case ELEMENT_PAIR:
        getIndexes(d->pair.first);
        getIndexes(d->pair.second);
        return;
    case ELEMENT_PAIRING:
        getIndexes(d->pring.first);
        getIndexes(d->pring.second);
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

Fp NIZKProof::real_eval(const FpData &d, const ProofData &instantiation,
                        const CRS &crs) const {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if (static_cast<size_t>(d.index) < instantiation.privFp.size())
            return instantiation.privFp[d.index];
        return additionalFp[d.index - instantiation.privFp.size()].value;
    case ELEMENT_CONST_INDEX:
        return instantiation.pubFp[d.index];
    case ELEMENT_CONST_VALUE:
        return d.el;
    case ELEMENT_PAIR:
        return real_eval(*d.pair.first, instantiation, crs) +
                real_eval(*d.pair.second, instantiation, crs);
    case ELEMENT_SCALAR:
        return real_eval(*d.pair.first, instantiation, crs) *
                real_eval(*d.pair.second, instantiation, crs);
    case ELEMENT_BASE:
        return Fp::getUnit();
    default:
        ASSERT(false /* Unexpected data type */);
        return Fp();
    }
}

G1 NIZKProof::real_eval(const G1Data &d, const ProofData &instantiation,
                        const CRS &crs) const {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if (static_cast<size_t>(d.index) < instantiation.privG1.size())
            return instantiation.privG1[d.index];
        return additionalG1[d.index - instantiation.privG1.size()].value;
    case ELEMENT_CONST_INDEX:
        return instantiation.pubG1[d.index];
    case ELEMENT_CONST_VALUE:
        return d.el;
    case ELEMENT_PAIR:
        return real_eval(*d.pair.first, instantiation, crs) +
                real_eval(*d.pair.second, instantiation, crs);
    case ELEMENT_SCALAR:
        return real_eval(*d.scalar.first, instantiation, crs) *
                real_eval(*d.scalar.second, instantiation, crs);
    case ELEMENT_BASE:
        return crs.getG1Base();
    default:
        ASSERT(false /* Unexpected data type */);
        return G1();
    }
}

G2 NIZKProof::real_eval(const G2Data &d, const ProofData &instantiation,
                        const CRS &crs) const {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if (static_cast<size_t>(d.index) < instantiation.privG2.size())
            return instantiation.privG2[d.index];
        return additionalG2[d.index - instantiation.privG2.size()].value;
    case ELEMENT_CONST_INDEX:
        return instantiation.pubG2[d.index];
    case ELEMENT_CONST_VALUE:
        return d.el;
    case ELEMENT_PAIR:
        return real_eval(*d.pair.first, instantiation, crs) +
                real_eval(*d.pair.second, instantiation, crs);
    case ELEMENT_SCALAR:
        return real_eval(*d.scalar.first, instantiation, crs) *
                real_eval(*d.scalar.second, instantiation, crs);
    case ELEMENT_BASE:
        return crs.getG2Base();
    default:
        ASSERT(false /* Unexpected data type */);
        return G2();
    }
}

GT NIZKProof::real_eval(const GTData &d, const ProofData &instantiation,
                        const CRS &crs) const {
    switch (d.type) {
    case ELEMENT_CONST_INDEX:
        return instantiation.pubGT[d.index];
    case ELEMENT_CONST_VALUE:
        return d.el;
    case ELEMENT_PAIR:
        return real_eval(*d.pair.first, instantiation, crs) *
                real_eval(*d.pair.second, instantiation, crs);
    case ELEMENT_PAIRING:
        return GT::pairing(real_eval(*d.pring.first, instantiation, crs),
                real_eval(*d.pring.second, instantiation, crs));
    case ELEMENT_BASE:
        return crs.getGTBase();
    default:
        ASSERT(false /* Unexpected data type */);
        return GT();
    }
}

void scalarCombine(const G1Commit &c1, const G2Commit &c2, ProofEls &p) {
    p.p2_v.type = VALUE_NULL;
    p.p2_w.type = VALUE_NULL;
    switch (c1.type) {
    case COMMIT_PRIV:
        p.p2_w = c2.c;
        p.p2_w *= c1.s;
        break;
    case COMMIT_ENC:
        p.p2_v = c2.c;
        p.p2_v *= c1.r;
    case COMMIT_PUB:
        break;
    }
    p.p1_v.type = VALUE_NULL;
    p.p1_w.type = VALUE_NULL;
    switch (c2.type) {
    case COMMIT_PRIV:
        p.p1_w = c1.c;
        p.p1_w *= c2.s;
        break;
    case COMMIT_ENC:
        p.p1_v = c1.c;
        p.p1_v *= c2.r;
    case COMMIT_PUB:
        break;
    }
}

void NIZKProof::getProof(const FpData &d, const CRS &crs) const {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addPiG1(el1.p1_v, el2.p1_v, proofEl->p1_v, crs);
            addPiG1(el1.p1_w, el2.p1_w, proofEl->p1_w, crs);
            addPiG2(el1.p2_v, el2.p2_v, proofEl->p2_v, crs);
            addPiG2(el1.p2_w, el2.p2_w, proofEl->p2_w, crs);
            return;
        }
    case ELEMENT_SCALAR:
        {
            getLeft(*d.pair.first, crs);
            getRight(*d.pair.second, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            scalarCombine(el1, el2, *proofEl);
            return;
        }
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getProof(const G1Data &d, const CRS &crs) const {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addPiG1(el1.p1_v, el2.p1_v, proofEl->p1_v, crs);
            addPiG1(el1.p1_w, el2.p1_w, proofEl->p1_w, crs);
            addPiG2(el1.p2_v, el2.p2_v, proofEl->p2_v, crs);
            addPiG2(el1.p2_w, el2.p2_w, proofEl->p2_w, crs);
            return;
        }
    case ELEMENT_SCALAR:
        {
            getLeft(*d.scalar.second, crs);
            getRight(*d.scalar.first, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            scalarCombine(el1, el2, *proofEl);
            return;
        }
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getProof(const G2Data &d, const CRS &crs) const {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addPiG1(el1.p1_v, el2.p1_v, proofEl->p1_v, crs);
            addPiG1(el1.p1_w, el2.p1_w, proofEl->p1_w, crs);
            addPiG2(el1.p2_v, el2.p2_v, proofEl->p2_v, crs);
            addPiG2(el1.p2_w, el2.p2_w, proofEl->p2_w, crs);
            return;
        }
    case ELEMENT_SCALAR:
        {
            getLeft(*d.scalar.first, crs);
            getRight(*d.scalar.second, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            scalarCombine(el1, el2, *proofEl);
            return;
        }
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getProof(const GTData &d, const CRS &crs) const {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addPiG1(el1.p1_v, el2.p1_v, proofEl->p1_v, crs);
            addPiG1(el1.p1_w, el2.p1_w, proofEl->p1_w, crs);
            addPiG2(el1.p2_v, el2.p2_v, proofEl->p2_v, crs);
            addPiG2(el1.p2_w, el2.p2_w, proofEl->p2_w, crs);
            return;
        }
    case ELEMENT_PAIRING:
        {
            getLeft(*d.pring.first, crs);
            getRight(*d.pring.second, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pring.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pring.second->d);
            scalarCombine(el1, el2, *proofEl);
            return;
        }
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getLeft(const FpData &d, const CRS &crs) const {
    if (d.d) return;
    G1Commit *c1 = new G1Commit;
    d.d = reinterpret_cast<void*>(c1);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        c1->type = COMMIT_PUB;
        c1->c.type = VALUE_Fp;
        c1->c.fpValue = d.el;
        return;
    case ELEMENT_PAIR:
        {
            getLeft(*d.pair.first, crs);
            getLeft(*d.pair.second, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G1Commit &el2 =
                    *reinterpret_cast<const G1Commit*>(d.pair.second->d);
            addCommitG1(el1, el2, *c1, crs);
            return;
        }
    case ELEMENT_BASE:
        c1->type = COMMIT_PUB;
        c1->c.type = VALUE_Fp;
        c1->c.fpValue = Fp::getUnit();
        return;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getLeft(const G1Data &d, const CRS &crs) const {
    if (d.d) return;
    G1Commit *c1 = new G1Commit;
    d.d = reinterpret_cast<void*>(c1);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        c1->type = COMMIT_PUB;
        c1->c.type = VALUE_G;
        c1->c.b1Value._2 = d.el;
        return;
    case ELEMENT_PAIR:
        {
            getLeft(*d.pair.first, crs);
            getLeft(*d.pair.second, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G1Commit &el2 =
                    *reinterpret_cast<const G1Commit*>(d.pair.second->d);
            addCommitG1(el1, el2, *c1, crs);
            return;
        }
    case ELEMENT_BASE:
        c1->type = COMMIT_PUB;
        c1->c.type = VALUE_G;
        c1->c.b1Value._2 = crs.getG1Base();
        return;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getRight(const FpData &d, const CRS &crs) const {
    if (d.d) return;
    G2Commit *c2 = new G2Commit;
    d.d = reinterpret_cast<void*>(c2);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        c2->type = COMMIT_PUB;
        c2->c.type = VALUE_Fp;
        c2->c.fpValue = d.el;
        return;
    case ELEMENT_PAIR:
        {
            getRight(*d.pair.first, crs);
            getRight(*d.pair.second, crs);
            const G2Commit &el1 =
                    *reinterpret_cast<const G2Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            addCommitG2(el1, el2, *c2, crs);
            return;
        }
    case ELEMENT_BASE:
        c2->type = COMMIT_PUB;
        c2->c.type = VALUE_Fp;
        c2->c.fpValue = Fp::getUnit();
        return;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void NIZKProof::getRight(const G2Data &d, const CRS &crs) const {
    if (d.d) return;
    G2Commit *c2 = new G2Commit;
    d.d = reinterpret_cast<void*>(c2);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        c2->type = COMMIT_PUB;
        c2->c.type = VALUE_G;
        c2->c.b2Value._2 = d.el;
        return;
    case ELEMENT_PAIR:
        {
            getRight(*d.pair.first, crs);
            getRight(*d.pair.second, crs);
            const G2Commit &el1 =
                    *reinterpret_cast<const G2Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            addCommitG2(el1, el2, *c2, crs);
            return;
        }
    case ELEMENT_BASE:
        c2->type = COMMIT_PUB;
        c2->c.type = VALUE_G;
        c2->c.b2Value._2 = crs.getG2Base();
        return;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void removeProof(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeft(*d.pair.first);
        removeRight(*d.pair.second);
        break;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void removeProof(const G1Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeft(*d.scalar.second);
        removeRight(*d.scalar.first);
        break;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void removeProof(const G2Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeft(*d.scalar.first);
        removeRight(*d.scalar.second);
        break;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void removeProof(const GTData &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_PAIRING:
        removeLeft(*d.pring.first);
        removeRight(*d.pring.second);
        break;
    default:
        ASSERT(false /* Unexpected data type */);
    }
}

void removeLeft(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<G1Commit*>(d.d);
    d.d = NULL;
    if (d.type == ELEMENT_PAIR) {
        removeLeft(*d.pair.first);
        removeLeft(*d.pair.second);
    }
}

void removeLeft(const G1Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<G1Commit*>(d.d);
    d.d = NULL;
    if (d.type == ELEMENT_PAIR) {
        removeLeft(*d.pair.first);
        removeLeft(*d.pair.second);
    }
}

void removeRight(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<G2Commit*>(d.d);
    d.d = NULL;
    if (d.type == ELEMENT_PAIR) {
        removeRight(*d.pair.first);
        removeRight(*d.pair.second);
    }
}

void removeRight(const G2Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<G2Commit*>(d.d);
    d.d = NULL;
    if (d.type == ELEMENT_PAIR) {
        removeRight(*d.pair.first);
        removeRight(*d.pair.second);
    }
}

} /* End of namespace nizk */
