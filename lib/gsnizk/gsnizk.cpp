#include "gsnizk.h"

namespace gsnizk {

FpElement::FpElement(const FpElement &other) : type(other.type) {
    switch (type) {
    case ELEMENT_CONST_VALUE:
        new (&el) Fp(other.el);
        return;
    case ELEMENT_PAIR:
        new (&ptr) SharedPtrFp(other.ptr);
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
            return *this;
        case ELEMENT_PAIR:
            ptr = other.ptr;
            return *this;
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
        ptr.~SharedPtrFp();
        return;
    default:
        return;
    }
}

} /* End of namespace nizk */
