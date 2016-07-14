/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "pairings.h"

#if !defined(LIB_COMPILATION)
#error LIB_COMPILATION should be defined for the compilation of this library.
#endif

#if defined(USE_MIRACL)
/* -------------------- MIRACL build -------------------- */

#define MR_PAIRING_BN // AES-128 or AES-192 security

#ifndef AES_SECURITY
#define AES_SECURITY 128
//#define AES_SECURITY 192
#endif /* Not AES_SECURITY */

#ifndef HASH_LEN_BITS
#define HASH_LEN_BITS 256
//#define HASH_LEN_BITS 512
#endif

#define HASH_LEN_BYTES  (HASH_LEN_BITS / 8)

#define BUFFER_SIZE 128

#include "pairing_3.h"

#include <cstring>
#include <ctime>

#define MIRACL_BYTES (MIRACL / 8)

namespace pairings {

static PFC *pfc;
static int big_size;

SharedData *Fp::zero = 0, *Fp::one = 0;


#ifdef GSNIZK_IOSTREAM_NOTHREADS
static char *iostream_nothreads_buffer;
#endif

template <typename T> inline const T &min(const T &a, const T &b) { return (a < b) ? a : b; }
template <typename T> inline const T &max(const T &a, const T &b) { return (a < b) ? b : a; }

// Note: The following is used more for an indication and hint for the readers
// rather than an efficiency improvement.
#ifdef __GNUC__
#define LIKELY(x)       __builtin_expect(!!(x), 1)
#define UNLIKELY(x)     __builtin_expect(!!(x), 0)
#define NB_BITS(x)      ((sizeof(unsigned long) * 8) - __builtin_clzl((unsigned long) (x)))
#else
#define LIKELY(x)       (x)
#define UNLIKELY(x)     (x)
inline int count_bits(unsigned long x) {
    int r = 1;
    while (x >>= 1) ++r;
    return r;
}
#define NB_BITS(x)      count_bits((unsigned long) (x))
#endif /* __GNUC__ */

/* Get the number of bits in the big number n */
/* (Note: unused)
int getNBits(const Big &n) {
    int i = n.len();
    while ((--i >= 0) && (!n[i]));
    if (i < 0) return 0;
    return NB_BITS(n[i]) + i * sizeof(mr_small);
}
// */

static ::GT *rndSeed = 0;

void initialize_pairings(int len, const char *rndData) {
    csprng *rnd = new csprng;
    // Note: Sorry about the following const_cast, const keyword simply missing in strong_init
    strong_init(rnd, len, const_cast<char*>(rndData), (unsigned int) clock());
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
    rndSeed = 0;
    if (Fp::zero->c) {
        --Fp::zero->c;
    } else {
        delete reinterpret_cast< ::Big* >(Fp::zero->p);
        delete Fp::zero;
    }
    if (Fp::one->c) {
        --Fp::one->c;
    } else {
        delete reinterpret_cast< ::Big* >(Fp::one->p);
        delete Fp::zero;
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

Fp::Fp(int i) : d(new SharedData(reinterpret_cast<void*>(i >= 0 ? (new ::Big(i)) :
        (new ::Big(*pfc->ord + Big(i)))))) {}

Fp::Fp(unsigned long i) : d(new SharedData(reinterpret_cast<void*>(new ::Big(i)))) {}

Fp Fp::operator-() const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    if (_this.iszero())
        return Fp(*this);
    return Fp(reinterpret_cast<void*>(new ::Big((*pfc->ord) - _this)));
}

Fp Fp::operator+(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big((_this + _other) % (*pfc->ord))));
}

Fp Fp::operator-(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big(((*pfc->ord + _this) - _other) % (*pfc->ord))));
}

Fp &Fp::operator+=(const Fp &other) {
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if (d->c) {
        --d->c;
        const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        d = new SharedData(reinterpret_cast<void*>(new ::Big((_this + _other) % (*pfc->ord))));
    } else {
        ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        _this += _other;
        _this %= *pfc->ord;
    }
    return *this;
}

Fp &Fp::operator-=(const Fp &other) {
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if (d->c) {
        --d->c;
        const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        d = new SharedData(reinterpret_cast<void*>(new ::Big(((*pfc->ord + _this) - _other) % (*pfc->ord))));
    } else {
        ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        _this += *pfc->ord;
        _this -= _other;
        _this %= *pfc->ord;
    }
    return *this;
}

