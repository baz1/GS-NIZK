#include "gsnizk.h"

#include <set>

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
    default:
        throw "Unexpected data type in gsnizk::endEquations";
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
    default:
        throw "Unexpected data type in gsnizk::endEquations";
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
    default:
        throw "Unexpected data type in gsnizk::endEquations";
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
        throw "Unexpected data type in gsnizk::endEquations";
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

void simplify(std::vector<SAT_NODE*> &nodes) {
    std::vector<SAT_NODE*>::iterator it = nodes.begin();
    while (it != nodes.end()) {
        simplify(*it);
        switch ((*it)->type) {
        case SAT_NODE_FALSE:
            throw "Cannot use ZK with the provided equations in gsnizk";
        case SAT_NODE_TRUE:
            it = nodes.erase(it);
            break;
        default:
            ++it;
        }
    }
}

void countIndexes(SAT_NODE *node, std::vector<int> &cnt[4]) {
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
        throw "Unexpected error";
    }
}

void countIndexes(std::vector<SAT_NODE*> &nodes, std::vector<int> &cnt[4]) {
    for (const SAT_NODE *node : nodes)
        countIndexes(node, cnt);
}

bool NIZKProof::endEquations() {
    /* Subsequent calls are ignored. */
    if (fixed) return true;
    std::vector<SAT_NODE*> nodes;
    SAT_NODE *current = NULL;
    {
        std::set<int> FpVars, FpConsts, G1Vars, G1Consts;
        std::set<int> G2Vars, G2Consts, GTVars, GTConsts;
        if (type == SelectedEncryption)
            current = new SAT_NODE;
        for (const PairFp &p : eqsFp) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts,
                       current);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                nodes.push_back(current);
                current = new SAT_NODE;
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        if (type == SelectedEncryption) {
            nodes.push_back(current);
            current = new SAT_NODE;
        }
        for (const PairG1 &p : eqsG1) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                nodes.push_back(current);
                current = new SAT_NODE;
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        if (type == SelectedEncryption) {
            nodes.push_back(current);
            current = new SAT_NODE;
        }
        for (const PairG2 &p : eqsG2) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                nodes.push_back(current);
                current = new SAT_NODE;
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        if (type == SelectedEncryption) {
            nodes.push_back(current);
            current = new SAT_NODE;
        }
        for (const PairGT &p : eqsGT) {
            getIndexes(*p.first, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
            if (!p.second) continue;
            if (type == SelectedEncryption) {
                nodes.push_back(current);
                current = new SAT_NODE;
            }
            getIndexes(*p.second, FpVars, FpConsts,
                       G1Vars, G1Consts, G2Vars, G2Consts, GTVars, GTConsts);
        }
        if (type == SelectedEncryption)
            nodes.push_back(current);
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

        // TODO selected commitments precomputation
    }
    fixed = true;
    return true;
}

} /* End of namespace nizk */
