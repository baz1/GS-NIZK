/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef MAPS_H
#define MAPS_H

#include <iostream>
#include <utility>
#include <vector>

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

/**
 * @brief The @f$\mathbb{B}_1@f$ class.
 *
 * Group of order @f$p@f$.
 */
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
     * @brief Maps from @f$\mathbb{G}_1@f$.
     * @param el Element from @f$\mathbb{G}_1@f$ to map to
     *   @f$\mathbb{B}_1@f$.
     */
    inline B1(const G1 &el);
    /**
     * @brief Maps from @f$\mathbb{F}_p@f$.
     * @param el Element from @f$\mathbb{F}_p@f$ to map to
     *   @f$\mathbb{B}_1@f$.
     * @param crs Reference to the CRS to use for mapping.
     */
    inline B1(const Fp &el, const CRS &crs);
    /**
     * @brief Unary minus operator.
     * @return Inverse of the element.
     */
    inline B1 operator-() const;
    /**
     * @brief Addition operator.
     * @param other Value to add to the current element.
     * @return The sum of the two elements.
     */
    inline B1 operator+(const B1 &other) const;
    /**
     * @brief Subtraction operator.
     * @param other Value to subtract to the current element.
     * @return The difference of the two elements.
     */
    inline B1 operator-(const B1 &other) const;
    /**
     * @brief Inplace addition operator.
     * @param other Value to append to the current element.
     * @return Reference to the modified element.
     */
    inline B1 &operator+=(const B1 &other);
    /**
     * @brief Inplace subtraction operator.
     * @param other Value to subtract to the current element.
     * @return Reference to the modified element.
     */
    inline B1 &operator-=(const B1 &other);
    /**
     * @brief Inplace multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return Reference to the modified element.
     */
    inline B1 &operator*=(const Fp &other);
    /**
     * @brief Multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return The product of the two elements.
     */
    inline B1 operator*(const Fp &other) const;
    /**
     * @brief Multiplication operator.
     * @param m Scalar value.
     * @param g Element value.
     * @return Product @f$m\cdot g@f$
     */
    inline friend B1 operator*(const Fp &m, const B1 &g);
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    inline bool operator==(const B1 &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `false` if the two values are equal, `true` otherwise.
     */
    inline bool operator!=(const B1 &other) const;
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
    /**
     * @brief Extracts the value in @f$\mathbb{G}_1@f$ that this
     *   commitment refers to.
     * @note If this value refers to a commitment in @f$\mathbb{F}_p@f$,
     *   the value that is returned will be equal to `crs.getG()`
     *   multiplied with the commited value.
     * @param crs CRS with information on the binding key.
     * @return Value in @f$\mathbb{G}_1@f$ that this commitment refers to.
     */
    G1 extract(const CRS &crs) const;
public:
    G1 _1, _2;
};

