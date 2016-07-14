/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef MAPS_H
#define MAPS_H

#include "pairings.h"

/**
 * @file maps.h
 * @brief Commitment space groups and maps.
 *
 * This header file contains typedefs for the main pairing groups,
 * as well as the commitment groups @f$\mathbb{B}_1=\mathbb{G}_1^2@f$,
 * @f$\mathbb{B}_2=\mathbb{G}_2^2@f$ and @f$\mathbb{B}_T=\mathbb{G}_T^4@f$.
 *
 * Maps are provided to and from the main groups given a reference to
 * the Common Reference String (CRS) that is used.
 * Functions for creating this CRS are also included,
 * as well as openings for simulated proofs.
 */

namespace gsnizk {

/**
 * @brief The @f$\mathbb{F}_p=\mathbb{Z}/p\mathbb{Z}@f$ class.
 *
 * This is a `typedef` extension of @ref pairings 's definition.
 * @sa pairings::Fp
 */
typedef pairings::Fp Fp;
/**
 * @brief The @f$\mathbb{G}_1@f$ class.
 *
 * This is a `typedef` extension of @ref pairings 's definition.
 * @sa pairings::G1
 */
typedef pairings::G1 G1;
/**
 * @brief The @f$\mathbb{G}_2@f$ class.
 *
 * This is a `typedef` extension of @ref pairings 's definition.
 * @sa pairings::G2
 */
typedef pairings::G2 G2;
/**
 * @brief The @f$\mathbb{G}_T@f$ class.
 *
 * This is a `typedef` extension of @ref pairings 's definition.
 * @sa pairings::GT
 */
typedef pairings::GT GT;

class CRS;

class B1 {
public:
    /**
     * @brief Constructs a null element.
     */
    inline B1();
    /**
     * @brief Constructs an element from its components
     * @param _1 First component.
     * @param _2 Second component.
     */
    inline B1(const G1 &_1, const G1 &_2);
    /**
     * @brief Writes this element to an output stream.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,B1&)
     */
    inline friend std::ostream &operator<<(std::ostream &stream, const B1 &el);
    /**
     * @brief Reads this element from an input stream.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const B1&)
     */
    inline friend std::istream &operator>>(std::istream &stream, B1 &el);
public:
    G1 _1, _2;
};

class B2 {
public:
    /**
     * @brief Constructs a null element.
     */
    inline B2();
    /**
     * @brief Constructs an element from its components
     * @param _1 First component.
     * @param _2 Second component.
     */
    inline B2(const G2 &_1, const G2 &_2);
    /**
     * @brief Writes this element to an output stream.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,B2&)
     */
    inline friend std::ostream &operator<<(std::ostream &stream, const B2 &el);
    /**
     * @brief Reads this element from an input stream.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const B2&)
     */
    inline friend std::istream &operator>>(std::istream &stream, B2 &el);
public:
    G2 _1, _2;
};

class BT {
public:
    /**
     * @brief Constructs a new unit element (value 1).
     */
    inline BT();
    /**
     * @brief Constructs an element from its components
     * @param _1 First component.
     * @param _2 Second component.
     */
    inline BT(const GT &_11, const GT &_12, const GT &_21, const GT &_22);
    /**
     * @brief Writes this element to an output stream.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,BT&)
     */
    inline friend std::ostream &operator<<(std::ostream &stream, const BT &el);
    /**
     * @brief Reads this element from an input stream.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const BT&)
     */
    inline friend std::istream &operator>>(std::istream &stream, BT &el);
public:
    GT _11, _12;
    GT _21, _22;
};

/* Below are the three different possible types of CRS */
#define CRS_TYPE_PUBLIC     0 /* No additional information */
#define CRS_TYPE_EXTRACT    1 /* Binding key with extraction parameters */
#define CRS_TYPE_ZK         2 /* Hiding key with simulation parameters */

/**
 * @brief The Common Reference String.
 */
class CRS {
    friend class B1;
    friend class B2;
    friend class BT;
public:
    /**
     * @brief CRS contructor.
     *
     * This constructor generates a whole new Common Reference String
     * that may be based either on a binding key (extraction parameters
     * will be included) or a hiding key (simulation parameters included).
     *
     * @param binding Whether this CRS is to be a binding (`true`)
     *   or hiding (`false`) CRS.
     */
    CRS(bool binding = true);
    /**
     * @brief Makes the CRS public.
     *
     * Here, making a CRS public means clearing any extra information
     * that may be linked to it, be it extraction parameters or simulation
     * parameters depending on the type of key that is involved.
     */
    void makePublic();
    /**
     * @brief Writes this CRS to an output stream.
     * @param stream Output stream.
     * @param crs CRS to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,CRS&)
     */
    friend std::ostream &operator<<(std::ostream &stream, const CRS &crs);
    /**
     * @brief Reads the CRS from an input stream.
     * @param stream Input stream.
     * @param crs CRS to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const CRS&)
     */
    friend std::istream &operator>>(std::istream &stream, CRS &crs);
private:
    void computeElements();
private:
    B1 u1, v1, w1;
    B2 u2, v2, w2;
    int type;
    Fp i1, j1, i2, j2;
};

/* Inline definitions: */

inline B1::B1() {}

inline B1::B1(const G1 &_1, const G1 &_2) : _1(_1), _2(_2) {}

inline std::ostream &operator<<(std::ostream &stream, const B1 &el) {
    stream << el._1 << el._2;
    return stream;
}

inline std::istream &operator>>(std::istream &stream, B1 &el) {
    stream >> el._1 >> el._2;
    return stream;
}

inline B2::B2() {}

inline B2::B2(const G2 &_1, const G2 &_2) : _1(_1), _2(_2) {}

inline std::ostream &operator<<(std::ostream &stream, const B2 &el) {
    stream << el._1 << el._2;
    return stream;
}

inline std::istream &operator>>(std::istream &stream, B2 &el) {
    stream >> el._1 >> el._2;
    return stream;
}

inline BT::BT() {}

inline BT::BT(const GT &_11, const GT &_12, const GT &_21, const GT &_22)
    : _11(_11), _12(_12), _21(_21), _22(_22) {}

inline std::ostream &operator<<(std::ostream &stream, const BT &el) {
    stream << el._11 << el._12;
    stream << el._21 << el._22;
    return stream;
}

inline std::istream &operator>>(std::istream &stream, BT &el) {
    stream >> el._21 >> el._22;
    stream >> el._21 >> el._22;
    return stream;
}

} /* End of gsnizk */

#endif /* End of MAPS_H */
