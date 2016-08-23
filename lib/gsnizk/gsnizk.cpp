/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "gsnizk.h"

/* Prioritize Qt's no-debug policy, if existent */
#if defined(QT_NO_DEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#ifdef DEBUG
#define ASSERT(X,Y) if (!(X)) { \
    std::cerr << "Error: Assert of " << #X << " at line " \
    << __LINE__ << " failed in gsnizk.cpp!" << std::endl \
    << Y << std::endl; \
    throw Y; \
    }
#else
#define ASSERT(X,Y) /* noop */
#endif

#include "bigendian.h"

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

GTElement GTBase() {
    GTData *d = new GTData(ELEMENT_BASE);
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
    case ELEMENT_BASE:
        node->type = SAT_NODE_TRUE;
        return node;
    default:
        ASSERT(false, "Unexpected data type");
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
        switch (node->pair.left->type) {
        case SAT_NODE_FALSE:
        {
            delete node->pair.left;
            SAT_NODE *tmp = node->pair.right;
            *node = *tmp;
            delete tmp;
            return;
        }
        case SAT_NODE_TRUE:
            delete node->pair.left;
            delNode(node->pair.right);
            node->type = SAT_NODE_TRUE;
            return;
        default:
            break;
        }
        switch (node->pair.right->type) {
        case SAT_NODE_FALSE:
        {
            delete node->pair.right;
            SAT_NODE *tmp = node->pair.left;
            *node = *tmp;
            delete tmp;
            return;
        }
        case SAT_NODE_TRUE:
            delNode(node->pair.left);
            delete node->pair.right;
            node->type = SAT_NODE_TRUE;
            return;
        default:
            break;
        }
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
    ASSERT((value == SAT_NODE_TRUE) || (value == SAT_NODE_FALSE),
           "Wrong value");
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

bool isZK(const FpData &d) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_BASE:
        return true;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        return false;
    case ELEMENT_PAIR:
        return isZK(*d.pair.first) && isZK(*d.pair.second);
    case ELEMENT_SCALAR:
        return isZK(*d.pair.first) || isZK(*d.pair.second);
    default:
        ASSERT(false, "Unexpected data type");
        return false;
    }
}

bool isZK(const G1Data &d, bool normalCommit) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        return normalCommit;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        return false;
    case ELEMENT_PAIR:
        return isZK(*d.pair.first, normalCommit) &&
                isZK(*d.pair.second, normalCommit);
    case ELEMENT_SCALAR:
        return isZK(*d.scalar.first) || isZK(*d.scalar.second, normalCommit);
    case ELEMENT_BASE:
        return true;
    default:
        ASSERT(false, "Unexpected data type");
        return false;
    }
}

bool isZK(const G2Data &d, bool normalCommit) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        return normalCommit;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        return false;
    case ELEMENT_PAIR:
        return isZK(*d.pair.first, normalCommit) &&
                isZK(*d.pair.second, normalCommit);
    case ELEMENT_SCALAR:
        return isZK(*d.scalar.first) || isZK(*d.scalar.second, normalCommit);
    case ELEMENT_BASE:
        return true;
    default:
        ASSERT(false, "Unexpected data type");
        return false;
    }
}

