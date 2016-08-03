#include "gsnizk.h"

namespace gsnizk {

FpElement::FpElement(const FpElement &other) : type(other.type) {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        new (&el) Fp(other.el);
        return;
    case ELEMENT_PAIR:
        new (&pair) PairFp(other.pair);
        return;
    default:
        index = other.index;
    }
}

FpElement &FpElement::operator=(const FpElement &other) {
    if (type == other.type) {
        switch (type) {
        case ELEMENT_CONST_VALUE:
            el = other.el;
            break;
        case ELEMENT_PAIR:
            pair = other.pair;
            break;
        default:
            index = other.index;
        }
        return *this;
    }
    this->~FpElement();
    new (this) FpElement(other);
    return *this;
}

FpElement::~FpElement() {
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

FpElement &FpElement::operator*=(const FpElement &other) {
    PairFp tmp = PairFp(new (std::pair<FpElement,FpElement>)(*this, other));
    if (type == ELEMENT_PAIR) {
        pair = tmp;
    } else {
        this->~FpElement();
        type = ELEMENT_PAIR;
        new (&pair) PairFp(tmp);
    }
    return *this;
}

G1Element::G1Element(const G1Element &other) : type(other.type) {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        new (&el) G1(other.el);
        return;
    case ELEMENT_PAIR:
        new (&pair) PairG1(other.pair);
        return;
    case ELEMENT_SCALAR:
        new (&scalar) ScalarG1(other.scalar);
        return;
    default:
        index = other.index;
    }
}

G1Element &G1Element::operator=(const G1Element &other) {
    if (type == other.type) {
        switch (type) {
        case ELEMENT_CONST_VALUE:
            el = other.el;
            break;
        case ELEMENT_PAIR:
            pair = other.pair;
            break;
        case ELEMENT_SCALAR:
            scalar = other.scalar;
            break;
        default:
            index = other.index;
        }
        return *this;
    }
    this->~G1Element();
    new (this) G1Element(other);
    return *this;
}

G1Element::~G1Element() {
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

G1Element &G1Element::operator+=(const G1Element &other) {
    PairG1 tmp = PairG1(new (std::pair<G1Element,G1Element>)(*this, other));
    if (type == ELEMENT_PAIR) {
        pair = tmp;
    } else {
        this->~G1Element();
        type = ELEMENT_PAIR;
        new (&pair) PairG1(tmp);
    }
    return *this;
}

G2Element::G2Element(const G2Element &other) : type(other.type) {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        new (&el) G2(other.el);
        return;
    case ELEMENT_PAIR:
        new (&pair) PairG2(other.pair);
        return;
    case ELEMENT_SCALAR:
        new (&scalar) ScalarG2(other.scalar);
        return;
    default:
        index = other.index;
    }
}

G2Element &G2Element::operator=(const G2Element &other) {
    if (type == other.type) {
        switch (type) {
        case ELEMENT_CONST_VALUE:
            el = other.el;
            break;
        case ELEMENT_PAIR:
            pair = other.pair;
            break;
        case ELEMENT_SCALAR:
            scalar = other.scalar;
            break;
        default:
            index = other.index;
        }
        return *this;
    }
    this->~G2Element();
    new (this) G2Element(other);
    return *this;
}

G2Element::~G2Element() {
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

G2Element &G2Element::operator+=(const G2Element &other) {
    PairG2 tmp = PairG2(new (std::pair<G2Element,G2Element>)(*this, other));
    if (type == ELEMENT_PAIR) {
        pair = tmp;
    } else {
        this->~G2Element();
        type = ELEMENT_PAIR;
        new (&pair) PairG2(tmp);
    }
    return *this;
}

GTElement::GTElement(const GTElement &other) : type(other.type) {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        new (&el) GT(other.el);
        return;
    case ELEMENT_PAIR:
        new (&pair) PairGT(other.pair);
        return;
    case ELEMENT_SCALAR:
        new (&scalar) ScalarGT(other.scalar);
        return;
    case ELEMENT_PAIRING:
        new (&pring) PairingGT(other.pring);
        return;
    default:
        index = other.index;
    }
}

GTElement &GTElement::operator=(const GTElement &other) {
    if (type == other.type) {
        switch (type) {
        case ELEMENT_CONST_VALUE:
            el = other.el;
            break;
        case ELEMENT_PAIR:
            pair = other.pair;
            break;
        case ELEMENT_SCALAR:
            scalar = other.scalar;
            break;
        case ELEMENT_PAIRING:
            pring = other.pring;
            break;
        default:
            index = other.index;
        }
        return *this;
    }
    this->~GTElement();
    new (this) GTElement(other);
    return *this;
}

GTElement::~GTElement() {
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

GTElement &GTElement::operator*=(const GTElement &other) {
    PairGT tmp = PairGT(new (std::pair<GTElement,GTElement>)(*this, other));
    if (type == ELEMENT_PAIR) {
        pair = tmp;
    } else {
        this->~GTElement();
        type = ELEMENT_PAIR;
        new (&pair) PairGT(tmp);
    }
    return *this;
}

} /* End of namespace nizk */
