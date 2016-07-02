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

void initialize_pairings(int len, char *rndData) {
    csprng *rnd = new csprng;
    strong_init(rnd, len, rndData, (unsigned int) clock());
    pfc = new PFC(AES_SECURITY, rnd);
    Fp::zero = new SharedData(reinterpret_cast<void*>(new ::Big()));
    Fp::one = new SharedData(reinterpret_cast<void*>(new ::Big(1)));
}

void terminate_pairings() {
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

Fp::Fp(int i) : d(new SharedData(reinterpret_cast<void*>(new ::Big(i)))) {}

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

int Fp::getDataLen() const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    return _this.len() * (MIRACL / 8);
}

void Fp::getData(char *data) const {
    const ::Big &_this = *reinterpret_cast< ::Big* >(d->p);
    to_binary(_this, _this.len() * (MIRACL / 8), data, TRUE);
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
    return Fp(reinterpret_cast<void*>(new ::Big(from_binary(len, data))));
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
    return G1(reinterpret_cast<void*>(new ::G1(_this + _other)));
}

G1 G1::operator-(const G1 other) const {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) return G1(reinterpret_cast<void*>(new ::G1(-_other)));
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    return G1(reinterpret_cast<void*>(new ::G1(_this + (-_other))));
}

G1 &G1::operator+=(const G1 other) {
    if (!other.d) return *this;
    const ::G1 &_other = *reinterpret_cast< ::G1* >(other.d->p);
    if (!d) {
        ++(d = other.d)->c;
        return *this;
    }
    const ::G1 &_this = *reinterpret_cast< ::G1* >(d->p);
    void *result = reinterpret_cast<void*>(new ::G1(_this + _other));
    if (d->c) {
        --d->c;
        d = new SharedData(result);
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = result;
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
    void *result = reinterpret_cast<void*>(new ::G1(_this + (-_other)));
    if (d->c) {
        --d->c;
        d = new SharedData(result);
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        d->p = result;
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

G1 operator*(const Fp &m, const G1 &g) {
    if (!m.d) return m;
    if (g.isNull()) return G1();
    const ::G1 &_g = *reinterpret_cast< ::G1* >(g.d->p);
    const ::Big &_m = *reinterpret_cast< ::Big* >(m.d->p);
    return G1(reinterpret_cast<void*>(new ::G1(pfc->mult(_g, _m))));
}

void G1::deref() {
    if (d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::G1* >(d->p);
        delete d;
    }
}

void G2::deref() {
    if (!d->c) {
        --d->c;
    } else {
        delete reinterpret_cast< ::G2* >(d->p);
        delete d;
    }
}

void GT::deref() {
    if (!d->c) {
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
