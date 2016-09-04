/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "pairings.h"

#if !defined(LIB_COMPILATION)
#error LIB_COMPILATION should be defined for the compilation of this library.
#endif

/* Prioritize Qt's no-debug policy, if existent */
#if defined(QT_NO_DEBUG) && defined(DEBUG)
#undef DEBUG
#endif

// Note: The following is used more for an indication and hint for the readers
// rather than an efficiency improvement.
#ifdef __GNUC__
#define LIKELY(x)       __builtin_expect(!!(x), 1)
#define UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#define LIKELY(x)       (x)
#define UNLIKELY(x)     (x)
#endif /* __GNUC__ */

#ifdef DEBUG
#define ASSERT(X,Y) if (UNLIKELY(!(X))) { \
    std::cerr << "Error: Assert of " << #X << " at line " \
    << __LINE__ << " failed in " << __FILE__ << "!" << std::endl \
    << Y << std::endl; \
    throw Y; \
    }
#else
#define ASSERT(X,Y) /* noop */
#endif

#ifndef HASH_LEN_BITS
#define HASH_LEN_BITS 256
//#define HASH_LEN_BITS 512
#endif

#define NULL_ELEMENT_BYTE_VALUE 2

#define HASH_LEN_BYTES (HASH_LEN_BITS / 8)

#if defined(USE_MIRACL)
/* -------------------- MIRACL build -------------------- */

#define MR_PAIRING_BN // AES-128 or AES-192 security

#ifndef AES_SECURITY
#define AES_SECURITY 128
//#define AES_SECURITY 192
#endif /* Not AES_SECURITY */

#define BUFFER_SIZE 128

#include "pairing_3.h"

#include <cstring>
#include <ctime>

#define MIRACL_BYTES (MIRACL / 8)

namespace pairings {

static PFC *pfc;
static int big_size;

SharedData *Fp::zero = NULL, *Fp::one = NULL;


#ifdef GSNIZK_IOSTREAM_NOTHREADS
static char *iostream_nothreads_buffer;
#endif

template <typename T> inline const T &min(const T &a, const T &b) {
    return (a < b) ? a : b;
}

template <typename T> inline const T &max(const T &a, const T &b) {
    return (a < b) ? b : a;
}

/* Get the number of bits in the big number n */
/* (Note: unused)

#ifdef __GNUC__
#define NB_BITS(x)      ((sizeof(unsigned long) * 8) - \
                            __builtin_clzl((unsigned long) (x)))
#else
inline int count_bits(unsigned long x) {
    int r = 1;
    while (x >>= 1) ++r;
    return r;
}
#define NB_BITS(x)      count_bits((unsigned long) (x))
#endif // __GNUC__

int getNBits(const Big &n) {
    int i = n.len();
    while ((--i >= 0) && (!n[i]));
    if (i < 0) return 0;
    return NB_BITS(n[i]) + i * sizeof(mr_small);
}

// */

static ::GT *rndSeed = NULL;

void initialize_pairings(int len, const char *data) {
    ASSERT(len >= 0, "Negative length");
    csprng *rnd = new csprng;
    // Note: const keyword simply missing in strong_init
    strong_init(rnd, len, const_cast<char*>(data), (unsigned int) clock());
    pfc = new PFC(AES_SECURITY, rnd);
    Fp::zero = new SharedData(reinterpret_cast<void*>(new ::Big()));
    Fp::one = new SharedData(reinterpret_cast<void*>(new ::Big(1)));
    ::G1 g1;
    ::G2 g2;
    pfc->random(g1);
    pfc->random(g2);
    big_size = pfc->mod->len() * MIRACL_BYTES;
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    iostream_nothreads_buffer = new char[big_size];
#endif
    rndSeed = new ::GT(pfc->pairing(g2, g1));
}

void terminate_pairings() {
    if (!rndSeed) return;
    delete rndSeed;
    rndSeed = NULL;
    if (Fp::zero->c) {
        ASSERT(false, "Trailing references");
        --Fp::zero->c;
    } else {
        delete reinterpret_cast< ::Big* >(Fp::zero->p);
        delete Fp::zero;
    }
    if (Fp::one->c) {
        ASSERT(false, "Trailing references");
        --Fp::one->c;
    } else {
        delete reinterpret_cast< ::Big* >(Fp::one->p);
        delete Fp::one;
    }
    strong_kill(pfc->RNG);
    delete pfc->RNG;
    delete pfc;
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    delete[] iostream_nothreads_buffer;
#endif
}

int getHashLen() {
    return HASH_LEN_BYTES;
}

void getHash(const char *data, int len, char *hash) {
#if HASH_LEN_BITS == 256
    sha256 sh;
    shs256_init(&sh);
    for (int i = 0; i < len; i++)
    {
        shs256_process(&sh, (int) data[i]);
    }
    shs256_hash(&sh, hash);
#elif HASH_LEN_BITS == 512
    sha512 sh;
    shs512_init(&sh);
    for (int i = 0; i < len; i++)
    {
        shs512_process(&sh, (int) data[i]);
    }
    shs512_hash(&sh, hash);
#else
#error Error: Invalid value of HASH_LEN_BITS (pairings)
#endif
}

bool hasPrecomputations() {
    return true;
}

bool iostream_nothreads() {
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    return true;
#else
    return false;
#endif
}

/* Efficiency-improved hash-map function */
void hashToZZn(const char *hash, const ::Big &n, ::Big &result) {
    result = n;
    char *ptr = reinterpret_cast<char*>(&result[0]);
    int remaining = result.len() * MIRACL_BYTES;
    while (remaining) {
        int len = min(remaining, HASH_LEN_BYTES);
        memcpy(ptr, hash, len);
        remaining -= len;
        ptr += len;
    }
    result %= n;
}

void hashToZZn(const char *data, int len, const ::Big &n, ::Big &result) {
    char s[HASH_LEN_BYTES];
    getHash(data, len, s);
    hashToZZn(s, n, result);
}

Fp::Fp(int i) : d(new SharedData(reinterpret_cast<void*>(i >= 0 ?
        (new ::Big(i)) : (new ::Big(*pfc->ord - Big(-i)))))) {}

Fp::Fp(unsigned long i)
    : d(new SharedData(reinterpret_cast<void*>(new ::Big(i)))) {}

Fp Fp::operator-() const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    if ((d == zero) || _this.iszero())
        return Fp(*this);
    return Fp(reinterpret_cast<void*>(new ::Big((*pfc->ord) - _this)));
}

Fp Fp::operator+(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((d == zero) || _this.iszero()) return other;
    if ((other.d == zero) || _other.iszero()) return *this;
    ::Big *_el = new ::Big(_this);
    *_el += _other;
    if (*_el >= *pfc->ord)
        *_el -= *pfc->ord;
    return Fp(reinterpret_cast<void*>(_el));
}

Fp Fp::operator-(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((other.d == zero) || _other.iszero()) return *this;
    if (_this >= _other)
        return Fp(reinterpret_cast<void*>(new ::Big(_this - _other)));
    return Fp(reinterpret_cast<void*>(new ::Big((*pfc->ord + _this) - _other)));
}