bool isZK(const GTData &d, bool normalCommit) {
    switch (d.type) {
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        return false;
    case ELEMENT_PAIR:
        return isZK(*d.pair.first, normalCommit) &&
                isZK(*d.pair.second, normalCommit);
    case ELEMENT_PAIRING:
        return isZK(*d.pring.first, normalCommit) ||
                isZK(*d.pring.second, normalCommit);
    case ELEMENT_BASE:
        return true;
    default:
        ASSERT(false, "Unexpected data type");
        return false;
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
    int max = 0, mi = 0, mj = 0;
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
            ASSERT(false, "Unexpected error");
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

void endRewrite(const FpData &d);
void endRewrite(const G1Data &d);
void endRewrite(const G2Data &d);
void endRewriteLeft(const FpData &d);
void endRewriteLeft(const G1Data &d);
void endRewriteRight(const FpData &d);
void endRewriteRight(const G2Data &d);

bool NIZKProof::endEquations() {
    /* Subsequent calls are ignored. */
    if (fixed) return true;
    /* Getting indexes, pointers and removing duplicates */
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
    /* Rewrite equations */
    varsFpInB1.resize(varsFp.size());
    cstsFpInB1.resize(cstsFp.size());
    {
        int checkedFp = 0, checkedG1 = 0, checkedG2 = 0, checkedGT = 0;
        while (true) {
            if ((checkedFp == (int) eqsFp.size()) &&
                    (checkedG1 == (int) eqsG1.size()) &&
                    (checkedG2 == (int) eqsG2.size()) &&
                    (checkedGT == (int) eqsGT.size())) break;
            while (checkedFp < (int) eqsFp.size()) {
                checkoutAsFp(eqsFp[checkedFp].first);
                checkoutAsFp(eqsFp[checkedFp].second);
                ++checkedFp;
            }
            while (checkedG1 < (int) eqsG1.size()) {
                checkoutAsG1(eqsG1[checkedG1].first);
                checkoutAsG1(eqsG1[checkedG1].second);
                ++checkedG1;
            }
            while (checkedG2 < (int) eqsG2.size()) {
                checkoutAsG2(eqsG2[checkedG2].first);
                checkoutAsG2(eqsG2[checkedG2].second);
                ++checkedG2;
            }
            while (checkedGT < (int) eqsGT.size()) {
                checkoutAsGT(eqsGT[checkedGT].first);
                checkoutAsGT(eqsGT[checkedGT].second);
                ++checkedGT;
            }
        }
        for (int i = eqsFp.size(); i-- > 0;) {
            const FpData &left = *eqsFp[i].first;
            const FpData &right = *eqsFp[i].second;
            endRewrite(left);
            endRewrite(right);
        }
        for (int i = eqsG1.size(); i-- > 0;) {
            const G1Data &left = *eqsG1[i].first;
            const G1Data &right = *eqsG1[i].second;
            endRewrite(left);
            endRewrite(right);
        }
        for (int i = eqsG2.size(); i-- > 0;) {
            const G2Data &left = *eqsG2[i].first;
            const G2Data &right = *eqsG2[i].second;
            endRewrite(left);
            endRewrite(right);
        }
    }
    /* Selected Encryption selection */
    zk = true;
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
    } else {
        bool normalCommit = (type == NormalCommit);
        for (const PairFp &p : eqsFp) {
            if ((!isZK(*p.first)) || (p.second && (!isZK(*p.second)))) {
                zk = false;
                goto endZKTests;
            }
        }
        for (const PairG1 &p : eqsG1) {
            if ((!isZK(*p.first, normalCommit)) ||
                    (p.second && (!isZK(*p.second, normalCommit)))) {
                zk = false;
                goto endZKTests;
            }
        }
        for (const PairG2 &p : eqsG2) {
            if ((!isZK(*p.first, normalCommit)) ||
                    (p.second && (!isZK(*p.second, normalCommit)))) {
                zk = false;
                goto endZKTests;
            }
        }
        for (const PairGT &p : eqsGT) {
            if ((!isZK(*p.first, normalCommit)) ||
                    (p.second && (!isZK(*p.second, normalCommit)))) {
                zk = false;
                goto endZKTests;
            }
        }
endZKTests:
        ;
    }
    /* Equation types for the proofs */
    getEqProofTypes();
    fixed = true;
    return true;
}

inline void put_integer(std::ostream &stream, const int v) {
    ASSERT(stream.good(), "Stream is not good");
    ASSERT(v < 0x10000, "Integer value is unexpectedly big");
    uint16_t i = htons(static_cast<uint16_t>(v));
    stream.write(reinterpret_cast<const char*>(&i), 2);
    ASSERT(stream.good(), "Stream is not good");
}

inline int get_integer(std::istream &stream) {
    uint16_t i;
    ASSERT(stream.good(), "Stream is not good");
    stream.read(reinterpret_cast<char*>(&i), 2);
    ASSERT(!stream.fail(), "Stream failed");
    return static_cast<int>(ntohs(i));
}

std::ostream &operator<<(std::ostream &stream, const EqProofType &t) {
    ASSERT(stream.good(), "Stream is not good");
    stream.put(static_cast<char>(t));
    ASSERT(stream.good(), "Stream is not good");
    return stream;
}

std::istream &operator>>(std::istream &stream, EqProofType &t) {
    ASSERT(stream.good(), "Stream is not good");
    t = static_cast<EqProofType>(stream.get());
    ASSERT(!stream.fail(), "Stream failed");
    return stream;
}

template <typename T> inline void writeVectorToStream(std::ostream &stream,
        const std::vector<T> &v) {
    int i = v.size();
    put_integer(stream, i);
    while (i-- > 0)
        stream << v[i];
    ASSERT(stream.good(), "Stream is not good");
}

template <typename T> inline void readVectorFromStream(std::istream &stream,
        std::vector<T> &v) {
    int i = get_integer(stream);
    v.resize(i);
    while (i-- > 0)
        stream >> v[i];
}

void writeToStream(std::ostream &stream, const FpData &d) {
    ASSERT(stream.good(), "Stream is not good");
    stream.put(static_cast<char>(d.type));
    ASSERT(stream.good(), "Stream is not good");
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
        put_integer(stream, d.index);
        break;
    case ELEMENT_CONST_VALUE:
        stream << d.el;
        break;
    case ELEMENT_PAIR:
    case ELEMENT_SCALAR:
        writeToStream(stream, *d.pair.first);
        writeToStream(stream, *d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void writeToStream(std::ostream &stream, const G1Data &d) {
    ASSERT(stream.good(), "Stream is not good");
    stream.put(static_cast<char>(d.type));
    ASSERT(stream.good(), "Stream is not good");
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
        put_integer(stream, d.index);
        break;
    case ELEMENT_CONST_VALUE:
        stream << d.el;
        break;
    case ELEMENT_PAIR:
        writeToStream(stream, *d.pair.first);
        writeToStream(stream, *d.pair.second);
        break;
    case ELEMENT_SCALAR:
        writeToStream(stream, *d.scalar.second);
        writeToStream(stream, *d.scalar.first);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void writeToStream(std::ostream &stream, const G2Data &d) {
    ASSERT(stream.good(), "Stream is not good");
    stream.put(static_cast<char>(d.type));
    ASSERT(stream.good(), "Stream is not good");
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
        put_integer(stream, d.index);
        break;
    case ELEMENT_CONST_VALUE:
        stream << d.el;
        break;
    case ELEMENT_PAIR:
        writeToStream(stream, *d.pair.first);
        writeToStream(stream, *d.pair.second);
        break;
    case ELEMENT_SCALAR:
        writeToStream(stream, *d.scalar.first);
        writeToStream(stream, *d.scalar.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void writeToStream(std::ostream &stream, const GTData &d) {
    ASSERT(stream.good(), "Stream is not good");
    stream.put(static_cast<char>(d.type));
    ASSERT(stream.good(), "Stream is not good");
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
        put_integer(stream, d.index);
        break;
    case ELEMENT_CONST_VALUE:
        stream << d.el;
        break;
    case ELEMENT_PAIR:
        writeToStream(stream, *d.pair.first);
        writeToStream(stream, *d.pair.second);
        break;
    case ELEMENT_PAIRING:
        writeToStream(stream, *d.pring.first);
        writeToStream(stream, *d.pring.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::readFromStream(std::istream &stream,
                               std::shared_ptr<FpData> &dp,
                               int side) {
    int mtype, mindex;
    ASSERT(stream.good(), "Stream is not good");
    mtype = stream.get();
    ASSERT(!stream.fail(), "Stream failed");
    if (mtype <= 1) { /* ELEMENT_VARIABLE or ELEMENT_CONST_INDEX */
        ASSERT(side != 0, "Wrong data");
        mindex = get_integer(stream);
        if (mtype == ELEMENT_VARIABLE) {
            ASSERT((0 <= mindex) && (mindex < (int) varsFp.size()),
                    "Wrong data");
            if (varsFp[mindex]) {
                dp = varsFp[mindex];
                ASSERT((varsFpInB1[mindex] == (side < 0)) || (side == -2),
                        "Wrong data");
                return;
            } else {
                ASSERT(side != -2, "Wrong data");
                varsFp[mindex] = (dp = std::shared_ptr<FpData>(
                            new FpData((ElementType) mtype)));
                varsFpInB1[mindex] = (side < 0);
            }
        } else {
            ASSERT((0 <= mindex) && (mindex < (int) cstsFp.size()),
                    "Wrong data");
            if (cstsFp[mindex]) {
                dp = cstsFp[mindex];
                ASSERT((cstsFpInB1[mindex] == (side < 0)) || (side == -2),
                        "Wrong data");
                return;
            } else {
                ASSERT(side != -2, "Wrong data");
                cstsFp[mindex] = (dp = std::shared_ptr<FpData>(
                            new FpData((ElementType) mtype)));
                cstsFpInB1[mindex] = (side < 0);
            }
        }
        dp->index = mindex;
        return;
    }
    dp = std::shared_ptr<FpData>(new FpData((ElementType) mtype));
    switch (mtype) {
    case ELEMENT_CONST_VALUE:
        new (&dp->el) Fp();
        stream >> dp->el;
        return;
    case ELEMENT_PAIR:
        new (&dp->pair) PairFp();
        readFromStream(stream, dp->pair.first, side);
        readFromStream(stream, dp->pair.second, side);
        return;
    case ELEMENT_SCALAR:
        ASSERT((side == 0) || (side == -2), "Wrong data");
        new (&dp->pair) PairFp();
        readFromStream(stream, dp->pair.first, -1);
        readFromStream(stream, dp->pair.second, 1);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::readFromStream(std::istream &stream,
                               std::shared_ptr<G1Data> &dp) {
    int mtype, mindex;
    ASSERT(stream.good(), "Stream is not good");
    mtype = stream.get();
    ASSERT(!stream.fail(), "Stream failed");
    if (mtype <= 1) { /* ELEMENT_VARIABLE or ELEMENT_CONST_INDEX */
        mindex = get_integer(stream);
        if (mtype == ELEMENT_VARIABLE) {
            ASSERT((0 <= mindex) && (mindex < (int) varsG1.size()),
                    "Wrong data");
            if (varsG1[mindex]) {
                dp = varsG1[mindex];
                return;
            } else {
                varsG1[mindex] = (dp = std::shared_ptr<G1Data>(
                            new G1Data((ElementType) mtype)));
            }
        } else {
            ASSERT((0 <= mindex) && (mindex < (int) cstsG1.size()),
                    "Wrong data");
            if (cstsG1[mindex]) {
                dp = cstsG1[mindex];
                return;
            } else {
                cstsG1[mindex] = (dp = std::shared_ptr<G1Data>(
                            new G1Data((ElementType) mtype)));
            }
        }
        dp->index = mindex;
        return;
    }
    dp = std::shared_ptr<G1Data>(new G1Data((ElementType) mtype));
    switch (mtype) {
    case ELEMENT_CONST_VALUE:
        new (&dp->el) G1();
        stream >> dp->el;
        return;
    case ELEMENT_PAIR:
        new (&dp->pair) PairG1();
        readFromStream(stream, dp->pair.first);
        readFromStream(stream, dp->pair.second);
        return;
    case ELEMENT_SCALAR:
        new (&dp->scalar) ScalarG1();
        readFromStream(stream, dp->scalar.second);
        readFromStream(stream, dp->scalar.first, 1);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::readFromStream(std::istream &stream,
                               std::shared_ptr<G2Data> &dp) {
    int mtype, mindex;
    ASSERT(stream.good(), "Stream is not good");
    mtype = stream.get();
    ASSERT(!stream.fail(), "Stream failed");
    if (mtype <= 1) { /* ELEMENT_VARIABLE or ELEMENT_CONST_INDEX */
        mindex = get_integer(stream);
        if (mtype == ELEMENT_VARIABLE) {
            ASSERT((0 <= mindex) && (mindex < (int) varsG2.size()),
                    "Wrong data");
            if (varsG2[mindex]) {
                dp = varsG2[mindex];
                return;
            } else {
                varsG2[mindex] = (dp = std::shared_ptr<G2Data>(
                            new G2Data((ElementType) mtype)));
            }
        } else {
            ASSERT((0 <= mindex) && (mindex < (int) cstsG2.size()),
                    "Wrong data");
            if (cstsG2[mindex]) {
                dp = cstsG2[mindex];
                return;
            } else {
                cstsG2[mindex] = (dp = std::shared_ptr<G2Data>(
                            new G2Data((ElementType) mtype)));
            }
        }
        dp->index = mindex;
        return;
    }
    dp = std::shared_ptr<G2Data>(new G2Data((ElementType) mtype));
    switch (mtype) {
    case ELEMENT_CONST_VALUE:
        new (&dp->el) G2();
        stream >> dp->el;
        return;
    case ELEMENT_PAIR:
        new (&dp->pair) PairG2();
        readFromStream(stream, dp->pair.first);
        readFromStream(stream, dp->pair.second);
        return;
    case ELEMENT_SCALAR:
        new (&dp->scalar) ScalarG2();
        readFromStream(stream, dp->scalar.first, -1);
        readFromStream(stream, dp->scalar.second);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::readFromStream(std::istream &stream,
                               std::shared_ptr<GTData> &dp) {
    int mtype, mindex;
    ASSERT(stream.good(), "Stream is not good");
    mtype = stream.get();
    ASSERT(!stream.fail(), "Stream failed");
    if (mtype == ELEMENT_CONST_INDEX) {
        mindex = get_integer(stream);
        ASSERT((0 <= mindex) && (mindex < (int) cstsGT.size()),
                "Wrong data");
        if (cstsGT[mindex]) {
            dp = cstsGT[mindex];
            return;
        } else {
            cstsGT[mindex] = (dp = std::shared_ptr<GTData>(
                        new GTData((ElementType) mtype)));
        }
        dp->index = mindex;
        return;
    }
    dp = std::shared_ptr<GTData>(new GTData((ElementType) mtype));
    switch (mtype) {
    case ELEMENT_CONST_VALUE:
        new (&dp->el) GT();
        stream >> dp->el;
        return;
    case ELEMENT_PAIR:
        new (&dp->pair) PairGT();
        readFromStream(stream, dp->pair.first);
        readFromStream(stream, dp->pair.second);
        return;
    case ELEMENT_PAIRING:
        new (&dp->pring) PairingGT();
        readFromStream(stream, dp->pring.first);
        readFromStream(stream, dp->pring.second);
        return;
    case ELEMENT_BASE:
        return;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

std::ostream &operator<<(std::ostream &stream, const PairFp &p) {
    writeToStream(stream, *p.first);
    writeToStream(stream, *p.second);
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const PairG1 &p) {
    writeToStream(stream, *p.first);
    writeToStream(stream, *p.second);
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const PairG2 &p) {
    writeToStream(stream, *p.first);
    writeToStream(stream, *p.second);
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const PairGT &p) {
    writeToStream(stream, *p.first);
    writeToStream(stream, *p.second);
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const AdditionalFp &a) {
    writeToStream(stream, *a.formula);
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const AdditionalG1 &a) {
    writeToStream(stream, *a.formula);
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const AdditionalG2 &a) {
    writeToStream(stream, *a.formula);
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const NIZKProof &p) {
    if (!p.fixed) return stream;
    ASSERT(stream.good(), "Stream is not good");
    stream.put(static_cast<char>(p.type));
    stream.put(static_cast<char>(p.zk));
    ASSERT(stream.good(), "Stream is not good");
    put_integer(stream, p.varsFp.size());
    put_integer(stream, p.cstsFp.size());
    put_integer(stream, p.varsG1.size());
    put_integer(stream, p.cstsG1.size());
    put_integer(stream, p.varsG2.size());
    put_integer(stream, p.cstsG2.size());
    put_integer(stream, p.cstsGT.size());
    ASSERT(stream.good(), "Stream is not good");
    writeVectorToStream(stream, p.eqsFp);
    writeVectorToStream(stream, p.eqsG1);
    writeVectorToStream(stream, p.eqsG2);
    writeVectorToStream(stream, p.eqsGT);
    writeVectorToStream(stream, p.sEnc[0]);
    writeVectorToStream(stream, p.sEnc[1]);
    writeVectorToStream(stream, p.tFp);
    writeVectorToStream(stream, p.tG1);
    writeVectorToStream(stream, p.tG2);
    writeVectorToStream(stream, p.tGT);
    writeVectorToStream(stream, p.additionalFp);
    writeVectorToStream(stream, p.additionalG1);
    writeVectorToStream(stream, p.additionalG2);
    ASSERT(stream.good(), "Stream is not good");
    return stream;
}

NIZKProof::NIZKProof(std::istream &stream) : fixed(true) {
    ASSERT(stream.good(), "Stream is not good");
    type = static_cast<CommitType>(stream.get());
    ASSERT(stream.good(), "Stream is not good");
    zk = static_cast<bool>(stream.get());
    ASSERT(stream.good(), "Stream is not good");
    int s;
    varsFp.resize(s = get_integer(stream));
    varsFpInB1.resize(s);
    cstsFp.resize(s = get_integer(stream));
    cstsFpInB1.resize(s);
    varsG1.resize(get_integer(stream));
    cstsG1.resize(get_integer(stream));
    varsG2.resize(get_integer(stream));
    cstsG2.resize(get_integer(stream));
    cstsGT.resize(get_integer(stream));
    eqsFp.resize(s = get_integer(stream));
    while (s-- > 0) {
        readFromStream(stream, eqsFp[s].first, 0);
        readFromStream(stream, eqsFp[s].second, 0);
    }
    eqsG1.resize(s = get_integer(stream));
    while (s-- > 0) {
        readFromStream(stream, eqsG1[s].first);
        readFromStream(stream, eqsG1[s].second);
    }
    eqsG2.resize(s = get_integer(stream));
    while (s-- > 0) {
        readFromStream(stream, eqsG2[s].first);
        readFromStream(stream, eqsG2[s].second);
    }
    eqsGT.resize(s = get_integer(stream));
    while (s-- > 0) {
        readFromStream(stream, eqsGT[s].first);
        readFromStream(stream, eqsGT[s].second);
    }
    readVectorFromStream(stream, sEnc[0]);
    readVectorFromStream(stream, sEnc[1]);
    readVectorFromStream(stream, tFp);
    readVectorFromStream(stream, tG1);
    readVectorFromStream(stream, tG2);
    readVectorFromStream(stream, tGT);
    additionalFp.resize(s = get_integer(stream));
    while (s-- > 0)
        readFromStream(stream, additionalFp[s].formula, -2);
    additionalG1.resize(s = get_integer(stream));
    while (s-- > 0)
        readFromStream(stream, additionalG1[s].formula);
    additionalG2.resize(s = get_integer(stream));
    while (s-- > 0)
        readFromStream(stream, additionalG2[s].formula);
}

std::istream &operator>>(std::istream &stream, NIZKProof &p) {
    p.~NIZKProof();
    new (&p) NIZKProof(stream);
    return stream;
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
    COMMIT_PUB = 0,
    COMMIT_ENC = 1,
    COMMIT_PRIV = 2
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
    if (a.type == VALUE_NULL) {
        result = b;
        return;
    }
    if (b.type == VALUE_NULL) {
        result = a;
        return;
    }
    result.type = ((a.type == b.type) ? a.type : VALUE_B);
    if (result.type == VALUE_Fp) {
        result.fpValue = a.fpValue + b.fpValue;
        return;
    }
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
    if (a.type == VALUE_NULL) {
        result = b;
        return;
    }
    if (b.type == VALUE_NULL) {
        result = a;
        return;
    }
    result.type = ((a.type == b.type) ? a.type : VALUE_B);
    if (result.type == VALUE_Fp) {
        result.fpValue = a.fpValue + b.fpValue;
        return;
    }
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
    if (b.type == VALUE_NULL) {
        result = a;
        return;
    }
    if (a.type == VALUE_NULL) {
        result = b;
        switch (result.type) {
        case VALUE_Fp:
            result.fpValue = -result.fpValue;
            break;
        case VALUE_G:
            result.b1Value._2 = -result.b1Value._2;
            break;
        default:
            result.b1Value = -result.b1Value;
            break;
        }
        return;
    }
    result.type = ((a.type == b.type) ? a.type : VALUE_B);
    if (result.type == VALUE_Fp) {
        result.fpValue = a.fpValue - b.fpValue;
        return;
    }
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
    if (b.type == VALUE_NULL) {
        result = a;
        return;
    }
    if (a.type == VALUE_NULL) {
        result = b;
        switch (result.type) {
        case VALUE_Fp:
            result.fpValue = -result.fpValue;
            break;
        case VALUE_G:
            result.b2Value._2 = -result.b2Value._2;
            break;
        default:
            result.b2Value = -result.b2Value;
            break;
        }
        return;
    }
    result.type = ((a.type == b.type) ? a.type : VALUE_B);
    if (result.type == VALUE_Fp) {
        result.fpValue = a.fpValue - b.fpValue;
        return;
    }
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
    cr.type = ((c1.type < c2.type) ? c2.type : c1.type);
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
    cr.c.type = ((c1.c.type == c2.c.type) ? c1.c.type : VALUE_B);
    if (c1.c.type == VALUE_Fp) {
        if (c2.c.type == VALUE_Fp) {
            cr.c.fpValue = c1.c.fpValue + c2.c.fpValue;
        } else {
            cr.c.type = VALUE_Fp;
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
    cr.c.type = ((c1.c.type == c2.c.type) ? c1.c.type : VALUE_B);
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

void getProof(const FpData &d, const CRS &crs);
void getProof(const G1Data &d, const CRS &crs);
void getProof(const G2Data &d, const CRS &crs);
void getProof(const GTData &d, const CRS &crs);
void getLeft(const FpData &d, const CRS &crs);
void getLeft(const G1Data &d, const CRS &crs);
void getRight(const FpData &d, const CRS &crs);
void getRight(const G2Data &d, const CRS &crs);

void removeProof(const FpData &d);
void removeProof(const G1Data &d);
void removeProof(const G2Data &d);
void removeProof(const GTData &d);
void removeLeft(const FpData &d);
void removeLeft(const G1Data &d);
void removeRight(const FpData &d);
void removeRight(const G2Data &d);

void convToB(PiG1 &v, const CRS &crs) {
    switch (v.type) {
    case VALUE_NULL:
        break;
    case VALUE_Fp:
        v.b1Value = v.fpValue * crs.getB1Unit();
        break;
    case VALUE_G:
        break;
    case VALUE_B:
        return;
    }
    v.type = VALUE_B;
}

void convToB(PiG2 &v, const CRS &crs) {
    switch (v.type) {
    case VALUE_NULL:
        break;
    case VALUE_Fp:
        v.b2Value = v.fpValue * crs.getB2Unit();
        break;
    case VALUE_G:
        break;
    case VALUE_B:
        return;
    }
    v.type = VALUE_B;
}

void NIZKProof::writeEqProof(std::ostream &stream, const void *leftp,
                  const void *rightp, EqProofType expectedType,
                  const CRS &crs) const {
    const ProofEls &left = *reinterpret_cast<const ProofEls*>(leftp);
    const ProofEls &right = *reinterpret_cast<const ProofEls*>(rightp);
    ProofEls result;
    subPiG1(left.p1_v, right.p1_v, result.p1_v, crs);
    subPiG1(left.p1_w, right.p1_w, result.p1_w, crs);
    subPiG2(left.p2_v, right.p2_v, result.p2_v, crs);
    subPiG2(left.p2_w, right.p2_w, result.p2_w, crs);
    switch (expectedType) {
    case EQ_TYPE_PPE:
    {
        Fp alpha = Fp::getRand(), beta = Fp::getRand();
        Fp gamma = Fp::getRand(), delta = Fp::getRand();
        convToB(result.p1_v, crs);
        convToB(result.p1_w, crs);
        convToB(result.p2_v, crs);
        convToB(result.p2_w, crs);
        if (crs.type == CRS_TYPE_PRIVATE) {
            result.p2_v.b2Value += (alpha + crs.i2 * beta) * crs.v2;
            result.p2_w.b2Value += (gamma + crs.i2 * delta) * crs.v2;
            result.p1_v.b1Value -= (alpha + crs.i1 * gamma) * crs.v1;
            result.p1_w.b1Value -= (beta + crs.i1 * delta) * crs.v1;
        } else {
            result.p2_v.b2Value += alpha * crs.v2 + beta * crs.w2;
            result.p2_w.b2Value += gamma * crs.v2 + delta * crs.w2;
            result.p1_v.b1Value -= alpha * crs.v1 + gamma * crs.w1;
            result.p1_w.b1Value -= beta * crs.v1 + delta * crs.w1;
        }
        stream << result.p1_v.b1Value;
        stream << result.p1_w.b1Value;
        stream << result.p2_v.b2Value;
        stream << result.p2_w.b2Value;
        break;
    }
    case EQ_TYPE_PEnc_G:
    case EQ_TYPE_ME_H:
    {
        Fp alpha = Fp::getRand(), beta = Fp::getRand();
        convToB(result.p1_v, crs);
        convToB(result.p1_w, crs);
        convToB(result.p2_v, crs);
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        if (crs.type == CRS_TYPE_PRIVATE) {
            result.p2_v.b2Value += (alpha + crs.i2 * beta) * crs.v2;
        } else {
            result.p2_v.b2Value += alpha * crs.v2 + beta * crs.w2;
        }
        result.p1_v.b1Value -= alpha * crs.v1;
        result.p1_w.b1Value -= beta * crs.v1;
        stream << result.p1_v.b1Value;
        stream << result.p1_w.b1Value;
        stream << result.p2_v.b2Value;
        break;
    }
    case EQ_TYPE_PConst_G:
    {
        ASSERT((result.p1_v.type == VALUE_NULL) ||
                (result.p1_v.type == VALUE_G), "Unexpected type");
        ASSERT((result.p1_w.type == VALUE_NULL) ||
                (result.p1_w.type == VALUE_G), "Unexpected type");
        ASSERT(result.p2_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        stream << result.p1_v.b1Value._2;
        stream << result.p1_w.b1Value._2;
        break;
    }
    case EQ_TYPE_PEnc_H:
    case EQ_TYPE_ME_G:
    {
        Fp alpha = Fp::getRand(), gamma = Fp::getRand();
        convToB(result.p1_v, crs);
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        convToB(result.p2_v, crs);
        convToB(result.p2_w, crs);
        result.p2_v.b2Value += alpha * crs.v2;
        result.p2_w.b2Value += gamma * crs.v2;
        if (crs.type == CRS_TYPE_PRIVATE) {
            result.p1_v.b1Value -= (alpha + crs.i1 * gamma) * crs.v1;
        } else {
            result.p1_v.b1Value -= alpha * crs.v1 + gamma * crs.w1;
        }
        stream << result.p1_v.b1Value;
        stream << result.p2_v.b2Value;
        stream << result.p2_w.b2Value;
        break;
    }
    case EQ_TYPE_PConst_H:
    {
        ASSERT(result.p1_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        ASSERT((result.p2_v.type == VALUE_NULL) ||
                (result.p2_v.type == VALUE_G), "Unexpected type");
        ASSERT((result.p2_w.type == VALUE_NULL) ||
                (result.p2_w.type == VALUE_G), "Unexpected type");
        stream << result.p2_v.b2Value._2;
        stream << result.p2_w.b2Value._2;
        break;
    }
    case EQ_TYPE_MEnc_G:
    case EQ_TYPE_MEnc_H:
    case EQ_TYPE_QE:
    {
        Fp alpha = Fp::getRand();
        convToB(result.p1_v, crs);
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        convToB(result.p2_v, crs);
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        result.p2_v.b2Value += alpha * crs.v2;
        result.p1_v.b1Value -= alpha * crs.v1;
        stream << result.p1_v.b1Value;
        stream << result.p2_v.b2Value;
        break;
    }
    case EQ_TYPE_MConst_G:
    {
        ASSERT((result.p1_v.type == VALUE_NULL) ||
                (result.p1_v.type == VALUE_G), "Unexpected type");
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p2_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        stream << result.p1_v.b1Value._2;
        break;
    }
    case EQ_TYPE_MLin_G:
    {
        ASSERT(result.p1_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        ASSERT((result.p2_v.type == VALUE_NULL) ||
                (result.p2_v.type == VALUE_Fp), "Unexpected type");
        ASSERT((result.p2_w.type == VALUE_NULL) ||
                (result.p2_w.type == VALUE_Fp), "Unexpected type");
        stream << result.p2_v.fpValue;
        stream << result.p2_w.fpValue;
        break;
    }
    case EQ_TYPE_MConst_H:
    {
        ASSERT(result.p1_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        ASSERT((result.p2_v.type == VALUE_NULL) ||
                (result.p2_v.type == VALUE_G), "Unexpected type");
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        stream << result.p2_v.b2Value._2;
        break;
    }
    case EQ_TYPE_MLin_H:
    {
        ASSERT((result.p1_v.type == VALUE_NULL) ||
                (result.p1_v.type == VALUE_Fp), "Unexpected type");
        ASSERT((result.p1_w.type == VALUE_NULL) ||
                (result.p1_w.type == VALUE_Fp), "Unexpected type");
        ASSERT(result.p2_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        stream << result.p1_v.fpValue;
        stream << result.p1_w.fpValue;
        break;
    }
    case EQ_TYPE_QConst_G:
    {
        ASSERT((result.p1_v.type == VALUE_NULL) ||
                (result.p1_v.type == VALUE_Fp), "Unexpected type");
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p2_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        stream << result.p1_v.fpValue;
        break;
    }
    case EQ_TYPE_QConst_H:
    {
        ASSERT(result.p1_v.type == VALUE_NULL, "Unexpected type");
        ASSERT(result.p1_w.type == VALUE_NULL, "Unexpected type");
        ASSERT((result.p2_v.type == VALUE_NULL) ||
                (result.p2_v.type == VALUE_Fp), "Unexpected type");
        ASSERT(result.p2_w.type == VALUE_NULL, "Unexpected type");
        stream << result.p2_v.fpValue;
        break;
    }
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
    ASSERT(varsFp.size() == varsFpInB1.size(), "Array sizes do not match");
    ASSERT(cstsFp.size() == cstsFpInB1.size(), "Array sizes do not match");
    G1Commit c1;
    G2Commit c2;
    int j = varsFp.size(), i = additionalFp.size();
    c1.type = COMMIT_ENC;
    c1.c.type = VALUE_Fp;
    c2.type = COMMIT_ENC;
    c2.c.type = VALUE_B;
    while (i-- > 0) {
        if (varsFpInB1[--j]) {
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
    while (i-- > 0) {
        --j;
        c1.r = Fp::getRand();
        c1.c.b1Value._2 = additionalG1[i].value;
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G1][j])) {
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
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G1][j])) {
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
    while (i-- > 0) {
        --j;
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G2][j])) {
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
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G2][j])) {
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
    c2.c.b2Value._1.clear();
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
        writeEqProof(stream, left.d, right.d, tFp[i], crs);
    }
    for (i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, left.d, right.d, tG1[i], crs);
    }
    for (i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, left.d, right.d, tG2[i], crs);
    }
    for (i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        getProof(left, crs);
        getProof(right, crs);
        writeEqProof(stream, left.d, right.d, tGT[i], crs);
    }
    for (int i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (int i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (int i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (int i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        removeProof(left);
        removeProof(right);
    }
}

bool NIZKProof::checkInstantiation(const ProofData &instantiation) const {
    return (instantiation.pubFp.size() == cstsFp.size()) &&
        (instantiation.pubG1.size() == cstsG1.size()) &&
        (instantiation.pubG2.size() == cstsG2.size()) &&
        (instantiation.pubGT.size() == cstsGT.size()) &&
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutAsFp(std::shared_ptr<FpData> &d) {
    switch (d->type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutAsFp(d->pair.first);
        checkoutAsFp(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        checkoutLeft(d->pair.first);
        checkoutRight(d->pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutAsG1(std::shared_ptr<G1Data> &d) {
    switch (d->type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutAsG1(d->pair.first);
        checkoutAsG1(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        checkoutLeft(d->scalar.second);
        checkoutRight(d->scalar.first);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutAsG2(std::shared_ptr<G2Data> &d) {
    switch (d->type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutAsG2(d->pair.first);
        checkoutAsG2(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        checkoutLeft(d->scalar.first);
        checkoutRight(d->scalar.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutAsGT(std::shared_ptr<GTData> &d) {
    switch (d->type) {
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        break;
    case ELEMENT_PAIR:
        checkoutAsGT(d->pair.first);
        checkoutAsGT(d->pair.second);
        break;
    case ELEMENT_PAIRING:
        checkoutLeft(d->pring.first);
        checkoutRight(d->pring.second);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

PairFp getFpVarEq(const std::shared_ptr<FpData> &v1,
                  const std::shared_ptr<FpData> &v2) {
    FpData *left = new FpData(ELEMENT_SCALAR);
    new (&left->pair) PairFp(v1,
            std::shared_ptr<FpData>(new FpData(ELEMENT_BASE)));
    FpData *right = new FpData(ELEMENT_SCALAR);
    new (&right->pair) PairFp(
            std::shared_ptr<FpData>(new FpData(ELEMENT_BASE)), v2);
    return PairFp(std::shared_ptr<FpData>(left),
                  std::shared_ptr<FpData>(right));
}

void NIZKProof::checkoutLeft(std::shared_ptr<FpData> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
        if (d->d) {
            if (!varsFpInB1[d->index]) {
                int &idx = *reinterpret_cast<int*>(d->d);
                if (idx < 0) {
                    idx = varsFp.size();
                    std::shared_ptr<FpData> newVar = std::shared_ptr<FpData>(
                            new FpData(ELEMENT_VARIABLE));
                    newVar->index = idx;
                    newVar->d = reinterpret_cast<void*>(new int(d->index));
                    eqsFp.push_back(getFpVarEq(newVar, d));
                    varsFp.push_back(newVar);
                    varsFpInB1.push_back(true);
                    additionalFp.push_back(AdditionalFp(d));
                    d = newVar;
                } else {
                    d = varsFp[idx];
                }
            }
        } else {
            varsFpInB1[d->index] = true;
            d->d = reinterpret_cast<void*>(new int(-1));
        }
        break;
    case ELEMENT_CONST_INDEX:
        if (d->d) {
            if (!cstsFpInB1[d->index])
                throw "Fp constant used both in B1 and B2";
        } else {
            d->d = reinterpret_cast<void*>(d.get());
            cstsFpInB1[d->index] = true;
        }
        break;
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutLeft(d->pair.first);
        checkoutLeft(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d->d) {
            const int &idx = *reinterpret_cast<int*>(d->d);
            d = varsFp[idx];
        } else {
            std::shared_ptr<FpData> newVar = std::shared_ptr<FpData>(
                    new FpData(ELEMENT_VARIABLE));
            newVar->index = varsFp.size();
            newVar->d = reinterpret_cast<void*>(new int(-1));
            d->d = reinterpret_cast<void*>(new int(newVar->index));
            varsFp.push_back(newVar);
            varsFpInB1.push_back(true);
            FpData *right = new FpData(ELEMENT_SCALAR);
            new (&right->pair) PairFp(newVar,
                    std::shared_ptr<FpData>(new FpData(ELEMENT_BASE)));
            eqsFp.push_back(PairFp(d, std::shared_ptr<FpData>(right)));
            additionalFp.push_back(AdditionalFp(d));
            d = newVar;
        }
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutLeft(std::shared_ptr<G1Data> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutLeft(d->pair.first);
        checkoutLeft(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d->d) {
            const int &idx = *reinterpret_cast<int*>(d->d);
            d = varsG1[idx];
        } else {
            std::shared_ptr<G1Data> newVar = std::shared_ptr<G1Data>(
                    new G1Data(ELEMENT_VARIABLE));
            newVar->index = varsG1.size();
            d->d = reinterpret_cast<void*>(new int(newVar->index));
            varsG1.push_back(newVar);
            G1Data *right = new G1Data(ELEMENT_SCALAR);
            new (&right->scalar) ScalarG1(
                    std::shared_ptr<FpData>(new FpData(ELEMENT_BASE)), newVar);
            eqsG1.push_back(PairG1(d, std::shared_ptr<G1Data>(right)));
            additionalG1.push_back(AdditionalG1(d));
            d = newVar;
        }
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutRight(std::shared_ptr<FpData> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
        if (d->d) {
            if (varsFpInB1[d->index]) {
                int &idx = *reinterpret_cast<int*>(d->d);
                if (idx < 0) {
                    idx = varsFp.size();
                    std::shared_ptr<FpData> newVar = std::shared_ptr<FpData>(
                            new FpData(ELEMENT_VARIABLE));
                    newVar->index = idx;
                    newVar->d = reinterpret_cast<void*>(new int(d->index));
                    eqsFp.push_back(getFpVarEq(d, newVar));
                    varsFp.push_back(newVar);
                    varsFpInB1.push_back(false);
                    additionalFp.push_back(AdditionalFp(d));
                    d = newVar;
                } else {
                    d = varsFp[idx];
                }
            }
        } else {
            varsFpInB1[d->index] = false;
            d->d = reinterpret_cast<void*>(new int(-1));
        }
        break;
    case ELEMENT_CONST_INDEX:
        if (d->d) {
            if (cstsFpInB1[d->index])
                throw "Fp constant used both in B1 and B2";
        } else {
            d->d = reinterpret_cast<void*>(d.get());
            cstsFpInB1[d->index] = false;
        }
        break;
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutRight(d->pair.first);
        checkoutRight(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d->d) {
            const int &idx = *reinterpret_cast<int*>(d->d);
            d = varsFp[idx];
        } else {
            std::shared_ptr<FpData> newVar = std::shared_ptr<FpData>(
                    new FpData(ELEMENT_VARIABLE));
            newVar->index = varsFp.size();
            newVar->d = reinterpret_cast<void*>(new int(-1));
            d->d = reinterpret_cast<void*>(new int(newVar->index));
            varsFp.push_back(newVar);
            varsFpInB1.push_back(false);
            FpData *right = new FpData(ELEMENT_SCALAR);
            new (&right->pair) PairFp(
                    std::shared_ptr<FpData>(new FpData(ELEMENT_BASE)), newVar);
            eqsFp.push_back(PairFp(d, std::shared_ptr<FpData>(right)));
            additionalFp.push_back(AdditionalFp(d));
            d = newVar;
        }
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void NIZKProof::checkoutRight(std::shared_ptr<G2Data> &d) {
    switch (d->type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        checkoutRight(d->pair.first);
        checkoutRight(d->pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d->d) {
            const int &idx = *reinterpret_cast<int*>(d->d);
            d = varsG2[idx];
        } else {
            std::shared_ptr<G2Data> newVar = std::shared_ptr<G2Data>(
                    new G2Data(ELEMENT_VARIABLE));
            newVar->index = varsG2.size();
            d->d = reinterpret_cast<void*>(new int(newVar->index));
            varsG2.push_back(newVar);
            G2Data *right = new G2Data(ELEMENT_SCALAR);
            new (&right->scalar) ScalarG2(
                    std::shared_ptr<FpData>(new FpData(ELEMENT_BASE)), newVar);
            eqsG2.push_back(PairG2(d, std::shared_ptr<G2Data>(right)));
            additionalG2.push_back(AdditionalG2(d));
            d = newVar;
        }
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewrite(const FpData &d) {
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewrite(*d.pair.first);
        endRewrite(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d.d) {
            delete reinterpret_cast<int*>(d.d);
            d.d = NULL;
        }
        endRewriteLeft(*d.pair.first);
        endRewriteRight(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewrite(const G1Data &d) {
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewrite(*d.pair.first);
        endRewrite(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d.d) {
            delete reinterpret_cast<int*>(d.d);
            d.d = NULL;
        }
        endRewriteLeft(*d.scalar.second);
        endRewriteRight(*d.scalar.first);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewrite(const G2Data &d) {
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewrite(*d.pair.first);
        endRewrite(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        if (d.d) {
            delete reinterpret_cast<int*>(d.d);
            d.d = NULL;
        }
        endRewriteLeft(*d.scalar.first);
        endRewriteRight(*d.scalar.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewriteLeft(const FpData &d) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if (d.d) {
            delete reinterpret_cast<int*>(d.d);
            d.d = NULL;
        }
        break;
    case ELEMENT_CONST_INDEX:
        d.d = NULL;
        break;
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewriteLeft(*d.pair.first);
        endRewriteLeft(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewriteLeft(const G1Data &d) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewriteLeft(*d.pair.first);
        endRewriteLeft(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewriteRight(const FpData &d) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if (d.d) {
            delete reinterpret_cast<int*>(d.d);
            d.d = NULL;
        }
        break;
    case ELEMENT_CONST_INDEX:
        d.d = NULL;
        break;
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewriteRight(*d.pair.first);
        endRewriteRight(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void endRewriteRight(const G2Data &d) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        endRewriteRight(*d.pair.first);
        endRewriteRight(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
        ASSERT(false, "Unexpected data type");
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
    case COMMIT_ENC:
        p.p1_v = c1.c;
        p.p1_v *= c2.r;
    case COMMIT_PUB:
        break;
    }
}

void addAllPi(const ProofEls &el1, const ProofEls &el2, ProofEls &result,
              const CRS &crs) {
    addPiG1(el1.p1_v, el2.p1_v, result.p1_v, crs);
    addPiG1(el1.p1_w, el2.p1_w, result.p1_w, crs);
    addPiG2(el1.p2_v, el2.p2_v, result.p2_v, crs);
    addPiG2(el1.p2_w, el2.p2_w, result.p2_w, crs);
}

void getProof(const FpData &d, const CRS &crs) {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        proofEl->p1_v.type = VALUE_NULL;
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_v.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        return;
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
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
        ASSERT(false, "Unexpected data type");
    }
}

void getProof(const G1Data &d, const CRS &crs) {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        proofEl->p1_v.type = VALUE_NULL;
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_v.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        return;
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
            return;
        }
    case ELEMENT_SCALAR:
        {
            getLeft(*d.scalar.second, crs);
            getRight(*d.scalar.first, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.scalar.second->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.scalar.first->d);
            scalarCombine(el1, el2, *proofEl);
            return;
        }
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void getProof(const G2Data &d, const CRS &crs) {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        proofEl->p1_v.type = VALUE_NULL;
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_v.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        return;
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
            return;
        }
    case ELEMENT_SCALAR:
        {
            getLeft(*d.scalar.first, crs);
            getRight(*d.scalar.second, crs);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.scalar.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.scalar.second->d);
            scalarCombine(el1, el2, *proofEl);
            return;
        }
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void getProof(const GTData &d, const CRS &crs) {
    if (d.d) return;
    ProofEls *proofEl = new ProofEls;
    d.d = reinterpret_cast<void*>(proofEl);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        proofEl->p1_v.type = VALUE_NULL;
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_v.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        return;
    case ELEMENT_PAIR:
        {
            getProof(*d.pair.first, crs);
            getProof(*d.pair.second, crs);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
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
        ASSERT(false, "Unexpected data type");
    }
}

void getLeft(const FpData &d, const CRS &crs) {
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
        ASSERT(false, "Unexpected data type");
    }
}

void getLeft(const G1Data &d, const CRS &crs) {
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
        ASSERT(false, "Unexpected data type");
    }
}

void getRight(const FpData &d, const CRS &crs) {
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
        ASSERT(false, "Unexpected data type");
    }
}

void getRight(const G2Data &d, const CRS &crs) {
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
        ASSERT(false, "Unexpected data type");
    }
}

void removeProof(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeft(*d.pair.first);
        removeRight(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeProof(const G1Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeft(*d.scalar.second);
        removeRight(*d.scalar.first);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeProof(const G2Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeft(*d.scalar.first);
        removeRight(*d.scalar.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeProof(const GTData &d) {
    if (!d.d) return;
    delete reinterpret_cast<ProofEls*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeProof(*d.pair.first);
        removeProof(*d.pair.second);
        break;
    case ELEMENT_PAIRING:
        removeLeft(*d.pring.first);
        removeRight(*d.pring.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
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

void combinePTResults(std::pair<ElTypeSet,ElTypeSet> &result,
                      const std::pair<ElTypeSet,ElTypeSet> &other) {
    for (const EL_TYPE_PT &el : other.first)
        result.first.insert(el);
    for (const EL_TYPE_PT &el : other.second)
        result.second.insert(el);
}

std::pair<ElTypeSet,ElTypeSet> NIZKProof::getPType(const FpData &d) {
    std::pair<ElTypeSet,ElTypeSet> result;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        result.first.insert(EL_TYPE_UNIT_G);
        result.second.insert(EL_TYPE_UNIT_H);
        break;
    case ELEMENT_PAIR:
        {
            result = getPType(*d.pair.first);
            combinePTResults(result, getPType(*d.pair.second));
            break;
        }
    case ELEMENT_SCALAR:
        result.first = getPTLeft(*d.pair.first);
        result.second = getPTRight(*d.pair.second);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

std::pair<ElTypeSet,ElTypeSet> NIZKProof::getPType(const G1Data &d) {
    std::pair<ElTypeSet,ElTypeSet> result;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        result.first.insert(EL_TYPE_PUB_G);
        result.second.insert(EL_TYPE_UNIT_H);
        break;
    case ELEMENT_BASE:
        result.first.insert(EL_TYPE_BASE_G);
        result.second.insert(EL_TYPE_UNIT_H);
        break;
    case ELEMENT_PAIR:
        {
            result = getPType(*d.pair.first);
            combinePTResults(result, getPType(*d.pair.second));
            break;
        }
    case ELEMENT_SCALAR:
        result.first = getPTLeft(*d.scalar.second);
        result.second = getPTRight(*d.scalar.first);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

std::pair<ElTypeSet,ElTypeSet> NIZKProof::getPType(const G2Data &d) {
    std::pair<ElTypeSet,ElTypeSet> result;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        result.first.insert(EL_TYPE_UNIT_G);
        result.second.insert(EL_TYPE_PUB_H);
        break;
    case ELEMENT_BASE:
        result.first.insert(EL_TYPE_UNIT_G);
        result.second.insert(EL_TYPE_BASE_H);
        break;
    case ELEMENT_PAIR:
        {
            result = getPType(*d.pair.first);
            combinePTResults(result, getPType(*d.pair.second));
            break;
        }
    case ELEMENT_SCALAR:
        result.first = getPTLeft(*d.scalar.first);
        result.second = getPTRight(*d.scalar.second);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

std::pair<ElTypeSet,ElTypeSet> NIZKProof::getPType(const GTData &d) {
    std::pair<ElTypeSet,ElTypeSet> result;
    switch (d.type) {
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        result.first.insert(EL_TYPE_PUB_G);
        result.second.insert(EL_TYPE_PUB_H);
        break;
    case ELEMENT_BASE:
        result.first.insert(EL_TYPE_BASE_G);
        result.second.insert(EL_TYPE_BASE_H);
        break;
    case ELEMENT_PAIR:
        {
            result = getPType(*d.pair.first);
            combinePTResults(result, getPType(*d.pair.second));
            break;
        }
    case ELEMENT_PAIRING:
        result.first = getPTLeft(*d.pring.first);
        result.second = getPTRight(*d.pring.second);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

ElTypeSet NIZKProof::getPTLeft(const FpData &d) {
    ElTypeSet result;
    switch (d.type) {
    case ELEMENT_VARIABLE:
        result.insert(EL_TYPE_SCA_G);
        break;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        result.insert(EL_TYPE_UNIT_G);
        break;
    case ELEMENT_PAIR:
        result = getPTLeft(*d.pair.first);
        for (const EL_TYPE_PT &el : getPTLeft(*d.pair.second))
            result.insert(el);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

ElTypeSet NIZKProof::getPTLeft(const G1Data &d) {
    ElTypeSet result;
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if ((type == AllEncrypted) || ((type == SelectedEncryption) &&
                sEnc[INDEX_TYPE_G1][d.index])) {
            result.insert(EL_TYPE_ENC_G);
        } else {
            result.insert(EL_TYPE_COM_G);
        }
        break;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        result.insert(EL_TYPE_PUB_G);
        break;
    case ELEMENT_BASE:
        result.insert(EL_TYPE_BASE_G);
        break;
    case ELEMENT_PAIR:
        result = getPTLeft(*d.pair.first);
        for (const EL_TYPE_PT &el : getPTLeft(*d.pair.second))
            result.insert(el);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

ElTypeSet NIZKProof::getPTRight(const FpData &d) {
    ElTypeSet result;
    switch (d.type) {
    case ELEMENT_VARIABLE:
        result.insert(EL_TYPE_SCA_H);
        break;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        result.insert(EL_TYPE_UNIT_H);
        break;
    case ELEMENT_PAIR:
        result = getPTRight(*d.pair.first);
        for (const EL_TYPE_PT &el : getPTRight(*d.pair.second))
            result.insert(el);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

ElTypeSet NIZKProof::getPTRight(const G2Data &d) {
    ElTypeSet result;
    switch (d.type) {
    case ELEMENT_VARIABLE:
        if ((type == AllEncrypted) || ((type == SelectedEncryption) &&
                sEnc[INDEX_TYPE_G2][d.index])) {
            result.insert(EL_TYPE_ENC_H);
        } else {
            result.insert(EL_TYPE_COM_H);
        }
        break;
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        result.insert(EL_TYPE_PUB_H);
        break;
    case ELEMENT_BASE:
        result.insert(EL_TYPE_BASE_H);
        break;
    case ELEMENT_PAIR:
        result = getPTRight(*d.pair.first);
        for (const EL_TYPE_PT &el : getPTRight(*d.pair.second))
            result.insert(el);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return result;
}

bool ElTSContains(const ElTypeSet &ets, EL_TYPE_PT el) {
    return ets.find(el) != ets.cend();
}

void NIZKProof::getEqProofTypes() {
    std::pair<ElTypeSet,ElTypeSet> result;
    int i = eqsFp.size();
    tFp.resize(i);
    while (i-- > 0) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        result = getPType(left);
        combinePTResults(result, getPType(right));
        if (!ElTSContains(result.first, EL_TYPE_SCA_G)) {
            tFp[i] = EQ_TYPE_QConst_G;
        } else if (!ElTSContains(result.second, EL_TYPE_SCA_H)) {
            tFp[i] = EQ_TYPE_QConst_H;
        } else {
            tFp[i] = EQ_TYPE_QE;
        }
    }
    i = eqsG1.size();
    tG1.resize(i);
    while (i-- > 0) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        result = getPType(left);
        combinePTResults(result, getPType(right));
        if (ElTSContains(result.second, EL_TYPE_SCA_H) ||
                ElTSContains(result.first, EL_TYPE_PUB_G)) {
            if (ElTSContains(result.first, EL_TYPE_COM_G)) {
                tG1[i] = EQ_TYPE_ME_G;
            } else if (ElTSContains(result.first, EL_TYPE_ENC_G)) {
                tG1[i] = EQ_TYPE_MEnc_G;
            } else {
                tG1[i] = EQ_TYPE_MConst_G;
            }
        } else {
            if (ElTSContains(result.first, EL_TYPE_ENC_G)) {
                tG1[i] = EQ_TYPE_MEnc_G;
            } else {
                tG1[i] = EQ_TYPE_MLin_G;
            }
        }
    }
    i = eqsG2.size();
    tG2.resize(i);
    while (i-- > 0) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        result = getPType(left);
        combinePTResults(result, getPType(right));
        if (ElTSContains(result.first, EL_TYPE_SCA_G) ||
                ElTSContains(result.second, EL_TYPE_PUB_H)) {
            if (ElTSContains(result.second, EL_TYPE_COM_H)) {
                tG2[i] = EQ_TYPE_ME_H;
            } else if (ElTSContains(result.second, EL_TYPE_ENC_H)) {
                tG2[i] = EQ_TYPE_MEnc_H;
            } else {
                tG2[i] = EQ_TYPE_MConst_H;
            }
        } else {
            if (ElTSContains(result.second, EL_TYPE_ENC_H)) {
                tG2[i] = EQ_TYPE_MEnc_H;
            } else {
                tG2[i] = EQ_TYPE_MLin_H;
            }
        }
    }
    i = eqsGT.size();
    tGT.resize(i);
    while (i-- > 0) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        result = getPType(left);
        combinePTResults(result, getPType(right));
        bool pubL = ElTSContains(result.first, EL_TYPE_PUB_G) ||
                ElTSContains(result.second, EL_TYPE_COM_H);
        bool pubR = ElTSContains(result.second, EL_TYPE_PUB_H) ||
                ElTSContains(result.first, EL_TYPE_COM_G);
        if (pubL && pubR) {
            tGT[i] = EQ_TYPE_PPE;
        } else if (pubL) {
            if (ElTSContains(result.second, EL_TYPE_ENC_H)) {
                tGT[i] = EQ_TYPE_PPE;
            } else if (ElTSContains(result.first, EL_TYPE_ENC_G)) {
                tGT[i] = EQ_TYPE_PEnc_G;
            } else {
                tGT[i] = EQ_TYPE_PConst_G;
            }
        } else if (pubR) {
            if (ElTSContains(result.first, EL_TYPE_ENC_G)) {
                tGT[i] = EQ_TYPE_PPE;
            } else if (ElTSContains(result.second, EL_TYPE_ENC_H)) {
                tGT[i] = EQ_TYPE_PEnc_H;
            } else {
                tGT[i] = EQ_TYPE_PConst_H;
            }
        } else {
            if (ElTSContains(result.first, EL_TYPE_ENC_G)) {
                if (ElTSContains(result.second, EL_TYPE_ENC_H)) {
                    tGT[i] = EQ_TYPE_PPE;
                } else {
                    tGT[i] = EQ_TYPE_PEnc_G;
                }
            } else {
                if (ElTSContains(result.second, EL_TYPE_ENC_H)) {
                    tGT[i] = EQ_TYPE_PEnc_H;
                } else {
                    tGT[i] = EQ_TYPE_PConst_G; // or EQ_TYPE_PConst_H
                }
            }
        }
    }
}

BT calcExpr(const FpData &d, const CRS &crs);
BT calcExpr(const G1Data &d, const CRS &crs);
BT calcExpr(const G2Data &d, const CRS &crs);
BT calcExpr(const GTData &d, const CRS &crs);
B1 calcLeft(const FpData &d, const CRS &crs);
B1 calcLeft(const G1Data &d, const CRS &crs);
B2 calcRight(const FpData &d, const CRS &crs);
B2 calcRight(const G2Data &d, const CRS &crs);

void removeCalculations(const FpData &d);
void removeCalculations(const G1Data &d);
void removeCalculations(const G2Data &d);
void removeCalculations(const GTData &d);
void removeLeftCalc(const FpData &d);
void removeLeftCalc(const G1Data &d);
void removeRightCalc(const FpData &d);
void removeRightCalc(const G2Data &d);

BT NIZKProof::getRndProofPart(std::istream &stream, EqProofType t,
                              const CRS &crs) const {
    switch (t) {
    case EQ_TYPE_PPE:
    {
        std::vector< std::pair<B1,B2> > pairs;
        {
            B1 b1;
            stream >> b1;
            pairs.push_back(std::pair<B1,B2>(b1, crs.v2));
            stream >> b1;
            pairs.push_back(std::pair<B1,B2>(b1, crs.w2));
        }
        {
            B2 b2;
            stream >> b2;
            pairs.push_back(std::pair<B1,B2>(crs.v1, b2));
            stream >> b2;
            pairs.push_back(std::pair<B1,B2>(crs.w1, b2));
        }
        return BT::pairing(pairs);
    }
    case EQ_TYPE_PEnc_G:
    case EQ_TYPE_ME_H:
    {
        std::vector< std::pair<B1,B2> > pairs;
        {
            B1 b1;
            stream >> b1;
            pairs.push_back(std::pair<B1,B2>(b1, crs.v2));
            stream >> b1;
            pairs.push_back(std::pair<B1,B2>(b1, crs.w2));
        }
        {
            B2 b2;
            stream >> b2;
            pairs.push_back(std::pair<B1,B2>(crs.v1, b2));
        }
        return BT::pairing(pairs);
    }
    case EQ_TYPE_PConst_G:
    {
        std::vector< std::pair<B1,B2> > pairs;
        G1 g1;
        stream >> g1;
        pairs.push_back(std::pair<B1,B2>(B1(g1), crs.v2));
        stream >> g1;
        pairs.push_back(std::pair<B1,B2>(B1(g1), crs.w2));
        return BT::pairing(pairs);
    }
    case EQ_TYPE_PEnc_H:
    case EQ_TYPE_ME_G:
    {
        std::vector< std::pair<B1,B2> > pairs;
        {
            B1 b1;
            stream >> b1;
            pairs.push_back(std::pair<B1,B2>(b1, crs.v2));
        }
        {
            B2 b2;
            stream >> b2;
            pairs.push_back(std::pair<B1,B2>(crs.v1, b2));
            stream >> b2;
            pairs.push_back(std::pair<B1,B2>(crs.w1, b2));
        }
        return BT::pairing(pairs);
    }
    case EQ_TYPE_PConst_H:
    {
        std::vector< std::pair<B1,B2> > pairs;
        G2 g2;
        stream >> g2;
        pairs.push_back(std::pair<B1,B2>(crs.v1, B2(g2)));
        stream >> g2;
        pairs.push_back(std::pair<B1,B2>(crs.w1, B2(g2)));
        return BT::pairing(pairs);
    }
    case EQ_TYPE_MEnc_G:
    case EQ_TYPE_MEnc_H:
    case EQ_TYPE_QE:
    {
        std::vector< std::pair<B1,B2> > pairs;
        {
            B1 b1;
            stream >> b1;
            pairs.push_back(std::pair<B1,B2>(b1, crs.v2));
        }
        {
            B2 b2;
            stream >> b2;
            pairs.push_back(std::pair<B1,B2>(crs.v1, b2));
        }
        return BT::pairing(pairs);
    }
    case EQ_TYPE_MConst_G:
    {
        G1 g1;
        stream >> g1;
        return BT::pairing(B1(g1), crs.v2);
    }
    case EQ_TYPE_MLin_G:
    {
        Fp k;
        stream >> k;
        B1 b1 = k * crs.v1;
        stream >> k;
        b1 += k * crs.w1;
        return BT::pairing(b1, crs.u2);
    }
    case EQ_TYPE_MConst_H:
    {
        G2 g2;
        stream >> g2;
        return BT::pairing(crs.v1, B2(g2));
    }
    case EQ_TYPE_MLin_H:
    {
        Fp k;
        stream >> k;
        B2 b2 = k * crs.v2;
        stream >> k;
        b2 += k * crs.w2;
        return BT::pairing(crs.u1, b2);
    }
    case EQ_TYPE_QConst_G:
    {
        Fp k;
        stream >> k;
        return BT::pairing(k * crs.u1, crs.v2);
    }
    case EQ_TYPE_QConst_H:
    {
        Fp k;
        stream >> k;
        return BT::pairing(k * crs.v1, crs.u2);
    }
    default:
        ASSERT(false, "Unexpected data type");
        return BT();
    }
}

bool NIZKProof::checkProof(std::istream &stream, const CRS &crs,
                const ProofData &instantiation) const {
    if (!fixed) return false;
    if ((instantiation.pubFp.size() != cstsFp.size()) ||
            (instantiation.pubG1.size() != cstsG1.size()) ||
            (instantiation.pubG2.size() != cstsG2.size()) ||
            (instantiation.pubGT.size() != cstsGT.size()))
        return false;
    ASSERT(varsFp.size() == varsFpInB1.size(), "Array sizes do not match");
    ASSERT(cstsFp.size() == cstsFpInB1.size(), "Array sizes do not match");
    B1 b1;
    B2 b2;
    for (int i = varsFp.size(); i-- > 0;) {
        if (varsFpInB1[i]) {
            stream >> b1;
            varsFp[i]->d = reinterpret_cast<void*>(new B1(b1));
        } else {
            stream >> b2;
            varsFp[i]->d = reinterpret_cast<void*>(new B2(b2));
        }
    }
    for (int i = varsG1.size(); i-- > 0;) {
        stream >> b1;
        varsG1[i]->d = reinterpret_cast<void*>(new B1(b1));
    }
    for (int i = varsG2.size(); i-- > 0;) {
        stream >> b2;
        varsG2[i]->d = reinterpret_cast<void*>(new B2(b2));
    }
    for (int i = cstsFp.size(); i-- > 0;) {
        if (cstsFpInB1[i]) {
            varsFp[i]->d = reinterpret_cast<void*>(new B1(
                    instantiation.pubFp[i], crs));
        } else {
            varsFp[i]->d = reinterpret_cast<void*>(new B2(
                    instantiation.pubFp[i], crs));
        }
    }
    for (int i = cstsG1.size(); i-- > 0;)
        cstsG1[i]->d = reinterpret_cast<void*>(new B1(instantiation.pubG1[i]));
    for (int i = cstsG2.size(); i-- > 0;)
        cstsG2[i]->d = reinterpret_cast<void*>(new B2(instantiation.pubG2[i]));
    for (int i = cstsGT.size(); i-- > 0;)
        cstsGT[i]->d = reinterpret_cast<void*>(new BT(instantiation.pubGT[i]));
    bool result = false;
    BT rndProofPart;
    for (int i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        rndProofPart = getRndProofPart(stream, tFp[i], crs);
        if (calcExpr(left, crs) != calcExpr(right, crs) * rndProofPart)
            goto cleanup;
    }
    for (int i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        rndProofPart = getRndProofPart(stream, tG1[i], crs);
        if (calcExpr(left, crs) != calcExpr(right, crs) * rndProofPart)
            goto cleanup;
    }
    for (int i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        rndProofPart = getRndProofPart(stream, tG2[i], crs);
        if (calcExpr(left, crs) != calcExpr(right, crs) * rndProofPart)
            goto cleanup;
    }
    for (int i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        rndProofPart = getRndProofPart(stream, tGT[i], crs);
        if (calcExpr(left, crs) != calcExpr(right, crs) * rndProofPart)
            goto cleanup;
    }
    result = true;
cleanup:
    for (int i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        removeCalculations(left);
        removeCalculations(right);
    }
    for (int i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        removeCalculations(left);
        removeCalculations(right);
    }
    for (int i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        removeCalculations(left);
        removeCalculations(right);
    }
    for (int i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        removeCalculations(left);
        removeCalculations(right);
    }
    return result;
}

BT calcExpr(const FpData &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<BT*>(d.d);
    BT *result = new BT;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = BT(d.el, crs);
        break;
    case ELEMENT_PAIR:
        *result = calcExpr(*d.pair.first, crs) *
                calcExpr(*d.pair.second, crs);
        break;
    case ELEMENT_SCALAR:
        *result = BT::pairing(calcLeft(*d.pair.first, crs),
                calcRight(*d.pair.second, crs));
        break;
    case ELEMENT_BASE:
        /* Note: Could be precomputed, but not often used in practice */
        *result = BT(Fp::getUnit(), crs);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

BT calcExpr(const G1Data &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<BT*>(d.d);
    BT *result = new BT;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = BT(d.el, crs);
        break;
    case ELEMENT_PAIR:
        *result = calcExpr(*d.pair.first, crs) *
                calcExpr(*d.pair.second, crs);
        break;
    case ELEMENT_SCALAR:
        *result = BT::pairing(calcLeft(*d.scalar.second, crs),
                calcRight(*d.scalar.first, crs));
        break;
    case ELEMENT_BASE:
        /* Note: Could be precomputed, but not often used in practice */
        *result = BT(crs.getG1Base(), crs);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

BT calcExpr(const G2Data &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<BT*>(d.d);
    BT *result = new BT;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = BT(d.el, crs);
        break;
    case ELEMENT_PAIR:
        *result = calcExpr(*d.pair.first, crs) *
                calcExpr(*d.pair.second, crs);
        break;
    case ELEMENT_SCALAR:
        *result = BT::pairing(calcLeft(*d.scalar.first, crs),
                calcRight(*d.scalar.second, crs));
        break;
    case ELEMENT_BASE:
        /* Note: Could be precomputed, but not often used in practice */
        *result = BT(crs.getG2Base(), crs);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

BT calcExpr(const GTData &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<BT*>(d.d);
    BT *result = new BT;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = BT(d.el);
        break;
    case ELEMENT_PAIR:
        *result = calcExpr(*d.pair.first, crs) *
                calcExpr(*d.pair.second, crs);
        break;
    case ELEMENT_PAIRING:
        *result = BT::pairing(calcLeft(*d.pring.first, crs),
                calcRight(*d.pring.second, crs));
        break;
    case ELEMENT_BASE:
        *result = BT(crs.getGTBase());
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

B1 calcLeft(const FpData &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<B1*>(d.d);
    B1 *result = new B1;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = B1(d.el, crs);
        break;
    case ELEMENT_PAIR:
        *result = calcLeft(*d.pair.first, crs) +
                calcLeft(*d.pair.second, crs);
        break;
    case ELEMENT_BASE:
        *result = crs.getB1Unit();
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

B1 calcLeft(const G1Data &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<B1*>(d.d);
    B1 *result = new B1;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = B1(d.el);
        break;
    case ELEMENT_PAIR:
        *result = calcLeft(*d.pair.first, crs) +
                calcLeft(*d.pair.second, crs);
        break;
    case ELEMENT_BASE:
        *result = B1(crs.getG1Base());
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

B2 calcRight(const FpData &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<B2*>(d.d);
    B2 *result = new B2;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = B2(d.el, crs);
        break;
    case ELEMENT_PAIR:
        *result = calcRight(*d.pair.first, crs) +
                calcRight(*d.pair.second, crs);
        break;
    case ELEMENT_BASE:
        *result = crs.getB2Unit();
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

B2 calcRight(const G2Data &d, const CRS &crs) {
    if (d.d) return *reinterpret_cast<B2*>(d.d);
    B2 *result = new B2;
    d.d = reinterpret_cast<void*>(result);
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        *result = B2(d.el);
        break;
    case ELEMENT_PAIR:
        *result = calcRight(*d.pair.first, crs) +
                calcRight(*d.pair.second, crs);
        break;
    case ELEMENT_BASE:
        *result = B2(crs.getG2Base());
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    return *result;
}

void removeCalculations(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<BT*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeCalculations(*d.pair.first);
        removeCalculations(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeftCalc(*d.pair.first);
        removeRightCalc(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeCalculations(const G1Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<BT*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeCalculations(*d.pair.first);
        removeCalculations(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeftCalc(*d.scalar.second);
        removeRightCalc(*d.scalar.first);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeCalculations(const G2Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<BT*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeCalculations(*d.pair.first);
        removeCalculations(*d.pair.second);
        break;
    case ELEMENT_SCALAR:
        removeLeftCalc(*d.scalar.first);
        removeRightCalc(*d.scalar.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeCalculations(const GTData &d) {
    if (!d.d) return;
    delete reinterpret_cast<BT*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
    case ELEMENT_BASE:
        break;
    case ELEMENT_PAIR:
        removeCalculations(*d.pair.first);
        removeCalculations(*d.pair.second);
        break;
    case ELEMENT_PAIRING:
        removeLeftCalc(*d.pring.first);
        removeRightCalc(*d.pring.second);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeLeftCalc(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<B1*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeLeftCalc(*d.pair.first);
        removeLeftCalc(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeLeftCalc(const G1Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<B1*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeLeftCalc(*d.pair.first);
        removeLeftCalc(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeRightCalc(const FpData &d) {
    if (!d.d) return;
    delete reinterpret_cast<B2*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeRightCalc(*d.pair.first);
        removeRightCalc(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeRightCalc(const G2Data &d) {
    if (!d.d) return;
    delete reinterpret_cast<B2*>(d.d);
    d.d = NULL;
    switch (d.type) {
    case ELEMENT_VARIABLE:
    case ELEMENT_CONST_INDEX:
    case ELEMENT_CONST_VALUE:
        break;
    case ELEMENT_PAIR:
        removeRightCalc(*d.pair.first);
        removeRightCalc(*d.pair.second);
        break;
    case ELEMENT_BASE:
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
}

void removeProofZK(const FpData &d);
void removeProofZK(const G1Data &d);
void removeProofZK(const G2Data &d);
void removeProofZK(const GTData &d);
void removeLeftZK(const FpData &d);
void removeLeftZK(const G1Data &d);
void removeRightZK(const FpData &d);
void removeRightZK(const G2Data &d);

void NIZKProof::simulateProof(std::ostream &stream, const CRS &crs,
                              const ProofData &instantiation) const {
    if ((!zk) || (!crs.isSimulationReady())) return;
    if ((instantiation.pubFp.size() != cstsFp.size()) ||
            (instantiation.pubG1.size() != cstsG1.size()) ||
            (instantiation.pubG2.size() != cstsG2.size()) ||
            (instantiation.pubGT.size() != cstsGT.size()))
        throw "Wrong instantiation in NIZKProof::simulateProof!";
    ASSERT(varsFp.size() == varsFpInB1.size(), "Array sizes do not match");
    ASSERT(cstsFp.size() == cstsFpInB1.size(), "Array sizes do not match");
    ASSERT(cstsGT.empty(), "Unexpected non-ZK property");
    G1Commit c1;
    G2Commit c2;
    int j = varsFp.size(), i = additionalFp.size();
    c1.type = COMMIT_ENC;
    c1.c.type = VALUE_NULL;
    c2.type = COMMIT_ENC;
    c2.c.type = VALUE_B;
    while (i-- > 0) {
        if (varsFpInB1[--j]) {
            c1.r = Fp::getRand();
            varsFp[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
            stream << B1::commit(Fp(), c1.r, crs);
        } else {
            c2.r = Fp::getRand();
            c2.c.b2Value = B2::commit(Fp(), c2.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
            stream << c2.c.b2Value;
        }
    }
    while (j-- > 0) {
        if (varsFpInB1[j]) {
            c1.r = Fp::getRand();
            varsFp[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
            stream << B1::commit(c1.c.fpValue, c1.r, crs);
        } else {
            c2.r = Fp::getRand();
            c2.c.b2Value = B2::commit(Fp(), c2.r, crs);
            varsFp[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
            stream << c2.c.b2Value;
        }
    }
    j = varsG1.size();
    i = additionalG1.size();
    while (i-- > 0) {
        --j;
        c1.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G1][j])) {
            c1.type = COMMIT_ENC;
            stream << B1::commit(B1(), c1.r, crs);
        } else {
            c1.type = COMMIT_PRIV;
            c1.s = Fp::getRand();
            stream << B1::commit(B1(), c1.r, c1.s, crs);
        }
        varsG1[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
    }
    while (j-- > 0) {
        c1.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G1][j])) {
            c1.type = COMMIT_ENC;
            stream << B1::commit(B1(), c1.r, crs);
        } else {
            c1.type = COMMIT_PRIV;
            c1.s = Fp::getRand();
            stream << B1::commit(B1(), c1.r, c1.s, crs);
        }
        varsG1[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
    }
    j = varsG2.size();
    i = additionalG2.size();
    c2.c.type = VALUE_B;
    while (i-- > 0) {
        --j;
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G2][j])) {
            c2.type = COMMIT_ENC;
            c2.c.b2Value = B2::commit(G2(), c2.r, crs);
        } else {
            c2.type = COMMIT_PRIV;
            c2.s = Fp::getRand();
            c2.c.b2Value = B2::commit(G2(), c2.r, c2.s, crs);
        }
        stream << c2.c.b2Value;
        varsG2[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
    }
    while (j-- > 0) {
        c2.r = Fp::getRand();
        if ((type == AllEncrypted) ||
                ((type == SelectedEncryption) && sEnc[INDEX_TYPE_G2][j])) {
            c2.type = COMMIT_ENC;
            c2.c.b2Value = B2::commit(G2(), c2.r, crs);
        } else {
            c2.type = COMMIT_PRIV;
            c2.s = Fp::getRand();
            c2.c.b2Value = B2::commit(G2(), c2.r, c2.s, crs);
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
            c1.r = c1.c.fpValue * crs.i1;
            cstsFp[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
        } else {
            c2.c.fpValue = instantiation.pubFp[j];
            c2.r = c2.c.fpValue * crs.i2;
            cstsFp[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
        }
    }
    c1.c.type = VALUE_G;
    for (j = cstsG1.size(); j-- > 0;) {
        c1.c.b1Value._2 = instantiation.pubG1[j];
        cstsG1[j]->d = reinterpret_cast<void*>(new G1Commit(c1));
    }
    c2.c.type = VALUE_G;
    c2.c.b2Value._1.clear();
    for (j = cstsG2.size(); j-- > 0;) {
        c2.c.b2Value._2 = instantiation.pubG2[j];
        cstsG2[j]->d = reinterpret_cast<void*>(new G2Commit(c2));
    }
    for (i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        getProofZK(left, crs, tFp[i]);
        getProofZK(right, crs, tFp[i]);
        writeEqProof(stream, left.d, right.d, tFp[i], crs);
    }
    for (i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        getProofZK(left, crs, tG1[i]);
        getProofZK(right, crs, tG1[i]);
        writeEqProof(stream, left.d, right.d, tG1[i], crs);
    }
    for (i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        getProofZK(left, crs, tG2[i]);
        getProofZK(right, crs, tG2[i]);
        writeEqProof(stream, left.d, right.d, tG2[i], crs);
    }
    for (i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        getProofZK(left, crs, tGT[i]);
        getProofZK(right, crs, tGT[i]);
        writeEqProof(stream, left.d, right.d, tGT[i], crs);
    }
    for (int i = eqsFp.size(); i-- > 0;) {
        const FpData &left = *eqsFp[i].first;
        const FpData &right = *eqsFp[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (int i = eqsG1.size(); i-- > 0;) {
        const G1Data &left = *eqsG1[i].first;
        const G1Data &right = *eqsG1[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (int i = eqsG2.size(); i-- > 0;) {
        const G2Data &left = *eqsG2[i].first;
        const G2Data &right = *eqsG2[i].second;
        removeProof(left);
        removeProof(right);
    }
    for (int i = eqsGT.size(); i-- > 0;) {
        const GTData &left = *eqsGT[i].first;
        const GTData &right = *eqsGT[i].second;
        removeProof(left);
        removeProof(right);
    }
}

void NIZKProof::getProofZK(const FpData &d, const CRS &crs,
                           EqProofType t) const {
    ProofEls *proofEl = (d.d ? (reinterpret_cast<ProofEls*>(d.d))
                             : (new ProofEls));
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        if (t == EQ_TYPE_QConst_H) {
            proofEl->p2_v.type = VALUE_Fp;
            proofEl->p2_v.fpValue = d.el * crs.i1;
            proofEl->p1_v.type = VALUE_NULL;
        } else {
            proofEl->p1_v.type = VALUE_Fp;
            proofEl->p1_v.fpValue = d.el * crs.i2;
            proofEl->p2_v.type = VALUE_NULL;
        }
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        break;
    case ELEMENT_BASE:
        if (t == EQ_TYPE_QConst_H) {
            proofEl->p2_v.type = VALUE_Fp;
            proofEl->p2_v.fpValue = crs.i1;
            proofEl->p1_v.type = VALUE_NULL;
        } else {
            proofEl->p1_v.type = VALUE_Fp;
            proofEl->p1_v.fpValue = crs.i2;
            proofEl->p2_v.type = VALUE_NULL;
        }
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        break;
    case ELEMENT_PAIR:
        {
            getProofZK(*d.pair.first, crs, t);
            getProofZK(*d.pair.second, crs, t);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
            break;
        }
    case ELEMENT_SCALAR:
        {
            getLeftZK(*d.pair.first, crs, t);
            getRightZK(*d.pair.second, crs, t);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            scalarCombine(el1, el2, *proofEl);
            break;
        }
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(proofEl);
}

void NIZKProof::getProofZK(const G1Data &d, const CRS &crs,
                           EqProofType t) const {
    ProofEls *proofEl = (d.d ? (reinterpret_cast<ProofEls*>(d.d))
                             : (new ProofEls));
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        proofEl->p1_v.type = VALUE_G;
        proofEl->p1_v.b1Value._2 = crs.i2 * d.el;
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_v.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        break;
    case ELEMENT_BASE:
        if (t != EQ_TYPE_MLin_G) {
            proofEl->p1_v.type = VALUE_G;
            proofEl->p1_v.b1Value._2 = crs.i2 * d.el;
            proofEl->p1_w.type = VALUE_NULL;
            proofEl->p2_v.type = VALUE_NULL;
            proofEl->p2_w.type = VALUE_NULL;
        } else {
            proofEl->p1_v.type = VALUE_NULL;
            proofEl->p1_w.type = VALUE_NULL;
            proofEl->p2_v.type = VALUE_Fp;
            proofEl->p2_v.fpValue = crs.i1;
            proofEl->p2_w.type = VALUE_Fp;
            proofEl->p2_w.fpValue = Fp(-1);
        }
        break;
    case ELEMENT_PAIR:
        {
            getProofZK(*d.pair.first, crs, t);
            getProofZK(*d.pair.second, crs, t);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
            break;
        }
    case ELEMENT_SCALAR:
        {
            getLeftZK(*d.scalar.second, crs, t);
            getRightZK(*d.scalar.first, crs, t);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.scalar.second->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.scalar.first->d);
            scalarCombine(el1, el2, *proofEl);
            break;
        }
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(proofEl);
}

void NIZKProof::getProofZK(const G2Data &d, const CRS &crs,
                           EqProofType t) const {
    ProofEls *proofEl = (d.d ? (reinterpret_cast<ProofEls*>(d.d))
                             : (new ProofEls));
    switch (d.type) {
    case ELEMENT_CONST_VALUE:
        proofEl->p2_v.type = VALUE_G;
        proofEl->p2_v.b2Value._2 = crs.i1 * d.el;
        proofEl->p1_v.type = VALUE_NULL;
        proofEl->p1_w.type = VALUE_NULL;
        proofEl->p2_w.type = VALUE_NULL;
        break;
    case ELEMENT_BASE:
        if (t != EQ_TYPE_MLin_H) {
            proofEl->p2_v.type = VALUE_G;
            proofEl->p2_v.b2Value._2 = crs.i1 * d.el;
            proofEl->p1_v.type = VALUE_NULL;
            proofEl->p1_w.type = VALUE_NULL;
            proofEl->p2_w.type = VALUE_NULL;
        } else {
            proofEl->p1_v.type = VALUE_Fp;
            proofEl->p1_v.fpValue = crs.i2;
            proofEl->p1_w.type = VALUE_Fp;
            proofEl->p1_w.fpValue = Fp(-1);
            proofEl->p2_v.type = VALUE_NULL;
            proofEl->p2_w.type = VALUE_NULL;
        }
        break;
    case ELEMENT_PAIR:
        {
            getProofZK(*d.pair.first, crs, t);
            getProofZK(*d.pair.second, crs, t);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
            break;
        }
    case ELEMENT_SCALAR:
        {
            getLeftZK(*d.scalar.first, crs, t);
            getRightZK(*d.scalar.second, crs, t);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.scalar.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.scalar.second->d);
            scalarCombine(el1, el2, *proofEl);
            break;
        }
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(proofEl);
}

void NIZKProof::getProofZK(const GTData &d, const CRS &crs,
                           EqProofType t) const {
    ProofEls *proofEl = (d.d ? (reinterpret_cast<ProofEls*>(d.d))
                             : (new ProofEls));
    switch (d.type) {
    case ELEMENT_BASE:
        if ((t == EQ_TYPE_PEnc_G) || (t == EQ_TYPE_PConst_G)) {
            proofEl->p1_v.type = VALUE_G;
            proofEl->p1_v.b1Value._2 = crs.i2 * crs.getG1Base();
            proofEl->p1_w.type = VALUE_G;
            proofEl->p1_w.b1Value._2 = -crs.getG1Base();
            proofEl->p2_v.type = VALUE_NULL;
            proofEl->p2_w.type = VALUE_NULL;
        } else {
            proofEl->p1_v.type = VALUE_NULL;
            proofEl->p1_w.type = VALUE_NULL;
            proofEl->p2_v.type = VALUE_G;
            proofEl->p2_v.b2Value._2 = crs.i1 * crs.getG2Base();
            proofEl->p2_w.type = VALUE_G;
            proofEl->p2_w.b2Value._2 = -crs.getG2Base();
        }
        break;
    case ELEMENT_PAIR:
        {
            getProofZK(*d.pair.first, crs, t);
            getProofZK(*d.pair.second, crs, t);
            const ProofEls &el1 =
                    *reinterpret_cast<const ProofEls*>(d.pair.first->d);
            const ProofEls &el2 =
                    *reinterpret_cast<const ProofEls*>(d.pair.second->d);
            addAllPi(el1, el2, *proofEl, crs);
            break;
        }
    case ELEMENT_PAIRING:
        {
            getLeftZK(*d.pring.first, crs, t);
            getRightZK(*d.pring.second, crs, t);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pring.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pring.second->d);
            scalarCombine(el1, el2, *proofEl);
            break;
        }
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(proofEl);
}

bool cheatLeft(EqProofType t) {
    switch (t) {
    case EQ_TYPE_PPE:
    case EQ_TYPE_PEnc_H:
    case EQ_TYPE_PConst_H:
    case EQ_TYPE_MLin_G:
    case EQ_TYPE_ME_H:
    case EQ_TYPE_MEnc_H:
    case EQ_TYPE_MConst_H:
    case EQ_TYPE_QE:
    case EQ_TYPE_QConst_H:
        return true;
    case EQ_TYPE_PEnc_G:
    case EQ_TYPE_PConst_G:
    case EQ_TYPE_MLin_H:
    case EQ_TYPE_ME_G:
    case EQ_TYPE_MEnc_G:
    case EQ_TYPE_MConst_G:
    case EQ_TYPE_QConst_G:
        return false;
    default:
        ASSERT(false, "Unexpected equation type");
        return false;
    }
}

bool cheatRight(EqProofType t) {
    switch (t) {
    case EQ_TYPE_PEnc_H:
    case EQ_TYPE_PConst_H:
    case EQ_TYPE_MLin_G:
    case EQ_TYPE_ME_H:
    case EQ_TYPE_MEnc_H:
    case EQ_TYPE_MConst_H:
    case EQ_TYPE_QConst_H:
        return false;
    case EQ_TYPE_PPE:
    case EQ_TYPE_PEnc_G:
    case EQ_TYPE_PConst_G:
    case EQ_TYPE_MLin_H:
    case EQ_TYPE_ME_G:
    case EQ_TYPE_MEnc_G:
    case EQ_TYPE_MConst_G:
    case EQ_TYPE_QE:
    case EQ_TYPE_QConst_G:
        return true;
    default:
        ASSERT(false, "Unexpected equation type");
        return false;
    }
}

void NIZKProof::getLeftZK(const FpData &d, const CRS &crs,
                          EqProofType t) const {
    G1Commit *c1 = (d.d ? (reinterpret_cast<G1Commit*>(d.d)) : (new G1Commit));
    switch (d.type) {
    case ELEMENT_VARIABLE:
        ASSERT(d.d, "Variable not instantiated");
        return;
    case ELEMENT_CONST_INDEX:
        ASSERT(d.d, "Constant not instantiated");
        if (cheatLeft(t)) {
            c1->type = COMMIT_ENC;
            c1->c.type = VALUE_NULL;
        } else {
            c1->type = COMMIT_PUB;
            c1->c.type = VALUE_Fp;
        }
        return;
    case ELEMENT_CONST_VALUE:
        if (cheatLeft(t)) {
            c1->type = COMMIT_ENC;
            c1->c.type = VALUE_NULL;
        } else {
            c1->type = COMMIT_PUB;
            c1->c.type = VALUE_Fp;
        }
        if (d.d) return;
        c1->c.fpValue = d.el;
        c1->r = d.el * crs.i1;
        break;
    case ELEMENT_PAIR:
        {
            getLeftZK(*d.pair.first, crs, t);
            getLeftZK(*d.pair.second, crs, t);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G1Commit &el2 =
                    *reinterpret_cast<const G1Commit*>(d.pair.second->d);
            addCommitG1(el1, el2, *c1, crs);
            break;
        }
    case ELEMENT_BASE:
        if (cheatLeft(t)) {
            c1->type = COMMIT_ENC;
            c1->c.type = VALUE_NULL;
        } else {
            c1->type = COMMIT_PUB;
            c1->c.type = VALUE_Fp;
        }
        if (d.d) return;
        c1->c.fpValue = Fp::getUnit();
        c1->r = crs.i1;
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(c1);
}

void NIZKProof::getLeftZK(const G1Data &d, const CRS &crs,
                          EqProofType t) const {
    G1Commit *c1 = (d.d ? (reinterpret_cast<G1Commit*>(d.d)) : (new G1Commit));
    switch (d.type) {
    case ELEMENT_VARIABLE:
        ASSERT(d.d, "Variable not instantiated");
        return;
    case ELEMENT_CONST_INDEX:
        ASSERT(d.d, "Constant not instantiated");
        return;
    case ELEMENT_CONST_VALUE:
        if (d.d) return;
        c1->type = COMMIT_PUB;
        c1->c.type = VALUE_G;
        c1->c.b1Value._2 = d.el;
        break;
    case ELEMENT_PAIR:
        {
            getLeftZK(*d.pair.first, crs, t);
            getLeftZK(*d.pair.second, crs, t);
            const G1Commit &el1 =
                    *reinterpret_cast<const G1Commit*>(d.pair.first->d);
            const G1Commit &el2 =
                    *reinterpret_cast<const G1Commit*>(d.pair.second->d);
            addCommitG1(el1, el2, *c1, crs);
            break;
        }
    case ELEMENT_BASE:
        if (cheatLeft(t)) {
            c1->type = COMMIT_PRIV;
            c1->c.type = VALUE_NULL;
        } else {
            c1->type = COMMIT_PUB;
            c1->c.type = VALUE_G;
        }
        if (d.d) return;
        c1->c.b1Value._2 = crs.getG1Base();
        c1->r = crs.i1;
        c1->s = Fp(-1);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(c1);
}

void NIZKProof::getRightZK(const FpData &d, const CRS &crs,
                           EqProofType t) const {
    G2Commit *c2 = (d.d ? (reinterpret_cast<G2Commit*>(d.d)) : (new G2Commit));
    switch (d.type) {
    case ELEMENT_VARIABLE:
        ASSERT(d.d, "Variable not instantiated");
        return;
    case ELEMENT_CONST_INDEX:
        ASSERT(d.d, "Constant not instantiated");
        c2->type = (cheatRight(t) ? COMMIT_ENC : COMMIT_PUB);
        return;
    case ELEMENT_CONST_VALUE:
        c2->type = (cheatRight(t) ? COMMIT_ENC : COMMIT_PUB);
        if (d.d) return;
        c2->r = d.el * crs.i2;
        c2->c.fpValue = d.el;
        break;
    case ELEMENT_PAIR:
        {
            getRightZK(*d.pair.first, crs, t);
            getRightZK(*d.pair.second, crs, t);
            const G2Commit &el1 =
                    *reinterpret_cast<const G2Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            addCommitG2(el1, el2, *c2, crs);
            break;
        }
    case ELEMENT_BASE:
        c2->type = (cheatRight(t) ? COMMIT_ENC : COMMIT_PUB);
        if (d.d) return;
        c2->c.type = VALUE_Fp;
        c2->c.fpValue = Fp::getUnit();
        c2->r = crs.i2;
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(c2);
}

void NIZKProof::getRightZK(const G2Data &d, const CRS &crs,
                           EqProofType t) const {
    G2Commit *c2 = (d.d ? (reinterpret_cast<G2Commit*>(d.d)) : (new G2Commit));
    switch (d.type) {
    case ELEMENT_VARIABLE:
        ASSERT(d.d, "Variable not instantiated");
        return;
    case ELEMENT_CONST_INDEX:
        ASSERT(d.d, "Constant not instantiated");
        return;
    case ELEMENT_CONST_VALUE:
        if (d.d) return;
        c2->type = COMMIT_PUB;
        c2->c.type = VALUE_G;
        c2->c.b2Value._2 = d.el;
        break;
    case ELEMENT_PAIR:
        {
            getRightZK(*d.pair.first, crs, t);
            getRightZK(*d.pair.second, crs, t);
            const G2Commit &el1 =
                    *reinterpret_cast<const G2Commit*>(d.pair.first->d);
            const G2Commit &el2 =
                    *reinterpret_cast<const G2Commit*>(d.pair.second->d);
            addCommitG2(el1, el2, *c2, crs);
            break;
        }
    case ELEMENT_BASE:
        c2->type = (cheatLeft(t) ? COMMIT_PRIV : COMMIT_PUB);
        if (d.d) return;
        c2->c.type = VALUE_G;
        c2->c.b2Value._2 = crs.getG2Base();
        c2->r = crs.i2;
        c2->s = Fp(-1);
        break;
    default:
        ASSERT(false, "Unexpected data type");
    }
    d.d = reinterpret_cast<void*>(c2);
}

} /* End of namespace nizk */
