/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef PAIRINGS_H
#define PAIRINGS_H

#include <iostream>
#include <utility>
#include <vector>

/**
 * @file pairings.h
 * @brief Pairing-based cryptography wrapper - @ref pairings namespace.
 *
 * This file defines the @ref pairings namespace that is at the core
 * of pairing-based cryptography.
 */

/**
 * @brief Pairing-based cryptography wrapper.
 *
 * This namespace contains the main four groups that are used in
 * pairing-based cryptography over type-3 curves:
 * the scalar group @f$\mathbb{F}_p@f$,
 * @f$\mathbb{G}_1@f$, @f$\mathbb{G}_2@f$ and @f$\mathbb{G}_T@f$.
 * These group are reprensented respectively by the classes
 * @ref Fp, @ref G1, @ref G2 and @ref GT.
 * Groups @f$\mathbb{G}_1@f$ and @f$\mathbb{G}_2@f$ are written
 * additively while @f$\mathbb{G}_T@f$ is written multiplicatively.
 *
 * Before using any of the following pairing-based related functions,
 * you should first call the function initialize_pairings(int,const char*)
 * once and call the function terminate_pairings() when you are done
 * in order to release memory.
 * Note that you can call the initialization function again after
 * a call to terminate_pairings(), but you should not initialize twice
 * nor not at all if you are using the following functions.
 */
