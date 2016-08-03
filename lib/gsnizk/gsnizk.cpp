#include "gsnizk.h"

namespace gsnizk {

FpData::~FpData() {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        el.~Fp();
        return;
    case ELEMENT_PAIR:
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

FpElement &FpElement::operator*=(const FpElement &other) {
    FpData *d = new FpData(ELEMENT_PAIR);
    new (&d->pair) PairFp(data, other.data);
    data = std::shared_ptr<FpData>(d);
    return *this;
}

FpElement FpElement::operator*(const FpElement &other) const {
    FpData *d = new FpData(ELEMENT_PAIR);
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

} /* End of namespace nizk */
