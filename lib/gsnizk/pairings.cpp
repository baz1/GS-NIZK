#include "pairings.h"

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

namespace pairings {

static PFC *pfc;

SharedData *Fp::zero = 0, *Fp::one = 0;

template <typename T> inline const T &min(const T &a, const T &b) { return (a < b) ? a : b; }
template <typename T> inline const T &max(const T &a, const T &b) { return (a < b) ? b : a; }

// Note: The following is used more for an indication and hint for the readers
// rather than an efficiency improvement.
#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif /* __GNUC__ */

#define MIRACL_BYTES (MIRACL / 8)

static ::GT *rndSeed = 0;

void initialize_pairings(int len, char *rndData) {
    csprng *rnd = new csprng;
    strong_init(rnd, len, rndData, (unsigned int) clock());
    pfc = new PFC(AES_SECURITY, rnd);
    Fp::zero = new SharedData(reinterpret_cast<void*>(new ::Big()));
    Fp::one = new SharedData(reinterpret_cast<void*>(new ::Big(1)));
    ::G1 g1;
    ::G2 g2;
    pfc->random(g1);
    pfc->random(g2);
    rndSeed = new ::GT(pfc->pairing(g2, g1));
}

void terminate_pairings() {
    delete rndSeed;
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

Fp Fp::operator+(const Fp other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big((_this + _other) % (*pfc->ord))));
}

Fp Fp::operator-(const Fp other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big(((*pfc->ord + _this) - _other) % (*pfc->ord))));
}

Fp &Fp::operator+=(const Fp other) {
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

Fp &Fp::operator-=(const Fp other) {
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

Fp Fp::operator*(const Fp other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big(modmult(_this, _other, *pfc->ord))));
}

Fp Fp::operator/(const Fp other) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return Fp(reinterpret_cast<void*>(new ::Big(moddiv(_this, _other, *pfc->ord))));
}

