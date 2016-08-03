#ifndef GSNIZK_H
#define GSNIZK_H

#include "maps.h"

#include <memory>

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
    ELEMENT_CONST_VALUE,
    ELEMENT_PAIR,
    ELEMENT_SCALAR,
    ELEMENT_PAIRING
};
class FpElement;
class G1Element;
class G2Element;
class GTElement;
typedef std::shared_ptr< std::pair<FpElement,FpElement> > PairFp;
typedef std::shared_ptr< std::pair<G1Element,G1Element> > PairG1;
typedef std::shared_ptr< std::pair<FpElement,G1Element> > ScalarG1;
typedef std::shared_ptr< std::pair<G2Element,G2Element> > PairG2;
typedef std::shared_ptr< std::pair<FpElement,G2Element> > ScalarG2;
typedef std::shared_ptr< std::pair<GTElement,GTElement> > PairGT;
typedef std::shared_ptr< std::pair<FpElement,GTElement> > ScalarGT;
typedef std::shared_ptr< std::pair<G1Element,G2Element> > PairingGT;
/**
 * @endcond
 */

/**
 * @brief A @f$\mathbb{F}_p@f$ element inside an equation.
 */
class FpElement {
public:
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    FpElement(const FpElement &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    FpElement &operator=(const FpElement &other);
    /**
     * @brief Destructs the element.
     */
    ~FpElement();
    /**
     * @brief Multiplies two elements in @f$\mathbb{F}_p@f$.
     * @param other Second element in @f$\mathbb{F}_p@f$.
     * @return Reference to the modified element.
     */
    inline FpElement &operator*=(const FpElement &other);
    /**
     * @brief Multiplies two elements in @f$\mathbb{F}_p@f$.
     * @param other Second element in @f$\mathbb{F}_p@f$.
     * @return Product element, in @f$\mathbb{F}_p@f$.
     */
    inline FpElement operator*(const FpElement &other) const;
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
        PairFp pair;
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
        PairG1 pair;
        ScalarG1 scalar;
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
        PairG2 pair;
        ScalarG2 scalar;
    };
};

/**
 * @brief A @f$\mathbb{G}_T@f$ element inside an equation.
 */
class GTElement {
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    inline GTElement(const GTElement &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    inline GTElement &operator=(const GTElement &other);
    /**
     * @brief Destructs the element.
     */
    inline ~GTElement();
private:
    inline GTElement(ElementType type);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_T@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_T@f$.
     * @return The variable element.
     */
    inline friend GTElement GTVar(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_T@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_T@f$.
     * @return The constant element.
     */
    inline friend GTElement GTConst(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_T@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_T@f$.
     * @return The constant element.
     */
    inline friend GTElement GTConst(GT value);
private:
    ElementType type;
    union {
        int index;
        GT el;
        PairGT pair;
        ScalarGT scalar;
        PairingGT pring;
    };
};

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

inline FpElement FpElement::operator*(const FpElement &other) const {
    FpElement el(ELEMENT_PAIR);
    new (&el.pair) PairFp(
            new (std::pair<FpElement,FpElement>)(*this, other));
    return el;
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

inline GTElement::GTElement(ElementType type) : type(type) {}

inline GTElement GTVar(int index) {
    GTElement el(ELEMENT_VARIABLE);
    el.index = index;
    return el;
}

inline GTElement GTConst(int index) {
    GTElement el(ELEMENT_CONST_INDEX);
    el.index = index;
    return el;
}

inline GTElement GTConst(GT value) {
    GTElement el(ELEMENT_CONST_VALUE);
    new (&el.el) GT(value);
    return el;
}

} /* End of namespace nizk */

#endif /* End of GSNIZK_H */
