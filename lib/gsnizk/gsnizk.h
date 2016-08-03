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
private:
    std::shared_ptr<FpData> data;
};

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

inline FpElement::FpElement() {}

inline FpElement::FpElement(std::shared_ptr<FpData> d) : data(d) {}

} /* End of namespace nizk */

#endif /* End of GSNIZK_H */
