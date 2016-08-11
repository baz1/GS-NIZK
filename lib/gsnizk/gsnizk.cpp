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
    case ELEMENT_SCALAR:
        scalar.~ScalarGT();
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

GTElement operator^(const GTElement &e, const FpElement &s) {
    GTData *d = new GTData(ELEMENT_SCALAR);
    new (&d->scalar) ScalarGT(s.data, e.data);
    return GTElement(std::shared_ptr<GTData>(d));
}

GTElement e(const G1Element &a, const G2Element &b) {
    GTData *d = new GTData(ELEMENT_PAIRING);
    new (&d->pring) PairingGT(a.data, b.data);
    return GTElement(std::shared_ptr<GTData>(d));
}

GTElement GTVar(int index) {
    GTData *d = new GTData(ELEMENT_VARIABLE);
    d->index = index;
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

#define INDEX_TYPE_Fp   0
#define INDEX_TYPE_G1   1
#define INDEX_TYPE_G2   2
#define INDEX_TYPE_GT   3

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
        node->type = SAT_NODE_INDEX;
        node->idx.index_type = INDEX_TYPE_Fp;
        node->idx.index = d.index;
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
    case ELEMENT_BASE:
        node->type = SAT_NODE_TRUE;
        return node;
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
    case ELEMENT_VARIABLE:
        node->type = SAT_NODE_INDEX;
        node->idx.index_type = INDEX_TYPE_GT;
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

void countIndexes(SAT_NODE *node, std::vector<int> cnt[4]) {
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
    cnt[2].clear();
    cnt[2].resize(val[2].size(), 0);
    cnt[3].clear();
    cnt[3].resize(val[3].size(), 0);
    countIndexes(root, cnt);
    int max = 0, mi, mj;
    for (int i = 4; i--;) {
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
    std::vector<int> valcp[4];
    valcp[0] = val[0];
    valcp[1] = val[1];
    valcp[2] = val[2];
    valcp[3] = val[3];
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
    val[2] = valcp[2];
    val[3] = valcp[3];
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
              checkIndexesSet(varsGT) && checkIndexesSet(cstsGT))) {
            varsFp.clear();
            cstsFp.clear();
            varsG1.clear();
            cstsG1.clear();
            varsG2.clear();
            cstsG2.clear();
            varsGT.clear();
            cstsGT.clear();
            return false;
        }
    }
    // TODO rewrite equations
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
        std::vector<int> cnt[4];
        sEnc[0].resize(varsFp.size(), SAT_VALUE_UNSET);
        cnt[0].reserve(varsFp.size());
        sEnc[1].resize(varsG1.size(), SAT_VALUE_UNSET);
        cnt[1].reserve(varsG1.size());
        sEnc[2].resize(varsG2.size(), SAT_VALUE_UNSET);
        cnt[2].reserve(varsG2.size());
        sEnc[3].resize(varsGT.size(), SAT_VALUE_UNSET);
        cnt[3].reserve(varsGT.size());
        if (tryPermutation(root, sEnc, cnt) < 0)
            throw "Cannot use ZK with the equations provided (in gsnizk)";
        /* Converting to 0/1 (boolean) values; 1 for encrypted */
        for (int i = 4; i--;) {
            for (int j = sEnc[i].size(); j--;)
                --sEnc[i][j];
        }
    }
    // TODO fill the equation proof types and varsFpInB1
    fixed = true;
    return true;
}


