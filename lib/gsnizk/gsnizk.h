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
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    inline FpElement(const FpElement &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    inline FpElement &operator=(const FpElement &other);
    /**
     * @brief Destructs the element.
     */
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
 * @brief A @f$\mathbb{G}_1@f$ element inside an equation.
 */
class G1Element {
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    inline G1Element(const G1Element &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    inline G1Element &operator=(const G1Element &other);
    /**
     * @brief Destructs the element.
     */
    inline ~G1Element();
private:
    inline G1Element(ElementType type);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_1@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_1@f$.
     * @return The variable element.
     */
    inline friend G1Element G1Var(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_1@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_1@f$.
     * @return The constant element.
     */
    inline friend G1Element G1Const(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_1@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_1@f$.
     * @return The constant element.
     */
    inline friend G1Element G1Const(G1 value);
private:
    ElementType type;
    union {
        int index;
        G1 el;
    };
};

/**
 * @brief A @f$\mathbb{G}_2@f$ element inside an equation.
 */
class G2Element {
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    inline G2Element(const G2Element &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    inline G2Element &operator=(const G2Element &other);
    /**
     * @brief Destructs the element.
     */
    inline ~G2Element();
private:
    inline G2Element(ElementType type);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_2@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_2@f$.
     * @return The variable element.
     */
    inline friend G2Element G2Var(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_2@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_2@f$.
     * @return The constant element.
     */
    inline friend G2Element G2Const(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_2@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_2@f$.
     * @return The constant element.
     */
    inline friend G2Element G2Const(G2 value);
private:
    ElementType type;
    union {
        int index;
        G2 el;
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

inline G1Element::G1Element(const G1Element &other) : type(other.type) {
    if (type == ELEMENT_CONST_VALUE)
        new (&el) G1(other.el);
    else
        index = other.index;
}

inline G1Element &G1Element::operator=(const G1Element &other) {
    if (type == ELEMENT_CONST_VALUE) {
        if (other.type == ELEMENT_CONST_VALUE) {
            el = other.el;
        } else {
            el.~G1();
            index = other.index;
        }
    } else {
        if (other.type == ELEMENT_CONST_VALUE) {
            new (&el) G1(other.el);
        } else {
            index = other.index;
        }
    }
    type = other.type;
    return *this;
}

inline G1Element::~G1Element() {
    if (type == ELEMENT_CONST_VALUE)
        el.~G1();
}

inline G1Element::G1Element(ElementType type) : type(type) {}

inline G1Element G1Var(int index) {
    G1Element el(ELEMENT_VARIABLE);
    el.index = index;
    return el;
}

inline G1Element G1Const(int index) {
    G1Element el(ELEMENT_CONST_INDEX);
    el.index = index;
    return el;
}

inline G1Element G1Const(G1 value) {
    G1Element el(ELEMENT_CONST_VALUE);
    new (&el.el) G1(value);
    return el;
}

inline G2Element::G2Element(const G2Element &other) : type(other.type) {
    if (type == ELEMENT_CONST_VALUE)
        new (&el) G2(other.el);
    else
        index = other.index;
}

inline G2Element &G2Element::operator=(const G2Element &other) {
    if (type == ELEMENT_CONST_VALUE) {
        if (other.type == ELEMENT_CONST_VALUE) {
            el = other.el;
        } else {
            el.~G2();
            index = other.index;
        }
    } else {
        if (other.type == ELEMENT_CONST_VALUE) {
            new (&el) G2(other.el);
        } else {
            index = other.index;
        }
    }
    type = other.type;
    return *this;
}

inline G2Element::~G2Element() {
    if (type == ELEMENT_CONST_VALUE)
        el.~G2();
}

inline G2Element::G2Element(ElementType type) : type(type) {}

inline G2Element G2Var(int index) {
    G2Element el(ELEMENT_VARIABLE);
    el.index = index;
    return el;
}

inline G2Element G2Const(int index) {
    G2Element el(ELEMENT_CONST_INDEX);
    el.index = index;
    return el;
}

inline G2Element G2Const(G2 value) {
    G2Element el(ELEMENT_CONST_VALUE);
    new (&el.el) G2(value);
    return el;
}

} /* End of gsnizk */

#endif // GSNIZK_H
