/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef GSNIZK_H
#define GSNIZK_H

#include "maps.h"

#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <set>

/**
 * @file gsnizk.h
 * @brief Groth and Sahai's NIZK proof system.
 *
 * This is the main file to include to use the @ref gsnizk
 * namespace which is at the center of this library.
 */

/**
 * @brief Groth and Sahai's NIZK proof system.
 *
 * This namespace contains a complete implementation
 * of the Non-Interactive Zero Knowledge (NIZK) proof
 * scheme originally designed by Groth and Sahai @cite Groth2007,
 * with corrections as suggested by Ghadafi et al. @cite Ghadafi2009,
 * Blazy et al. @cite Blazy2010 and Escala et al. @cite Escala2013.
 */
namespace gsnizk {

/**
 * @cond INTERNAL_DATA_STRUCT
 */

enum ElementType {
    ELEMENT_VARIABLE    = 0,
    ELEMENT_CONST_INDEX = 1,
    ELEMENT_CONST_VALUE = 2,
    ELEMENT_PAIR        = 3,
    ELEMENT_SCALAR      = 4,
    ELEMENT_PAIRING     = 5,
    ELEMENT_BASE        = 6
};

struct FpData;
struct G1Data;
struct G2Data;
struct GTData;

typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<FpData> > PairFp;
typedef std::pair< std::shared_ptr<G1Data>, std::shared_ptr<G1Data> > PairG1;
typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<G1Data> > ScalarG1;
typedef std::pair< std::shared_ptr<G2Data>, std::shared_ptr<G2Data> > PairG2;
typedef std::pair< std::shared_ptr<FpData>, std::shared_ptr<G2Data> > ScalarG2;
typedef std::pair< std::shared_ptr<GTData>, std::shared_ptr<GTData> > PairGT;
typedef std::pair< std::shared_ptr<G1Data>, std::shared_ptr<G2Data> > PairingGT;

struct FpData {
    ElementType type;
    union {
        int index;
        Fp el;
        PairFp pair;
    };
    mutable void *d;
    inline FpData(ElementType type) : type(type), d(NULL) {}
    ~FpData();
};

struct G1Data {
    ElementType type;
    union {
        int index;
        G1 el;
        PairG1 pair;
        ScalarG1 scalar;
    };
    mutable void *d;
    inline G1Data(ElementType type) : type(type), d(NULL) {}
    ~G1Data();
};

struct G2Data {
    ElementType type;
    union {
        int index;
        G2 el;
        PairG2 pair;
        ScalarG2 scalar;
    };
    mutable void *d;
    inline G2Data(ElementType type) : type(type), d(NULL) {}
    ~G2Data();
};

struct GTData {
    ElementType type;
    union {
        int index;
        GT el;
        PairGT pair;
        PairingGT pring;
    };
    mutable void *d;
    inline GTData(ElementType type) : type(type), d(NULL) {}
    ~GTData();
};

class FpElement;
class G1Element;
class G2Element;
class GTElement;

/* Pre-definitions for visibility inside the scope: */
FpElement FpVar(int index);
FpElement FpConst(int index);
FpElement FpConst(Fp value);
FpElement FpUnit();
G1Element G1Var(int index);
G1Element G1Const(int index);
G1Element G1Const(G1 value);
G1Element G1Base();
G2Element G2Var(int index);
G2Element G2Const(int index);
G2Element G2Const(G2 value);
G2Element G2Base();
GTElement GTConst(int index);
GTElement GTConst(GT value);
GTElement GTBase();
G1Element operator*(const FpElement &s, const G1Element &e);
G2Element operator*(const FpElement &s, const G2Element &e);
GTElement e(const G1Element &a, const G2Element &b);

/* Element types : see @cite Escala2016. */
enum EL_TYPE_PT {
    /* Elements in G1 */
    EL_TYPE_BASE_G,
    EL_TYPE_PUB_G,
    EL_TYPE_ENC_G,
    EL_TYPE_COM_G,
    /* Elements in G2 */
    EL_TYPE_BASE_H,
    EL_TYPE_PUB_H,
    EL_TYPE_ENC_H,
    EL_TYPE_COM_H,
    /* Left elements in Fp */
    EL_TYPE_UNIT_G,
    EL_TYPE_SCA_G,
    /* Right elements in Fp */
    EL_TYPE_UNIT_H,
    EL_TYPE_SCA_H
};

typedef std::set<EL_TYPE_PT> ElTypeSet;

/* Equation proof types : see @cite Escala2016. */
enum EqProofType {
    /* Equations in GT */
    EQ_TYPE_PPE         = 0,
    EQ_TYPE_PEnc_G      = 1,
    EQ_TYPE_PConst_G    = 2,
    EQ_TYPE_PEnc_H      = 3,
    EQ_TYPE_PConst_H    = 4,
    /* Equations in G1 */
    EQ_TYPE_ME_G        = 5,
    EQ_TYPE_MEnc_G      = 6,
    EQ_TYPE_MConst_G    = 7,
    EQ_TYPE_MLin_G      = 8,
    /* Equations in G2 */
    EQ_TYPE_ME_H        = 9,
    EQ_TYPE_MEnc_H      = 10,
    EQ_TYPE_MConst_H    = 11,
    EQ_TYPE_MLin_H      = 12,
    /* Equations in Fp */
    EQ_TYPE_QE          = 13,
    EQ_TYPE_QConst_G    = 14,
    EQ_TYPE_QConst_H    = 15
};

struct AdditionalFp {
    inline AdditionalFp() {}
    inline AdditionalFp(const std::shared_ptr<FpData> &d) : formula(d) {}
    std::shared_ptr<FpData> formula;
    mutable Fp value;
};

struct AdditionalG1 {
    inline AdditionalG1() {}
    inline AdditionalG1(const std::shared_ptr<G1Data> &d) : formula(d) {}
    std::shared_ptr<G1Data> formula;
    mutable G1 value;
};

struct AdditionalG2 {
    inline AdditionalG2() {}
    inline AdditionalG2(const std::shared_ptr<G2Data> &d) : formula(d) {}
    std::shared_ptr<G2Data> formula;
    mutable G2 value;
};

/**
 * @endcond
 */

/**
 * @brief A @f$\mathbb{F}_p@f$ element inside an equation.
 */
class FpElement {
    friend class NIZKProof;
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{F}_p@f$.
     */
    inline FpElement();
    /**
     * @brief Adds two elements in @f$\mathbb{F}_p@f$.
     * @param other Second element in @f$\mathbb{F}_p@f$.
     * @return Reference to the modified element.
     */
    FpElement &operator+=(const FpElement &other);
    /**
     * @brief Adds two elements in @f$\mathbb{F}_p@f$.
     * @param other Second element in @f$\mathbb{F}_p@f$.
     * @return Product element, in @f$\mathbb{F}_p@f$.
     */
    FpElement operator+(const FpElement &other) const;
    /**
     * @brief Multiplies two elements in @f$\mathbb{F}_p@f$.
     * @param other Second element in @f$\mathbb{F}_p@f$.
     * @return Reference to the modified element.
     */
    FpElement &operator*=(const FpElement &other);
    /**
     * @brief Multiplies two elements in @f$\mathbb{F}_p@f$.
     * @param other Second element in @f$\mathbb{F}_p@f$.
     * @return Product element, in @f$\mathbb{F}_p@f$.
     */
    FpElement operator*(const FpElement &other) const;
private:
    inline FpElement(std::shared_ptr<FpData> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{F}_p@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{F}_p@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The variable element.
     */
    friend FpElement FpVar(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{F}_p@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{F}_p@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The constant element.
     */
    friend FpElement FpConst(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{F}_p@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{F}_p@f$.
     * @return The constant element.
     */
    friend FpElement FpConst(Fp value);
    /**
     * @brief Creates a unit element of @f$\mathbb{F}_p@f$.
     * @return The unit element.
     */
    friend FpElement FpUnit();
    /* Pre-definition */
    friend G1Element operator*(const FpElement &s, const G1Element &e);
    friend G2Element operator*(const FpElement &s, const G2Element &e);
    friend GTElement operator^(const GTElement &e, const FpElement &s);
private:
    std::shared_ptr<FpData> data;
};

/**
 * @brief A @f$\mathbb{G}_1@f$ element inside an equation.
 */
class G1Element {
    friend class NIZKProof;
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{G}_1@f$.
     */
    inline G1Element();
    /**
     * @brief Adds two elements in @f$\mathbb{G}_1@f$.
     * @param other Second element in @f$\mathbb{G}_1@f$.
     * @return Reference to the modified element.
     */
    G1Element &operator+=(const G1Element &other);
    /**
     * @brief Adds two elements in @f$\mathbb{G}_1@f$.
     * @param other Second element in @f$\mathbb{G}_1@f$.
     * @return Product element, in @f$\mathbb{G}_1@f$.
     */
    G1Element operator+(const G1Element &other) const;
    /**
     * @brief Multiplies an element of @f$\mathbb{G}_1@f$ with a scalar.
     * @param s Scalar used in the multiplication.
     * @param e Element to multiply the scalar.
     * @return Result of the multiplication.
     */
    friend G1Element operator*(const FpElement &s, const G1Element &e);
private:
    inline G1Element(std::shared_ptr<G1Data> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_1@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_1@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The variable element.
     */
    friend G1Element G1Var(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_1@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_1@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The constant element.
     */
    friend G1Element G1Const(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_1@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_1@f$.
     * @return The constant element.
     */
    friend G1Element G1Const(G1 value);
    /**
     * @brief Creates the special constant that is the base element of
     *   @f$\mathbb{G}_1@f$.
     * @return The base element.
     */
    friend G1Element G1Base();
    /* Pre-definition */
    friend GTElement e(const G1Element &a, const G2Element &b);
private:
    std::shared_ptr<G1Data> data;
};

/**
 * @brief A @f$\mathbb{G}_2@f$ element inside an equation.
 */
class G2Element {
    friend class NIZKProof;
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{G}_2@f$.
     */
    inline G2Element();
    /**
     * @brief Adds two elements in @f$\mathbb{G}_2@f$.
     * @param other Second element in @f$\mathbb{G}_2@f$.
     * @return Reference to the modified element.
     */
    G2Element &operator+=(const G2Element &other);
    /**
     * @brief Adds two elements in @f$\mathbb{G}_2@f$.
     * @param other Second element in @f$\mathbb{G}_2@f$.
     * @return Product element, in @f$\mathbb{G}_2@f$.
     */
    G2Element operator+(const G2Element &other) const;
    /**
     * @brief Multiplies an element of @f$\mathbb{G}_2@f$ with a scalar.
     * @param s Scalar used in the multiplication.
     * @param e Element to multiply the scalar.
     * @return Result of the multiplication.
     */
    friend G2Element operator*(const FpElement &s, const G2Element &e);
private:
    inline G2Element(std::shared_ptr<G2Data> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_2@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_2@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The variable element.
     */
    friend G2Element G2Var(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_2@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_2@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The constant element.
     */
    friend G2Element G2Const(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_2@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_2@f$.
     * @return The constant element.
     */
    friend G2Element G2Const(G2 value);
    /**
     * @brief Creates the special constant that is the base element of
     *   @f$\mathbb{G}_2@f$.
     * @return The base element.
     */
    friend G2Element G2Base();
    /* Pre-definition */
    friend GTElement e(const G1Element &a, const G2Element &b);
private:
    std::shared_ptr<G2Data> data;
};

/**
 * @brief A @f$\mathbb{G}_T@f$ element inside an equation.
 */
class GTElement {
    friend class NIZKProof;
public:
    /**
     * @brief Constructs an invalid element of @f$\mathbb{G}_T@f$.
     */
    inline GTElement();
    /**
     * @brief Multiplies two elements in @f$\mathbb{G}_T@f$.
     * @param other Second element in @f$\mathbb{G}_T@f$.
     * @return Reference to the modified element.
     */
    GTElement &operator*=(const GTElement &other);
    /**
     * @brief Multiplies two elements in @f$\mathbb{G}_T@f$.
     * @param other Second element in @f$\mathbb{G}_T@f$.
     * @return Product element, in @f$\mathbb{G}_T@f$.
     */
    GTElement operator*(const GTElement &other) const;
    /**
     * @brief Computes a pairing (bilinear map).
     * @param a Element of @f$\mathbb{G}_1@f$.
     * @param b Element of @f$\mathbb{G}_2@f$.
     * @return Result of the pairing @f$e(a,b)@f$ in @f$\mathbb{G}_T@f$.
     */
    friend GTElement e(const G1Element &a, const G2Element &b);
private:
    inline GTElement(std::shared_ptr<GTData> d);
public:
    /**
     * @brief Creates a variable in @f$\mathbb{G}_T@f$ with index @a index.
     * @param index Index of the variable in @f$\mathbb{G}_T@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The variable element.
     */
    friend GTElement GTVar(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_T@f$ with index @a index.
     * @param index Index of the constant in @f$\mathbb{G}_T@f$.
     * @note Indexes start at 0, are group and variable/constant dependent,
     *   and should not skip any value.
     * @return The constant element.
     */
    friend GTElement GTConst(int index);
    /**
     * @brief Creates a constant in @f$\mathbb{G}_T@f$ with a specific value.
     * @param value Value of the constant in @f$\mathbb{G}_T@f$.
     * @return The constant element.
     */
    friend GTElement GTConst(GT value);
    /**
     * @brief Creates the special constant that is the base element of
     *   @f$\mathbb{G}_T@f$.
     * @note This is the pairing product of the two base elements of
     *   @f$\mathbb{G}_1@f$ and @f$\mathbb{G}_2@f$.
     * @return The base element.
     */
    friend GTElement GTBase();
private:
    std::shared_ptr<GTData> data;
};

/**
 * @brief The ProofData structure.
 *
 * This structure holds the values of public constants and
 * private variables in the equations, in order to either
 * check a solution of generate a NIZK proof.
 */
struct ProofData {
    /**
     * @brief The public constant values in @f$\mathbb{F}_p@f$.
     */
    std::vector<Fp> pubFp;
    /**
     * @brief The private variable values in @f$\mathbb{F}_p@f$.
     */
    std::vector<Fp> privFp;
    /**
     * @brief The public constant values in @f$\mathbb{G}_1@f$.
     */
    std::vector<G1> pubG1;
    /**
     * @brief The private variable values in @f$\mathbb{G}_1@f$.
     */
    std::vector<G1> privG1;
    /**
     * @brief The public constant values in @f$\mathbb{G}_2@f$.
     */
    std::vector<G2> pubG2;
    /**
     * @brief The private variable values in @f$\mathbb{G}_2@f$.
     */
    std::vector<G2> privG2;
    /**
     * @brief The public constant values in @f$\mathbb{G}_T@f$.
     */
    std::vector<GT> pubGT;
};

/**
 * @brief The main class that generates and verifies NIZK proofs.
 */
class NIZKProof {
public:
    enum CommitType {
        NormalCommit = 0,
        SelectedEncryption = 1,
        AllEncrypted = 2
    };
public:
    /**
     * @brief Contructs a new NIZKProof object ready to accept equations.
     * @param type The desired type of commitments.
     */
    inline NIZKProof(CommitType type = CommitType::SelectedEncryption);
    /**
     * @brief Constructs a deep copy of another NIZKProof object.
     * @param other The NIZKProof object to copy.
     */
    NIZKProof(const NIZKProof &other);
    /**
     * @brief Contructs a new NIZKProof object from an input stream.
     * @param stream Input stream.
     * @note The created system of equations will be fixed,
     *   as if @ref endEquations() had been called.
     * @sa operator<<(std::ostream&,const NIZKProof&)
     */
    NIZKProof(std::istream &stream);
    /**
     * @brief Appends an equation in @f$\mathbb{F}_p@f$ to the proof.
     * @param leftHandSide Left hand side of the equation.
     * @param rightHandSide Right hand side of the equation,
     *   0 by default.
     */
    void addEquation(const FpElement &leftHandSide,
                     const FpElement &rightHandSide = FpElement());
    /**
     * @brief Appends an equation in @f$\mathbb{G}_1@f$ to the proof.
     * @param leftHandSide Left hand side of the equation.
     * @param rightHandSide Right hand side of the equation,
     *   @f$\mathcal{O}@f$ by default.
     */
    void addEquation(const G1Element &leftHandSide,
                     const G1Element &rightHandSide = G1Element());
    /**
     * @brief Appends an equation in @f$\mathbb{G}_2@f$ to the proof.
     * @param leftHandSide Left hand side of the equation.
     * @param rightHandSide Right hand side of the equation,
     *   @f$\mathcal{O}@f$ by default.
     */
    void addEquation(const G2Element &leftHandSide,
                     const G2Element &rightHandSide = G2Element());
    /**
     * @brief Appends an equation in @f$\mathbb{G}_T@f$ to the proof.
     * @param leftHandSide Left hand side of the equation.
     * @param rightHandSide Right hand side of the equation,
     *   1 by default.
     */
    void addEquation(const GTElement &leftHandSide,
                     const GTElement &rightHandSide = GTElement());
    /**
     * @brief Ends the equation listing step.
     *
     * After calling this function, no more equations can be appended
     * to the future proof.
     * It will calculate the number of constants and variables in each
     * group, and check that there is no index gap.
     * If the selected encryption mode has been selected,
     * this function will also calculate the set of variables to encrypt.
     *
     * @return `true` if the indexes are consistent, `false` otherwise.
     */
    bool endEquations();
    /**
     * @brief Performs a deep copy of the NIZKProof object @a other.
     * @param other NIZKProof object to copy.
     * @return Reference to the copy.
     */
    NIZKProof &operator=(const NIZKProof &other);
    /**
     * @brief Writes this system of equations to an output stream.
     * @warning You need to call the function @ref endEquations()
     *   before calling this function, or else nothing will be
     *   written to the stream.
     * @param stream Output stream.
     * @param p The system of equations to write in the stream.
     * @return Reference to the output stream.
     * @sa NIZKProof::endEquations()
     * @sa operator>>(std::ostream&,NIZKProof&)
     * @sa NIZKProof::NIZKProof(std::istream&)
     */
    friend std::ostream &operator<<(std::ostream &stream, const NIZKProof &p);
    /**
     * @brief Reads a system of equations from an input stream.
     * @warning The previous value of @a p will be completely erased.
     * @note The created system of equations will be fixed,
     *   as if @ref endEquations() had been called.
     * @param stream Input stream.
     * @param p System of equations to read from the stream.
     * @return Reference to the input stream.
     * @sa operator<<(std::ostream&,const NIZKProof&)
     */
    friend std::istream &operator>>(std::istream &stream, NIZKProof &p);
    /**
     * @brief Verifies that a given couple of values are solution of the
     *   equations.
     * @note The user should call the function @ref endEquations()
     *   before calling this function.
     * @param instantiation Instantiation values for the constants and
     *   variables.
     * @param crs The Common Reference String that is used for the
     *   base elements. A dummy CRS may be used in cases where no
     *   base element is used (excluding @f$\mathbb{F}_p@f$'s base element).
     * @note This function returns false if the instantiation vectors do
     *   not have the required size.
     * @return `true` if the values are a solution to the equations,
     *   `false` otherwise.
     * @sa NIZKProof::endEquations()
     */
    bool verifySolution(const ProofData &instantiation,
                        const CRS &crs = CRS()) const;
    /**
     * @brief Writes a NIZK proof to a stream.
     * @warning The user should call the function @ref endEquations()
     *   before calling this function.
     * @warning It is assumed that the given values are a solution of the
     *   equations for the proof to be correct. This can be checked with
     *   the method @ref verifySolution(). If the instantiation size is not
     *   correct, the function leads to undefined behavior.
     * @param stream Output stream to which the NIZK proof shall be written.
     * @param crs Common Reference String to use for this proof.
     * @param instantiation Instantiation values for the constants and
     *   variables.
     * @sa NIZKProof::endEquations()
     * @sa NIZKProof::checkProof(std::istream&,const CRS&, const ProofData&)
     * @sa NIZKProof::verifySolution(const ProofData&,const CRS&)
     */
    void writeProof(std::ostream &stream, const CRS &crs,
                    const ProofData &instantiation) const;
    /**
     * @brief Checks a NIZK proof from a stream.
     * @warning The user should call the function @ref endEquations()
     *   before calling this function.
     * @param stream Input stream from which the NIZK proof is to be read.
     * @param crs Common Reference String to use for this proof.
     * @param instantiation Instantiation values for the constants.
     * @note The instantiation vectors for the variables are ignored.
     * @return `true` if the NIZK proof verifies, `false` otherwise.
     * @sa NIZKProof::endEquations()
     * @sa NIZKProof::writeProof(std::ostream&,const CRS&, const ProofData&)
     */
    bool checkProof(std::istream &stream, const CRS &crs,
                    const ProofData &instantiation) const;
    /**
     * @brief Checks if the system of equations is Zero-Knowledge.
     * @note This function always returns `false` if the system has not
     *   been fixed yet, i.e. if the function @ref endEquations() has
     *   not been called yet.
     * @return `true` if it is, `false` otherwise.
     * @sa NIZKProof::endEquations()
     */
    inline bool isZeroKnowledge();
    /**
     * @brief Writes a simulated proof to a stream.
     * @warning The user should call the function @ref endEquations()
     *   before calling this function.
     * @warning The system of equations is supposed to be Zero-Knowledge.
     *   This can be checked with the function @ref isZeroKnowledge().
     * @param stream Output stream to which the NIZK proof shall be written.
     * @param crs Common Reference String that is assumed to be a
     *   private hiding one. If it is not, nothing will be written to the
     *   stream.
     * @param instantiation Instantiation values for the constants.
     * @note The instantiation vectors for the variables are ignored.
     * @sa NIZKProof::endEquations()
     * @sa NIZKProof::isZeroKnowledge()
     * @sa CRS::isSimulationReady()
     * @sa NIZKProof::checkProof(std::istream&,const CRS&, const ProofData&)
     */
    void simulateProof(std::ostream &stream, const CRS &crs,
                       const ProofData &instantiation) const;
private:
    bool checkInstantiation(const ProofData &instantiation) const;
    void getIndexes(std::shared_ptr<FpData> &d);
    void getIndexes(std::shared_ptr<G1Data> &d);
    void getIndexes(std::shared_ptr<G2Data> &d);
    void getIndexes(std::shared_ptr<GTData> &d);
    void checkoutAsFp(std::shared_ptr<FpData> &d);
    void checkoutAsG1(std::shared_ptr<G1Data> &d);
    void checkoutAsG2(std::shared_ptr<G2Data> &d);
    void checkoutAsGT(std::shared_ptr<GTData> &d);
    void checkoutLeft(std::shared_ptr<FpData> &d);
    void checkoutLeft(std::shared_ptr<G1Data> &d);
    void checkoutRight(std::shared_ptr<FpData> &d);
    void checkoutRight(std::shared_ptr<G2Data> &d);
    Fp real_eval(const FpData &d, const ProofData &instantiation,
                 const CRS &crs) const;
    G1 real_eval(const G1Data &d, const ProofData &instantiation,
                 const CRS &crs) const;
    G2 real_eval(const G2Data &d, const ProofData &instantiation,
                 const CRS &crs) const;
    GT real_eval(const GTData &d, const ProofData &instantiation,
                 const CRS &crs) const;
    void readFromStream(std::istream &stream, std::shared_ptr<FpData> &dp,
                        int side);
    void readFromStream(std::istream &stream, std::shared_ptr<G1Data> &dp);
    void readFromStream(std::istream &stream, std::shared_ptr<G2Data> &dp);
    void readFromStream(std::istream &stream, std::shared_ptr<GTData> &dp);
    std::pair<ElTypeSet,ElTypeSet> getPType(const FpData &d);
    std::pair<ElTypeSet,ElTypeSet> getPType(const G1Data &d);
    std::pair<ElTypeSet,ElTypeSet> getPType(const G2Data &d);
    std::pair<ElTypeSet,ElTypeSet> getPType(const GTData &d);
    ElTypeSet getPTLeft(const FpData &d);
    ElTypeSet getPTLeft(const G1Data &d);
    ElTypeSet getPTRight(const FpData &d);
    ElTypeSet getPTRight(const G2Data &d);
    void writeEqProof(std::ostream &stream, const void *leftp,
                      const void *rightp, EqProofType expectedType,
                      const CRS &crs) const;
    void getEqProofTypes();
    BT getRndProofPart(std::istream &stream, EqProofType t,
                       const CRS &crs) const;
    void getProofZK(const FpData &d, const CRS &crs, EqProofType t) const;
    void getProofZK(const G1Data &d, const CRS &crs, EqProofType t) const;
    void getProofZK(const G2Data &d, const CRS &crs, EqProofType t) const;
    void getProofZK(const GTData &d, const CRS &crs, EqProofType t) const;
    void getLeftZK(const FpData &d, const CRS &crs, EqProofType t) const;
    void getLeftZK(const G1Data &d, const CRS &crs, EqProofType t) const;
    void getRightZK(const FpData &d, const CRS &crs, EqProofType t) const;
    void getRightZK(const G2Data &d, const CRS &crs, EqProofType t) const;
private:
    CommitType type;
    bool zk;
    std::vector<PairFp> eqsFp;
    std::vector<PairG1> eqsG1;
    std::vector<PairG2> eqsG2;
    std::vector<PairGT> eqsGT;
    bool fixed;
    std::vector< std::shared_ptr<FpData> > varsFp, cstsFp;
    std::vector< std::shared_ptr<G1Data> > varsG1, cstsG1;
    std::vector< std::shared_ptr<G2Data> > varsG2, cstsG2;
    std::vector< std::shared_ptr<GTData> > cstsGT;
    std::vector<bool> varsFpInB1, cstsFpInB1;
    std::vector<int> sEnc[2];
    std::vector<EqProofType> tFp, tG1, tG2, tGT;
    std::vector<AdditionalFp> additionalFp;
    std::vector<AdditionalG1> additionalG1;
    std::vector<AdditionalG2> additionalG2;
};

/**
 * @todo Continue and finish the proof system.
 */

/* Inline definitions: */

inline FpElement::FpElement() {}

inline FpElement::FpElement(std::shared_ptr<FpData> d) : data(d) {}

inline G1Element::G1Element() {}

inline G1Element::G1Element(std::shared_ptr<G1Data> d) : data(d) {}

inline G2Element::G2Element() {}

inline G2Element::G2Element(std::shared_ptr<G2Data> d) : data(d) {}

inline GTElement::GTElement() {}

inline GTElement::GTElement(std::shared_ptr<GTData> d) : data(d) {}

inline NIZKProof::NIZKProof(CommitType type)
    : type(type), zk(false), fixed(false) {}

inline bool NIZKProof::isZeroKnowledge() { return zk; }

} /* End of namespace nizk */

#endif /* End of GSNIZK_H */
