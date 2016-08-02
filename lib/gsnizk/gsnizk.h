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
 * @brief A @f$\mathbb{F}_p@f$ element inside an equation.
 */
class FpElement {
};

/**
 * @brief A variable in @f$\mathbb{F}_p@f$.
 */
class FpVar : public FpElement {
    /**
     * @brief Creates a variable with index @a index.
     * @param index Index of the variable in @f$\mathbb{F}_p@f$.
     */
    inline FpVar(int index);
private:
    int index;
};

/**
 * @brief A constant in @f$\mathbb{F}_p@f$.
 */
class FpConst : public FpElement {
    /**
     * @brief Creates a constant with index @a index.
     * @param index Index of the constant in @f$\mathbb{F}_p@f$.
     */
    inline FpConst(int index);
    /**
     * @brief Creates a constant with value @a value.
     * @param value Value of the constant in @f$\mathbb{F}_p@f$.
     */
    inline FpConst(Fp value);
private:
    int index;
    Fp el;
};

/**
 * @brief A @f$\mathbb{G}_1@f$ element inside an equation.
 */
class G1Element {
};

/**
 * @brief A variable in @f$\mathbb{G}_1@f$.
 */
class G1Var : public G1Element {
    /**
     * @brief Creates a variable with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_1@f$.
     */
    inline G1Var(int index);
private:
    int index;
};

/**
 * @brief A constant in @f$\mathbb{G}_1@f$.
 */
class G1Const : public G1Element {
    /**
     * @brief Creates a constant with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_1@f$.
     */
    inline G1Const(int index);
    /**
     * @brief Creates a constant with value @a value.
     * @param value Value of the constant in @f$\mathbb{G}_1@f$.
     */
    inline G1Const(G1 value);
private:
    int index;
    G1 el;
};

/**
 * @brief A @f$\mathbb{G}_2@f$ element inside an equation.
 */
class G2Element {
};

/**
 * @brief A variable in @f$\mathbb{G}_2@f$.
 */
class G2Var : public G2Element {
    /**
     * @brief Creates a variable with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_2@f$.
     */
    inline G2Var(int index);
private:
    int index;
};

/**
 * @brief A constant in @f$\mathbb{G}_2@f$.
 */
class G2Const : public G2Element {
    /**
     * @brief Creates a constant with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_2@f$.
     */
    inline G2Const(int index);
    /**
     * @brief Creates a constant with value @a value.
     * @param value Value of the constant in @f$\mathbb{G}_2@f$.
     */
    inline G2Const(G2 value);
private:
    int index;
    G2 el;
};

/**
 * @brief A @f$\mathbb{G}_T@f$ element inside an equation.
 */
class GTElement {
};

/**
 * @brief A variable in @f$\mathbb{G}_T@f$.
 */
class GTVar : public GTElement {
    /**
     * @brief Creates a variable with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_T@f$.
     */
    inline GTVar(int index);
private:
    int index;
};

/**
 * @brief A constant in @f$\mathbb{G}_T@f$.
 */
class GTConst : public GTElement {
    /**
     * @brief Creates a constant with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_T@f$.
     */
    inline GTConst(int index);
    /**
     * @brief Creates a constant with value @a value.
     * @param value Value of the constant in @f$\mathbb{G}_T@f$.
     */
    inline GTConst(GT value);
private:
    int index;
    GT el;
};

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

inline FpVar::FpVar(int index) : FpElement(), index(index) {}

inline FpConst::FpConst(int index) : FpElement(), index(index), el() {}

inline FpConst::FpConst(Fp value) : FpElement(), index(-1), el(value) {}

inline G1Var::G1Var(int index) : G1Element(), index(index) {}

inline G1Const::G1Const(int index) : G1Element(), index(index), el() {}

inline G1Const::G1Const(G1 value) : G1Element(), index(-1), el(value) {}

inline G2Var::G2Var(int index) : G2Element(), index(index) {}

inline G2Const::G2Const(int index) : G2Element(), index(index), el() {}

inline G2Const::G2Const(G2 value) : G2Element(), index(-1), el(value) {}

inline GTVar::GTVar(int index) : GTElement(), index(index) {}

inline GTConst::GTConst(int index) : GTElement(), index(index), el() {}

inline GTConst::GTConst(GT value) : GTElement(), index(-1), el(value) {}

} /* End of gsnizk */

#endif // GSNIZK_H
