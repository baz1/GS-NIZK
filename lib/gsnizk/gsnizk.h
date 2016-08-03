#ifndef GSNIZK_H
#define GSNIZK_H

#include "maps.h"

#include <memory>
#include <utility>

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
struct FpData;
struct G1Data;
struct G2Data;
struct GTData;
typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<FpData> > PairFp;
typedef std::pair< std::shared_ptr<G1Data>, std::shared_ptr<G1Data> > PairG1;
typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<G1Data> > ScalarG1;
typedef std::pair< std::shared_ptr<G2Data>, std::shared_ptr<G2Data> > PairG2;
typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<G2Data> > ScalarG2;
typedef std::pair< std::shared_ptr<GTData>, std::shared_ptr<GTData> > PairGT;
typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<GTData> > ScalarGT;
typedef std::pair< std::shared_ptr<G1Data>, std::shared_ptr<G2Data> > PairingGT;
struct FpData {
    ElementType type;
    union {
        int index;
        Fp el;
        PairFp pair;
    };
    inline FpData(ElementType type) : type(type) {}
    ~FpData();
};
struct G1Data {
    ElementType type;
    union {
        int index;
        G1 el;
        PairG1 pair;
        ScalarG1 scalar;
    };
    inline G1Data(ElementType type) : type(type) {}
    ~G1Data();
};
struct G2Data {
    ElementType type;
    union {
        int index;
        G2 el;
        PairG2 pair;
        ScalarG2 scalar;
    };
    inline G2Data(ElementType type) : type(type) {}
    ~G2Data();
};
struct GTData {
    ElementType type;
    union {
        int index;
        GT el;
        PairGT pair;
        ScalarGT scalar;
        PairingGT pring;
    };
    inline GTData(ElementType type) : type(type) {}
    ~GTData();
};
class FpElement;
class G1Element;
class G2Element;
class GTElement;
/**
 * @endcond
 */

/**
 * @brief A @f$\mathbb{F}_p@f$ element inside an equation.
 */
class FpElement {
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{F}_p@f$.
     */
    inline FpElement();
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
    FpElement operator*(const FpElement &other) const;
private:
    inline FpElement(std::shared_ptr<FpData> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{F}_p@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{F}_p@f$.
     * @return The variable element.
     */
    friend FpElement FpVar(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{F}_p@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{F}_p@f$.
     * @return The constant element.
     */
    friend FpElement FpConst(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{F}_p@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{F}_p@f$.
     * @return The constant element.
     */
    friend FpElement FpConst(Fp value);
    /* Pre-definition */
    friend G1Element operator*(const FpElement &s, const G1Element &e);
    friend G2Element operator*(const FpElement &s, const G2Element &e);
    friend GTElement operator^(const GTElement &e, const FpElement &s);
private:
    std::shared_ptr<FpData> data;
};

/**
 * @brief A @f$\mathbb{G}_1@f$ element inside an equation.
 */
class G1Element {
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{G}_1@f$.
     */
    inline G1Element();
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
    G1Element operator+(const G1Element &other) const;
    /**
     * @brief Multiplies an element of @f$\mathbb{G}_1@f$ with a scalar.
     * @param s Scalar used in the multiplication.
     * @param e Element to multiply the scalar.
     * @return Result of the multiplication.
     */
    friend G1Element operator*(const FpElement &s, const G1Element &e);
private:
    inline G1Element(std::shared_ptr<G1Data> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_1@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_1@f$.
     * @return The variable element.
     */
    friend G1Element G1Var(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_1@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_1@f$.
     * @return The constant element.
     */
    friend G1Element G1Const(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_1@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_1@f$.
     * @return The constant element.
     */
    friend G1Element G1Const(G1 value);
    /* Pre-definition */
    friend GTElement e(const G1Element &a, const G2Element &b);
private:
    std::shared_ptr<G1Data> data;
};

/**
 * @brief A @f$\mathbb{G}_2@f$ element inside an equation.
 */
class G2Element {
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{G}_2@f$.
     */
    inline G2Element();
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
    G2Element operator+(const G2Element &other) const;
    /**
     * @brief Multiplies an element of @f$\mathbb{G}_2@f$ with a scalar.
     * @param s Scalar used in the multiplication.
     * @param e Element to multiply the scalar.
     * @return Result of the multiplication.
     */
    friend G2Element operator*(const FpElement &s, const G2Element &e);
private:
    inline G2Element(std::shared_ptr<G2Data> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_2@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_2@f$.
     * @return The variable element.
     */
    friend G2Element G2Var(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_2@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_2@f$.
     * @return The constant element.
     */
    friend G2Element G2Const(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_2@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_2@f$.
     * @return The constant element.
     */
    friend G2Element G2Const(G2 value);
    /* Pre-definition */
    friend GTElement e(const G1Element &a, const G2Element &b);
private:
    std::shared_ptr<G2Data> data;
};

/**
 * @brief A @f$\mathbb{G}_T@f$ element inside an equation.
 */
class GTElement {
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{G}_T@f$.
     */
    inline GTElement();
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
    GTElement operator*(const GTElement &other) const;
    /**
     * @brief Puts an element of @f$\mathbb{G}_T@f$ to a certain power.
     * @param s Exponent used in the power.
     * @param e Element to raise to a certain power.
     * @return Result of the power.
     */
    friend GTElement operator^(const GTElement &e, const FpElement &s);
    /**
     * @brief Computes a pairing (bilinear map).
     * @param a Element of @f$\mathbb{G}_1@f$.
     * @param b Element of @f$\mathbb{G}_2@f$.
     * @return Result of the pairing @f$e(a,b)@f$ in @f$\mathbb{G}_T@f$.
     */
    friend GTElement e(const G1Element &a, const G2Element &b);
private:
    inline GTElement(std::shared_ptr<GTData> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_T@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_T@f$.
     * @return The variable element.
     */
    friend GTElement GTVar(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_T@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_T@f$.
     * @return The constant element.
     */
    friend GTElement GTConst(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_T@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_T@f$.
     * @return The constant element.
     */
    friend GTElement GTConst(GT value);
private:
    std::shared_ptr<GTData> data;
};

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

inline FpElement::FpElement() {}

inline FpElement::FpElement(std::shared_ptr<FpData> d) : data(d) {}

inline G1Element::G1Element() {}

inline G1Element::G1Element(std::shared_ptr<G1Data> d) : data(d) {}

inline G2Element::G2Element() {}

inline G2Element::G2Element(std::shared_ptr<G2Data> d) : data(d) {}

inline GTElement::GTElement() {}

inline GTElement::GTElement(std::shared_ptr<GTData> d) : data(d) {}

} /* End of namespace nizk */

#endif /* End of GSNIZK_H */