Fp &Fp::operator*=(const Fp other) {
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

Fp &Fp::operator/=(const Fp other) {
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

bool Fp::operator==(const Fp other) const {
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

G1 G1::operator+(const G1 other) const {
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

G1 G1::operator-(const G1 other) const {
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

G1 &G1::operator+=(const G1 other) {
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

G1 &G1::operator-=(const G1 other) {
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

G1 &G1::operator*=(const Fp other) {
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

G1 G1::operator*(const Fp other) const {
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

bool G1::operator==(const G1 other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    return (_this == _other);
}

int G1::getDataLen(bool compressed) const {
    if (!d) return 0;
    if (compressed) {
        return pfc->mod->len() * MIRACL_BYTES + 1;
    } else {
        return pfc->mod->len() * 2 * MIRACL_BYTES;
    }
}

void G1::getData(char *data, bool compressed) const {
    if (!d) return;
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    int len = pfc->mod->len() * MIRACL_BYTES;
    if (compressed) {
        ::Big x;
        int lsb = _this.g.get(x);
        *data = (char) lsb;
        to_binary(x, len, data + 1, TRUE);
    } else {
        ::Big x, y;
        _this.g.get(x, y);
        to_binary(x, len, data, TRUE);
        to_binary(y, len, data + len, TRUE);
    }
}

G1 G1::getRand() {
    ::G1 *el = new ::G1();
    pfc->random(*el);
    // Following is very unlikely, but we still want to handle it
    if (unlikely(el->g.iszero())) {
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

/* Replacement for faulty "G2 operator+(const G2& x,const G2& y)" */
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

G2 G2::operator+(const G2 other) const {
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

G2 G2::operator-(const G2 other) const {
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

G2 &G2::operator+=(const G2 other) {
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

G2 &G2::operator-=(const G2 other) {
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

G2 &G2::operator*=(const Fp other) {
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

G2 G2::operator*(const Fp other) const {
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

bool G2::operator==(const G2 other) const {
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
        return pfc->mod->len() * (2 * MIRACL_BYTES) + 1;
    } else {
        return pfc->mod->len() * (4 * MIRACL_BYTES);
    }
}

void G2::getData(char *data, bool compressed) const {
    if (!d) return;
    const ::G2 &_this = *reinterpret_cast< ::G2* >(d->p);
    int len = pfc->mod->len() * MIRACL_BYTES;
    ::ZZn2 x, y;
    _this.g.get(x, y);
    if (compressed) {
        ::Big b1, b2;
        y.get(b1);
        *data = (char) (b1.get(1) & 1);
        x.get(b1, b2);
        to_binary(b1, len, ++data, TRUE);
        to_binary(b2, len, data + len, TRUE);
    } else {
        ::Big b1, b2, b3, b4;
        x.get(b1, b2);
        y.get(b3, b4);
        to_binary(b1, len, data, TRUE);
        to_binary(b2, len, data += len, TRUE);
        to_binary(b3, len, data += len, TRUE);
        to_binary(b4, len, data + len, TRUE);
    }
}

G2 G2::getRand() {
    ::G2 *el = new ::G2();
    pfc->random(*el);
    // Following is very unlikely, but we still want to handle it
    if (unlikely(el->g.iszero())) {
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

void G2::deref() {
    if (d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        delete d;
    }
}

GT GT::operator*(const GT other) const {
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

GT GT::operator/(const GT other) const {
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

GT &GT::operator*=(const GT other) {
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

GT &GT::operator/=(const GT other) {
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

GT &GT::operator^=(const Fp other) {
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

GT GT::operator^(const Fp other) const {
    if (!d) return *this;
    if (other.isNull()) return GT();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::Big &_other = *reinterpret_cast< ::Big* >(other.d->p);
    return GT(reinterpret_cast<void*>(new ::GT(pfc->power(_this, _other))));
}

GT operator^(const Fp &m, const GT &g) {
    if (!g.d) return g;
    if (m.isNull()) return GT();
    // Note: Since neither this group element nor the scalar are null,
    // the result won't be null either.
    const ::GT &_g = *reinterpret_cast< ::GT* >(g.d->p);
    const ::Big &_m = *reinterpret_cast< ::Big* >(m.d->p);
    return GT(reinterpret_cast<void*>(new ::GT(pfc->power(_g, _m))));
}

bool GT::operator==(const GT other) const {
    if (!d) return !other.d;
    if (!other.d) return false;
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    const ::GT &_other = *reinterpret_cast< ::GT* >(other.d->p);
    return (_this == _other);
}

int GT::getDataLen() const {
    if (!d) return 0;
    return pfc->mod->len() * 12 * MIRACL_BYTES;
}

void GT::getData(char *data) const {
    if (!d) return;
    const ::GT &_this = *reinterpret_cast< ::GT* >(d->p);
    int len = pfc->mod->len() * MIRACL_BYTES;
    ::ZZn4 a, b, c;
    ::ZZn2 x, y;
    ::Big b1, b2;
    _this.g.get(a, b, c);
    a.get(x, y);
    x.get(b1, b2);
    to_binary(b1, len, data, TRUE);
    to_binary(b2, len, data += len, TRUE);
    y.get(b1, b2);
    to_binary(b1, len, data += len, TRUE);
    to_binary(b2, len, data += len, TRUE);
    b.get(x, y);
    x.get(b1, b2);
    to_binary(b1, len, data += len, TRUE);
    to_binary(b2, len, data += len, TRUE);
    y.get(b1, b2);
    to_binary(b1, len, data += len, TRUE);
    to_binary(b2, len, data += len, TRUE);
    c.get(x, y);
    x.get(b1, b2);
    to_binary(b1, len, data += len, TRUE);
    to_binary(b2, len, data += len, TRUE);
    y.get(b1, b2);
    to_binary(b1, len, data += len, TRUE);
    to_binary(b2, len, data + len, TRUE);
}

GT GT::getRand() {
    ::Big b;
    pfc->random(b);
    ::GT *el = new ::GT(pfc->power(*rndSeed, b));
    // Following is very unlikely, but we still want to handle it
    if (unlikely(el->g.iszero())) {
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

GT GT::pairing(const G1 a, const G2 b) {
    if ((!a.d) || (!b.d))
        return GT();
    const ::G1 &_a = *reinterpret_cast< ::G1* >(a.d->p);
    const ::G2 &_b = *reinterpret_cast< ::G2* >(b.d->p);
    ::GT *el = new ::GT(pfc->pairing(_b, _a));
    return GT(reinterpret_cast<void*>(el));
}

GT GT::pairing(const std::vector< std::pair<G1,G2> > lst) {
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

// TODO

} /* End of namespace pairings */

#elif defined(USE_PBC)
/* -------------------- PBC build -------------------- */

// TODO

#else
#error Neither MIRACL nor PBC have been specified for this build
#endif /* Type of build */