Fp Fp::operator*(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big(modmult(_this, _other, *pfc->ord))));
}

Fp Fp::operator/(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big(moddiv(_this, _other, *pfc->ord))));
}

Fp &Fp::operator*=(const Fp &other) {
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if (d->c) {
        --d->c;
        const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        d = new SharedData(reinterpret_cast<void*>(new ::Big(modmult(_this, _other, *pfc->ord))));
    } else {
        ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        _this = modmult(_this, _other, *pfc->ord);
    }
    return *this;
}

Fp &Fp::operator/=(const Fp &other) {
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    if (d->c) {
        --d->c;
        const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        d = new SharedData(reinterpret_cast<void*>(new ::Big(moddiv(_this, _other, *pfc->ord))));
    } else {
        ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
        _this = moddiv(_this, _other, *pfc->ord);
    }
    return *this;
}

bool Fp::operator==(const Fp &other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return _this == _other;
}

int Fp::getDataLen() const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    return _this.len() * MIRACL_BYTES;
}

void Fp::getData(char *data) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    to_binary(_this, _this.len() * MIRACL_BYTES, data, TRUE);
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
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    return _this.iszero();
}

Fp Fp::getUnit() {
    return Fp(one);
}

Fp Fp::getRand() {
    return Fp(reinterpret_cast<void*>(new ::Big(strong_rand(pfc->RNG, *pfc->ord))));
}

Fp Fp::getValue(const char *data, int len) {
    // Note: Sorry about the following const_cast, const keyword simply missing in from_binary
    return Fp(reinterpret_cast<void*>(new ::Big(from_binary(len, const_cast<char*>(data)))));
}

Fp Fp::fromHash(const char *data, int len) {
    ::Big *result = new ::Big();
    hashToZZn(data, len, *pfc->ord, *result);
    return Fp(reinterpret_cast<void*>(result));
}

Fp Fp::fromHash(const char *hash) {
    ::Big *result = new ::Big();
    hashToZZn(hash, *pfc->ord, *result);
    return Fp(reinterpret_cast<void*>(result));
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
    ::G1 *result = new ::G1(_this + _other);
    if (result->g.iszero()) {
        delete result;
        return G1();
    }
    return G1(reinterpret_cast<void*>(result));
}

G1 G1::operator-(const G1 &other) const {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) return G1(reinterpret_cast<void*>(new ::G1(-_other)));
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    ::G1 *result = new ::G1(_this + (-_other));
    if (result->g.iszero()) {
        delete result;
        return G1();
    }
    return G1(reinterpret_cast<void*>(result));
}

G1 &G1::operator+=(const G1 &other) {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    ::G1 *result = new ::G1(_this + _other);
    if (result->g.iszero()) {
        delete result;
        deref();
        d = 0;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(result));
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = reinterpret_cast<void*>(result);
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
    ::G1 *result = new ::G1(_this + (-_other));
    if (result->g.iszero()) {
        delete result;
        deref();
        d = 0;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(result));
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = reinterpret_cast<void*>(result);
    }
    return *this;
}

G1 &G1::operator*=(const Fp &other) {
    if (!d) return *this;
    if (other.isNull()) {
        deref();
        d = 0;
        return *this;
    }
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    void *result = reinterpret_cast<void*>(new ::G1(pfc->mult(_this, _other)));
    if (d->c) {
        --d->c;
        d = new SharedData(result);
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = result;
    }
    return *this;
}

G1 G1::operator*(const Fp &other) const {
    if (!d) return *this;
    if (other.isNull()) return G1();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return G1(reinterpret_cast<void*>(new ::G1(pfc->mult(_this, _other))));
}

G1 operator*(const Fp &m, const G1 &g) {
    if (!g.d) return g;
    if (m.isNull()) return G1();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::G1 &_g = *reinterpret_cast< ::G1* >(g.d->p);
    const ::Big &_m = *reinterpret_cast< ::Big* >(m.d->p);
    return G1(reinterpret_cast<void*>(new ::G1(pfc->mult(_g, _m))));
}

bool G1::operator==(const G1 &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    return (_this == _other);
}

