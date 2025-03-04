/** @file
 *****************************************************************************
 Declaration of arithmetic in the finite field F[p], for prime p of fixed
 length.
 *****************************************************************************
 * @author     This file is part of libff, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef FP_HPP_
#define FP_HPP_

#include <libff/algebra/exponentiation/exponentiation.hpp>
#include <libff/algebra/fields/bigint.hpp>

namespace libff
{

template<mp_size_t n, const bigint<n> &modulus> class Fp_model;

template<mp_size_t n, const bigint<n> &modulus>
std::ostream &operator<<(std::ostream &, const Fp_model<n, modulus> &);

template<mp_size_t n, const bigint<n> &modulus>
std::istream &operator>>(std::istream &, Fp_model<n, modulus> &);

/// Arithmetic in the finite field F[p], for prime p of fixed length.
///
/// This class implements Fp-arithmetic, for a large prime p, using a fixed
/// number of words. It is optimized for tight memory consumption, so the
/// modulus p is passed as a template parameter, to avoid per-element overheads.
///
/// The implementation is mostly a wrapper around GMP's MPN (constant-size
/// integers). But for the integer sizes of interest for libff (3 to 5 limbs of
/// 64 bits each), we implement performance-critical routines, like addition and
/// multiplication, using hand-optimzied assembly code.
template<mp_size_t n, const bigint<n> &modulus> class Fp_model
{
public:
    typedef Fp_model<n, modulus> my_Fp;
    typedef bigint<n> bigint_t;
    bigint<n> mont_repr;

    static void static_init();

    static const mp_size_t num_limbs = n;
    static const constexpr bigint<n> &mod = modulus;
#ifdef PROFILE_OP_COUNTS
    static long long add_cnt;
    static long long sub_cnt;
    static long long mul_cnt;
    static long long sqr_cnt;
    static long long inv_cnt;
#endif

    /// The "base"/"ground" field
    static const size_t tower_extension_degree = 1;

    static size_t num_bits;
    /// (modulus-1)/2
    static bigint<n> euler;
    /// modulus = 2^s * t + 1
    static size_t s;
    /// with t odd
    static bigint<n> t;
    /// (t-1)/2
    static bigint<n> t_minus_1_over_2;
    /// a quadratic nonresidue
    static Fp_model<n, modulus> nqr;
    /// nqr^t
    static Fp_model<n, modulus> nqr_to_t;
    /// generator of Fp^*
    static Fp_model<n, modulus> multiplicative_generator;
    /// generator^((modulus-1)/2^s)
    static Fp_model<n, modulus> root_of_unity;
    /// -modulus^(-1) mod W, where W = 2^(word size)
    static mp_limb_t inv;
    /// R^2, where R = W^k, where k = ??
    static bigint<n> Rsquared;
    /// R^3
    static bigint<n> Rcubed;

    static bool modulus_is_valid()
    {
        return modulus.data[n - 1] != 0;
    } // mpn inverse assumes that highest limb is non-zero

    Fp_model(){};
    Fp_model(const bigint<n> &b);
    Fp_model(const long x, const bool is_unsigned = false);

    void set_ulong(const unsigned long x);

    void mul_reduce(const bigint<n> &other);

    void clear();

    /// Return the standard (not Montgomery) representation of the Field
    /// element's requivalence class. I.e. Fp(2).as_bigint() would return
    /// bigint(2)
    bigint<n> as_bigint() const;
    /// Return the last limb of the standard representation of the field
    /// element. E.g. on 64-bit architectures Fp(123).as_ulong() and
    /// Fp(2^64+123).as_ulong() would both return 123.
    unsigned long as_ulong() const;

    bool operator==(const Fp_model &other) const;
    bool operator!=(const Fp_model &other) const;
    bool is_zero() const;

    void print() const;

    Fp_model &operator+=(const Fp_model &other);
    Fp_model &operator-=(const Fp_model &other);
    Fp_model &operator*=(const Fp_model &other);
    Fp_model &operator^=(const unsigned long pow);

    template<mp_size_t m> Fp_model &operator^=(const bigint<m> &pow);

    Fp_model operator+(const Fp_model &other) const;
    Fp_model operator-(const Fp_model &other) const;
    Fp_model operator*(const Fp_model &other) const;
    Fp_model operator-() const;
    Fp_model squared() const;
    Fp_model &invert();
    Fp_model inverse() const;
    /// HAS TO BE A SQUARE (else does not terminate)
    Fp_model sqrt() const;

    Fp_model operator^(const unsigned long pow) const;
    template<mp_size_t m> Fp_model operator^(const bigint<m> &pow) const;

    static size_t size_in_bits() { return num_bits; }
    static size_t capacity() { return num_bits - 1; }
    static const bigint<n> &field_char() { return modulus; }
    static constexpr size_t extension_degree() { return 1; }

    static const Fp_model<n, modulus> &zero();
    static const Fp_model<n, modulus> &one();

    /// returns random element of Fp_model
    static Fp_model<n, modulus> random_element();

    /// generator^k, for k = 1 to m, domain size m
    static Fp_model<n, modulus> geometric_generator();

    /// generator++, for k = 1 to m, domain size m
    static Fp_model<n, modulus> arithmetic_generator();

protected:
    static bool s_initialized;
    static Fp_model<n, modulus> s_zero;
    static Fp_model<n, modulus> s_one;

    friend std::ostream &operator<<<n, modulus>(
        std::ostream &out, const Fp_model<n, modulus> &p);
    friend std::istream &operator>>
        <n, modulus>(std::istream &in, Fp_model<n, modulus> &p);
};

#ifdef PROFILE_OP_COUNTS
template<mp_size_t n, const bigint<n> &modulus>
long long Fp_model<n, modulus>::add_cnt = 0;

template<mp_size_t n, const bigint<n> &modulus>
long long Fp_model<n, modulus>::sub_cnt = 0;

template<mp_size_t n, const bigint<n> &modulus>
long long Fp_model<n, modulus>::mul_cnt = 0;

template<mp_size_t n, const bigint<n> &modulus>
long long Fp_model<n, modulus>::sqr_cnt = 0;

template<mp_size_t n, const bigint<n> &modulus>
long long Fp_model<n, modulus>::inv_cnt = 0;
#endif

template<mp_size_t n, const bigint<n> &modulus>
size_t Fp_model<n, modulus>::num_bits;

template<mp_size_t n, const bigint<n> &modulus>
bigint<n> Fp_model<n, modulus>::euler;

template<mp_size_t n, const bigint<n> &modulus> size_t Fp_model<n, modulus>::s;

template<mp_size_t n, const bigint<n> &modulus>
bigint<n> Fp_model<n, modulus>::t;

template<mp_size_t n, const bigint<n> &modulus>
bigint<n> Fp_model<n, modulus>::t_minus_1_over_2;

template<mp_size_t n, const bigint<n> &modulus>
Fp_model<n, modulus> Fp_model<n, modulus>::nqr;

template<mp_size_t n, const bigint<n> &modulus>
Fp_model<n, modulus> Fp_model<n, modulus>::nqr_to_t;

template<mp_size_t n, const bigint<n> &modulus>
Fp_model<n, modulus> Fp_model<n, modulus>::multiplicative_generator;

template<mp_size_t n, const bigint<n> &modulus>
Fp_model<n, modulus> Fp_model<n, modulus>::root_of_unity;

template<mp_size_t n, const bigint<n> &modulus>
mp_limb_t Fp_model<n, modulus>::inv;

template<mp_size_t n, const bigint<n> &modulus>
bigint<n> Fp_model<n, modulus>::Rsquared;

template<mp_size_t n, const bigint<n> &modulus>
bigint<n> Fp_model<n, modulus>::Rcubed;

} // namespace libff

#include <libff/algebra/fields/fp.tcc>

#endif // FP_HPP_
