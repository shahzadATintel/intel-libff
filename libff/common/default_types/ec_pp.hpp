/** @file
 *****************************************************************************

 This file defines default_ec_pp based on the CURVE=... make flag, which selects
 which elliptic curve is used to implement group arithmetic and pairings.

 *****************************************************************************
 * @author     This file is part of libff, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef EC_PP_HPP_
#define EC_PP_HPP_

/************************ Pick the elliptic curve ****************************/

#ifdef CURVE_BLS12_381
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/bls12_381/bls12_381_pp.hpp>
namespace libff
{
typedef bls12_381_pp default_ec_pp;
} // namespace libff
#endif


#ifdef CURVE_ALT_BN128
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp>
namespace libff
{
typedef alt_bn128_pp default_ec_pp;
} // namespace libff
#endif

#ifdef CURVE_BN128
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/bn128/bn128_pp.hpp>
namespace libff
{
typedef bn128_pp default_ec_pp;
} // namespace libff
#endif

#ifdef CURVE_EDWARDS
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/edwards/edwards_pp.hpp>
namespace libff
{
typedef edwards_pp default_ec_pp;
} // namespace libff
#endif

#ifdef CURVE_MNT4
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/mnt/mnt4/mnt4_pp.hpp>
namespace libff
{
typedef mnt4_pp default_ec_pp;
} // namespace libff
#endif

#ifdef CURVE_MNT6
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/mnt/mnt6/mnt6_pp.hpp>
namespace libff
{
typedef mnt6_pp default_ec_pp;
} // namespace libff
#endif

#ifdef CURVE_BLS12_377
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/bls12_377/bls12_377_pp.hpp>
namespace libff
{
typedef bls12_377_pp default_ec_pp;
} // namespace libff
#endif

#ifdef CURVE_BW6_761
#define LIBFF_DEFAULT_EC_PP_DEFINED
#include <libff/algebra/curves/bw6_761/bw6_761_pp.hpp>
namespace libff
{
typedef bw6_761_pp default_ec_pp;
} // namespace libff
#endif

#ifndef LIBFF_DEFAULT_EC_PP_DEFINED
#error You must define one of the CURVE_* symbols to pick a curve for pairings.
#endif

#endif // EC_PP_HPP_
