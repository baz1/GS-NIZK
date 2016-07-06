#ifndef PAIRINGS_H
#define PAIRINGS_H

#include <vector>

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
 * @param len Length of the data pointed to by @p rndData.
 * @param rndData Pointer to some random data used to
 *   initialize the pseudo-random number generator.
 *   `/dev/random` or `/dev/urandom` might be used under Linux.
 */
void initialize_pairings(int len, const char *rndData);

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
    friend void initialize_pairings(int, const char*);
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
     * @return `true` if the two values are equal, `false` otherwise.
     */
    inline bool operator!=(const Fp &other) const;
    /**
     * @brief Gets the length of this element's data.
     *
     * @warning This function might not always return the same
     * number, depending on the value of the element to convert.
     *
     * @return Number of bytes needed to encode the element.
     */
    int getDataLen() const;
    /**
     * @brief Retrieves this element's data.
     *
     * The data memory space is to be allocated before calling
     * this function, and thus the parameter @p data should point
     * to some accessible space containing at least getHashLen()
     * available bytes.
     *
     * The data will always contain exactly the number of bytes
     * returned by the Fp::getDataLen() function.
     *
     * It can then be converted back to the corresponding element
     * value thanks to the Fp::getValue(const char*,int) function.
     *
     * @param data Pointer to where the data is to be stored.
     * @sa Fp::getDataLen()
     * @sa Fp::getValue(const char*,int)
     */
    void getData(char *data) const;
    /**
     * @brief Checks if the element is null.
     * @return `true` if the element is null, `false` otherwise.
     */
    bool isNull() const;
public:
    /**
     * @brief Gets the unity element.
     *
     * Note that this is equivalent to `Fp(1)`.
     *
     * @return The unity element.
     * @sa Fp::Fp(int)
     */
    static Fp getUnit();
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
     * @brief Gets an element value from its data.
     *
     * @p data and @p len should be respectively what the functions
     * Fp::getData(char*) and Fp::getDataLen() give for the element
     * to be reconstructed.
     *
     * @param data Pointer to the data.
     * @param len Length of the data.
     * @return The element value corresponding to that data.
     */
    static Fp getValue(const char *data, int len);
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

class G1 {
    friend class GT;
public:
    inline G1();
    inline G1(const G1 &other);
    inline ~G1();
    inline G1 &operator=(const G1 &other);
    G1 operator-() const;
    G1 operator+(const G1 &other) const;
    G1 operator-(const G1 &other) const;
    G1 &operator+=(const G1 &other);
    G1 &operator-=(const G1 &other);
    G1 &operator*=(const Fp &other);
    G1 operator*(const Fp &other) const;
    friend G1 operator*(const Fp &m, const G1 &g);
    bool operator==(const G1 &other) const;
    inline bool operator!=(const G1 &other) const;
    int getDataLen(bool compressed = false) const;
    void getData(char *data, bool compressed = false) const;
    inline bool isNull() const;
public:
    static G1 getRand();
    static G1 getValue(const char *data, int len, bool compressed = false);
    static G1 fromHash(const char *data, int len);
    static G1 fromHash(const char *hash);
private:
    inline explicit G1(void *v);
    inline explicit G1(SharedData *d);
    void deref();
private:
    SharedData *d;
};

class G2 {
    friend class GT;
public:
    inline G2();
    inline G2(const G2 &other);
    inline ~G2();
    inline G2 &operator=(const G2 &other);
    G2 operator-() const;
    G2 operator+(const G2 &other) const;
    G2 operator-(const G2 &other) const;
    G2 &operator+=(const G2 &other);
    G2 &operator-=(const G2 &other);
    G2 &operator*=(const Fp &other);
    G2 operator*(const Fp &other) const;
    friend G2 operator*(const Fp &m, const G2 &g);
    bool operator==(const G2 &other) const;
    inline bool operator!=(const G2 &other) const;
    int getDataLen(bool compressed = false) const;
    void getData(char *data, bool compressed = false) const;
    inline bool isNull() const;
public:
    static G2 getRand();
    static G2 getValue(const char *data, int len, bool compressed = false);
    static G2 fromHash(const char *data, int len);
    static G2 fromHash(const char *hash);
private:
    inline explicit G2(void *v);
    inline explicit G2(SharedData *d);
    void deref();
private:
    SharedData *d;
};

class GT {
public:
    inline GT();
    inline GT(const GT &other);
    inline ~GT();
    inline GT &operator=(const GT &other);
    GT operator*(const GT &other) const;
    GT operator/(const GT &other) const;
    GT &operator*=(const GT &other);
    GT &operator/=(const GT &other);
    GT &operator^=(const Fp &other);
    GT operator^(const Fp &other) const;
    bool operator==(const GT &other) const;
    inline bool operator!=(const GT &other) const;
    int getDataLen() const;
    void getData(char *data) const;
    inline bool isUnity() const;
public:
    static GT getRand();
    static GT getValue(const char *data, int len);
    static GT pairing(const G1 &a, const G2 &b);
    static GT pairing(const std::vector<std::pair<G1, G2> > &lst);
private:
    inline explicit GT(void *v);
    inline explicit GT(SharedData *d);
    void deref();
private:
    SharedData *d;
};

/* Inline definitions: */

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

inline Fp::Fp(void *v) : d(new SharedData(v)) {}

inline Fp::Fp(SharedData *d) : d(d) { ++d->c; }

inline G1::G1() : d(0) {}

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

inline G1::G1(void *v) : d(new SharedData(v)) {}

inline G1::G1(SharedData *d) : d(d) { ++d->c; }

inline G2::G2() : d(0) {}

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

inline G2::G2(void *v) : d(new SharedData(v)) {}

inline G2::G2(SharedData *d) : d(d) { ++d->c; }

inline GT::GT() : d(0) {}

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

inline bool GT::isUnity() const { return !d; }

inline GT::GT(void *v) : d(new SharedData(v)) {}

inline GT::GT(SharedData *d) : d(d) { ++d->c; }

} /* End of namespace pairings */

#endif // PAIRINGS_H
