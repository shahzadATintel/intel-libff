/** @file
 *****************************************************************************
 * @author     This file is part of libff, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef BLS12_381_G1_HPP_
#define BLS12_381_G1_HPP_
#include <libff/algebra/curves/bls12_381/bls12_381_init.hpp>
#include <libff/algebra/curves/curve_utils.hpp>
#include <vector>
static long int _add_ec_point_count=0;
namespace libff
{

class bls12_381_G1;
std::ostream &operator<<(std::ostream &, const bls12_381_G1 &);
std::istream &operator>>(std::istream &, bls12_381_G1 &);

class bls12_381_G1
{
public:
#ifdef PROFILE_OP_COUNTS
    static long long add_cnt;
    static long long dbl_cnt;
#endif
    static std::vector<std::size_t> wnaf_window_table;
    static std::vector<std::size_t> fixed_base_exp_window_table;
    static bls12_381_G1 G1_zero;
    static bls12_381_G1 G1_one;
    static bls12_381_Fq coeff_a;
    static bls12_381_Fq coeff_b;

    typedef bls12_381_Fq base_field;
    typedef bls12_381_Fr scalar_field;

    // Cofactor
    static const mp_size_t h_bitcount = 126;
    static const mp_size_t h_limbs =
        (h_bitcount + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS;
    static bigint<h_limbs> h;

    bls12_381_Fq X, Y, Z;

    // using Jacobian coordinates
    bls12_381_G1();
    bls12_381_G1(
        const bls12_381_Fq &X, const bls12_381_Fq &Y, const bls12_381_Fq &Z)
        : X(X), Y(Y), Z(Z){};

    void print() const;
    void print_coordinates() const;

    void to_affine_coordinates();
    void to_special();
    bool is_special() const;

    bool is_zero() const;

    bool operator==(const bls12_381_G1 &other) const;
    bool operator!=(const bls12_381_G1 &other) const;

    bls12_381_G1 operator+(const bls12_381_G1 &other) const;
    bls12_381_G1 operator-() const;
    bls12_381_G1 operator-(const bls12_381_G1 &other) const;

    bls12_381_G1 add(const bls12_381_G1 &other) const;
    bls12_381_G1 mixed_add(const bls12_381_G1 &other) const;
    bls12_381_G1 dbl() const;
    bls12_381_G1 mul_by_cofactor() const;

    bool is_well_formed() const;
    bool is_in_safe_subgroup() const;

    static const bls12_381_G1 &zero();
    static const bls12_381_G1 &one();
    static bls12_381_G1 random_element();

    static std::size_t size_in_bits() { return base_field::size_in_bits() + 1; }
    static bigint<base_field::num_limbs> base_field_char()
    {
        return base_field::field_char();
    }
    static bigint<scalar_field::num_limbs> order()
    {
        return scalar_field::field_char();
    }

    void write_uncompressed(std::ostream &) const;
    void write_compressed(std::ostream &) const;
    static void read_uncompressed(std::istream &, bls12_381_G1 &);
    static void read_compressed(std::istream &, bls12_381_G1 &);

    static void batch_to_special_all_non_zeros(std::vector<bls12_381_G1> &vec);
};

template<mp_size_t m>
bls12_381_G1 operator*(const bigint<m> &lhs, const bls12_381_G1 &rhs)
{
    return scalar_mul<bls12_381_G1, m>(rhs, lhs);
}

template<mp_size_t m, const bigint<m> &modulus_p>
bls12_381_G1 operator*(
    const Fp_model<m, modulus_p> &lhs, const bls12_381_G1 &rhs)
{
    return scalar_mul<bls12_381_G1, m>(rhs, lhs.as_bigint());
}

} // namespace libff

#endif // BLS12_381_G1_HPP_