namespace pairings {

/**
 * @brief Initializes pairing-based cryptography.
 *
 * You must call this function exactly once before using this
 * namespace. You may call it again after having called
 * terminate_pairings().
 *
 * @param len Length of the data pointed to by @p data.
 * @param data With MIRACL: Pointer to some random data used to
 *   initialize the pseudo-random number generator.
 *   Data from `/dev/random` or `/dev/urandom` might be used under Linux.
 *   With PBC: Text data containing the curve parameters
 *   to build up the pairing groups and primitives.
 */
void initialize_pairings(int len, const char *data);

/**
 * @brief Releases memory used for pairing-based cryptography.
 *
 * Note that you may call this function multiple times;
 * additional calls will be ignored.
 */
void terminate_pairings();

/**
 * @cond INTERNAL_DATA_STRUCT
 */
struct SharedData {
    unsigned int c;
    void *p;
    inline SharedData(void *v);
};
/**
 * @endcond
 */

class Fp;
class G1;
class G2;
class GT;

/**
 * @brief Gets the length of a hash.
 * @return Length of a hash in bytes.
 */
int getHashLen();

/**
 * @brief Hashes some data.
 *
 * The hash memory space is to be allocated before calling
 * this function, and thus the parameter @p hash should point
 * to some accessible space containing at least getHashLen()
 * available bytes.
 *
 * The hash will always contain exactly the number of bytes
 * returned by the getHashLen() function.
 *
 * @param data Pointer to the data to hash.
 * @param len Length of the data to hash.
 * @param hash Pointer to where the hash is to be stored.
 * @sa getHashLen()
 */
void getHash(const char *data, int len, char *hash);

/**
 * @brief Checks if the underlying library handles precomputations.
 * @return `true` if precomputations are supported, `false` otherwise.
 */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
inline bool hasPrecomputations() { return false; }
#else
bool hasPrecomputations();
#endif

/**
 * @brief Checks if the iostream implementations supports threads.
 *
 * This function returns `true` if the iostream-based functions
 * do NOT support threads. Otherwise, it returns `false`.
 * @note No thread support implies a memory and efficiency gain.
 * @note In order to enable or disable thread support,
 *   you can define the macro `GSNIZK_IOSTREAM_NOTHREADS`
 *   when compiling the library.
 */
#if defined(GSNIZK_IOSTREAM_NOTHREADS) && !defined(LIB_COMPILATION)
inline bool iostream_nothreads() { return true; }
#else
bool iostream_nothreads();
#endif

/**
 * @brief The @f$\mathbb{F}_p=\mathbb{Z}/p\mathbb{Z}@f$ class.
 *
 * Objects of this class represent integers modulo @f$p@f$ where
 * @f$p@f$ is the prime order of all the groups in the namespace
 * @ref pairings.
 */
class Fp {
    friend class G1;
    friend class G2;
    friend class GT;
    friend void initialize_pairings(int len, const char *rndData);
    friend void terminate_pairings();
    friend G1 operator*(const Fp &m, const G1 &g);
    friend G2 operator*(const Fp &m, const G2 &g);
    friend GT operator^(const Fp &m, const GT &g);
public:
    /**
     * @brief Constructs a new null element.
     */
    inline Fp();
    /**
     * @brief Constructs a new element from an int.
     * @param i The element value, modulo @f$p@f$.
     */
    Fp(int i);
    /**
     * @brief Constructs a new element from an int.
     * @param i The element value, modulo @f$p@f$.
     */
    explicit Fp(unsigned long i);
    /**
     * @brief Performs a copy of an element.
     * @param other The element to be copied.
     */
    inline Fp(const Fp &other);
    /**
     * @brief Releases memory.
     */
    inline ~Fp();
    /**
     * @brief Assigns a new value to this element.
     * @param other New value for the element.
     * @return Reference to the current element.
     */
    inline Fp &operator=(const Fp &other);
    /**
     * @brief Unary minus operator.
     * @return Inverse of the element.
     */
    Fp operator-() const;
    /**
     * @brief Addition operator.
     * @param other Value to add to the current element.
     * @return The sum of the two elements.
     */
    Fp operator+(const Fp &other) const;
    /**
     * @brief Subtraction operator.
     * @param other Value to subtract to the current element.
     * @return The difference of the two elements.
     */
    Fp operator-(const Fp &other) const;
    /**
     * @brief Inplace addition operator.
     * @param other Value to append to the current element.
     * @return Reference to the modified element.
     */
    Fp &operator+=(const Fp &other);
    /**
     * @brief Inplace subtraction operator.
     * @param other Value to subtract to the current element.
     * @return Reference to the modified element.
     */
    Fp &operator-=(const Fp &other);
    /**
     * @brief Multiplication operator.
     * @param other Value to multiply with the current element.
     * @return The product of the two elements.
     */
    Fp operator*(const Fp &other) const;
    /**
     * @brief Division operator.
     * @param other Value with which to divide the current element.
     * @return The fraction of the two elements.
     */
    Fp operator/(const Fp &other) const;
    /**
     * @brief Inplace multiplication operator.
     * @param other Value to multiply with the current element.
     * @return Reference to the modified element.
     */
    Fp &operator*=(const Fp &other);
    /**
     * @brief Inplace division operator.
     * @param other Value with which to divide the current element.
     * @return Reference to the modified element.
     */
    Fp &operator/=(const Fp &other);
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    bool operator==(const Fp &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `false` if the two values are equal, `true` otherwise.
     */
    inline bool operator!=(const Fp &other) const;
    /**
     * @brief Retrieves this element's data.
     *
     * The data memory space is to be allocated before calling
     * this function, and thus the parameter @p data should point
     * to some accessible space containing at least @ref Fp::getDataLen()
     * available bytes.
     *
     * The data will always contain exactly the number of bytes
     * returned by the @ref Fp::getDataLen() function.
     *
     * It can then be converted back to the corresponding element
     * value thanks to the @ref Fp::getValue(const char*) function.
     *
     * @param data Pointer to where the data is to be stored.
     * @sa Fp::getDataLen()
     * @sa Fp::getValue(const char*)
     */
    void getData(char *data) const;
    /**
     * @brief Writes this element to an output stream.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,Fp&)
     */
    friend std::ostream &operator<<(std::ostream &stream, const Fp &el);
    /**
     * @brief Reads this element from an input stream.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const Fp&)
     */
    friend std::istream &operator>>(std::istream &stream, Fp &el);
    /**
     * @brief Checks if the element is null.
     * @return `true` if the element is null, `false` otherwise.
     */
    bool isNull() const;
    /**
     * @brief Checks if the element is the unity element.
     * @return `true` if the element is 1, `false` otherwise.
     */
    bool isUnit() const;
public:
    /**
     * @brief Gets the unity element.
     *
     * Note that this is equivalent to `Fp(1)`, but might be more efficient.
     *
     * @return The unity element.
     * @sa Fp::Fp(int)
     */
    inline static Fp getUnit();
    /**
     * @brief Get a random element.
     *
     * This function picks a random value chosen with uniform probability
     * within the whole set of possible values.
     * Note that in practise, the probability distribution might not be
     * exactly uniform, but this function ensures both that each value
     * may be reached, and that it is cryptographically secure for most
     * applications.
     *
     * @return A random element value.
     */
    static Fp getRand();
    /**
     * @brief Gets the length of an element's data.
     *
     * @note This function always returns the same number.
     *
     * @return Number of bytes needed to encode an element in Fp.
     * @sa Fp::getData(char*)
     * @sa Fp::getValue(const char*)
     */
    static int getDataLen();
    /**
     * @brief Gets an element value from its data.
     *
     * @p data should hold exactly what the function
     * @ref Fp::getData(char*) gives for the element
     * to be reconstructed.
     *
     * @param data Pointer to the data.
     * @return The element value corresponding to that data.
     * @sa Fp::getDataLen()
     * @sa Fp::getData(char*)
     */
    static Fp getValue(const char *data);
    /**
     * @brief Gets an element value from some data to hash.
     *
     * Note: This function is equivalent to calling successively
     * getHash(const char*,int,char*) and then Fp::fromHash(const char*)
     * with some user-allocated buffer.
     *
     * @param data Pointer to the data to hash.
     * @param len Length of the data to hash.
     * @return Element generated from the hash of the data.
     * @sa Fp::fromHash(const char*)
     */
    static Fp fromHash(const char *data, int len);
    /**
     * @brief Gets an element value from some hash.
     *
     * The hash value @p hash should be at least getHashLen()
     * bytes long, as would the getHash(const char*,int,char*)
     * function generate.
     *
     * @param hash A hash value.
     * @return Element generated from the hash value.
     * @sa getHashLen()
     * @sa getHash(const char*,int,char*)
     */
    static Fp fromHash(const char *hash);
private:
    inline explicit Fp(void *v);
    inline explicit Fp(SharedData *d);
    void deref();
    static SharedData *zero, *one;
private:
    SharedData *d;
};

/**
 * @brief The @f$\mathbb{G}_1@f$ class.
 *
 * Group of order @f$p@f$.
 */
class G1 {
    friend class GT;
public:
    /**
     * @brief Constructs a new null element.
     */
    inline G1();
    /**
     * @brief Performs a copy of an element.
     * @param other The element to be copied.
     */
    inline G1(const G1 &other);
    /**
     * @brief Releases memory.
     */
    inline ~G1();
    /**
     * @brief Assigns a new value to this element.
     * @param other New value for the element.
     * @return Reference to the current element.
     */
    inline G1 &operator=(const G1 &other);
    /**
     * @brief Unary minus operator.
     * @return Inverse of the element.
     */
    G1 operator-() const;
    /**
     * @brief Addition operator.
     * @param other Value to add to the current element.
     * @return The sum of the two elements.
     */
    G1 operator+(const G1 &other) const;
    /**
     * @brief Subtraction operator.
     * @param other Value to subtract to the current element.
     * @return The difference of the two elements.
     */
    G1 operator-(const G1 &other) const;
    /**
     * @brief Inplace addition operator.
     * @param other Value to append to the current element.
     * @return Reference to the modified element.
     */
    G1 &operator+=(const G1 &other);
    /**
     * @brief Inplace subtraction operator.
     * @param other Value to subtract to the current element.
     * @return Reference to the modified element.
     */
    G1 &operator-=(const G1 &other);
    /**
     * @brief Inplace multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return Reference to the modified element.
     */
    G1 &operator*=(const Fp &other);
    /**
     * @brief Multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return The product of the two elements.
     */
    G1 operator*(const Fp &other) const;
    /**
     * @brief Multiplication operator.
     * @param m Scalar value.
     * @param g Element value.
     * @return Product @f$m\cdot g@f$
     */
    friend G1 operator*(const Fp &m, const G1 &g);
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    bool operator==(const G1 &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    inline bool operator!=(const G1 &other) const;
    /**
     * @brief Retrieves this element's data.
     *
     * The data memory space is to be allocated before calling
     * this function, and thus the parameter @p data should point
     * to some accessible space containing at least @ref G1::getDataLen(bool)
     * available bytes.
     *
     * The data will always contain exactly the number of bytes
     * returned by the @ref G1::getDataLen(bool) function called with
     * the same @p compressed value.
     *
     * It can then be converted back to the corresponding element
     * value thanks to the @ref G1::getValue(const char*,bool) function.
     *
     * @note Compressed data has the advantage of being almost
     * twice smaller than raw data, but it might take longer to
     * reconstruct the element in the @ref G1::getValue(const char*,bool)
     * function.
     *
     * @param data Pointer to where the data is to be stored.
     * @param compressed Indicates whether the element data
     *   is to be compressed or not.
     * @sa G1::getDataLen(bool)
     * @sa G1::getValue(const char*,bool)
     */
    void getData(char *data, bool compressed = false) const;
    /**
     * @brief Writes this element to an output stream.
     * @note The element is written in its compressed form.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,G1&)
     */
    friend std::ostream &operator<<(std::ostream &stream, const G1 &el);
    /**
     * @brief Reads this element from an input stream.
     * @note The element is to be read from its compressed form.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const G1&)
     */
    friend std::istream &operator>>(std::istream &stream, G1 &el);
    /**
     * @brief Checks if the element is null.
     * @return `true` if the element is null, `false` otherwise.
     */
    inline bool isNull() const;
    /**
     * @brief Sets this element to the null value.
     */
    inline void clear();
    /**
     * @brief Does some precomputations in preparation for
     *   scalar multiplication.
     *
     * @note Does nothing if hasPrecomputations() returns false.
     *
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline void precomputeForMult() {}
#else
    void precomputeForMult();
#endif
    /**
     * @brief Saves the scalar multiplication precomputations in a buffer.
     *
     * @warning This function allocates data thanks to the C++ idiom
     *   `new char[size]`. It is thus expected of the user to delete
     *   the pointer value that is set after calling this function
     *   using the operation `delete[] data`.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Reference to the data pointer to be set.
     * @return The size of the data in bytes.
     * @sa G1::precomputeForMult()
     * @sa G1::loadMultPrecomputations(char*)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline int saveMultPrecomputations(char *&data) {
        /* Note: A good application should not call this function
         * if hasPrecomputations() returns false, but we still prefer
         * returning dummy values than throwing an exception for
         * ease-of-use reasons. */
        data = new char[1];
        *data = 0;
        return 1;
    }
#else
    int saveMultPrecomputations(char *&data);
#endif
    /**
     * @brief Loads the scalar multiplication precomputations from a buffer.
     *
     * @warning This function frees data thanks to the C++ idiom
     *   `delete[] data`. It is thus expected of the user provide a pointer
     *   value that was allocated thanks to the operation `new data[size]`.
     *
     * @warning The element on which this function is called should already
     *   have the same value as the one that was used to generate the
     *   precomputation data that is given.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Pointer to the data to be loaded.
     * @sa G1::saveMultPrecomputations(char*&)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    void loadMultPrecomputations(char *data) {
        delete[] data;
    }
#else
    void loadMultPrecomputations(char *data);
#endif
public:
    /**
     * @brief Get a random element.
     *
     * This function picks a random value chosen with uniform probability
     * within the whole set of possible values.
     * Note that in practise, the probability distribution might not be
     * exactly uniform, but this function ensures both that each value
     * may be reached, and that it is cryptographically secure for most
     * applications.
     *
     * @return A random element value.
     */
    static G1 getRand();
    /**
     * @brief Gets the length of an element's data.
     *
     * @note This function always returns the same number
     * for the same value of the @p compressed parameter.
     *
     * @note Compressed data has the advantage of being almost
     * twice smaller than raw data, but it might take longer to
     * reconstruct the element in the @ref G1::getValue(const char*,int,bool)
     * function.
     *
     * @param compressed Indicates whether the element data
     *   is to be compressed or not.
     * @return Number of bytes needed to encode an element in G1.
     * @sa G1::getDataLen(bool)
     * @sa G1::getValue(const char*,bool)
     */
    static int getDataLen(bool compressed = false);
    /**
     * @brief Gets an element value from its data.
     *
     * @p data should hold exactly what the function
     * @ref G1::getData(char*,bool) gives for the element
     * to be reconstructed, for the same value of @p compressed.
     *
     * @param data Pointer to the data.
     * @param compressed Whether the data was compressed or not.
     * @return The element value corresponding to that data.
     * @sa G1::getDataLen(bool)
     * @sa G1::getData(char*,bool)
     */
    static G1 getValue(const char *data, bool compressed = false);
    /**
     * @brief Gets an element value from some data to hash.
     *
     * Note: This function is equivalent to calling successively
     * getHash(const char*,int,char*) and then G1::fromHash(const char*)
     * with some user-allocated buffer.
     *
     * @param data Pointer to the data to hash.
     * @param len Length of the data to hash.
     * @return Element generated from the hash of the data.
     * @sa G1::fromHash(const char*)
     */
    static G1 fromHash(const char *data, int len);
    /**
     * @brief Gets an element value from some hash.
     *
     * The hash value @p hash should be at least getHashLen()
     * bytes long, as would the getHash(const char*,int,char*)
     * function generate.
     *
     * @param hash A hash value.
     * @return Element generated from the hash value.
     * @sa getHashLen()
     * @sa getHash(const char*,int,char*)
     */
    static G1 fromHash(const char *hash);
private:
    inline explicit G1(void *v);
    inline explicit G1(SharedData *d);
    void deref();
private:
    SharedData *d;
};

/**
 * @brief The @f$\mathbb{G}_2@f$ class.
 *
 * Group of order @f$p@f$.
 */
class G2 {
    friend class GT;
public:
    /**
     * @brief Constructs a new null element.
     */
    inline G2();
    /**
     * @brief Performs a copy of an element.
     * @param other The element to be copied.
     */
    inline G2(const G2 &other);
    /**
     * @brief Releases memory.
     */
    inline ~G2();
    /**
     * @brief Assigns a new value to this element.
     * @param other New value for the element.
     * @return Reference to the current element.
     */
    inline G2 &operator=(const G2 &other);
    /**
     * @brief Unary minus operator.
     * @return Inverse of the element.
     */
    G2 operator-() const;
    /**
     * @brief Addition operator.
     * @param other Value to add to the current element.
     * @return The sum of the two elements.
     */
    G2 operator+(const G2 &other) const;
    /**
     * @brief Subtraction operator.
     * @param other Value to subtract to the current element.
     * @return The difference of the two elements.
     */
    G2 operator-(const G2 &other) const;
    /**
     * @brief Inplace addition operator.
     * @param other Value to append to the current element.
     * @return Reference to the modified element.
     */
    G2 &operator+=(const G2 &other);
    /**
     * @brief Inplace subtraction operator.
     * @param other Value to subtract to the current element.
     * @return Reference to the modified element.
     */
    G2 &operator-=(const G2 &other);
    /**
     * @brief Inplace multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return Reference to the modified element.
     */
    G2 &operator*=(const Fp &other);
    /**
     * @brief Multiplication operator.
     * @param other Scalar to multiply with the current element.
     * @return The product of the two elements.
     */
    G2 operator*(const Fp &other) const;
    /**
     * @brief Multiplication operator.
     * @param m Scalar value.
     * @param g Element value.
     * @return Product @f$m\cdot g@f$
     */
    friend G2 operator*(const Fp &m, const G2 &g);
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    bool operator==(const G2 &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `false` if the two values are equal, `true` otherwise.
     */
    inline bool operator!=(const G2 &other) const;
    /**
     * @brief Retrieves this element's data.
     *
     * The data memory space is to be allocated before calling
     * this function, and thus the parameter @p data should point
     * to some accessible space containing at least @ref G2::getDataLen(bool)
     * available bytes.
     *
     * The data will always contain exactly the number of bytes
     * returned by the @ref G2::getDataLen(bool) function called with
     * the same @p compressed value.
     *
     * It can then be converted back to the corresponding element
     * value thanks to the @ref G2::getValue(const char*,bool) function.
     *
     * @note Compressed data has the advantage of being almost
     * twice smaller than raw data, but it might take longer to
     * reconstruct the element in the @ref G2::getValue(const char*,bool)
     * function.
     *
     * @param data Pointer to where the data is to be stored.
     * @param compressed Indicates whether the element data
     *   is to be compressed or not.
     * @sa G2::getDataLen(bool)
     * @sa G2::getValue(const char*,bool)
     */
    void getData(char *data, bool compressed = false) const;
    /**
     * @brief Writes this element to an output stream.
     * @note The element is written in its compressed form.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,G2&)
     */
    friend std::ostream &operator<<(std::ostream &stream, const G2 &el);
    /**
     * @brief Reads this element from an input stream.
     * @note The element is to be read from its compressed form.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const G2&)
     */
    friend std::istream &operator>>(std::istream &stream, G2 &el);
    /**
     * @brief Checks if the element is null.
     * @return `true` if the element is null, `false` otherwise.
     */
    inline bool isNull() const;
    /**
     * @brief Sets this element to the null value.
     */
    inline void clear();
    /**
     * @brief Does some precomputations in preparation for
     *   scalar multiplication.
     *
     * @note Does nothing if hasPrecomputations() returns false.
     *
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline void precomputeForMult() {}
#else
    void precomputeForMult();
#endif
    /**
     * @brief Saves the scalar multiplication precomputations in a buffer.
     *
     * @warning This function allocates data thanks to the C++ idiom
     *   `new char[size]`. It is thus expected of the user to delete
     *   the pointer value that is set after calling this function
     *   using the operation `delete[] data`.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Reference to the data pointer to be set.
     * @return The size of the data in bytes.
     * @sa G2::precomputeForMult()
     * @sa G2::loadMultPrecomputations(char*)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline int saveMultPrecomputations(char *&data) {
        /* Note: A good application should not call this function
         * if hasPrecomputations() returns false, but we still prefer
         * returning dummy values than throwing an exception for
         * ease-of-use reasons. */
        data = new char[1];
        *data = 0; // (would not be a big threat to leak 1 byte though)
        return 1;
    }
#else
    int saveMultPrecomputations(char *&data);
#endif
    /**
     * @brief Loads the scalar multiplication precomputations from a buffer.
     *
     * @warning This function frees data thanks to the C++ idiom
     *   `delete[] data`. It is thus expected of the user provide a pointer
     *   value that was allocated thanks to the operation `new data[size]`.
     *
     * @warning The element on which this function is called should already
     *   have the same value as the one that was used to generate the
     *   precomputation data that is given.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Pointer to the data to be loaded.
     * @sa G2::saveMultPrecomputations(char*&)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    void loadMultPrecomputations(char *data) {
        delete[] data;
    }
#else
    void loadMultPrecomputations(char *data);
#endif
    /**
     * @brief Does some precomputations in preparation for
     *   pairing computation.
     *
     * @note Does nothing if hasPrecomputations() returns false.
     *
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline void precomputeForPairing() {}
#else
    void precomputeForPairing();
#endif
    /**
     * @brief Saves the pairing precomputations in a buffer.
     *
     * @warning This function allocates data thanks to the C++ idiom
     *   `new char[size]`. It is thus expected of the user to delete
     *   the pointer value that is set after calling this function
     *   using the operation `delete[] data`.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Reference to the data pointer to be set.
     * @return The size of the data in bytes.
     * @sa G2::precomputeForPairing()
     * @sa G2::loadPairingPrecomputations(char*)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline int savePairingPrecomputations(char *&data) {
        /* Note: A good application should not call this function
         * if hasPrecomputations() returns false, but we still prefer
         * returning dummy values than throwing an exception for
         * ease-of-use reasons. */
        data = new char[1];
        *data = 0; // (would not be a big threat to leak 1 byte though)
        return 1;
    }
#else
    int savePairingPrecomputations(char *&data);
#endif
    /**
     * @brief Loads the pairing precomputations from a buffer.
     *
     * @warning This function frees data thanks to the C++ idiom
     *   `delete[] data`. It is thus expected of the user provide a pointer
     *   value that was allocated thanks to the operation `new data[size]`.
     *
     * @warning The element on which this function is called should already
     *   have the same value as the one that was used to generate the
     *   precomputation data that is given.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Pointer to the data to be loaded.
     * @sa G2::savePairingPrecomputations(char*&)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    void loadPairingPrecomputations(char *data) {
        delete[] data;
    }
#else
    void loadPairingPrecomputations(char *data);
#endif
public:
    /**
     * @brief Get a random element.
     *
     * This function picks a random value chosen with uniform probability
     * within the whole set of possible values.
     * Note that in practise, the probability distribution might not be
     * exactly uniform, but this function ensures both that each value
     * may be reached, and that it is cryptographically secure for most
     * applications.
     *
     * @return A random element value.
     */
    static G2 getRand();
    /**
     * @brief Gets the length of an element's data.
     *
     * @note This function always returns the same number
     * for the same value of the @p compressed parameter.
     *
     * @note Compressed data has the advantage of being almost
     * twice smaller than raw data, but it might take longer to
     * reconstruct the element in the @ref G2::getValue(const char*,int,bool)
     * function.
     *
     * @param compressed Indicates whether the element data
     *   is to be compressed or not.
     * @return Number of bytes needed to encode an element in G2.
     * @sa G2::getDataLen(bool)
     * @sa G2::getValue(const char*,bool)
     */
    static int getDataLen(bool compressed = false);
    /**
     * @brief Gets an element value from its data.
     *
     * @p data should hold exactly what the function
     * @ref G2::getData(char*,bool) gives for the element
     * to be reconstructed, for the same value of @p compressed.
     *
     * @param data Pointer to the data.
     * @param compressed Whether the data was compressed or not.
     * @return The element value corresponding to that data.
     * @sa G2::getDataLen(bool)
     * @sa G2::getData(char*,bool)
     */
    static G2 getValue(const char *data, bool compressed = false);
    /**
     * @brief Gets an element value from some data to hash.
     *
     * Note: This function is equivalent to calling successively
     * getHash(const char*,int,char*) and then G2::fromHash(const char*)
     * with some user-allocated buffer.
     *
     * @param data Pointer to the data to hash.
     * @param len Length of the data to hash.
     * @return Element generated from the hash of the data.
     * @sa G2::fromHash(const char*)
     */
    static G2 fromHash(const char *data, int len);
    /**
     * @brief Gets an element value from some hash.
     *
     * The hash value @p hash should be at least getHashLen()
     * bytes long, as would the getHash(const char*,int,char*)
     * function generate.
     *
     * @param hash A hash value.
     * @return Element generated from the hash value.
     * @sa getHashLen()
     * @sa getHash(const char*,int,char*)
     */
    static G2 fromHash(const char *hash);
private:
    inline explicit G2(void *v);
    inline explicit G2(SharedData *d);
    void deref();
private:
    SharedData *d;
};

/**
 * @brief The @f$\mathbb{G}_T@f$ class.
 *
 * Group of order @f$p@f$, provided with a bilinear map
 * going from @f$\mathbb{G}_1\times\mathbb{G}_2@f$
 * to @f$\mathbb{G}_T@f$.
 */
class GT {
public:
    /**
     * @brief Constructs a new unit element (value 1).
     */
    inline GT();
    /**
     * @brief Performs a copy of an element.
     * @param other The element to be copied.
     */
    inline GT(const GT &other);
    /**
     * @brief Releases memory.
     */
    inline ~GT();
    /**
     * @brief Assigns a new value to this element.
     * @param other New value for the element.
     * @return Reference to the current element.
     */
    inline GT &operator=(const GT &other);
    /**
     * @brief Multiplication operator.
     * @param other Value to multiply with the current element.
     * @return The product of the two elements.
     */
    GT operator*(const GT &other) const;
    /**
     * @brief Division operator.
     * @param other Value with which to divide the current element.
     * @return The fraction of the two elements.
     */
    GT operator/(const GT &other) const;
    /**
     * @brief Inplace multiplication operator.
     * @param other Value to multiply with the current element.
     * @return Reference to the modified element.
     */
    GT &operator*=(const GT &other);
    /**
     * @brief Inplace division operator.
     * @param other Value with which to divide the current element.
     * @return Reference to the modified element.
     */
    GT &operator/=(const GT &other);
    /**
     * @brief Inplace power operator.
     * @param other Exponent value.
     * @return Reference to the modified element.
     */
    GT &operator^=(const Fp &other);
    /**
     * @brief Power operator.
     * @param other Exponent value.
     * @return Power @f$this^{other}@f$
     */
    GT operator^(const Fp &other) const;
    /**
     * @brief Equality operator.
     * @param other Value with which to compare the current element.
     * @return `true` if the two values are equal, `false` otherwise.
     */
    bool operator==(const GT &other) const;
    /**
     * @brief Inequality operator.
     * @param other Value with which to compare the current element.
     * @return `false` if the two values are equal, `true` otherwise.
     */
    inline bool operator!=(const GT &other) const;
    /**
     * @brief Retrieves this element's data.
     *
     * The data memory space is to be allocated before calling
     * this function, and thus the parameter @p data should point
     * to some accessible space containing at least @ref GT::getDataLen()
     * available bytes.
     *
     * The data will always contain exactly the number of bytes
     * returned by the @ref GT::getDataLen() function.
     *
     * It can then be converted back to the corresponding element
     * value thanks to the @ref GT::getValue(const char*) function.
     *
     * @param data Pointer to where the data is to be stored.
     * @sa Fp::getDataLen()
     * @sa Fp::getValue(const char*)
     */
    void getData(char *data) const;
    /**
     * @brief Writes this element to an output stream.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Output stream.
     * @param el Element to write in the stream.
     * @return Reference to the output stream.
     * @sa operator>>(std::istream&,GT&)
     */
    friend std::ostream &operator<<(std::ostream &stream, const GT &el);
    /**
     * @brief Reads this element from an input stream.
     * @warning The iostream based function do not necessarily work
     *   with the same data as the `getData`-based functions.
     * @param stream Input stream.
     * @param el Element to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::istream&,const GT&)
     */
    friend std::istream &operator>>(std::istream &stream, GT &el);
    /**
     * @brief Checks if the element is 1.
     * @return `true` if the element is 1, `false` otherwise.
     */
    inline bool isUnit() const;
    /**
     * @brief Sets this element to 1.
     */
    inline void clear();
    /**
     * @brief Does some precomputations in preparation for
     *   scalar power.
     *
     * @note Does nothing if hasPrecomputations() returns false.
     *
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline void precomputeForPower() {}
#else
    void precomputeForPower();
#endif
    /**
     * @brief Saves the scalar power precomputations in a buffer.
     *
     * @warning This function allocates data thanks to the C++ idiom
     *   `new char[size]`. It is thus expected of the user to delete
     *   the pointer value that is set after calling this function
     *   using the operation `delete[] data`.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Reference to the data pointer to be set.
     * @return The size of the data in bytes.
     * @sa GT::precomputeForPower()
     * @sa GT::loadPowerPrecomputations(char*)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    inline int savePowerPrecomputations(char *&data) {
        /* Note: A good application should not call this function
         * if hasPrecomputations() returns false, but we still prefer
         * returning dummy values than throwing an exception for
         * ease-of-use reasons. */
        data = new char[1];
        *data = 0; // (would not be a big threat to leak 1 byte though)
        return 1;
    }
#else
    int savePowerPrecomputations(char *&data);
#endif
    /**
     * @brief Loads the scalar power precomputations from a buffer.
     *
     * @warning This function frees data thanks to the C++ idiom
     *   `delete[] data`. It is thus expected of the user provide a pointer
     *   value that was allocated thanks to the operation `new data[size]`.
     *
     * @warning The element on which this function is called should already
     *   have the same value as the one that was used to generate the
     *   precomputation data that is given.
     *
     * @note Has a dummy implementation if hasPrecomputations() returns false.
     *
     * @param data Pointer to the data to be loaded.
     * @sa G1::savePowerPrecomputations(char*&)
     * @sa hasPrecomputations()
     */
#if defined(USE_PBC) && !defined(LIB_COMPILATION)
    void loadPowerPrecomputations(char *data) {
        delete[] data;
    }
#else
    void loadPowerPrecomputations(char *data);
#endif
public:
    /**
     * @brief Get a random element.
     *
     * This function picks a random value chosen with uniform probability
     * within the whole set of possible values.
     * Note that in practise, the probability distribution might not be
     * exactly uniform, but this function ensures both that each value
     * may be reached, and that it is cryptographically secure for most
     * applications.
     *
     * @return A random element value.
     */
    static GT getRand();
    /**
     * @brief Gets the length of an element's data.
     *
     * @note This function always returns the same number.
     *
     * @return Number of bytes needed to encode an element in GT.
     * @sa GT::getDataLen()
     * @sa Fp::getValue(const char*)
     */
    static int getDataLen();
    /**
     * @brief Gets an element value from its data.
     *
     * @p data should hold exactly what the function
     * @ref GT::getData(char*) gives for the element
     * to be reconstructed.
     *
     * @param data Pointer to the data.
     * @return The element value corresponding to that data.
     * @sa GT::getDataLen()
     * @sa GT::getData(char*)
     */
    static GT getValue(const char *data);
    /**
     * @brief Computes a pairing of two elements.
     * @param a @f$\mathbb{G}_1@f$ member.
     * @param b @f$\mathbb{G}_2@f$ member.
     * @return @f$\mathbb{G}_T@f$ member that is the result of calling
     *   the bilinear map on @p a and @p b.
     */
    static GT pairing(const G1 &a, const G2 &b);
    /**
     * @brief Computes the product of multiple pairings.
     * @param lst List of couples in @f$(\mathbb{G}_1,\mathbb{G}_2)@f$.
     * @return Product of the pairings of each couple.
     */
    static GT pairing(const std::vector<std::pair<G1, G2> > &lst);
private:
    inline explicit GT(void *v);
    inline explicit GT(SharedData *d);
    void deref();
private:
    SharedData *d;
};