int G1::getDataLen(bool compressed) const {
    if (!d) return 0;
    if (compressed) {
        return big_size + 1;
    } else {
        return big_size << 1;
    }
}

void G1::getData(char *data, bool compressed) const {
    if (!d) return;
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
        to_binary(y, big_size, data + len, TRUE);
    }
}

std::ostream &operator<<(std::ostream &stream, const G1 &el) {
    char lsb;
    if (!el.d) {
        lsb = 0xFF;
        stream.write(&lsb, 1);
        return stream;
    }
    const ::G1 &_el = *reinterpret_cast< ::G1* >(el.d->p);
    ::Big x;
    lsb = (char) _el.g.get(x);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.write(&lsb, 1);
    to_binary(x, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
#else
    char *iostream_threads_buffer = new char[big_size];
    stream.write(&lsb, 1);
    to_binary(x, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, G1 &el) {
    ::G1 *_el = new ::G1();
    char lsb;
    stream.read(&lsb, 1);
    if (lsb == 0xFF) {
        if (el.d) el.deref();
        return stream;
    }
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
            el->d = new SharedData(reinterpret_cast<void*>(_el));
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
    ::G1 *el = new ::G1();
    pfc->random(*el);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(el));
}

G1 G1::getValue(const char *data, int len, bool compressed) {
    if (!len) return G1();
    ::G1 *el = new ::G1();
    // Note: Sorry about the following const_cast-s, const keyword simply missing in from_binary
    if (compressed) {
        int lsb = (int) *(data++);
        el->g.set(from_binary(len - 1, const_cast<char*>(data)), lsb);
    } else {
        if (len & 1)
            throw "pairings::G1::getValue: Unexpected data length";
        len >>= 1;
        el->g.set(from_binary(len, const_cast<char*>(data)), from_binary(len, const_cast<char*>(data + len)));
    }
    return G1(reinterpret_cast<void*>(el));
}

G1 G1::fromHash(const char *data, int len) {
    ::G1 *el = new ::G1();
    ::Big x0;
    hashToZZn(data, len, *pfc->mod, x0);
    while (!el->g.set(x0, x0)) ++x0;
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(el));
}

G1 G1::fromHash(const char *hash) {
    ::G1 *el = new ::G1();
    ::Big x0;
    hashToZZn(hash, *pfc->mod, x0);
    while (!el->g.set(x0, x0)) ++x0;
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return G1();
    }
    return G1(reinterpret_cast<void*>(el));
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
    // Note: Sorry about the following const_cast-s, const keyword simply missing in ECn2::type
    if (const_cast< ::ECn2& >(b.g).type() == MR_EPOINT_GENERAL) {
        if ((const_cast< ::ECn2& >(a.g).type() != MR_EPOINT_GENERAL) || (&a == &b)) {
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
    ::G2 *result = addG2(_this, _other);
    if (result->g.iszero()) {
        delete result;
        return G2();
    }
    return G2(reinterpret_cast<void*>(result));
}

G2 G2::operator-(const G2 &other) const {
    if (!other.d) return *this;
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    if (!d) return G2(reinterpret_cast<void*>(new ::G2(-_other)));
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::G2 m_other = -_other;
    ::G2 *result = addG2(_this, m_other);
    if (result->g.iszero()) {
        delete result;
        return G2();
    }
    return G2(reinterpret_cast<void*>(result));
}

G2 &G2::operator+=(const G2 &other) {
    if (!other.d) return *this;
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::G2 *result = addG2(_this, _other);
    if (result->g.iszero()) {
        delete result;
        deref();
        d = 0;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(result));
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        d->p = reinterpret_cast<void*>(result);
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
    ::G2 *result = addG2(_this, m_other);
    if (result->g.iszero()) {
        delete result;
        deref();
        d = 0;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(result));
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        d->p = reinterpret_cast<void*>(result);
    }
    return *this;
}

G2 &G2::operator*=(const Fp &other) {
    if (!d) return *this;
    if (other.isNull()) {
        deref();
        d = 0;
        return *this;
    }
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    void *result = reinterpret_cast<void*>(new ::G2(pfc->mult(_this, _other)));
    if (d->c) {
        --d->c;
        d = new SharedData(result);
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        d->p = result;
    }
    return *this;
}

