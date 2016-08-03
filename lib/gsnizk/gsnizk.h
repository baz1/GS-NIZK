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
    FpElement &operator*=(const FpElement &other);
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
public:
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    G1Element(const G1Element &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    G1Element &operator=(const G1Element &other);
    /**
     * @brief Destructs the element.
     */
    ~G1Element();
    /**
     * @brief Adds two elements in @f$\mathbb{G}_1@f$.
     * @param other Second element in @f$\mathbb{G}_1@f$.
     * @return Reference to the modified element.
     */
    G1Element &operator+=(const G1Element &other);
    /**
     * @brief Adds two elements in @f$\mathbb{G}_1@f$.
     * @param other Second element in @f$\mathbb{G}_1@f$.
     * @return Product element, in @f$\mathbb{G}_1@f$.
     */
    inline G1Element operator+(const G1Element &other) const;
    /**
     * @brief Multiplies an element of @f$\mathbb{G}_1@f$ with a scalar.
     * @param s Scalar used in the multiplication.
     * @param e Element to multiply the scalar.
     * @return Result of the multiplication.
     */
    inline friend G1Element operator*(const FpElement &s, const G1Element &e);
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
public:
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    G2Element(const G2Element &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    G2Element &operator=(const G2Element &other);
    /**
     * @brief Destructs the element.
     */
    ~G2Element();
    /**
     * @brief Adds two elements in @f$\mathbb{G}_2@f$.
     * @param other Second element in @f$\mathbb{G}_2@f$.
     * @return Reference to the modified element.
     */
    G2Element &operator+=(const G2Element &other);
    /**
     * @brief Adds two elements in @f$\mathbb{G}_2@f$.
     * @param other Second element in @f$\mathbb{G}_2@f$.
     * @return Product element, in @f$\mathbb{G}_2@f$.
     */
    inline G2Element operator+(const G2Element &other) const;
    /**
     * @brief Multiplies an element of @f$\mathbb{G}_2@f$ with a scalar.
     * @param s Scalar used in the multiplication.
     * @param e Element to multiply the scalar.
     * @return Result of the multiplication.
     */
    inline friend G2Element operator*(const FpElement &s, const G2Element &e);
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
public:
    /**
     * @brief Contructs a copy of an element.
     * @param other The element to copy.
     */
    GTElement(const GTElement &other);
    /**
     * @brief Sets this element to be equal to @a other.
     * @param The new value for this element.
     * @return Reference to the current element.
     */
    GTElement &operator=(const GTElement &other);
    /**
     * @brief Destructs the element.
     */
    ~GTElement();
    /**
     * @brief Multiplies two elements in @f$\mathbb{G}_T@f$.
     * @param other Second element in @f$\mathbb{G}_T@f$.
     * @return Reference to the modified element.
     */
    GTElement &operator*=(const GTElement &other);
    /**
     * @brief Multiplies two elements in @f$\mathbb{G}_T@f$.
     * @param other Second element in @f$\mathbb{G}_T@f$.
     * @return Product element, in @f$\mathbb{G}_T@f$.
     */
    inline GTElement operator*(const GTElement &other) const;
    /**
     * @brief Puts an element of @f$\mathbb{G}_T@f$ to a certain power.
     * @param s Exponent used in the power.
     * @param e Element to raise to a certain power.
     * @return Result of the power.
     */
    inline friend GTElement operator^(const GTElement &e, const FpElement &s);
    /**
     * @brief Computes a pairing (bilinear map).
     * @param a Element of @f$\mathbb{G}_1@f$.
     * @param b Element of @f$\mathbb{G}_2@f$.
     * @return Result of the pairing @f$e(a,b)@f$ in @f$\mathbb{G}_T@f$.
     */
    inline friend GTElement e(const G1Element &a, const G2Element &b);
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

inline G1Element G1Element::operator+(const G1Element &other) const {
    G1Element el(ELEMENT_PAIR);
    new (&el.pair) PairG1(
            new (std::pair<G1Element,G1Element>)(*this, other));
    return el;
}

inline G1Element operator*(const FpElement &s, const G1Element &e) {
    G1Element el(ELEMENT_SCALAR);
    new (&el.scalar) ScalarG1(
            new (std::pair<FpElement,G1Element>)(s, e));
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

inline G2Element G2Element::operator+(const G2Element &other) const {
    G2Element el(ELEMENT_PAIR);
    new (&el.pair) PairG2(
            new (std::pair<G2Element,G2Element>)(*this, other));
    return el;
}

inline G2Element operator*(const FpElement &s, const G2Element &e) {
    G2Element el(ELEMENT_SCALAR);
    new (&el.scalar) ScalarG2(
            new (std::pair<FpElement,G2Element>)(s, e));
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

inline GTElement GTElement::operator*(const GTElement &other) const {
    GTElement el(ELEMENT_PAIR);
    new (&el.pair) PairGT(
            new (std::pair<GTElement,GTElement>)(*this, other));
    return el;
}

inline GTElement operator^(const GTElement &e, const FpElement &s) {
    GTElement el(ELEMENT_SCALAR);
    new (&el.scalar) ScalarGT(
            new (std::pair<FpElement,GTElement>)(s, e));
    return el;
}

inline GTElement e(const G1Element &a, const G2Element &b) {
    GTElement el(ELEMENT_PAIRING);
    new (&el.pring) PairingGT(
            new (std::pair<G1Element,G2Element>)(a, b));
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
