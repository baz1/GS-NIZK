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

void getIndexes(const FpData &d,
                std::set<int> &FpVars, std::set<int> &FpConsts,
                std::set<int> &G1Vars, std::set<int> &G1Consts,
                std::set<int> &G2Vars, std::set<int> &G2Consts,
                std::set<int> &GTVars, std::set<int> &GTConsts,
                SAT_NODE *node = NULL) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        FpVars.insert(d.index);
        if (node) {
            node->type = SAT_NODE_INDEX;
            node->idx.index_type = INDEX_TYPE_Fp;
            node->idx.index = d.index;
        }
        return;
    case ELEMENT_CONST_INDEX:
        FpConsts.insert(d.index);
    case ELEMENT_CONST_VALUE:
        if (node)
            node->type = SAT_NODE_FALSE;
        return;
    case ELEMENT_PAIR:
        if (node)
            node->type = SAT_NODE_AND;
        break;
    case ELEMENT_SCALAR:
        if (node)
            node->type = SAT_NODE_OR;
        break;
    case ELEMENT_BASE:
        if (node)
            node->type = SAT_NODE_TRUE;
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
    if (node) {
        node->pair.left = new SAT_NODE;
        getIndexes(*d.pair.first, FpVars, FpConsts,
                   G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                   node->pair.left);
        node->pair.right = new SAT_NODE;
        getIndexes(*d.pair.second, FpVars, FpConsts,
                   G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                   node->pair.right);
    } else {
        getIndexes(*d.pair.first, FpVars, FpConsts,
                   G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        getIndexes(*d.pair.second, FpVars, FpConsts,
                   G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
    }
}

void getIndexes(const G1Data &d,
                std::set<int> &FpVars, std::set<int> &FpConsts,
                std::set<int> &G1Vars, std::set<int> &G1Consts,
                std::set<int> &G2Vars, std::set<int> &G2Consts,
                std::set<int> &GTVars, std::set<int> &GTConsts,
                SAT_NODE *node = NULL) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        G1Vars.insert(d.index);
        if (node) {
            node->type = SAT_NODE_INDEX;
            node->idx.index_type = INDEX_TYPE_G1;
            node->idx.index = d.index;
        }
        return;
    case ELEMENT_CONST_INDEX:
        G1Consts.insert(d.index);
    case ELEMENT_CONST_VALUE:
        if (node)
            node->type = SAT_NODE_FALSE;
        return;
    case ELEMENT_PAIR:
        if (node) {
            node->type = SAT_NODE_AND;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.pair.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.pair.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.pair.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.pair.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    case ELEMENT_SCALAR:
        if (node) {
            node->type = SAT_NODE_OR;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.scalar.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.scalar.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.scalar.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.scalar.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    case ELEMENT_BASE:
        if (node)
            node->type = SAT_NODE_TRUE;
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

void getIndexes(const G2Data &d,
                std::set<int> &FpVars, std::set<int> &FpConsts,
                std::set<int> &G1Vars, std::set<int> &G1Consts,
                std::set<int> &G2Vars, std::set<int> &G2Consts,
                std::set<int> &GTVars, std::set<int> &GTConsts,
                SAT_NODE *node = NULL) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        G2Vars.insert(d.index);
        if (node) {
            node->type = SAT_NODE_INDEX;
            node->idx.index_type = INDEX_TYPE_G2;
            node->idx.index = d.index;
        }
        return;
    case ELEMENT_CONST_INDEX:
        G2Consts.insert(d.index);
    case ELEMENT_CONST_VALUE:
        if (node)
            node->type = SAT_NODE_FALSE;
        return;
    case ELEMENT_PAIR:
        if (node) {
            node->type = SAT_NODE_AND;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.pair.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.pair.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.pair.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.pair.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    case ELEMENT_SCALAR:
        if (node) {
            node->type = SAT_NODE_OR;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.scalar.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.scalar.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.scalar.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.scalar.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    case ELEMENT_BASE:
        if (node)
            node->type = SAT_NODE_TRUE;
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

void getIndexes(const GTData &d,
                std::set<int> &FpVars, std::set<int> &FpConsts,
                std::set<int> &G1Vars, std::set<int> &G1Consts,
                std::set<int> &G2Vars, std::set<int> &G2Consts,
                std::set<int> &GTVars, std::set<int> &GTConsts,
                SAT_NODE *node = NULL) {
    switch (d.type) {
    case ELEMENT_VARIABLE:
        GTVars.insert(d.index);
        if (node) {
            node->type = SAT_NODE_INDEX;
            node->idx.index_type = INDEX_TYPE_GT;
            node->idx.index = d.index;
        }
        return;
    case ELEMENT_CONST_INDEX:
        GTConsts.insert(d.index);
    case ELEMENT_CONST_VALUE:
        if (node)
            node->type = SAT_NODE_FALSE;
        return;
    case ELEMENT_PAIR:
        if (node) {
            node->type = SAT_NODE_AND;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.pair.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.pair.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.pair.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.pair.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    case ELEMENT_SCALAR:
        if (node) {
            node->type = SAT_NODE_OR;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.scalar.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.scalar.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.scalar.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.scalar.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    case ELEMENT_PAIRING:
        if (node) {
            node->type = SAT_NODE_OR;
            node->pair.left = new SAT_NODE;
            getIndexes(*d.pring.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.left);
            node->pair.right = new SAT_NODE;
            getIndexes(*d.pring.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       node->pair.right);
        } else {
            getIndexes(*d.pring.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            getIndexes(*d.pring.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        return;
    default:
        ASSERT(false /* Unexpected data type */);
        return;
    }
}

int checkIndexesSet(const std::set<int> &s) {
    std::set<int>::const_iterator it = s.cbegin();
    int expect = 0;
    while (it != s.cend()) {
        if (*it != expect)
            return -1;
        ++expect;
        ++it;
    }
    return s.size();
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

bool NIZKProof::endEquations() {
    /* Subsequent calls are ignored. */
    if (fixed) return true;
    SAT_NODE *current = NULL, *root;
    {
        std::set<int> FpVars, FpConsts, G1Vars, G1Consts;
        std::set<int> G2Vars, G2Consts, GTVars, GTConsts;
        if (type == SelectedEncryption)
            root = (current = new SAT_NODE);
        for (const PairFp &p : eqsFp) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                current = new SAT_NODE;
                current->type = SAT_NODE_AND;
                current->pair.left = root;
                root = current;
                root->pair.right = (current = new SAT_NODE);
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
        }
        if (type == SelectedEncryption) {
            current = new SAT_NODE;
            current->type = SAT_NODE_AND;
            current->pair.left = root;
            root = current;
            root->pair.right = (current = new SAT_NODE);
        }
        for (const PairG1 &p : eqsG1) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                current = new SAT_NODE;
                current->type = SAT_NODE_AND;
                current->pair.left = root;
                root = current;
                root->pair.right = (current = new SAT_NODE);
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
        }
        if (type == SelectedEncryption) {
            current = new SAT_NODE;
            current->type = SAT_NODE_AND;
            current->pair.left = root;
            root = current;
            root->pair.right = (current = new SAT_NODE);
        }
        for (const PairG2 &p : eqsG2) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                current = new SAT_NODE;
                current->type = SAT_NODE_AND;
                current->pair.left = root;
                root = current;
                root->pair.right = (current = new SAT_NODE);
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
        }
        if (type == SelectedEncryption) {
            current = new SAT_NODE;
            current->type = SAT_NODE_AND;
            current->pair.left = root;
            root = current;
            root->pair.right = (current = new SAT_NODE);
        }
        for (const PairGT &p : eqsGT) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                current = new SAT_NODE;
                current->type = SAT_NODE_AND;
                current->pair.left = root;
                root = current;
                root->pair.right = (current = new SAT_NODE);
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
        }
        varFp = checkIndexesSet(FpVars);
        if (varFp < 0) return false;
        cstFp = checkIndexesSet(FpConsts);
        if (cstFp < 0) return false;
        varG1 = checkIndexesSet(G1Vars);
        if (varG1 < 0) return false;
        cstG1 = checkIndexesSet(G1Consts);
        if (cstG1 < 0) return false;
        varG2 = checkIndexesSet(G2Vars);
        if (varG2 < 0) return false;
        cstG2 = checkIndexesSet(G2Consts);
        if (cstG2 < 0) return false;
        varGT = checkIndexesSet(GTVars);
        if (varGT < 0) return false;
        cstGT = checkIndexesSet(GTConsts);
        if (cstGT < 0) return false;
    }
    if (type == SelectedEncryption) {
        std::vector<int> cnt[4];
        sEnc[0].resize(varFp, SAT_VALUE_UNSET);
        cnt[0].reserve(varFp);
        sEnc[1].resize(varG1, SAT_VALUE_UNSET);
        cnt[1].reserve(varG1);
        sEnc[2].resize(varG2, SAT_VALUE_UNSET);
        cnt[2].reserve(varG2);
        sEnc[3].resize(varGT, SAT_VALUE_UNSET);
        cnt[3].reserve(varGT);
        if (tryPermutation(root, sEnc, cnt) < 0)
            throw "Cannot use ZK with the equations provided (in gsnizk)";
        /* Converting to 0/1 (boolean) values; 1 for encrypted */
        for (int i = 4; i--;) {
            for (int j = sEnc[i].size(); j--;)
                --sEnc[i][j];
        }
    }
    // TODO fill the equation proof types
    fixed = true;
    return true;
}

void NIZKProof::writeProof(std::ostream &stream, const CRS &crs) {
    // TODO
}

} /* End of namespace nizk */