Fp &Fp::operator+=(const Fp &other) {
    ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((other.d == zero) || _other.iszero()) return *this;
    if ((d == zero) || _this.iszero()) return (*this = other);
    if (d->c) {
        --d->c;
        ::Big *_el = new ::Big(_this);
        *_el += _other;
        if (*_el >= *pfc->ord)
            *_el -= *pfc->ord;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        _this += _other;
        if (_this >= *pfc->ord)
            _this -= *pfc->ord;
    }
    return *this;
}

Fp &Fp::operator-=(const Fp &other) {
    ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((other.d == zero) || _other.iszero()) return *this;
    if (d->c) {
        --d->c;
        ::Big *_el = new ::Big(_this);
        if (*_el < _other)
            *_el += *pfc->ord;
        *_el -= _other;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        if (_this < _other)
            _this += *pfc->ord;
        _this -= _other;
    }
    return *this;
}

Fp Fp::operator*(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((d == zero) || (other.d == one) || _this.iszero() || _other.isone())
        return *this;
    if ((d == one) || (other.d == zero) || _other.iszero() || _this.isone())
        return other;
    return Fp(reinterpret_cast<void*>(new ::Big(modmult(_this, _other,
                                                        *pfc->ord))));
}

Fp Fp::operator/(const Fp &other) const {
    ASSERT(!other.isNull(), "Divide by zero");
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((d == zero) || (other.d == one) || _this.iszero() || _other.isone())
        return *this;
    return Fp(reinterpret_cast<void*>(new ::Big(moddiv(_this, _other,
                                                       *pfc->ord))));
}

Fp &Fp::operator*=(const Fp &other) {
    ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((d == zero) || (other.d == one) || _this.iszero() || _other.isone())
        return *this;
    if ((d == one) || (other.d == zero) || _other.iszero() || _this.isone())
        return (*this = other);
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(
            new ::Big(modmult(_this, _other, *pfc->ord))));
    } else {
        _this = modmult(_this, _other, *pfc->ord);
    }
    return *this;
}

Fp &Fp::operator/=(const Fp &other) {
    ASSERT(!other.isNull(), "Divide by zero");
    ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((d == zero) || (other.d == one) || _this.iszero() || _other.isone())
        return *this;
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(
            new ::Big(moddiv(_this, _other, *pfc->ord))));
    } else {
        _this = moddiv(_this, _other, *pfc->ord);
    }
    return *this;
}

bool Fp::operator==(const Fp &other) const {
    if (d == other.d) return true;
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return _this == _other;
}

void Fp::getData(char *data) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    to_binary(_this, big_size, data, TRUE);
}

