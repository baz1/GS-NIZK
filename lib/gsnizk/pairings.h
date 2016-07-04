#ifndef PAIRINGS_H
#define PAIRINGS_H

#include <vector>

namespace pairings {

void initialize_pairings(int len, char *rndData);
void terminate_pairings();

struct SharedData {
    unsigned int c;
    void *p;
    inline SharedData(void *v);
};

class G1;
class G2;
class GT;
class Fp;

int getHashLen();
void getHash(const char *data, int len, char *hash);

class Fp {
    friend class G1;
    friend class G2;
    friend class GT;
    friend void initialize_pairings(int, char*);
    friend void terminate_pairings();
    friend G1 operator*(const Fp &m, const G1 &g);
    friend G2 operator*(const Fp &m, const G2 &g);
    friend GT operator^(const Fp &m, const GT &g);
public:
    inline Fp();
    Fp(int i);
    explicit Fp(unsigned long i);
    inline Fp(const Fp &other);
    inline ~Fp();
    inline Fp &operator=(const Fp other);
    Fp operator-() const;
    Fp operator+(const Fp other) const;
    Fp operator-(const Fp other) const;
    Fp &operator+=(const Fp other);
    Fp &operator-=(const Fp other);
    Fp operator*(const Fp other) const;
    Fp operator/(const Fp other) const;
    Fp &operator*=(const Fp other);
    Fp &operator/=(const Fp other);
    bool operator==(const Fp other) const;
    inline bool operator!=(const Fp other) const;
    int getDataLen() const;
    void getData(char *data) const;
    bool isNull() const;
public:
    static Fp getUnit();
    static Fp getRand();
    static Fp getValue(const char *data, int len);
    static Fp fromHash(const char *data, int len);
    static Fp fromHash(const char *hash);
private:
    inline explicit Fp(void *v);
    inline explicit Fp(SharedData *d);
    void deref();
    static SharedData *zero, *one;
private:
    SharedData *d;
};

class G1 {
    friend class GT;
public:
    inline G1();
    inline G1(const G1 &other);
    inline ~G1();
    inline G1 &operator=(const G1 other);
    G1 operator-() const;
    G1 operator+(const G1 other) const;
    G1 operator-(const G1 other) const;
    G1 &operator+=(const G1 other);
    G1 &operator-=(const G1 other);
    G1 &operator*=(const Fp other);
    G1 operator*(const Fp other) const;
    friend G1 operator*(const Fp &m, const G1 &g);
    bool operator==(const G1 other) const;
    inline bool operator!=(const G1 other) const;
    int getDataLen(bool compressed = false) const;
    void getData(char *data, bool compressed = false) const;
    inline bool isNull() const;
public:
    static G1 getRand();
    static G1 getValue(const char *data, int len, bool compressed = false);
    static G1 fromHash(const char *data, int len);
    static G1 fromHash(const char *hash);
private:
    inline explicit G1(void *v);
    inline explicit G1(SharedData *d);
    void deref();
private:
    SharedData *d;
};

class G2 {
    friend class GT;
public:
    inline G2();
    inline G2(const G2 &other);
    inline ~G2();
    inline G2 &operator=(const G2 other);
    G2 operator-() const;
    G2 operator+(const G2 other) const;
    G2 operator-(const G2 other) const;
    G2 &operator+=(const G2 other);
    G2 &operator-=(const G2 other);
    G2 &operator*=(const Fp other);
    G2 operator*(const Fp other) const;
    friend G2 operator*(const Fp &m, const G2 &g);
    bool operator==(const G2 other) const;
    inline bool operator!=(const G2 other) const;
    int getDataLen(bool compressed = false) const;
    void getData(char *data, bool compressed = false) const;
    inline bool isNull() const;
public:
    static G2 getRand();
    static G2 getValue(const char *data, int len, bool compressed = false);
    static G2 fromHash(const char *data, int len);
    static G2 fromHash(const char *hash);
private:
    inline explicit G2(void *v);
    inline explicit G2(SharedData *d);
    void deref();
private:
    SharedData *d;
};

class GT {
public:
    inline GT();
    inline GT(const GT &other);
    inline ~GT();
    inline GT &operator=(const GT other);
    GT operator*(const GT other) const;
    GT operator/(const GT other) const;
    GT &operator*=(const GT other);
    GT &operator/=(const GT other);
    GT &operator^=(const Fp other);
    GT operator^(const Fp other) const;
    bool operator==(const GT other) const;
    inline bool operator!=(const GT other) const;
    int getDataLen() const;
    void getData(char *data) const;
    inline bool isUnity() const;
public:
    static GT getRand();
    static GT getValue(const char *data, int len);
    static GT pairing(const G1 a, const G2 b);
    static GT pairing(const std::vector< std::pair<G1,G2> > lst);
private:
    inline explicit GT(void *v);
    inline explicit GT(SharedData *d);
    void deref();
private:
    SharedData *d;
};

/* Inline definitions: */

inline SharedData::SharedData(void *p) : c(0), p(p) {}

inline Fp::Fp() { ++(d = zero)->c; }

inline Fp::Fp(const Fp &other) { ++(d = other.d)->c; }

inline Fp::~Fp() { deref(); }

inline Fp &Fp::operator=(const Fp other) {
    deref();
    ++(d = other.d)->c;
    return *this;
}

inline bool Fp::operator!=(const Fp other) const {
    return !(*this == other);
}

inline Fp::Fp(void *v) : d(new SharedData(v)) {}

inline Fp::Fp(SharedData *d) : d(d) { ++d->c; }

inline G1::G1() : d(0) {}

inline G1::G1(const G1 &other) {
    if ((d = other.d)) ++d->c;
}

inline G1::~G1() { if (d) deref(); }

inline G1 &G1::operator=(const G1 other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline bool G1::operator!=(const G1 other) const {
    return !(*this == other);
}

inline bool G1::isNull() const { return !d; }

inline G1::G1(void *v) : d(new SharedData(v)) {}

inline G1::G1(SharedData *d) : d(d) { ++d->c; }

inline G2::G2() : d(0) {}

inline G2::G2(const G2 &other) {
    if ((d = other.d)) ++d->c;
}

inline G2::~G2() { if (d) deref(); }

inline G2 &G2::operator=(const G2 other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline bool G2::operator!=(const G2 other) const {
    return !(*this == other);
}

inline bool G2::isNull() const { return !d; }

inline G2::G2(void *v) : d(new SharedData(v)) {}

inline G2::G2(SharedData *d) : d(d) { ++d->c; }

inline GT::GT() : d(0) {}

inline GT::GT(const GT &other) {
    if ((d = other.d)) ++d->c;
}

inline GT::~GT() { if (d) deref(); }

inline GT &GT::operator=(const GT other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline bool GT::operator!=(const GT other) const {
    return !(*this == other);
}

inline bool GT::isUnity() const { return !d; }

inline GT::GT(void *v) : d(new SharedData(v)) {}

inline GT::GT(SharedData *d) : d(d) { ++d->c; }

} /* End of namespace pairings */

#endif // PAIRINGS_H
