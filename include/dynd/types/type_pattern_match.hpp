//
// Copyright (C) 2011-14 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <map>

#include <dynd/type.hpp>
#include <dynd/string.hpp>

namespace dynd { namespace ndt {

/**
 * Matches the provided concrete type against the pattern type, which may
 * include type vars. Returns true if it matches, false otherwise.
 *
 * This version may be called multiple times in a row, building up the
 * typevars dictionary which is used to enforce consistent usage of
 * type vars.
 *
 * \param concrete  A concrete type to match against the symbolic ``pattern``.
 * \param pattern  A symbolic (or concrete) type to match against.
 * \param typevars  A map of names to matched type vars. If the type
 *                  has ``ndim`` == 0, it is a scalar type var match, if it
 *                  is a dim_fragment, it is an ellipsis type var match,
 *                  otherwise it is a dim type var match, and the first
 *                  dimension of the type only is what is relevant.
 */
bool pattern_match(const ndt::type &concrete, const ndt::type &pattern,
                   std::map<nd::string, ndt::type> &typevars);

/**
 * Matches the dimensions of the provided concrete type against the
 * dimensions of the pattern type, which may include type vars. Returns
 * true if it matches, false otherwise.
 *
 * When true is returned, the dtypes are returned in the
 * provided output references.
 *
 * This version may be called multiple times in a row, building up the
 * typevars dictionary which is used to enforce consistent usage of
 * type vars.
 *
 * \param concrete  A concrete type to match against the symbolic ``pattern``.
 * \param pattern  A symbolic (or concrete) type to match against.
 * \param typevars  A map of names to matched type vars. If the type
 *                  has ``ndim`` == 0, it is a scalar type var match, if it
 *                  is a dim_fragment, it is an ellipsis type var match,
 *                  otherwise it is a dim type var match, and the first
 *                  dimension of the type only is what is relevant.
 */
bool pattern_match_dims(const ndt::type &concrete, const ndt::type &pattern,
                        std::map<nd::string, ndt::type> &typevars,
                        ndt::type &out_concrete_dtype,
                        ndt::type &out_pattern_dtype);

/**
 * Matches the provided concrete type against the pattern type, which may
 * include type vars. Returns true if it matches, false otherwise.
 */
inline bool pattern_match(const ndt::type &concrete, const ndt::type &pattern)
{
  if (concrete.extended() == pattern.extended()) {
    return true;
  } else {
    std::map<nd::string, ndt::type> typevars;
    return pattern_match(concrete, pattern, typevars);
  }
}
}} // namespace dynd::ndt
