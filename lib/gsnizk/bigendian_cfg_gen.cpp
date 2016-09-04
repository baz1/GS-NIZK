#include <fstream>
#include <cstdint>

using namespace std;

int main() {
    ofstream out("bigendian_cfg.h");
    uint16_t i16 = 1;
    out << "#define BIGENDIAN_INV16 " <<
           ((*reinterpret_cast<uint8_t*>(&i16)) ? "1" : "0") << endl;
    uint32_t i32 = 1;
    out << "#define BIGENDIAN_INV32 " <<
           ((*reinterpret_cast<uint16_t*>(&i32)) ? "1" : "0") << endl;
    uint64_t i64 = 1;
    out << "#define BIGENDIAN_INV64 " <<
           ((*reinterpret_cast<uint32_t*>(&i64)) ? "1" : "0") << endl;
    out << "#define BIGENDIAN_CFG 1" << endl;
    out.close();
    return 0;
}
