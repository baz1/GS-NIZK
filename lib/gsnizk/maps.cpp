#include "maps.h"

namespace gsnizk {

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
    // TODO precompute as necessary to increase efficiency afterwards
    if (crs.type == CRS_TYPE_PUBLIC) {
        stream >> crs.u1 >> crs.v1 >> crs.w1;
        stream >> crs.u2 >> crs.v2 >> crs.w2;
    } else {
        stream >> crs.v1._2 >> crs.v2._2;
        stream >> crs.i1 >> crs.j1 >> crs.i2 >> crs.j2;
        crs.computeElements();
    }
    return stream;
}

void CRS::computeElements() {
    v1._2.precomputeForMult();
    v1._1 = j1 * v1._2;
    w1._1 = (i1 * j1) * v1._2;
    u1._1 = w1._1;
    v2._2.precomputeForMult();
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
}

}
