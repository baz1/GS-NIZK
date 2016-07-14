#include "maps.h"

namespace gsnizk {

BT BT::pairing(const B1 &a, const B2 &b) {
    // TODO precompute pairings?
    return BT(GT::pairing(a._1, b._1), GT::pairing(a._1, b._2),
              GT::pairing(a._2, b._1), GT::pairing(a._2, b._2));
}

BT BT::pairing(const std::vector<std::pair<B1, B2> > &lst) {
    // TODO precompute pairings?
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
        type = CRS_TYPE_PUBLIC;
    }
}

std::ostream &operator<<(std::ostream &stream, const CRS &crs) {
    stream << crs.type;
    if (crs.type == CRS_TYPE_PUBLIC) {
        stream << crs.u1 << crs.v1 << crs.w1;
        stream << crs.u2 << crs.v2 << crs.w2;
    } else {
        stream << crs.v1._2 << crs.v2._2;
        stream << crs.i1 << crs.j1 << crs.i2 << crs.j2;
    }
    return stream;
}

std::istream &operator>>(std::istream &stream, CRS &crs) {
    stream >> crs.type;
    // TODO precomputations?
    if (crs.type == CRS_TYPE_PUBLIC) {
        stream >> crs.u1 >> crs.v1 >> crs.w1;
        stream >> crs.u2 >> crs.v2 >> crs.w2;
#if !defined(USE_PBC)
        /* Precomputations of generators in G1 and G2 */
        crs.v1._2.precomputeForMult();
        crs.v2._2.precomputeForMult();
        /* Precomputations for commitments of scalars */
        crs.u1._1.precomputeForMult();
        crs.u1._2.precomputeForMult();
        crs.u2._1.precomputeForMult();
        crs.u2._2.precomputeForMult();
#endif
    } else {
        stream >> crs.v1._2 >> crs.v2._2;
        stream >> crs.i1 >> crs.j1 >> crs.i2 >> crs.j2;
        crs.computeElements();
    }
    return stream;
}

void CRS::computeElements() {
#if !defined(USE_PBC)
    /* Precomputations of generators in G1 and G2 */
    v1._2.precomputeForMult();
    v2._2.precomputeForMult();
#endif
    v1._1 = j1 * v1._2;
    w1._1 = (i1 * j1) * v1._2;
    u1._1 = w1._1;
    v2._1 = j2 * v2._2;
    w2._1 = (i2 * j2) * v2._2;
    u2._1 = w2._1;
    if (type == CRS_TYPE_EXTRACT) {
        w1._2 = i1 * v1._2;
        u1._2 = w1._2 + v1._2;
        w2._2 = i2 * v2._2;
        u2._2 = w2._2 + v2._2;
    } else {
        u1._2 = i1 * v1._2;
        w1._2 = w1._2 - v1._2;
        u2._2 = i2 * v2._2;
        w2._2 = w2._2 - v2._2;
    }
#if !defined(USE_PBC)
    /* Precomputations for commitments of scalars */
    crs.u1._1.precomputeForMult();
    crs.u1._2.precomputeForMult();
    crs.u2._1.precomputeForMult();
    crs.u2._2.precomputeForMult();
#endif
}

}