bool NIZKProof::verifySolution(const ProofData &instantiation,
                               const CRS &crs) const {
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

struct G1Commit {
    B1 c;
    Fp r, s;
};

struct G2Commit {
    B2 c;
    Fp r, s;
};

enum VALUE_TYPE {
    VALUE_Fp,
    VALUE_G,
    VALUE_B
};

struct LeftG1 {
    VALUE_TYPE type;
    Fp fpValue;
    B1 b1Value;
    inline LeftG1(VALUE_TYPE type) : type(type) {}
};

struct RightG2 {
    VALUE_TYPE type;
    Fp fpValue;
    B2 b2Value;
    inline RightG2(VALUE_TYPE type) : type(type) {}
};

void NIZKProof::writeProof(std::ostream &stream, const CRS &crs,
                           const ProofData &instantiation) const {
    if (!checkInstantiation(instantiation))
        throw "Wrong instantiation in NIZKProof::writeProof!";
    for (const AdditionalFp &aFp : additionalFp)
        aFp.value = real_eval(*aFp.formula, instantiation, crs);
    for (const AdditionalG1 &aG1 : additionalG1)
        aG1.value = real_eval(*aG1.formula, instantiation, crs);
    for (const AdditionalG2 &aG2 : additionalG2)
        aG2.value = real_eval(*aG2.formula, instantiation, crs);
    ASSERT(varsFp.size() == varsFpInB1.size());
    G1Commit c1;
    G2Commit c2;
    int j = varsFp.size(), i = additionalFp.size();
    while ((--j, i--) > 0) {
        if (varsFpInB1[j]) {
            c1.r = Fp::getRand();
            c1.c = B1::commit(additionalFp[i].value, c1.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(&c1);
            stream << c1.c;
        } else {
            c2.r = Fp::getRand();
            c2.c = B2::commit(additionalFp[i].value, c2.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(&c2);
            stream << c2.c;
        }
    }
    while (j-- > 0) {
        if (varsFpInB1[j]) {
            c1.r = Fp::getRand();
            c1.c = B1::commit(instantiation.privFp[j], c1.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(&c1);
            stream << c1.c;
        } else {
            c2.r = Fp::getRand();
            c2.c = B2::commit(instantiation.privFp[j], c2.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(&c2);
            stream << c2.c;
        }
    }
    j = varsG1.size();
    i = additionalG1.size();
    while ((--j, i--) > 0) {
        c1.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[1][j])) {
            c1.c = B1::commit(additionalG1[i].value, c1.r, crs);
        } else {
            c1.s = Fp::getRand();
            c1.c = B1::commit(additionalG1[i].value, c1.r, c1.s, crs);
        }
        varsG1[j]->d = reinterpret_cast<void*>(&c1);
        stream << c1.c;
    }
    while (j-- > 0) {
        c1.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[1][j])) {
            c1.c = B1::commit(instantiation.privG1[j].value, c1.r, crs);
        } else {
            c1.s = Fp::getRand();
            c1.c = B1::commit(instantiation.privG1[j].value, c1.r, c1.s, crs);
        }
        varsG1[j]->d = reinterpret_cast<void*>(&c1);
        stream << c1.c;
    }
    j = varsG2.size();
    i = additionalG2.size();
    while ((--j, i--) > 0) {
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[2][j])) {
            c2.c = B2::commit(additionalG2[i].value, c2.r, crs);
        } else {
            c2.s = Fp::getRand();
            c2.c = B2::commit(additionalG2[i].value, c2.r, c2.s, crs);
        }
        varsG2[j]->d = reinterpret_cast<void*>(&c2);
        stream << c2.c;
    }
    while (j-- > 0) {
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[2][j])) {
            c2.c = B2::commit(instantiation.privG2[j].value, c2.r, crs);
        } else {
            c2.s = Fp::getRand();
            c2.c = B2::commit(instantiation.privG2[j].value, c2.r, c2.s, crs);
        }
        varsG2[j]->d = reinterpret_cast<void*>(&c2);
        stream << c2.c;
    }
    // TODO
}

bool NIZKProof::checkInstantiation(const ProofData &instantiation) const {
    return (instantiation.pubFp.size() != cstsFp.size()) &&
        (instantiation.pubG1.size() != cstsG1.size()) &&
        (instantiation.pubG2.size() != cstsG2.size()) &&
        (instantiation.pubGT.size() != cstsGT.size()) &&
        (instantiation.privFp.size() + additionalFp.size() == varsFp.size()) &&
        (instantiation.privG1.size() + additionalG1.size() == varsG1.size()) &&
        (instantiation.privG2.size() + additionalG2.size() == varsG2.size()) &&
        (instantiation.privGT.size() == varsGT.size());
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
    case ELEMENT_VARIABLE:
        if (varsGT.size() <= static_cast<size_t>(d->index)) {
            varsGT.resize(d->index + 1);
            varsGT[d->index] = d;
        } else if (varsGT[d->index]) {
            d = varsGT[d->index];
        } else {
            varsGT[d->index] = d;
        }
        return;
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
    case ELEMENT_SCALAR:
        getIndexes(d->scalar.first);
        getIndexes(d->scalar.second);
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
    case ELEMENT_VARIABLE:
        return instantiation.privGT[d.index];
    case ELEMENT_CONST_INDEX:
        return instantiation.pubGT[d.index];
    case ELEMENT_CONST_VALUE:
        return d.el;
    case ELEMENT_PAIR:
        return real_eval(*d.pair.first, instantiation, crs) *
                real_eval(*d.pair.second, instantiation, crs);
    case ELEMENT_SCALAR:
        return real_eval(*d.scalar.second, instantiation, crs) ^
                real_eval(*d.scalar.first, instantiation, crs);
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

} /* End of namespace nizk */