G2 G2::operator*(const Fp &other) const {
    if (!d) return *this;
    if (other.isNull()) return G2();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return G2(reinterpret_cast<void*>(new ::G2(pfc->mult(_this, _other))));
}

G2 operator*(const Fp &m, const G2 &g) {
    if (!g.d) return g;
    if (m.isNull()) return G2();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::G2 &_g = *reinterpret_cast< ::G2* >(g.d->p);
    const ::Big &_m = *reinterpret_cast< ::Big* >(m.d->p);
    return G2(reinterpret_cast<void*>(new ::G2(pfc->mult(_g, _m))));
}

bool G2::operator==(const G2 &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    const ::G2 &_other = *reinterpret_cast< ::G2* >(other.d->p);
    // Note: Sorry about the following const_cast-s, const keyword simply missing in ::G2::operator==
    return (const_cast< ::G2&>(_this) == const_cast< ::G2&>(_other));
}

int G2::getDataLen(bool compressed) const {
    if (!d) return 0;
    if (compressed) {
        return (big_size << 1) + 1;
    } else {
        return big_size << 2;
    }
}

void G2::getData(char *data, bool compressed) const {
    if (!d) return;
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    ::ZZn2 x, y;
    _this.g.get(x, y);
    if (compressed) {
        ::Big b1, b2;
        y.get(b1);
        *data = (char) (b1.get(1) & 1);
        x.get(b1, b2);
        to_binary(b1, big_size, ++data, TRUE);
        to_binary(b2, big_size, data + big_size, TRUE);
    } else {
        ::Big b1, b2, b3, b4;
        x.get(b1, b2);
        y.get(b3, b4);
        to_binary(b1, big_size, data, TRUE);
        to_binary(b2, big_size, data += big_size, TRUE);
        to_binary(b3, big_size, data += big_size, TRUE);
        to_binary(b4, big_size, data + big_size, TRUE);
    }
}

