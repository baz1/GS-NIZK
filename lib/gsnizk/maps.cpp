/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "maps.h"

#ifdef DEBUG
#include <iostream>
#define ASSERT(X,Y) if (!(X)) { \
    std::cerr << "Error: Assert of " << #X << " at line " \
    << __LINE__ << " failed in " << __FILE__ << "!" << std::endl \
    << Y << std::endl; \
    throw Y; \
    }
#else
#define ASSERT(X,Y) /* noop */
#endif

namespace gsnizk {

G1 B1::extract(const CRS &crs) const {
    ASSERT(crs.type == CRS_TYPE_EXTRACT, "Wrong type of CRS");
    return _2 - (Fp::getUnit() / crs.j1) * _1;
}

B1 B1::commit(const Fp &el, const Fp &r, const CRS &crs) {
    return el * crs.u1 + r * crs.v1;
}

B1 B1::commit(const B1 &el, const Fp &r, const CRS &crs) {
    return el + r * crs.v1;
}

B1 B1::commit(const B1 &el, const Fp &r, const Fp &s, const CRS &crs) {
    if (crs.type & 1)
        return el + (r + crs.i1 * s) * crs.v1;
    return el + r * crs.v1 + s * crs.w1;
}

G2 B2::extract(const CRS &crs) const {
    ASSERT(crs.type == CRS_TYPE_EXTRACT, "Wrong type of CRS");
    return _2 - (Fp::getUnit() / crs.j2) * _1;
}

B2 B2::commit(const Fp &el, const Fp &r, const CRS &crs) {
    return el * crs.u2 + r * crs.v2;
}

B2 B2::commit(const B2 &el, const Fp &r, const CRS &crs) {
    return el + r * crs.v2;
}

B2 B2::commit(const B2 &el, const Fp &r, const Fp &s, const CRS &crs) {
    if (crs.type & 1)
        return el + (r + crs.i2 * s) * crs.v2;
    return el + r * crs.v2 + s * crs.w2;
}

GT BT::extract(const CRS &crs) const {
    ASSERT(crs.type == CRS_TYPE_EXTRACT, "Wrong type of CRS");
    Fp p = Fp(-1) / crs.j1;
    return (_22 * (_12 ^ p)) * ((_21 * (_11 ^ p)) ^ (Fp(-1) / crs.j2));
}

BT BT::pairing(const B1 &a, const B2 &b) {
    return BT(GT::pairing(a._1, b._1), GT::pairing(a._1, b._2),
              GT::pairing(a._2, b._1), GT::pairing(a._2, b._2));
}

BT BT::pairing(const std::vector< std::pair<B1,B2> > &lst) {
    if (lst.empty()) return BT();
    std::vector<std::pair<G1, G2> > p11, p12, p21, p22;
    p11.reserve(lst.size());
    p12.reserve(lst.size());
    p21.reserve(lst.size());
    p22.reserve(lst.size());
    for (const std::pair<B1, B2> p: lst) {
        p11.push_back(std::pair<G1, G2>(p.first._1, p.second._1));
        p12.push_back(std::pair<G1, G2>(p.first._1, p.second._2));
        p21.push_back(std::pair<G1, G2>(p.first._2, p.second._1));
        p22.push_back(std::pair<G1, G2>(p.first._2, p.second._2));
    }
    return BT(GT::pairing(p11), GT::pairing(p12),
              GT::pairing(p21), GT::pairing(p22));
}

CRS::CRS(bool binding) : v1(G1(), G1::getRand()), v2(G2(), G2::getRand()),
    type(binding ? CRS_TYPE_EXTRACT : CRS_TYPE_ZK),
    i1(Fp::getRand()), j1(Fp::getRand()),
    i2(Fp::getRand()), j2(Fp::getRand())
{
    computeElements();
}

void CRS::makePublic() {
    if (type != CRS_TYPE_PUBLIC) {
        i1 = Fp();
        i2 = Fp();
        j1 = Fp();
        j2 = Fp();
        type = CRS_TYPE_PUBLIC;
    }
}

CRS CRS::genPrivate(std::ostream &stream) const {
    if (type != CRS_TYPE_PUBLIC)
        throw "Unexpected use of gsnizk::genPrivate";
    CRS priv;
    priv.type = CRS_TYPE_PRIVATE;
    priv.v1 = v1;
    priv.i1 = Fp::getRand();
    priv.v2 = v2;
    priv.i2 = Fp::getRand();
    priv.computeElements(false);
    Fp r_rho = Fp::getRand(), r_sig = Fp::getRand();
    stream << (priv.i1 * u2 + r_rho * v2);
    stream << (priv.i2 * u1 + r_sig * v1);
    stream << (-r_rho) * v1;
    stream << (-r_sig) * v2;
    return priv;
}

bool CRS::checkPrivate(std::istream &stream, CRS priv) const {
    if ((v1 != priv.v1) || (v2 != priv.v2))
        return false;
    G2 c_rho_1, c_rho_2, p3, p4, g2;
    G1 c_sig_1, c_sig_2, p1, p2, g1;
    stream >> c_rho_1 >> c_rho_2;
    stream >> c_sig_1 >> c_sig_2;
    stream >> p1 >> p2 >> p3 >> p4;
    Fp c1_1 = Fp::getRand(), c1_2 = Fp::getRand();
    Fp c2_1 = Fp::getRand(), c2_2 = Fp::getRand();
    std::vector< std::pair<G1,G2> > pairs;
    pairs.reserve(3);
    g1 = -(c1_1 * priv.w1._1 + c1_2 * priv.w1._2);
    g2 = c2_1 * u2._1 + c2_2 * u2._2;
    pairs.push_back(std::pair<G1,G2>(g1, g2));
    g1 = c1_1 * v1._1 + c1_2 * v1._2;
    g2 = c2_1 * c_rho_1 + c2_2 * c_rho_2;
    pairs.push_back(std::pair<G1,G2>(g1, g2));
    g1 = c1_1 * p1 + c1_2 * p2;
    g2 = c2_1 * v2._1 + c2_2 * v2._2;
    pairs.push_back(std::pair<G1,G2>(g1, g2));
    if (!GT::pairing(pairs).isUnit())
        return false;
    pairs.clear();
    c1_1 = Fp::getRand(); c1_2 = Fp::getRand();
    c2_1 = Fp::getRand(); c2_2 = Fp::getRand();
    g1 = -(c1_1 * u1._1 + c1_2 * u1._2);
    g2 = c2_1 * priv.w2._1 + c2_2 * priv.w2._2;
    pairs.push_back(std::pair<G1,G2>(g1, g2));
    g1 = c1_1 * c_sig_1 + c1_2 * c_sig_2;
    g2 = c2_1 * v2._1 + c2_2 * v2._2;
    pairs.push_back(std::pair<G1,G2>(g1, g2));
    g1 = c1_1 * v1._1 + c1_2 * v1._2;
    g2 = c2_1 * p3 + c2_2 * p4;
    pairs.push_back(std::pair<G1,G2>(g1, g2));
    if (!GT::pairing(pairs).isUnit())
        return false;
    return true;
}

std::ostream &operator<<(std::ostream &stream, const CRS &crs) {
    stream << crs.type;
    if (crs.type == CRS_TYPE_PUBLIC) {
        stream << crs.v1 << crs.w1;
        stream << crs.v2 << crs.w2;
    } else if (crs.type == CRS_TYPE_PRIVATE) {
        stream << crs.v1 << crs.v2;
        stream << crs.i1 << crs.i2;
    } else {
        stream << crs.v1._2 << crs.v2._2;
        stream << crs.i1 << crs.j1 << crs.i2 << crs.j2;
    }
    return stream;
}

std::istream &operator>>(std::istream &stream, CRS &crs) {
    stream >> crs.type;
    if (crs.type == CRS_TYPE_PUBLIC) {
        stream >> crs.v1 >> crs.w1;
        stream >> crs.v2 >> crs.w2;
        crs.u1._1 = crs.w1._1;
        crs.u1._2 = crs.w1._2 + crs.v1._2;
        crs.u2._1 = crs.w2._1;
        crs.u2._2 = crs.w2._2 + crs.v2._2;
#if !defined(USE_PBC)
        /* Precomputations for commitments (of scalars and group elements) */
        crs.u1._1.precomputeForMult();
        crs.u1._2.precomputeForMult();
        crs.v1._1.precomputeForMult();
        crs.v1._2.precomputeForMult();
        crs.w1._2.precomputeForMult();
        crs.u2._1.precomputeForMult();
        crs.u2._2.precomputeForMult();
        crs.v2._1.precomputeForMult();
        crs.v2._2.precomputeForMult();
        crs.w2._2.precomputeForMult();
        /* Precomputations for proof verification */
        crs.u2._1.precomputeForPairing();
        crs.u2._2.precomputeForPairing();
        crs.v2._1.precomputeForPairing();
        crs.v2._2.precomputeForPairing();
        crs.w2._2.precomputeForPairing();
#endif
    } else if (crs.type == CRS_TYPE_PRIVATE) {
        stream >> crs.v1 >> crs.v2;
        stream >> crs.i1 >> crs.i2;
        crs.computeElements();
    } else {
        stream >> crs.v1._2 >> crs.v2._2;
        stream >> crs.i1 >> crs.j1 >> crs.i2 >> crs.j2;
        crs.computeElements();
    }
    return stream;
}

void CRS::computeElements(bool precompute_v) {
#if !defined(USE_PBC)
    /* Precomputations for commitments */
    if (precompute_v) {
        v1._2.precomputeForMult();
        v2._2.precomputeForMult();
    }
#else
    (void) precompute_v;
#endif
    if (type == CRS_TYPE_PRIVATE) {
#if !defined(USE_PBC)
        if (precompute_v) {
            /* Precomputations for commitments */
            v1._1.precomputeForMult();
            v2._1.precomputeForMult();
        }
#endif
        w1._1 = i1 * v1._1;
        w1._2 = i1 * v1._2;
        u1._1 = w1._1;
        u1._2 = w1._2 + v1._2;
        w2._1 = i2 * v2._1;
        w2._2 = i2 * v2._2;
        u2._1 = w2._1;
        u2._2 = w2._2 + v2._2;
    } else {
        v1._1 = j1 * v1._2;
        v2._1 = j2 * v2._2;
#if !defined(USE_PBC)
        /* Precomputations for commitments */
        v1._1.precomputeForMult();
        v2._1.precomputeForMult();
#endif
        w1._1 = i1 * v1._1;
        u1._1 = w1._1;
        w2._1 = i2 * v2._1;
        u2._1 = w2._1;
        if (type == CRS_TYPE_EXTRACT) {
            w1._2 = i1 * v1._2;
            u1._2 = w1._2 + v1._2;
            w2._2 = i2 * v2._2;
            u2._2 = w2._2 + v2._2;
        } else {
            u1._2 = i1 * v1._2;
            w1._2 = u1._2 - v1._2;
            u2._2 = i2 * v2._2;
            w2._2 = u2._2 - v2._2;
        }
    }
#if !defined(USE_PBC)
    /* Precomputations for commitments of scalars */
    u1._1.precomputeForMult();
    u1._2.precomputeForMult();
    u2._1.precomputeForMult();
    u2._2.precomputeForMult();
    /* Precomputations for commitments of group elements */
    w1._2.precomputeForMult();
    w2._2.precomputeForMult();
    /* Precomputations for proof verification */
    u2._1.precomputeForPairing();
    u2._2.precomputeForPairing();
    v2._1.precomputeForPairing();
    v2._2.precomputeForPairing();
    w2._2.precomputeForPairing();
#endif
}

} /* End of namespace nizk */
