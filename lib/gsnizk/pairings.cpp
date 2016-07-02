#include "pairings.h"

#include <cstring>

#if defined(USE_MIRACL)
/* -------------------- MIRACL build -------------------- */

#define MR_PAIRING_BN // AES-128 or AES-192 security

#ifndef AES_SECURITY
#define AES_SECURITY 128
//#define AES_SECURITY 192
#endif /* Not AES_SECURITY */

#define BUFFER_SIZE 128

#include "pairing_3.h"

namespace pairings {

static PFC *pfc;

void initialize_pairings() {
    pfc = new PFC(AES_SECURITY);
}

void terminate_pairings() {
    delete pfc;
}

Fp::Fp() {
    if (!zero)
        zero = new SharedData(reinterpret_cast<void*>(new ::Big()));
    d = zero;
    ++d->c;
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

Fp Fp::getUnit() {
    if (!one)
        one = new SharedData(reinterpret_cast<void*>(new ::Big(1)));
    return Fp(one);
}

Fp Fp::getRand() {
    // TODO
    return Fp();
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