/**
 * @brief The @f$\mathbb{B}_2@f$ class.
 *
 * Group of order @f$p@f$.
 */
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
     * @brief Maps from @f$\mathbb{G}_2@f$.
     * @param el Element from @f$\mathbb{G}_2@f$ to map to
     *   @f$\mathbb{B}_2@f$.
     */
    inline B2(const G2 &el);
    /**
     * @brief Maps from @f$\mathbb{F}_p@f$.
     * @param el Element from @f$\mathbb{F}_p@f$ to map to
     *   @f$\mathbb{B}_2@f$.
     * @param crs Reference to the CRS to use for mapping.
     */
    inline B2(const Fp &el, const CRS &crs);
    /**
     * @brief Unary minus operator.
     * @return Inverse of the element.
     */
    inline B2 operator-() const;
    /**
     * @brief Addition operator.
     * @param other Value to add to the current element.
     * @return The sum of the two elements.
     */
    inline B2 operator+(const B2 &other) const;
    /**
     * @brief Subtraction operator.
     * @param other Value to subtract to the current element.
     * @return The difference of the two elements.
     */
    inline B2 operator-(const B2 &other) const;
    /**
     * @brief Inplace addition operator.
     * @param other Value to append to the current element.
     * @return Reference to the modified element.
     */
    inline B2 &operator+=(const B2 &other);
    /**
     * @brief Inplace subtraction operator.
     * @param other Value to subtract to the current element.
     * @return Reference to the modified element.
     */
    inline B2 &operator-=(const B2 &other);
    /**
     * @brief Inplace multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return Reference to the modified element.
     */
    inline B2 &operator*=(const Fp &other);
    /**
     * @brief Multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return The product of the two elements.
     */
    inline B2 operator*(const Fp &other) const;
    /**
     * @brief Multiplication operator.
     * @param m Scalar value.
     * @param g Element value.
     * @return Product @f$m\cdot g@f$
     */
    inline friend B2 operator*(const Fp &m, const B2 &g);
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    inline bool operator==(const B2 &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `false` if the two values are equal, `true` otherwise.
     */
    inline bool operator!=(const B2 &other) const;
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
    /**
     * @brief Extracts the value in @f$\mathbb{G}_2@f$ that this
     *   commitment refers to.
     * @note If this value refers to a commitment in @f$\mathbb{F}_p@f$,
     *   the value that is returned will be equal to `crs.getH()`
     *   multiplied with the commited value.
     * @param crs CRS with information on the binding key.
     * @return Value in @f$\mathbb{G}_2@f$ that this commitment refers to.
     */
    G2 extract(const CRS &crs) const;
public:
    G2 _1, _2;
};

/**
 * @brief The @f$\mathbb{B}_T@f$ class.
 *
 * Group of order @f$p@f$, provided with a bilinear map
 * going from @f$\mathbb{B}_1\times\mathbb{B}_2@f$
 * to @f$\mathbb{B}_T@f$.
 */
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
     * @brief Maps from @f$\mathbb{F}_p@f$.
     * @param el Element from @f$\mathbb{F}_p@f$ to map to
     *   @f$\mathbb{B}_T@f$.
     * @param crs Reference to the CRS to use for mapping.
     */
    inline BT(const Fp &el, const CRS &crs);
    /**
     * @brief Maps from @f$\mathbb{G}_1@f$.
     * @param el Element from @f$\mathbb{G}_1@f$ to map to
     *   @f$\mathbb{B}_T@f$.
     * @param crs Reference to the CRS to use for mapping.
     */
    inline BT(const G1 &el, const CRS &crs);
    /**
     * @brief Maps from @f$\mathbb{G}_2@f$.
     * @param el Element from @f$\mathbb{G}_2@f$ to map to
     *   @f$\mathbb{B}_T@f$.
     * @param crs Reference to the CRS to use for mapping.
     */
    inline BT(const G2 &el, const CRS &crs);
    /**
     * @brief Maps from @f$\mathbb{G}_T@f$.
     * @param el Element from @f$\mathbb{G}_T@f$ to map to
     *   @f$\mathbb{B}_T@f$.
     */
    inline BT(const GT &el);
    /**
     * @brief Multiplication operator.
     * @param other Value to multiply with the current element.
     * @return The product of the two elements.
     */
    inline BT operator*(const BT &other) const;
    /**
     * @brief Division operator.
     * @param other Value with which to divide the current element.
     * @return The fraction of the two elements.
     */
    inline BT operator/(const BT &other) const;
    /**
     * @brief Inplace multiplication operator.
     * @param other Value to multiply with the current element.
     * @return Reference to the modified element.
     */
    inline BT &operator*=(const BT &other);
    /**
     * @brief Inplace division operator.
     * @param other Value with which to divide the current element.
     * @return Reference to the modified element.
     */
    inline BT &operator/=(const BT &other);
    /**
     * @brief Inplace power operator.
     * @param other Exponent value.
     * @return Reference to the modified element.
     */
    inline BT &operator^=(const Fp &other);
    /**
     * @brief Power operator.
     * @param other Exponent value.
     * @return Power @f$this^{other}@f$
     */
    inline BT operator^(const Fp &other) const;
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    inline bool operator==(const BT &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `false` if the two values are equal, `true` otherwise.
     */
    inline bool operator!=(const BT &other) const;
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
    /**
     * @brief Extracts the value that this commitment refers to,
     *   as an element of @f$\mathbb{G}_T@f$.
     * @param crs CRS with information on the binding key.
     * @return Value in @f$\mathbb{G}_T@f$ that this commitment refers to.
     */
    GT extract(const CRS &crs) const;
public:
    /**
     * @brief Computes a pairing of two elements.
     * @param a @f$\mathbb{B}_1@f$ member.
     * @param b @f$\mathbb{B}_2@f$ member.
     * @return @f$\mathbb{B}_T@f$ member that is the result of calling
     *   the bilinear map on @p a and @p b.
     */
    static BT pairing(const B1 &a, const B2 &b);
    /**
     * @brief Computes the product of multiple pairings.
     * @param lst List of couples in @f$(\mathbb{B}_1,\mathbb{B}_2)@f$.
     * @return Product of the pairings of each couple.
     */
    static BT pairing(const std::vector<std::pair<B1, B2> > &lst);
public:
    GT _11, _12;
    GT _21, _22;
};