std::ostream &operator<<(std::ostream &stream, const Fp &el) {
    const ::Big &_el = *reinterpret_cast< ::Big* >(el.d->p);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    to_binary(_el, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
#else
    char *iostream_threads_buffer = new char[big_size];
    to_binary(_el, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, Fp &el) {
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.read(iostream_nothreads_buffer, big_size);
    if (el.d->c) {
        --el.d->c;
        el.d = new SharedData(reinterpret_cast<void*>(
                new ::Big(from_binary(big_size, iostream_nothreads_buffer))));
    } else {
        ::Big &_el = *reinterpret_cast< ::Big* >(el.d->p);
        _el = from_binary(big_size, iostream_nothreads_buffer);
    }
#else
    char *iostream_threads_buffer = new char[big_size];
    stream.read(iostream_threads_buffer, big_size);
    if (el.d->c) {
        --el.d->c;
        el.d = new SharedData(reinterpret_cast<void*>(
                new ::Big(from_binary(big_size, iostream_threads_buffer))));
    } else {
        ::Big &_el = *reinterpret_cast< ::Big* >(el.d->p);
        _el = from_binary(big_size, iostream_threads_buffer);
    }
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

bool Fp::isNull() const {
    if (d == zero) return true;
    return reinterpret_cast< ::Big* >(d->p)->iszero();
}

bool Fp::isUnit() const {
    if (d == one) return true;
    return reinterpret_cast< ::Big* >(d->p)->isone();
}

Fp Fp::getRand() {
    return Fp(reinterpret_cast<void*>(
        new ::Big(strong_rand(pfc->RNG, *pfc->ord))));
}

int Fp::getDataLen() {
    return big_size;
}

Fp Fp::getValue(const char *data) {
    // Note: const keyword simply missing in from_binary
    return Fp(reinterpret_cast<void*>(
        new ::Big(from_binary(big_size, const_cast<char*>(data)))));
}

Fp Fp::fromHash(const char *data, int len) {
    ::Big *_el = new ::Big();
    hashToZZn(data, len, *pfc->ord, *_el);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp Fp::fromHash(const char *hash) {
    ::Big *_el = new ::Big();
    hashToZZn(hash, *pfc->ord, *_el);
    return Fp(reinterpret_cast<void*>(_el));
}

void Fp::deref() {
    if (d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::Big* >(d->p);
        delete d;
    }
}

G1 G1::operator-() const {
    if (!d) return G1();
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    return G1(reinterpret_cast<void*>(new ::G1(-_this)));
}

G1 G1::operator+(const G1 &other) const {
    if (!d) return other;
    if (!other.d) return *this;
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    ::G1 *_el = new ::G1(_this + _other);
    if (_el->g.iszero()) {
        delete _el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::operator-(const G1 &other) const {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) return G1(reinterpret_cast<void*>(new ::G1(-_other)));
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    ::G1 *_el = new ::G1(_this + (-_other));
    if (_el->g.iszero()) {
        delete _el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 &G1::operator+=(const G1 &other) {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    ::G1 *_el = new ::G1(_this + _other);
    if (_el->g.iszero()) {
        delete _el;
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

G1 &G1::operator-=(const G1 &other) {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) {
        d = new SharedData(reinterpret_cast<void*>(new ::G1(-_other)));
        return *this;
    }
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    ::G1 *_el = new ::G1(_this + (-_other));
    if (_el->g.iszero()) {
        delete _el;
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

G1 &G1::operator*=(const Fp &other) {
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((!d) || (other.d == Fp::one) || (_other.isone()))
        return *this;
    if ((other.d == Fp::zero) || _other.iszero()) {
        deref();
        d = NULL;
        return *this;
    }
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    void *_el = reinterpret_cast<void*>(
                new ::G1(pfc->mult(_this, _other)));
    if (d->c) {
        --d->c;
        d = new SharedData(_el);
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = _el;
    }
    return *this;
}

G1 G1::operator*(const Fp &other) const {
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((!d) || (other.d == Fp::one) || _other.isone()) return *this;
    if ((other.d == Fp::zero) || _other.iszero()) return G1();
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    return G1(reinterpret_cast<void*>(new ::G1(pfc->mult(_this, _other))));
}

G1 operator*(const Fp &m, const G1 &g) {
    const ::Big &_m = *reinterpret_cast< ::Big* >(m.d->p);
    if ((!g.d) || (m.d == Fp::one) || _m.isone()) return g;
    if ((m.d == Fp::zero) || _m.iszero()) return G1();
    const ::G1 &_g = *reinterpret_cast< ::G1* >(g.d->p);
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    return G1(reinterpret_cast<void*>(new ::G1(pfc->mult(_g, _m))));
}

bool G1::operator==(const G1 &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    return (_this == _other);
}

void G1::getData(char *data, bool compressed) const {
    if (!d) {
        if (compressed) {
            *data = NULL_ELEMENT_BYTE_VALUE;
        } else {
            const ::Big &_one = *reinterpret_cast< ::Big* >(Fp::one->p);
            to_binary(_one, big_size, data, TRUE);
            to_binary(_one, big_size, data + big_size, TRUE);
        }
        return;
    }
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    if (compressed) {
        ::Big x;
        int lsb = _this.g.get(x);
        *data = (char) lsb;
        to_binary(x, big_size, data + 1, TRUE);
    } else {
        ::Big x, y;
        _this.g.get(x, y);
        to_binary(x, big_size, data, TRUE);
        to_binary(y, big_size, data + big_size, TRUE);
    }
}

std::ostream &operator<<(std::ostream &stream, const G1 &el) {
    char lsb;
    if (!el.d) {
        lsb = NULL_ELEMENT_BYTE_VALUE;
        stream.write(&lsb, 1);
        return stream;
    }
    const ::G1 &_el = *reinterpret_cast< ::G1* >(el.d->p);
    ::Big x;
    lsb = (char) _el.g.get(x);
    stream.write(&lsb, 1);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    to_binary(x, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
#else
    char *iostream_threads_buffer = new char[big_size];
    to_binary(x, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, G1 &el) {
    char lsb;
    stream.read(&lsb, 1);
    if (lsb == NULL_ELEMENT_BYTE_VALUE) {
        if (el.d) el.deref();
        return stream;
    }
    ::G1 *_el = new ::G1();
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.read(iostream_nothreads_buffer, big_size);
    _el->g.set(from_binary(big_size, iostream_nothreads_buffer), (int) lsb);
#else
    char *iostream_threads_buffer = new char[big_size];
    stream.read(iostream_threads_buffer, big_size);
    _el->g.set(from_binary(big_size, iostream_threads_buffer), (int) lsb);
    delete[] iostream_threads_buffer;
#endif
    if (el.d) {
        if (el.d->c) {
            --el.d->c;
            el.d = new SharedData(reinterpret_cast<void*>(_el));
        } else {
            delete reinterpret_cast< ::G1* >(el.d->p);
            el.d->p = reinterpret_cast<void*>(_el);
        }
    } else {
        el.d = new SharedData(reinterpret_cast<void*>(_el));
    }
    return stream;
}

void G1::precomputeForMult() {
    if (!d) return;
    pfc->precomp_for_mult(*reinterpret_cast< ::G1* >(d->p));
}

int G1::saveMultPrecomputations(char *&data) {
    if (!d) {
        /* Dummy implementation */
        data = new char[1];
        *data = 0;
        return 1;
    }
    return reinterpret_cast< ::G1* >(d->p)->spill(data);
}

void G1::loadMultPrecomputations(char *data) {
    if (!d) {
        delete[] data;
        return;
    }
    ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    _this.restore(data);
    _this.mtbits = MR_ROUNDUP(bits(*pfc->ord),WINDOW_SIZE);
}

G1 G1::getRand() {
    ::G1 *_el = new ::G1();
    pfc->random(*_el);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(_el->g.iszero())) {
        delete _el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

int G1::getDataLen(bool compressed) {
    if (compressed) {
        return big_size + 1;
    } else {
        return big_size << 1;
    }
}

G1 G1::getValue(const char *data, bool compressed) {
    // Note: const keyword simply missing in from_binary
    ::G1 *_el;
    if (compressed) {
        int lsb = (int) *(data++);
        if (lsb == NULL_ELEMENT_BYTE_VALUE)
            return G1();
        _el = new ::G1();
        _el->g.set(from_binary(big_size, const_cast<char*>(data)), lsb);
    } else {
        _el = new ::G1();
        _el->g.set(from_binary(big_size, const_cast<char*>(data)),
                  from_binary(big_size, const_cast<char*>(data + big_size)));
        if (_el->g.iszero()) {
            delete _el;
            return G1();
        }
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::fromHash(const char *data, int len) {
    ::G1 *_el = new ::G1();
    ::Big x0;
    hashToZZn(data, len, *pfc->mod, x0);
    while (!_el->g.set(x0, x0)) ++x0;
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(_el->g.iszero())) {
        delete _el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::fromHash(const char *hash) {
    ::G1 *_el = new ::G1();
    ::Big x0;
    hashToZZn(hash, *pfc->mod, x0);
    while (!_el->g.set(x0, x0)) ++x0;
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(_el->g.iszero())) {
        delete _el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

void G1::deref() {
    if (d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        delete d;
    }
}

G2 G2::operator-() const {
    if (!d) return G2();
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    return G2(reinterpret_cast<void*>(new ::G2(-_this)));
}

/* Replacement for "::G2 operator+(const ::G2& x,const ::G2& y)" */
::G2 *addG2(const ::G2 &a, const ::G2 &b) {
    ::G2 *result;
    // Note: const keyword simply missing in ECn2::type
    if (const_cast< ::ECn2& >(b.g).type() == MR_EPOINT_GENERAL) {
        if ((const_cast< ::ECn2& >(a.g).type() != MR_EPOINT_GENERAL)
                || (&a == &b)) {
            result = new ::G2(b);
            result->g += a.g;
            return result;
        }
        b.g.norm();
    }
    result = new ::G2(a);
    result->g += b.g;
    return result;
}

G2 G2::operator+(const G2 &other) const {
    if (!d) return other;
    if (!other.d) return *this;
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    ::G2 *_el = addG2(_this, _other);
    if (_el->g.iszero()) {
        delete _el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(_el));
}

G2 G2::operator-(const G2 &other) const {
    if (!other.d) return *this;
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    if (!d) return G2(reinterpret_cast<void*>(new ::G2(-_other)));
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::G2 _m_other = -_other;
    ::G2 *_el = addG2(_this, _m_other);
    if (_el->g.iszero()) {
        delete _el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(_el));
}

G2 &G2::operator+=(const G2 &other) {
    if (!other.d) return *this;
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::G2 *_el = addG2(_this, _other);
    if (_el->g.iszero()) {
        delete _el;
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

G2 &G2::operator-=(const G2 &other) {
    if (!other.d) return *this;
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    if (!d) {
        d = new SharedData(reinterpret_cast<void*>(new ::G2(-_other)));
        return *this;
    }
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::G2 m_other = -_other;
    ::G2 *_el = addG2(_this, m_other);
    if (_el->g.iszero()) {
        delete _el;
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

G2 &G2::operator*=(const Fp &other) {
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((!d) || (other.d == Fp::one) || (_other.isone()))
        return *this;
    if ((other.d == Fp::zero) || _other.iszero()) {
        deref();
        d = NULL;
        return *this;
    }
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    void *_el = reinterpret_cast<void*>(
                new ::G2(pfc->mult(_this, _other)));
    if (d->c) {
        --d->c;
        d = new SharedData(_el);
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        d->p = _el;
    }
    return *this;
}

G2 G2::operator*(const Fp &other) const {
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((!d) || (other.d == Fp::one) || _other.isone()) return *this;
    if ((other.d == Fp::zero) || _other.iszero()) return G2();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    return G2(reinterpret_cast<void*>(new ::G2(pfc->mult(_this, _other))));
}

G2 operator*(const Fp &m, const G2 &g) {
    const ::G2 &_g = *reinterpret_cast< ::G2* >(g.d->p);
    const ::Big &_m = *reinterpret_cast< ::Big* >(m.d->p);
    if ((!g.d) || (m.d == Fp::one) || _m.isone()) return g;
    if ((m.d == Fp::zero) || _m.iszero()) return G2();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    return G2(reinterpret_cast<void*>(new ::G2(pfc->mult(_g, _m))));
}

bool G2::operator==(const G2 &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    // Note: const keyword simply missing in ::G2::operator==
    return (const_cast< ::G2&>(_this) == const_cast< ::G2&>(_other));
}

void G2::getData(char *data, bool compressed) const {
    if (!d) {
        if (compressed) {
            *data = NULL_ELEMENT_BYTE_VALUE;
        } else {
            ::Big b;
            to_binary(b, big_size, data, TRUE);
            to_binary(b, big_size, data += big_size, TRUE);
            to_binary(b, big_size, data += big_size, TRUE);
            to_binary(b, big_size, data + big_size, TRUE);
        }
        return;
    }
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::ZZn2 x, y;
    _this.g.get(x, y);
    ::Big b1, b2;
    if (compressed) {
        y.get(b1);
        *data = (char) (b1.get(1) & 1);
        x.get(b1, b2);
        to_binary(b1, big_size, ++data, TRUE);
        to_binary(b2, big_size, data + big_size, TRUE);
    } else {
        x.get(b1, b2);
        to_binary(b1, big_size, data, TRUE);
        to_binary(b2, big_size, data += big_size, TRUE);
        y.get(b1, b2);
        to_binary(b1, big_size, data += big_size, TRUE);
        to_binary(b2, big_size, data + big_size, TRUE);
    }
}

std::ostream &operator<<(std::ostream &stream, const G2 &el) {
    char lsb;
    if (!el.d) {
        lsb = NULL_ELEMENT_BYTE_VALUE;
        stream.write(&lsb, 1);
        return stream;
    }
    const ::G2 &_el = *reinterpret_cast< ::G2* >(el.d->p);
    ::ZZn2 x, y;
    _el.g.get(x, y);
    ::Big b1, b2;
    y.get(b1);
    lsb = (char) (b1.get(1) & 1);
    stream.write(&lsb, 1);
    x.get(b1, b2);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
#else
    char *iostream_threads_buffer = new char[big_size];
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, G2 &el) {
    char lsb;
    stream.read(&lsb, 1);
    if (lsb == NULL_ELEMENT_BYTE_VALUE) {
        if (el.d) el.deref();
        return stream;
    }
    ::G2 *_el = new ::G2();
    ::Big b1, b2;
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(big_size, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(big_size, iostream_nothreads_buffer);
#else
    char *iostream_threads_buffer = new char[big_size];
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(big_size, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(big_size, iostream_threads_buffer);
    delete[] iostream_threads_buffer;
#endif
    ZZn2 x, y;
    x.set(b1, b2);
    _el->g.set(x);
    _el->g.get(x, y);
    y.get(b1);
    if ((b1.get(1) & 1) != (int) lsb)
        *_el = -(*_el);
    if (el.d) {
        if (el.d->c) {
            --el.d->c;
            el.d = new SharedData(reinterpret_cast<void*>(_el));
        } else {
            delete reinterpret_cast< ::G2* >(el.d->p);
            el.d->p = reinterpret_cast<void*>(_el);
        }
    } else {
        el.d = new SharedData(reinterpret_cast<void*>(_el));
    }
    return stream;
}

void G2::precomputeForMult() {
    if (!d) return;
    pfc->precomp_for_mult(*reinterpret_cast< ::G2* >(d->p));
}

int G2::saveMultPrecomputations(char *&data) {
    if (!d) {
        /* Dummy implementation */
        data = new char[1];
        *data = 0;
        return 1;
    }
    return reinterpret_cast< ::G2* >(d->p)->spill(data);
}

void G2::loadMultPrecomputations(char *data) {
    if (!d) {
        delete[] data;
        return;
    }
    ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    _this.restore(data);
    _this.mtbits = MR_ROUNDUP(bits(*pfc->ord),WINDOW_SIZE);
}

void G2::precomputeForPairing() {
    if (!d) return;
    pfc->precomp_for_pairing(*reinterpret_cast< ::G2* >(d->p));
}

int G2::savePairingPrecomputations(char *&data) {
    if (!d) {
        /* Dummy implementation */
        data = new char[1];
        *data = 0;
        return 1;
    }
    return pfc->spill(*reinterpret_cast< ::G2* >(d->p), data);
}

void G2::loadPairingPrecomputations(char *data) {
    if (!d) {
        delete[] data;
        return;
    }
    pfc->restore(data, *reinterpret_cast< ::G2* >(d->p));
}

G2 G2::getRand() {
    ::G2 *_el = new ::G2();
    pfc->random(*_el);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(_el->g.iszero())) {
        delete _el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(_el));
}

int G2::getDataLen(bool compressed) {
    if (compressed) {
        return (big_size << 1) + 1;
    } else {
        return big_size << 2;
    }
}

G2 G2::getValue(const char *data, bool compressed) {
    ::G2 *_el;
    // Note: const keyword simply missing in from_binary
    if (compressed) {
        int lsb = (int) *(data++);
        if (lsb == NULL_ELEMENT_BYTE_VALUE)
            return G2();
        _el = new ::G2();
        ::Big b1, b2;
        b1 = from_binary(big_size, const_cast<char*>(data));
        b2 = from_binary(big_size, const_cast<char*>(data + big_size));
        ZZn2 x, y;
        x.set(b1, b2);
        _el->g.set(x);
        _el->g.get(x, y);
        y.get(b1);
        if ((b1.get(1) & 1) != lsb)
            *_el = -(*_el);
    } else {
        _el = new ::G2();
        ZZn2 x, y;
        ::Big b1, b2;
        b1 = from_binary(big_size, const_cast<char*>(data));
        b2 = from_binary(big_size, const_cast<char*>(data += big_size));
        x.set(b1, b2);
        b1 = from_binary(big_size, const_cast<char*>(data += big_size));
        b2 = from_binary(big_size, const_cast<char*>(data + big_size));
        y.set(b1, b2);
        _el->g.set(x, y);
        if (_el->g.iszero()) {
            delete _el;
            return G2();
        }
    }
    return G2(reinterpret_cast<void*>(_el));
}

/* Fast multiplication of A by q (for Trace-Zero group members only)
 * Calculate q*P. P(X,Y) -> P(X^p,Y^p))
 * Function from the MIRACL library */
void q_power_frobenius(::ECn2 &A, ::ZZn2 &F)
{
    ::ZZn2 x, y, z, w, r;
    A.get(x, y, z);
    w = F * F;
    r = F;
    x = w * conj(x);
    y = r * w * conj(y);
    z.conj();
    A.set(x, y, z);
}

/* Faster Hashing to G2 - Fuentes-Castaneda, Knapp and Rodriguez-Henriquez
 * Function from the MIRACL library */
inline void map(::ECn2& S, ::Big &x, ::ZZn2 &F)
{
    ::ECn2 T, K;
    T = S;
    T *= x; // one multiplication by x only
    T.norm();
    K = T + T;
    K += T;
    K.norm();
    q_power_frobenius(K, F);
    q_power_frobenius(S, F);
    q_power_frobenius(S, F);
    q_power_frobenius(S, F);
    S += T;
    S += K;
    q_power_frobenius(T, F);
    q_power_frobenius(T, F);
    S += T;
    S.norm();
}

void getG2FromBig(::G2 &el, ::Big &x0) {
    ZZn2 X;
    forever {
        ++x0;
        X.set(ZZn(1), ZZn(x0));
        /* TODO: Check that the LIKELY flag is not mistaken */
        if (LIKELY(el.g.set(X))) break;
    }
    map(el.g, *pfc->x, *pfc->frob);
}

G2 G2::fromHash(const char *data, int len) {
    ::G2 *_el = new ::G2();
    ::Big x0;
    hashToZZn(data, len, *pfc->mod, x0);
    getG2FromBig(*_el, x0);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(_el->g.iszero())) {
        delete _el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(_el));
}

G2 G2::fromHash(const char *hash) {
    ::G2 *el = new ::G2();
    ::Big x0;
    hashToZZn(hash, *pfc->mod, x0);
    getG2FromBig(*el, x0);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(el));
}

void G2::deref() {
    if (d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        delete d;
    }
}

GT GT::operator*(const GT &other) const {
    if (!d) return other;
    if (!other.d) return *this;
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    ::GT *_el = new ::GT(_this * _other);
    if (_el->g.isunity()) {
        delete _el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(_el));
}

GT GT::operator/(const GT &other) const {
    if (!other.d) return *this;
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    ::GT *_el;
    if (!d) {
        _el = new ::GT();
        _el->g = inverse(_other.g);
        return GT(reinterpret_cast<void*>(_el));
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    _el = new ::GT(_this);
    _el->g /= _other.g;
    if (_el->g.isunity()) {
        delete _el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(_el));
}

GT &GT::operator*=(const GT &other) {
    if (!other.d) return *this;
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    ::GT *_el = new ::GT(_this * _other);
    if (_el->g.isunity()) {
        delete _el;
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

GT &GT::operator/=(const GT &other) {
    if (!other.d) return *this;
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    ::GT *_el;
    if (!d) {
        _el = new ::GT();
        _el->g = inverse(_other.g);
        d = new SharedData(reinterpret_cast<void*>(_el));
        return *this;
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    _el = new ::GT(_this);
    _el->g /= _other.g;
    if (_el->g.isunity()) {
        delete _el;
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

GT &GT::operator^=(const Fp &other) {
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((!d) || (other.d == Fp::one) || (_other.isone()))
        return *this;
    if ((other.d == Fp::zero) || _other.iszero()) {
        deref();
        d = NULL;
        return *this;
    }
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    void *_el = reinterpret_cast<void*>(
                new ::GT(pfc->power(_this, _other)));
    if (d->c) {
        --d->c;
        d = new SharedData(_el);
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        d->p = _el;
    }
    return *this;
}

GT GT::operator^(const Fp &other) const {
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if ((!d) || (other.d == Fp::one) || _other.isone()) return *this;
    if ((other.d == Fp::zero) || _other.iszero()) return GT();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    return GT(reinterpret_cast<void*>(new ::GT(pfc->power(_this, _other))));
}

bool GT::operator==(const GT &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    return (_this == _other);
}

int GT::getDataLen() {
    return big_size * 12;
}

void GT::getData(char *data) const {
    if (!d) {
        ::Big b1 = ::Big(1);
        to_binary(b1, big_size, data, TRUE);
        b1 = ::Big();
        for (int i = 11; i-- > 0;) {
            to_binary(b1, big_size, data, TRUE);
            data += big_size;
        }
        return;
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    ::ZZn4 a, b, c;
    ::ZZn2 x, y;
    ::Big b1, b2;
    _this.g.get(a, b, c);
    a.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, data, TRUE);
    to_binary(b2, big_size, data += big_size, TRUE);
    y.get(b1, b2);
    to_binary(b1, big_size, data += big_size, TRUE);
    to_binary(b2, big_size, data += big_size, TRUE);
    b.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, data += big_size, TRUE);
    to_binary(b2, big_size, data += big_size, TRUE);
    y.get(b1, b2);
    to_binary(b1, big_size, data += big_size, TRUE);
    to_binary(b2, big_size, data += big_size, TRUE);
    c.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, data += big_size, TRUE);
    to_binary(b2, big_size, data += big_size, TRUE);
    y.get(b1, b2);
    to_binary(b1, big_size, data += big_size, TRUE);
    to_binary(b2, big_size, data + big_size, TRUE);
}

void write_zzn4(std::ostream &stream, const ::ZZn4 &v, char *buffer) {
    ::ZZn2 x, y;
    ::Big b1, b2;
    v.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, buffer, TRUE);
    stream.write(buffer, big_size);
    to_binary(b2, big_size, buffer, TRUE);
    stream.write(buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, buffer, TRUE);
    stream.write(buffer, big_size);
    to_binary(b2, big_size, buffer, TRUE);
    stream.write(buffer, big_size);
}

std::ostream &operator<<(std::ostream &stream, const GT &el) {
    if (!el.d) {
        ::Big b1 = ::Big(1);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
        to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
        stream.write(iostream_nothreads_buffer, big_size);
        b1 = ::Big();
        to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
        for (int i = 11; i-- > 0;)
            stream.write(iostream_nothreads_buffer, big_size);
#else
        char *iostream_threads_buffer = new char[big_size];
        to_binary(b1, big_size, iostream_threads_buffer, TRUE);
        stream.write(iostream_threads_buffer, big_size);
        b1 = ::Big();
        to_binary(b1, big_size, iostream_threads_buffer, TRUE);
        for (int i = 11; i-- > 0;)
            stream.write(iostream_threads_buffer, big_size);
        delete[] iostream_threads_buffer;
#endif
        return stream;
    }
    const ::GT &_el = *reinterpret_cast< ::GT* >(el.d->p);
    ::ZZn4 a, b, c;
    _el.g.get(a, b, c);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    write_zzn4(stream, a, iostream_nothreads_buffer);
    write_zzn4(stream, b, iostream_nothreads_buffer);
    write_zzn4(stream, c, iostream_nothreads_buffer);
#else
    char *iostream_threads_buffer = new char[big_size];
    write_zzn4(stream, a, iostream_threads_buffer);
    write_zzn4(stream, b, iostream_threads_buffer);
    write_zzn4(stream, c, iostream_threads_buffer);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

void read_zzn4(std::istream &stream, ::ZZn4 &v, char *buffer) {
    ::ZZn2 x, y;
    ::Big b1, b2;
    stream.read(buffer, big_size);
    b1 = from_binary(big_size, buffer);
    stream.read(buffer, big_size);
    b2 = from_binary(big_size, buffer);
    x.set(b1, b2);
    stream.read(buffer, big_size);
    b1 = from_binary(big_size, buffer);
    stream.read(buffer, big_size);
    b2 = from_binary(big_size, buffer);
    y.set(b1, b2);
    v.set(x, y);
}

std::istream &operator>>(std::istream &stream, GT &el) {
    ::GT *_el = new ::GT();
    ::ZZn4 a, b, c;
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    read_zzn4(stream, a, iostream_nothreads_buffer);
    read_zzn4(stream, b, iostream_nothreads_buffer);
    read_zzn4(stream, c, iostream_nothreads_buffer);
#else
    char *iostream_threads_buffer = new char[big_size];
    read_zzn4(stream, a, iostream_threads_buffer);
    read_zzn4(stream, b, iostream_threads_buffer);
    read_zzn4(stream, c, iostream_threads_buffer);
    delete[] iostream_threads_buffer;
#endif
    _el->g.set(a, b, c);
    if (_el->g.isunity()) {
        delete _el;
        if (el.d) {
            el.deref();
            el.d = NULL;
        }
    } else if (el.d) {
        if (el.d->c) {
            --el.d->c;
            el.d = new SharedData(reinterpret_cast<void*>(_el));
        } else {
            delete reinterpret_cast< ::GT* >(el.d->p);
            el.d->p = reinterpret_cast<void*>(_el);
        }
    } else {
        el.d = new SharedData(reinterpret_cast<void*>(_el));
    }
    return stream;
}

void GT::precomputeForPower() {
    if (!d) return;
    pfc->precomp_for_power(*reinterpret_cast< ::GT* >(d->p));
}

int GT::savePowerPrecomputations(char *&data) {
    if (!d) {
        /* Dummy implementation */
        data = new char[1];
        *data = 0;
        return 1;
    }
    return reinterpret_cast< ::GT* >(d->p)->spill(data);
}

void GT::loadPowerPrecomputations(char *data) {
    if (!d) {
        delete[] data;
        return;
    }
    ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    _this.restore(data);
    _this.etbits = MR_ROUNDUP(bits(*pfc->ord),WINDOW_SIZE);
}

GT GT::getRand() {
    ::Big b;
    pfc->random(b);
    ::GT *_el = new ::GT(pfc->power(*rndSeed, b));
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(_el->g.iszero())) {
        delete _el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(_el));
}

GT GT::getValue(const char *data) {
    ::GT *_el = new ::GT();
    // Note: const keyword simply missing in from_binary
    ::ZZn4 a, b, c;
    ::ZZn2 x, y;
    ::Big b1, b2;
    b1 = from_binary(big_size, const_cast<char*>(data));
    b2 = from_binary(big_size, const_cast<char*>(data += big_size));
    x.set(b1, b2);
    b1 = from_binary(big_size, const_cast<char*>(data += big_size));
    b2 = from_binary(big_size, const_cast<char*>(data += big_size));
    y.set(b1, b2);
    a.set(x, y);
    b1 = from_binary(big_size, const_cast<char*>(data += big_size));
    b2 = from_binary(big_size, const_cast<char*>(data += big_size));
    x.set(b1, b2);
    b1 = from_binary(big_size, const_cast<char*>(data += big_size));
    b2 = from_binary(big_size, const_cast<char*>(data += big_size));
    y.set(b1, b2);
    b.set(x, y);
    b1 = from_binary(big_size, const_cast<char*>(data += big_size));
    b2 = from_binary(big_size, const_cast<char*>(data += big_size));
    x.set(b1, b2);
    b1 = from_binary(big_size, const_cast<char*>(data += big_size));
    b2 = from_binary(big_size, const_cast<char*>(data + big_size));
    y.set(b1, b2);
    c.set(x, y);
    _el->g.set(a, b, c);
    if (_el->g.isunity()) {
        delete _el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(_el));
}

GT GT::pairing(const G1 &a, const G2 &b) {
    if ((!a.d) || (!b.d))
        return GT();
    const ::G1 &_a = *reinterpret_cast< ::G1* >(a.d->p);
    const ::G2 &_b = *reinterpret_cast< ::G2* >(b.d->p);
    ::GT *_el = new ::GT(pfc->pairing(_b, _a));
    return GT(reinterpret_cast<void*>(_el));
}

GT GT::pairing(const std::vector< std::pair<G1,G2> > &lst) {
    if (lst.empty()) return GT();
    ::G1 **a = new ::G1*[lst.size()];
    ::G2 **b = new ::G2*[lst.size()];
    int i = 0;
    for (const std::pair<G1,G2> &p : lst) {
        if ((!p.first.d) || (!p.second.d))
            continue;
        a[i] = reinterpret_cast< ::G1* >(p.first.d->p);
        b[i] = reinterpret_cast< ::G2* >(p.second.d->p);
        ++i;
    }
    if (!i) {
        delete[] a;
        delete[] b;
        return GT();
    }
    ::GT *_el = new ::GT(pfc->multi_pairing(i, b, a));
    delete[] a;
    delete[] b;
    if (_el->g.isunity()) {
        delete _el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(_el));
}

void GT::deref() {
    if (d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        delete d;
    }
}

} /* End of namespace pairings */

#elif defined(USE_PBC)
/* -------------------- PBC build -------------------- */

#ifdef PBC_STATIC
#include <pbc.h>
#else
#include <pbc/pbc.h>
#endif

#if HASH_LEN_BITS == 256
#include "sha256.h"
#elif HASH_LEN_BITS == 512
#include "sha512.h"
#else
#error Error: Invalid value of HASH_LEN_BITS (pairings)
#endif

namespace pairings {

static pairing_t p_params;

SharedData *Fp::zero = NULL, *Fp::one = NULL;

#ifdef GSNIZK_IOSTREAM_NOTHREADS
static char *iostream_nothreads_buffer;
#endif

void freeElement(void *ptr) {
#ifdef ZERO_MEMORY
    element_random(reinterpret_cast<element_ptr>(ptr));
#endif
    element_clear(reinterpret_cast<element_ptr>(ptr));
    delete reinterpret_cast<element_ptr>(ptr);
}

void initialize_pairings(int len, const char *data) {
    ASSERT(len >= 0, "Negative length");
    pairing_init_set_buf(p_params, data, len);
    ASSERT(!pairing_is_symmetric(p_params), "pairing is symmetric");
    element_ptr el = new element_s;
    element_init_Zr(el, p_params);
    element_set0(el);
    Fp::zero = new SharedData(reinterpret_cast<void*>(el));
    el = new element_s;
    element_init_Zr(el, p_params);
    element_set1(el);
    Fp::one = new SharedData(reinterpret_cast<void*>(el));
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    int buffer_size = pairing_length_in_bytes_GT(p_params), cmp;
    cmp = pairing_length_in_bytes_x_only_G2(p_params) + 1;
    if (UNLIKELY(cmp > buffer_size)) buffer_size = cmp;
    cmp = pairing_length_in_bytes_x_only_G1(p_params) + 1;
    if (UNLIKELY(cmp > buffer_size)) buffer_size = cmp;
    cmp = pairing_length_in_bytes_Zr(p_params);
    if (UNLIKELY(cmp > buffer_size)) buffer_size = cmp;
    iostream_nothreads_buffer = new char[buffer_size];
#endif
}

void terminate_pairings() {
    if (!Fp::zero) return;
    if (Fp::zero->c) {
        ASSERT(false, "Trailing references");
        --Fp::zero->c;
    } else {
        freeElement(Fp::zero->p);
        delete Fp::zero;
    }
    if (Fp::one->c) {
        ASSERT(false, "Trailing references");
        --Fp::one->c;
    } else {
        freeElement(Fp::one->p);
        delete Fp::one;
    }
    Fp::zero = NULL;
    pairing_clear(p_params);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    delete[] iostream_nothreads_buffer;
#endif
}

int getHashLen() {
    return HASH_LEN_BYTES;
}

void getHash(const char *data, int len, char *hash) {
#if HASH_LEN_BITS == 256
    hash_sha256(data, len, hash);
#elif HASH_LEN_BITS == 512
    hash_sha512(data, len, hash);
#else
#error Error: Invalid value of HASH_LEN_BITS (pairings)
#endif
}

bool hasPrecomputations() {
    return false;
}

bool iostream_nothreads() {
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    return true;
#else
    return false;
#endif
}

Fp::Fp(int i) {
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_set_si(_el, i);
    d = new SharedData(reinterpret_cast<void*>(_el));
}

Fp::Fp(unsigned long i) {
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    mpz_t v;
    mpz_init(v);
    mpz_set_ui(v, i);
    element_set_mpz(_el, v);
    mpz_clear(v);
    d = new SharedData(reinterpret_cast<void*>(_el));
}

Fp Fp::operator-() const {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    if ((d == zero) || element_is0(_this))
        return Fp(*this);
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_neg(_el, _this);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp Fp::operator+(const Fp &other) const {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((d == zero) || element_is0(_this)) return other;
    if ((other.d == zero) || element_is0(_other)) return *this;
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_add(_el, _this, _other);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp Fp::operator-(const Fp &other) const {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((other.d == zero) || element_is0(_other)) return *this;
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_sub(_el, _this, _other);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp &Fp::operator+=(const Fp &other) {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((other.d == zero) || element_is0(_other)) return *this;
    if ((d == zero) || element_is0(_this)) return (*this = other);
    if (d->c) {
        --d->c;
        element_ptr _el = new element_s;
        element_init_Zr(_el, p_params);
        element_add(_el, _this, _other);
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        element_add(_this, _this, _other);
    }
    return *this;
}

Fp &Fp::operator-=(const Fp &other) {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((other.d == zero) || element_is0(_other)) return *this;
    if (d->c) {
        --d->c;
        element_ptr _el = new element_s;
        element_init_Zr(_el, p_params);
        element_sub(_el, _this, _other);
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        element_sub(_this, _this, _other);
    }
    return *this;
}

Fp Fp::operator*(const Fp &other) const {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((d == zero) || (other.d == one) || element_is0(_this) ||
            element_is1(_other)) return *this;
    if ((d == one) || (other.d == zero) || element_is0(_other) ||
            element_is1(_this)) return other;
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_mul(_el, _this, _other);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp Fp::operator/(const Fp &other) const {
    ASSERT(!other.isNull(), "Divide by zero");
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((d == zero) || (other.d == one) || element_is0(_this) ||
            element_is1(_other)) return *this;
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_div(_el, _this, _other);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp &Fp::operator*=(const Fp &other) {
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((d == zero) || (other.d == one) || element_is0(_this) ||
            element_is1(_other)) return *this;
    if ((d == one) || (other.d == zero) || element_is0(_other) ||
            element_is1(_this)) return (*this = other);
    if (d->c) {
        --d->c;
        element_ptr _el = new element_s;
        element_init_Zr(_el, p_params);
        element_mul(_el, _this, _other);
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        element_mul(_this, _this, _other);
    }
    return *this;
}

Fp &Fp::operator/=(const Fp &other) {
    ASSERT(!other.isNull(), "Divide by zero");
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((d == zero) || (other.d == one) || element_is0(_this) ||
            element_is1(_other)) return *this;
    if (d->c) {
        --d->c;
        element_ptr _el = new element_s;
        element_init_Zr(_el, p_params);
        element_div(_el, _this, _other);
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        element_div(_this, _this, _other);
    }
    return *this;
}

bool Fp::operator==(const Fp &other) const {
    if (d == other.d) return true;
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    return !element_cmp(_this, _other);
}

void Fp::getData(char *data) const {
    element_to_bytes(reinterpret_cast<unsigned char*>(data),
                     reinterpret_cast<element_ptr>(d->p));
}

std::ostream &operator<<(std::ostream &stream, const Fp &el) {
    const element_ptr &_el = reinterpret_cast<element_ptr>(el.d->p);
    int size = pairing_length_in_bytes_Zr(p_params);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                           iostream_nothreads_buffer));
    stream.write(iostream_nothreads_buffer, size);
#else
    char *iostream_threads_buffer = new char[size];
    element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                           iostream_threads_buffer));
    stream.write(iostream_threads_buffer, size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, Fp &el) {
    int size = pairing_length_in_bytes_Zr(p_params);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.read(iostream_nothreads_buffer, size);
    if (el.d->c) {
        --el.d->c;
        element_ptr _el = new element_s;
        element_init_Zr(_el, p_params);
        element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                               iostream_nothreads_buffer));
        el.d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        const element_ptr &_el = reinterpret_cast<element_ptr>(el.d->p);
        element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                               iostream_nothreads_buffer));
    }
#else
    char *iostream_threads_buffer = new char[size];
    stream.read(iostream_threads_buffer, size);
    if (el.d->c) {
        --el.d->c;
        element_ptr _el = new element_s;
        element_init_Zr(_el, p_params);
        element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                               iostream_threads_buffer));
        el.d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        element_ptr &_el = reinterpret_cast<element_ptr>(el.d->p);
        element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                               iostream_threads_buffer));
    }
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

bool Fp::isNull() const {
    if (d == zero) return true;
    return element_is0(reinterpret_cast<element_ptr>(d->p));
}

bool Fp::isUnit() const {
    if (d == one) return true;
    return element_is1(reinterpret_cast<element_ptr>(d->p));
}

Fp Fp::getRand() {
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_random(_el);
    return Fp(reinterpret_cast<void*>(_el));
}

int Fp::getDataLen() {
    return pairing_length_in_bytes_Zr(p_params);
}

Fp Fp::getValue(const char *data) {
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    // Note: const keyword simply missing in element_from_bytes
    element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                           const_cast<char*>(data)));
    return Fp(reinterpret_cast<void*>(_el));
}

void element_from_unhashed_data(const char *data, int len, element_ptr v) {
    char s[HASH_LEN_BYTES];
#if HASH_LEN_BITS == 256
    hash_sha256(data, len, s);
#elif HASH_LEN_BITS == 512
    hash_sha512(data, len, s);
#else
#error Error: Invalid value of HASH_LEN_BITS (pairings)
#endif
    element_from_hash(v, s, HASH_LEN_BYTES);
}

Fp Fp::fromHash(const char *data, int len) {
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    element_from_unhashed_data(data, len, _el);
    return Fp(reinterpret_cast<void*>(_el));
}

Fp Fp::fromHash(const char *hash) {
    element_ptr _el = new element_s;
    element_init_Zr(_el, p_params);
    // Note: const keyword simply missing in element_from_hash
    element_from_hash(_el, const_cast<char*>(hash), HASH_LEN_BYTES);
    return Fp(reinterpret_cast<void*>(_el));
}

void Fp::deref() {
    if (d->c) {
        --d->c;
    } else {
        freeElement(d->p);
        delete d;
    }
}

G1 G1::operator-() const {
    if (!d) return G1();
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_neg(_el, _this);
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::operator+(const G1 &other) const {
    if (!d) return other;
    if (!other.d) return *this;
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_add(_el, _this, _other);
    if (element_is0(_el)) {
        freeElement(_el);
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::operator-(const G1 &other) const {
    if (!other.d) return *this;
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    if (!d) {
        element_neg(_el, _other);
        return G1(reinterpret_cast<void*>(_el));
    }
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    element_sub(_el, _this, _other);
    if (element_is0(_el)) {
        freeElement(_el);
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 &G1::operator+=(const G1 &other) {
    if (!other.d) return *this;
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_add(_el, _this, _other);
    if (element_is0(_el)) {
        freeElement(_el);
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        freeElement(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

G1 &G1::operator-=(const G1 &other) {
    if (!other.d) return *this;
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    if (!d) {
        element_neg(_el, _other);
        d = new SharedData(reinterpret_cast<void*>(_el));
        return *this;
    }
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    element_sub(_el, _this, _other);
    if (element_is0(_el)) {
        freeElement(_el);
        deref();
        d = NULL;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(_el));
    } else {
        freeElement(d->p);
        d->p = reinterpret_cast<void*>(_el);
    }
    return *this;
}

G1 &G1::operator*=(const Fp &other) {
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((!d) || (other.d == Fp::one) || element_is1(_other))
        return *this;
    if ((other.d == Fp::zero) || element_is0(_other)) {
        deref();
        d = NULL;
        return *this;
    }
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_mul_zn(_el, _this, _other);
    if (d->c) {
        --d->c;
        d = new SharedData(_el);
    } else {
        freeElement(d->p);
        d->p = _el;
    }
    return *this;
}

G1 G1::operator*(const Fp &other) const {
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    if ((!d) || (other.d == Fp::one) || element_is1(_other)) return *this;
    if ((other.d == Fp::zero) || element_is0(_other)) return G1();
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_mul_zn(_el, _this, _other);
    return G1(reinterpret_cast<void*>(_el));
}

G1 operator*(const Fp &m, const G1 &g) {
    const element_ptr &_m = reinterpret_cast<element_ptr>(m.d->p);
    if ((!g.d) || (m.d == Fp::one) || element_is1(_m)) return g;
    if ((m.d == Fp::zero) || element_is0(_m)) return G1();
    const element_ptr &_g = reinterpret_cast<element_ptr>(g.d->p);
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_mul_zn(_el, _g, _m);
    return G1(reinterpret_cast<void*>(_el));
}

bool G1::operator==(const G1 &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    const element_ptr &_other = reinterpret_cast<element_ptr>(other.d->p);
    return !element_cmp(_this, _other);
}

void G1::getData(char *data, bool compressed) const {
    if (!d) {
        if (compressed) {
            *data = NULL_ELEMENT_BYTE_VALUE;
        } else {
            element_t zero;
            element_init_G1(zero, p_params);
            element_set0(zero);
            element_to_bytes(reinterpret_cast<unsigned char*>(data), zero);
            element_clear(zero);
        }
        return;
    }
    const element_ptr &_this = reinterpret_cast<element_ptr>(d->p);
    if (compressed) {
        *data = (element_sign(element_y(_this)) > 0) ? 1 : 0;
        element_to_bytes_x_only(reinterpret_cast<unsigned char*>(data + 1),
                                _this);
    } else {
        element_to_bytes(reinterpret_cast<unsigned char*>(data), _this);
    }
}

std::ostream &operator<<(std::ostream &stream, const G1 &el) {
    char lsb;
    if (!el.d) {
        lsb = NULL_ELEMENT_BYTE_VALUE;
        stream.write(&lsb, 1);
        return stream;
    }
    const element_ptr &_el = reinterpret_cast<element_ptr>(el.d->p);
    lsb = (element_sign(element_y(_el)) > 0) ? 1 : 0;
    stream.write(&lsb, 1);
    int size = pairing_length_in_bytes_x_only_G1(p_params);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    element_to_bytes_x_only(
        reinterpret_cast<unsigned char*>(iostream_nothreads_buffer), _el);
    stream.write(iostream_nothreads_buffer, size);
#else
    char *iostream_threads_buffer = new char[size];
    element_to_bytes_x_only(
        reinterpret_cast<unsigned char*>(iostream_threads_buffer), _el);
    stream.write(iostream_threads_buffer, size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, G1 &el) {
    char lsb;
    stream.read(&lsb, 1);
    if (lsb == NULL_ELEMENT_BYTE_VALUE) {
        if (el.d) el.deref();
        return stream;
    }
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    int size = pairing_length_in_bytes_x_only_G1(p_params);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.read(iostream_nothreads_buffer, size);
    element_from_bytes_x_only(_el, reinterpret_cast<unsigned char*>(
                                  iostream_nothreads_buffer));
#else
    char *iostream_threads_buffer = new char[size];
    stream.read(iostream_threads_buffer, size);
    element_from_bytes_x_only(_el, reinterpret_cast<unsigned char*>(
                                  iostream_threads_buffer));
    delete[] iostream_threads_buffer;
#endif
    if (lsb != ((element_sign(element_y(_el)) > 0) ? 1 : 0))
        element_neg(_el, _el);
    if (el.d) {
        if (el.d->c) {
            --el.d->c;
            el.d = new SharedData(reinterpret_cast<void*>(_el));
        } else {
            freeElement(el.d->p);
            el.d->p = reinterpret_cast<void*>(_el);
        }
    } else {
        el.d = new SharedData(reinterpret_cast<void*>(_el));
    }
    return stream;
}

void G1::precomputeForMult() {}

int G1::saveMultPrecomputations(char *&data) {
    /* Note: A good application should not call this function
     * if hasPrecomputations() returns false, but we still prefer
     * returning dummy values than throwing an exception for
     * ease-of-use reasons. */
    data = new char[1];
    *data = 0;
    return 1;
}

void G1::loadMultPrecomputations(char *data) {
    delete[] data;
}

G1 G1::getRand() {
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_random(_el);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(element_is0(_el))) {
        freeElement(_el);
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

int G1::getDataLen(bool compressed) {
    if (compressed) {
        return pairing_length_in_bytes_x_only_G1(p_params) + 1;
    } else {
        return pairing_length_in_bytes_G1(p_params);
    }
}

G1 G1::getValue(const char *data, bool compressed) {
    // Note: const keyword simply missing in element_from_bytes_[...]
    element_ptr _el;
    if (compressed) {
        int lsb = (int) *(data++);
        if (lsb == NULL_ELEMENT_BYTE_VALUE)
            return G1();
        _el = new element_s;
        element_init_G1(_el, p_params);
        element_from_bytes_x_only(_el, reinterpret_cast<unsigned char*>(
                                      const_cast<char*>(data)));
        if (lsb != ((element_sign(element_y(_el)) > 0) ? 1 : 0))
            element_neg(_el, _el);
    } else {
        _el = new element_s;
        element_init_G1(_el, p_params);
        element_from_bytes(_el, reinterpret_cast<unsigned char*>(
                                      const_cast<char*>(data)));
        if (element_is0(_el)) {
            freeElement(_el);
            return G1();
        }
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::fromHash(const char *data, int len) {
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    element_from_unhashed_data(data, len, _el);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(element_is0(_el))) {
        freeElement(_el);
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

G1 G1::fromHash(const char *hash) {
    element_ptr _el = new element_s;
    element_init_G1(_el, p_params);
    // Note: const keyword simply missing in element_from_hash
    element_from_hash(_el, const_cast<char*>(hash), HASH_LEN_BYTES);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(element_is0(_el))) {
        freeElement(_el);
        return G1();
    }
    return G1(reinterpret_cast<void*>(_el));
}

void G1::deref() {
    if (d->c) {
        --d->c;
    } else {
        freeElement(d->p);
        delete d;
    }
}

// TODO

//#error PBC implementation has not been done yet!

} /* End of namespace pairings */

#else
#error Neither MIRACL nor PBC have been specified for this build
#endif /* Type of build */
