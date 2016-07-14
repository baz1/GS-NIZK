#include "maps.h"

namespace gsnizk {

CRS::CRS(bool binding) {
    // TODO
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
        // TODO: compressed send
    }
}

std::istream &operator>>(std::istream &stream, CRS &crs) {
    stream >> crs.type;
    if (crs.type == CRS_TYPE_PUBLIC) {
        stream >> crs.u1 >> crs.v1 >> crs.w1;
        stream >> crs.u2 >> crs.v2 >> crs.w2;
    } else {
        // TODO: compressed receive
    }
}

}