/* Below are the three different possible types of CRS */
#define CRS_TYPE_PUBLIC     0 /* No additional information */
#define CRS_TYPE_EXTRACT    1 /* Binding key with extraction parameters */
#define CRS_TYPE_ZK         2 /* Hiding key with simulation parameters */
#define CRS_TYPE_PRIVATE    3 /* Binding key with efficiency parameters */

/**
 * @brief The Common Reference String.
 */
class CRS {
    friend class B1;
    friend class B2;
    friend class BT;
public:
    /**
     * @brief CRS dummmy constructor.
     *
     * Constructs an invalid CRS.
     *
     * @warning This constructor is only meant to be used as a way to
     * define CRS variables before assigning them
     * (`operator=` or `operator<<`). Do not use any such uninitialized
     * CRS in any way, since no protection is implemented to prevent
     * undefined behavior.
     * @sa operator<<(std::istream&,const CRS&)
     */
    inline CRS();
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
    CRS(bool binding);
    /**
     * @brief Makes the CRS public.
     *
     * Here, making a CRS public means clearing any extra information
     * that may be linked to it, be it extraction parameters or simulation
     * parameters depending on the type of key that is involved.
     */
    void makePublic();
    /**
     * @brief Generates a private CRS from this public CRS.
     * @param stream Output stream to which the proof that the returned
     *   private CRS is well-formed will be written.
     * @return Private CRS.
     */
    CRS genPrivate(std::ostream &stream) const;
    /**
     * @brief checkPrivate Checks the proof that a private CRS has been
     *   correctly generated.
     * @param stream Input stream from which the proof will be read.
     * @param priv The private CRS to check.
     * @return `true` if the proof is correct, `false` otherwise.
     */
    bool checkPrivate(std::istream &stream, CRS priv) const;
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
    /* Notations u, v and w come from the paper by Alex Escala
     * and Jens Groth.
     * type==CRS_TYPE_PUBLIC:
     *   i1,j1,i2,j2 are not used.
     * type==CRS_TYPE_EXTRACT||type==CRS_TYPE_ZK:
     *   i1 stands for rho and j1 for epsilon in Escala's paper,
     *   with similar bindings for i2 and j2 to sigma and psi.
     * type==CRS_TYPE_PRIVATE:
     *   Only i1 and i2 are available; they stand for rho_P
     *   and sigma_P in Escala's paper.
     */
    B1 u1, v1, w1;
    B2 u2, v2, w2;
    int type;
    Fp i1, j1, i2, j2;
};

/* Inline definitions: */

inline B1::B1() {}

inline B1::B1(const G1 &_1, const G1 &_2) : _1(_1), _2(_2) {}

inline B1::B1(const G1 &el) : _1(), _2(el) {}

inline B1::B1(const Fp &el, const CRS &crs)
    : _1(el * crs.u1._1), _2(el * crs.u1._2) {}

inline B1 B1::operator-() const {
    return B1(-_1, -_2);
}

inline B1 B1::operator+(const B1 &other) const {
    return B1(_1 + other._1, _2 + other._2);
}

