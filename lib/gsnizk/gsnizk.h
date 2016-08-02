#ifndef GSNIZK_H
#define GSNIZK_H

#include "maps.h"

/**
 * @file gsnizk.h
 * @brief Groth and Sahai's NIZK proof system.
 *
 * This is the main file to include to use the @ref gsnizk
 * namespace which is at the center of this library.
 */

/**
 * @brief Groth and Sahai's NIZK proof system.
 *
 * This namespace contains a complete implementation
 * of the Non-Interactive Zero Knowledge (NIZK) proof
 * scheme originally designed by Groth and Sahai @cite Groth2007,
 * with corrections as suggested by Ghadafi et al. @cite Ghadafi2009,
 * Blazy et al. @cite Blazy2010 and Escala et al. @cite Escala2013.
 */
namespace gsnizk {

/**
 * @cond INTERNAL_DATA_STRUCT
 */
enum ElementType {
    ELEMENT_VARIABLE,
    ELEMENT_CONST_INDEX,
    ELEMENT_CONST_VALUE
};
/**
 * @endcond
 */

/**
 * @brief A @f$\mathbb{F}_p@f$ element inside an equation.
 */
class FpElement {
    inline FpElement(const FpElement &other);
    inline FpElement &operator=(const FpElement &other);
    inline ~FpElement();
private:
    inline FpElement(ElementType type);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{F}_p@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{F}_p@f$.
     * @return The variable element.
     */
    inline friend FpElement FpVar(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{F}_p@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{F}_p@f$.
     * @return The constant element.
     */
    inline friend FpElement FpConst(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{F}_p@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{F}_p@f$.
     * @return The constant element.
     */
    inline friend FpElement FpConst(Fp value);
private:
    ElementType type;
    union {
        int index;
        Fp el;
    };
};

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

inline FpElement::FpElement(const FpElement &other) : type(other.type) {
    if (type == ELEMENT_CONST_VALUE)
        new (&el) Fp(other.el);
    else
        index = other.index;
}

inline FpElement &FpElement::operator=(const FpElement &other) {
    if (type == ELEMENT_CONST_VALUE) {
        if (other.type == ELEMENT_CONST_VALUE) {
            el = other.el;
        } else {
            el.~Fp();
            index = other.index;
        }
    } else {
        if (other.type == ELEMENT_CONST_VALUE) {
            new (&el) Fp(other.el);
        } else {
            index = other.index;
        }
    }
    type = other.type;
    return *this;
}

inline FpElement::~FpElement() {
    if (type == ELEMENT_CONST_VALUE)
        el.~Fp();
}

inline FpElement::FpElement(ElementType type) : type(type) {}

inline FpElement FpVar(int index) {
    FpElement el(ELEMENT_VARIABLE);
    el.index = index;
    return el;
}

inline FpElement FpConst(int index) {
    FpElement el(ELEMENT_CONST_INDEX);
    el.index = index;
    return el;
}

inline FpElement FpConst(Fp value) {
    FpElement el(ELEMENT_CONST_VALUE);
    new (&el.el) Fp(value);
    return el;
}

} /* End of gsnizk */

#endif // GSNIZK_H