std::ostream &operator<<(std::ostream &stream, const G2 &el) {
    char lsb;
    if (!el.d) {
        lsb = 0xFF;
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
    ::G2 *_el = new ::G2();
    char lsb;
    stream.read(&lsb, 1);
    if (lsb == 0xFF) {
        if (el.d) el.deref();
        return stream;
    }
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
    el->g.set(x);
    el->g.get(x, y);
    y.get(b1);
    if ((b1.get(1) & 1) != (int) lsb)
        *el = -(*el);
    if (el.d) {
        if (el.d->c) {
            --el.d->c;
            el->d = new SharedData(reinterpret_cast<void*>(_el));
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
    ::G2 *el = new ::G2();
    pfc->random(*el);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(el));
}

G2 G2::getValue(const char *data, int len, bool compressed) {
    if (!len) return G2();
    ::G2 *el = new ::G2();
    // Note: Sorry about the following const_cast-s, const keyword simply missing in from_binary
    if (compressed) {
        int lsb = (int) *(data++);
        if ((--len) & 1)
            throw "pairings::G2::getValue: Unexpected data length";
        len >>= 1;
        ::Big b1, b2;
        b1 = from_binary(len, const_cast<char*>(data));
        b2 = from_binary(len, const_cast<char*>(data + len));
        ZZn2 x, y;
        x.set(b1, b2);
        el->g.set(x);
        el->g.get(x, y);
        y.get(b1);
        if ((b1.get(1) & 1) != lsb)
            *el = -(*el);
    } else {
        if (len & 3)
            throw "pairings::G2::getValue: Unexpected data length";
        len >>= 2;
        ::Big b1, b2, b3, b4;
        b1 = from_binary(len, const_cast<char*>(data));
        b2 = from_binary(len, const_cast<char*>(data += len));
        b3 = from_binary(len, const_cast<char*>(data += len));
        b4 = from_binary(len, const_cast<char*>(data + len));
        ZZn2 x, y;
        x.set(b1, b2);
        y.set(b3, b4);
        el->g.set(x, y);
    }
    return G2(reinterpret_cast<void*>(el));
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
    ::G2 *el = new ::G2();
    ::Big x0;
    hashToZZn(data, len, *pfc->mod, x0);
    getG2FromBig(*el, x0);
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return G2();
    }
    return G2(reinterpret_cast<void*>(el));
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
    ::GT *result = new ::GT(_this * _other);
    if (result->g.isunity()) {
        delete result;
        return GT();
    }
    return GT(reinterpret_cast<void*>(result));
}

GT GT::operator/(const GT &other) const {
    if (!other.d) return *this;
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    ::GT *result;
    if (!d) {
        result = new ::GT();
        result->g = inverse(_other.g);
        return GT(reinterpret_cast<void*>(result));
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    result = new ::GT(_this);
    result->g /= _other.g;
    if (result->g.isunity()) {
        delete result;
        return GT();
    }
    return GT(reinterpret_cast<void*>(result));
}

GT &GT::operator*=(const GT &other) {
    if (!other.d) return *this;
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    ::GT *result = new ::GT(_this * _other);
    if (result->g.isunity()) {
        delete result;
        deref();
        d = 0;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(result));
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        d->p = reinterpret_cast<void*>(result);
    }
    return *this;
}

GT &GT::operator/=(const GT &other) {
    if (!other.d) return *this;
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    ::GT *result;
    if (!d) {
        result = new ::GT();
        result->g = inverse(_other.g);
        d = new SharedData(reinterpret_cast<void*>(result));
        return *this;
    }
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    result = new ::GT(_this);
    result->g /= _other.g;
    if (result->g.isunity()) {
        delete result;
        deref();
        d = 0;
        return *this;
    }
    if (d->c) {
        --d->c;
        d = new SharedData(reinterpret_cast<void*>(result));
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        d->p = reinterpret_cast<void*>(result);
    }
    return *this;
}

GT &GT::operator^=(const Fp &other) {
    if (!d) return *this;
    if (other.isNull()) {
        deref();
        d = 0;
        return *this;
    }
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    void *result = reinterpret_cast<void*>(new ::GT(pfc->power(_this, _other)));
    if (d->c) {
        --d->c;
        d = new SharedData(result);
    } else {
        delete reinterpret_cast< ::GT* >(d->p);
        d->p = result;
    }
    return *this;
}

GT GT::operator^(const Fp &other) const {
    if (!d) return *this;
    if (other.isNull()) return GT();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return GT(reinterpret_cast<void*>(new ::GT(pfc->power(_this, _other))));
}

bool GT::operator==(const GT &other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    return (_this == _other);
}

int GT::getDataLen() const {
    if (!d) return 0;
    return big_size * 12;
}

void GT::getData(char *data) const {
    if (!d) return;
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

std::ostream &operator<<(std::ostream &stream, const G2 &el) {
    char lsb;
    if (!el.d) {
        lsb = 0xFF;
        stream.write(&lsb, 1);
        return stream;
    }
    lsb = 0;
    stream.write(&lsb, 1);
    const ::GT &_el = *reinterpret_cast< ::GT* >(el.d->p);
    ::ZZn4 a, b, c;
    ::ZZn2 x, y;
    ::Big b1, b2;
    _el.g.get(a, b, c);
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    a.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    b.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    c.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
    to_binary(b2, big_size, iostream_nothreads_buffer, TRUE);
    stream.write(iostream_nothreads_buffer, big_size);
#else
    char *iostream_threads_buffer = new char[big_size];
    a.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    b.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    c.get(x, y);
    x.get(b1, b2);
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    y.get(b1, b2);
    to_binary(b1, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    to_binary(b2, big_size, iostream_threads_buffer, TRUE);
    stream.write(iostream_threads_buffer, big_size);
    delete[] iostream_threads_buffer;
#endif
    return stream;
}

std::istream &operator>>(std::istream &stream, G2 &el) {
    ::GT *_el = new ::GT();
    char lsb;
    stream.read(&lsb, 1);
    if (lsb == 0xFF) {
        if (el.d) el.deref();
        return stream;
    }
    ::ZZn4 a, b, c;
    ::ZZn2 x, y;
    ::Big b1, b2;
#ifdef GSNIZK_IOSTREAM_NOTHREADS
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(len, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(len, iostream_nothreads_buffer);
    x.set(b1, b2);
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(len, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(len, iostream_nothreads_buffer);
    y.set(b1, b2);
    a.set(x, y);
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(len, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(len, iostream_nothreads_buffer);
    x.set(b1, b2);
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(len, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(len, iostream_nothreads_buffer);
    y.set(b1, b2);
    b.set(x, y);
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(len, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(len, iostream_nothreads_buffer);
    x.set(b1, b2);
    stream.read(iostream_nothreads_buffer, big_size);
    b1 = from_binary(len, iostream_nothreads_buffer);
    stream.read(iostream_nothreads_buffer, big_size);
    b2 = from_binary(len, iostream_nothreads_buffer);
    y.set(b1, b2);
    c.set(x, y);
#else
    char *iostream_threads_buffer = new char[big_size];
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(len, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(len, iostream_threads_buffer);
    x.set(b1, b2);
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(len, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(len, iostream_threads_buffer);
    y.set(b1, b2);
    a.set(x, y);
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(len, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(len, iostream_threads_buffer);
    x.set(b1, b2);
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(len, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(len, iostream_threads_buffer);
    y.set(b1, b2);
    b.set(x, y);
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(len, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(len, iostream_threads_buffer);
    x.set(b1, b2);
    stream.read(iostream_threads_buffer, big_size);
    b1 = from_binary(len, iostream_threads_buffer);
    stream.read(iostream_threads_buffer, big_size);
    b2 = from_binary(len, iostream_threads_buffer);
    y.set(b1, b2);
    c.set(x, y);
    delete[] iostream_threads_buffer;
#endif
    el->g.set(a, b, c);
    if (el.d) {
        if (el.d->c) {
            --el.d->c;
            el->d = new SharedData(reinterpret_cast<void*>(_el));
        } else {
            delete reinterpret_cast< ::G2* >(el.d->p);
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
    ::GT *el = new ::GT(pfc->power(*rndSeed, b));
    // Following is very unlikely, but we still want to handle it
    if (UNLIKELY(el->g.iszero())) {
        delete el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(el));
}

GT GT::getValue(const char *data, int len) {
    if (!len) return GT();
    ::GT *el = new ::GT();
    // Note: Sorry about the following const_cast-s, const keyword simply missing in from_binary
    if (len % 12)
        throw "pairings::GT::getValue: Unexpected data length";
    len /= 12;
    ::ZZn4 a, b, c;
    ::ZZn2 x, y;
    ::Big b1, b2;
    b1 = from_binary(len, const_cast<char*>(data));
    b2 = from_binary(len, const_cast<char*>(data += len));
    x.set(b1, b2);
    b1 = from_binary(len, const_cast<char*>(data += len));
    b2 = from_binary(len, const_cast<char*>(data += len));
    y.set(b1, b2);
    a.set(x, y);
    b1 = from_binary(len, const_cast<char*>(data += len));
    b2 = from_binary(len, const_cast<char*>(data += len));
    x.set(b1, b2);
    b1 = from_binary(len, const_cast<char*>(data += len));
    b2 = from_binary(len, const_cast<char*>(data += len));
    y.set(b1, b2);
    b.set(x, y);
    b1 = from_binary(len, const_cast<char*>(data += len));
    b2 = from_binary(len, const_cast<char*>(data += len));
    x.set(b1, b2);
    b1 = from_binary(len, const_cast<char*>(data += len));
    b2 = from_binary(len, const_cast<char*>(data + len));
    y.set(b1, b2);
    c.set(x, y);
    el->g.set(a, b, c);
    return GT(reinterpret_cast<void*>(el));
}

GT GT::pairing(const G1 &a, const G2 &b) {
    if ((!a.d) || (!b.d))
        return GT();
    const ::G1 &_a = *reinterpret_cast< ::G1* >(a.d->p);
    const ::G2 &_b = *reinterpret_cast< ::G2* >(b.d->p);
    ::GT *el = new ::GT(pfc->pairing(_b, _a));
    return GT(reinterpret_cast<void*>(el));
}

GT GT::pairing(const std::vector< std::pair<G1,G2> > &lst) {
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
    ::GT *el = new ::GT(pfc->multi_pairing(i, b, a));
    delete[] a;
    delete[] b;
    if (el->g.isunity()) {
        delete el;
        return GT();
    }
    return GT(reinterpret_cast<void*>(el));
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

// TODO
#error PBC implementation has not been done yet!

#else
#error Neither MIRACL nor PBC have been specified for this build
#endif /* Type of build */
