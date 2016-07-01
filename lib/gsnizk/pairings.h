#ifndef PAIRINGS_H
#define PAIRINGS_H

#include <vector>

namespace pairing {

void initialize_pairings();
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

class Fp {
    friend class G1;
    friend class G2;
    friend class GT;
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
    int getData(char *&data) const;
public:
    static Fp getUnit();
    static Fp getRand();
    static Fp getValue(const char *data, int len);
private:
    inline explicit Fp(void *v);
    void deref();
private:
    SharedData *d;
};

class G1 {
    friend class GT;
public:
    inline G1();
    inline G1(const G1 &other);
    explicit G1(const Fp v);
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
    int getData(char *&data) const;
public:
    static G1 getGen();
    static G1 getRand();
    static G1 getValue(const char *data, int len);
private:
    void deref();
private:
    SharedData *d;
};

class G2 {
    friend class GT;
public:
    inline G2();
    inline G2(const G2 &other);
    explicit G2(const Fp v);
    inline ~G2();
    inline G2 &operator=(const G2 other);
    G2 operator-() const;
    G2 operator+(const G2 other) const;
    G2 operator-(const G2 other) const;
    G2 &operator+=(const G2 other);
    G2 &operator-=(const G2 other);
    G1 &operator*=(const Fp other);
    G1 operator*(const Fp other) const;
    friend G1 operator*(const Fp &m, const G1 &g);
    int getData(char *&data) const;
public:
    static G2 getGen();
    static G2 getRand();
    static G2 getValue(const char *data, int len);
private:
    void deref();
private:
    SharedData *d;
};

class GT {
public:
    inline GT();
    inline GT(const GT &other);
    explicit GT(const Fp v);
    inline ~GT();
    inline GT &operator=(const GT other);
    GT operator*(const GT other) const;
    GT operator/(const GT other) const;
    GT &operator*=(const GT other);
    GT &operator/=(const GT other);
    G1 &operator^=(const Fp other);
    G1 operator^(const Fp other) const;
    friend G1 operator^(const Fp &m, const G1 &g);
    int getData(char *&data) const;
public:
    static GT getGen();
    static GT getRand();
    static GT getValue(const char *data, int len);
    static GT pairing(const G1 a, const G2 b);
    static GT pairing(const std::vector< std::pair<G1,G2> > p);
private:
    void deref();
private:
    SharedData *d;
};

/* Inline definitions: */

inline SharedData::SharedData(void *p) : c(0), p(p) {}

inline Fp::Fp() : d(0) {}

inline Fp::Fp(const Fp &other) {
    if ((d = other.d)) ++d->c;
}

inline Fp::~Fp() { if (d) deref(); }

inline Fp &Fp::operator=(const Fp other) {
    if (d) deref();
    if ((d = other.d)) ++d->c;
    return *this;
}

inline Fp::Fp(void *v) : d(new SharedData(v)) {}

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

}

#endif // PAIRINGS_H