/* Inline definitions: */

/**
 * @cond INTERNAL_DATA_STRUCT
 */

inline SharedData::SharedData(void *p) : c(0), p(p) {}

inline Fp::Fp() { ++(d = zero)->c; }

inline Fp::Fp(const Fp &other) { ++(d = other.d)->c; }

inline Fp::~Fp() { deref(); }

inline Fp &Fp::operator=(const Fp &other) {
    deref();
    ++(d = other.d)->c;
    return *this;
}

inline bool Fp::operator!=(const Fp &other) const {
    return !(*this == other);
}

inline Fp Fp::getUnit() { return Fp(one); }

inline Fp::Fp(void *v) : d(new SharedData(v)) {}

inline Fp::Fp(SharedData *d) : d(d) { ++d->c; }

inline G1::G1() : d(NULL) {}

inline G1::G1(const G1 &other) {
    if ((d = other.d)) ++d->c;
}

inline G1::~G1() { if (d) deref(); }

inline G1 &G1::operator=(const G1 &other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline bool G1::operator!=(const G1 &other) const {
    return !(*this == other);
}

inline bool G1::isNull() const { return !d; }

inline void G1::clear() {
    if (d) {
        deref();
        d = NULL;
    }
}

inline G1::G1(void *v) : d(new SharedData(v)) {}

inline G1::G1(SharedData *d) : d(d) { ++d->c; }

inline G2::G2() : d(NULL) {}

inline G2::G2(const G2 &other) {
    if ((d = other.d)) ++d->c;
}

inline G2::~G2() { if (d) deref(); }

inline G2 &G2::operator=(const G2 &other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline bool G2::operator!=(const G2 &other) const {
    return !(*this == other);
}

inline bool G2::isNull() const { return !d; }

inline void G2::clear() {
    if (d) {
        deref();
        d = NULL;
    }
}

inline G2::G2(void *v) : d(new SharedData(v)) {}

inline G2::G2(SharedData *d) : d(d) { ++d->c; }

inline GT::GT() : d(NULL) {}

inline GT::GT(const GT &other) {
    if ((d = other.d)) ++d->c;
}

inline GT::~GT() { if (d) deref(); }

inline GT &GT::operator=(const GT &other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline bool GT::operator!=(const GT &other) const {
    return !(*this == other);
}

inline bool GT::isUnit() const { return !d; }

inline void GT::clear() {
    if (d) {
        deref();
        d = NULL;
    }
}

inline GT::GT(void *v) : d(new SharedData(v)) {}

inline GT::GT(SharedData *d) : d(d) { ++d->c; }

/**
 * @endcond
 */

} /* End of namespace pairings */

#endif /* End of PAIRINGS_H */
