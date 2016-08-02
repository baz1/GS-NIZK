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
    friend class FpVar;
private:
    ElementType type;
    union {
        int index;
        Fp el;
    } data;
};

/**
 * @brief Creates a variable in @f$\mathbb{F}_p@f$ with index @a index.
 * @param index Index of the variable in @f$\mathbb{F}_p@f$.
 * @return The variable element.
 */
inline FpElement FpVar(int index);

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

} /* End of gsnizk */

#endif // GSNIZK_H