inline B1 B1::operator-(const B1 &other) const {
    return B1(_1 - other._1, _2 - other._2);
}

inline B1 &B1::operator+=(const B1 &other) {
    _1 += other._1;
    _2 += other._2;
    return *this;
}

inline B1 &B1::operator-=(const B1 &other) {
    _1 -= other._1;
    _2 -= other._2;
    return *this;
}

inline B1 &B1::operator*=(const Fp &other) {
    _1 *= other;
    _2 *= other;
    return *this;
}

inline B1 B1::operator*(const Fp &other) const {
    return B1(other * _1, other * _2);
}

inline B1 operator*(const Fp &m, const B1 &g) {
    return B1(m * g._1, m * g._2);
}

inline bool B1::operator==(const B1 &other) const {
    return (_1 == other._1) && (_2 == other._2);
}

inline bool B1::operator!=(const B1 &other) const {
    return !((_1 == other._1) && (_2 == other._2));
}

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

inline B2::B2(const G2 &el) : _1(), _2(el) {}

inline B2::B2(const Fp &el, const CRS &crs)
    : _1(el * crs.u2._1), _2(el * crs.u2._2) {}

inline B2 B2::operator-() const {
    return B2(-_1, -_2);
}

inline B2 B2::operator+(const B2 &other) const {
    return B2(_1 + other._1, _2 + other._2);
}

inline B2 B2::operator-(const B2 &other) const {
    return B2(_1 - other._1, _2 - other._2);
}

inline B2 &B2::operator+=(const B2 &other) {
    _1 += other._1;
    _2 += other._2;
    return *this;
}

inline B2 &B2::operator-=(const B2 &other) {
    _1 -= other._1;
    _2 -= other._2;
    return *this;
}

inline B2 &B2::operator*=(const Fp &other) {
    _1 *= other;
    _2 *= other;
    return *this;
}

inline B2 B2::operator*(const Fp &other) const {
    return B2(other * _1, other * _2);
}

inline B2 operator*(const Fp &m, const B2 &g) {
    return B2(m * g._1, m * g._2);
}

inline bool B2::operator==(const B2 &other) const {
    return (_1 == other._1) && (_2 == other._2);
}

inline bool B2::operator!=(const B2 &other) const {
    return !((_1 == other._1) && (_2 == other._2));
}

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

inline BT::BT(const Fp &el, const CRS &crs) {
    *this = BT::pairing(el * crs.u1, crs.u2);
}

inline BT::BT(const G1 &el, const CRS &crs) : _11(), _12(),
    _21(GT::pairing(el, crs.u2._1)), _22(GT::pairing(el, crs.u2._2)) {}

inline BT::BT(const G2 &el, const CRS &crs) : _11(),
    _12(GT::pairing(crs.u1._1, el)), _21(), _22(GT::pairing(crs.u1._2, el)) {}

inline BT::BT(const GT &el) : _11(), _12(), _21(), _22(el) {}

inline BT BT::operator*(const BT &other) const {
    return BT(_11 * other._11, _12 * other._12,
              _21 * other._21, _22 * other._22);
}

inline BT BT::operator/(const BT &other) const {
    return BT(_11 / other._11, _12 / other._12,
              _21 / other._21, _22 / other._22);
}

inline BT &BT::operator*=(const BT &other) {
    _11 *= other._11;
    _12 *= other._12;
    _21 *= other._21;
    _22 *= other._22;
    return *this;
}

inline BT &BT::operator/=(const BT &other) {
    _11 /= other._11;
    _12 /= other._12;
    _21 /= other._21;
    _22 /= other._22;
    return *this;
}

inline bool BT::operator==(const BT &other) const {
    return (_11 == other._11) && (_12 == other._12) &&
            (_21 == other._21) && (_22 == other._22);
}

inline bool BT::operator!=(const BT &other) const {
    return !((_11 == other._11) && (_12 == other._12) &&
             (_21 == other._21) && (_22 == other._22));
}

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

inline CRS::CRS() {}

} /* End of namespace nizk */

#endif /* End of MAPS_H */
