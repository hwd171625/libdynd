//
// Copyright (C) 2011-14 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <iostream>
#include <stdexcept>
#include "inc_gtest.hpp"

#include <dynd/types/type_pattern_match.hpp>
#include <dynd/types/substitute_typevars.hpp>
#include <dynd/types/substitute_shape.hpp>
#include <dynd/types/dim_fragment_type.hpp>
#include <dynd/types/typevar_dim_type.hpp>

using namespace std;
using namespace dynd;

TEST(SubstituteTypeVars, SimpleNoSubstitutions)
{
  map<nd::string, ndt::type> typevars;
  EXPECT_EQ(ndt::type("int32"),
            ndt::substitute(ndt::type("int32"), typevars, false));
  EXPECT_EQ(ndt::type("int32"),
            ndt::substitute(ndt::type("int32"), typevars, true));
  EXPECT_EQ(ndt::type("T"), ndt::substitute(ndt::type("T"), typevars, false));
  EXPECT_THROW(ndt::substitute(ndt::type("T"), typevars, true),
               invalid_argument);
  EXPECT_EQ(ndt::type("A... * int32"),
            ndt::substitute(ndt::type("A... * int32"), typevars, false));
  EXPECT_THROW(ndt::substitute(ndt::type("A... * int32"), typevars, true),
               invalid_argument);
}

TEST(SubstituteTypeVars, SimpleSubstitution)
{
  map<nd::string, ndt::type> typevars;
  typevars["Tint"] = ndt::type("int32");
  typevars["Tsym"] = ndt::type("S");
  typevars["Mfixed_sym"] = ndt::type("fixed * void");
  typevars["Mfixed"] = ndt::type("8 * void");
  typevars["Mvar"] = ndt::type("var * void");
  typevars["Msym"] = ndt::type("TV * void");
  typevars["Aempty"] = ndt::make_dim_fragment(0, ndt::make_type<void>());
  typevars["Afixed_sym"] = ndt::make_dim_fragment(1, ndt::type("fixed * void"));
  typevars["Afixed"] = ndt::make_dim_fragment(1, ndt::type("5 * void"));
  typevars["Avar"] = ndt::make_dim_fragment(1, ndt::type("var * void"));
  typevars["Amulti"] =
      ndt::make_dim_fragment(3, ndt::type("fixed * var * 3 * void"));

  typevars["N"] = ndt::type("3 * void");

  EXPECT_EQ(ndt::type("int32"),
            ndt::substitute(ndt::type("Tint"), typevars, false));
  EXPECT_EQ(ndt::type("int32"),
            ndt::substitute(ndt::type("Tint"), typevars, true));
  EXPECT_EQ(ndt::type("?int32"),
            ndt::substitute(ndt::type("?Tint"), typevars, false));
  EXPECT_EQ(ndt::type("?int32"),
            ndt::substitute(ndt::type("?Tint"), typevars, true));
  EXPECT_EQ(ndt::type("S"),
            ndt::substitute(ndt::type("Tsym"), typevars, false));
  EXPECT_THROW(ndt::substitute(ndt::type("Tsym"), typevars, true),
               invalid_argument);
  EXPECT_EQ(ndt::type("fixed * int32"),
            ndt::substitute(ndt::type("Mfixed_sym * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("8 * int32"),
            ndt::substitute(ndt::type("Mfixed * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("8 * int32"),
            ndt::substitute(ndt::type("Mfixed * Tint"), typevars, true));
  EXPECT_EQ(ndt::type("var * int32"),
            ndt::substitute(ndt::type("Mvar * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("var * int32"),
            ndt::substitute(ndt::type("Mvar * Tint"), typevars, true));

  EXPECT_EQ(
      ndt::type("fixed**3 * int32"),
      ndt::substitute(ndt::type("Mfixed_sym**N * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("8**3 * int32"),
            ndt::substitute(ndt::type("Mfixed**N * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("8**3 * int32"),
            ndt::substitute(ndt::type("Mfixed**N * Tint"), typevars, true));
  EXPECT_EQ(ndt::type("var**3 * int32"),
            ndt::substitute(ndt::type("Mvar**N * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("var**3 * int32"),
            ndt::substitute(ndt::type("Mvar**N * Tint"), typevars, true));
  EXPECT_EQ(ndt::type("TV**3 * int32"),
            ndt::substitute(ndt::type("Msym**N * Tint"), typevars, false));

  EXPECT_EQ(
      ndt::type("fixed**X * int32"),
      ndt::substitute(ndt::type("Mfixed_sym**X * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("8**X * int32"),
            ndt::substitute(ndt::type("Mfixed**X * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("var**X * int32"),
            ndt::substitute(ndt::type("Mvar**X * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("TV**X * int32"),
            ndt::substitute(ndt::type("Msym**X * Tint"), typevars, false));
  EXPECT_EQ(ndt::type("8**TV * int32"),
            ndt::substitute(ndt::type("Mfixed**Msym * Tint"), typevars, false));

  EXPECT_EQ(
      ndt::type("var * int32"),
      ndt::substitute(ndt::type("Aempty... * Mvar * Tint"), typevars, false));
  EXPECT_EQ(
      ndt::type("var * int32"),
      ndt::substitute(ndt::type("Mvar * Aempty... * Tint"), typevars, true));
  EXPECT_EQ(ndt::type("fixed * var * int32"),
            ndt::substitute(ndt::type("Afixed_sym... * Mvar * Tint"), typevars,
                            false));
  EXPECT_EQ(ndt::type("var * fixed * int32"),
            ndt::substitute(ndt::type("Mvar * Afixed_sym... * Tint"), typevars,
                            false));
  EXPECT_EQ(
      ndt::type("5 * var * int32"),
      ndt::substitute(ndt::type("Afixed... * Mvar * Tint"), typevars, false));
  EXPECT_EQ(
      ndt::type("var * 5 * int32"),
      ndt::substitute(ndt::type("Mvar * Afixed... * Tint"), typevars, true));
  EXPECT_EQ(
      ndt::type("var * var * int32"),
      ndt::substitute(ndt::type("Avar... * Mvar * Tint"), typevars, false));
  EXPECT_EQ(
      ndt::type("var * var * int32"),
      ndt::substitute(ndt::type("Mvar * Avar... * Tint"), typevars, true));
  EXPECT_EQ(
      ndt::type("fixed * var * 3 * var * int32"),
      ndt::substitute(ndt::type("Amulti... * Mvar * Tint"), typevars, false));
  EXPECT_EQ(
      ndt::type("var * fixed * var * 3 * int32"),
      ndt::substitute(ndt::type("Mvar * Amulti... * Tint"), typevars, false));

  EXPECT_THROW(
      ndt::substitute(ndt::type("Mfixed_sym**N * Tint"), typevars, true),
      invalid_argument);
}

TEST(SubstituteTypeVars, Tuple)
{
  map<nd::string, ndt::type> typevars;
  typevars["T"] = ndt::type("int32");
  typevars["M"] = ndt::type("3 * void");
  typevars["A"] =
      ndt::make_dim_fragment(3, ndt::type("var * fixed * 4 * void"));

  EXPECT_EQ(ndt::type("(int, real)"),
            ndt::substitute(ndt::type("(int, real)"), typevars, false));
  EXPECT_EQ(ndt::type("(int, real)"),
            ndt::substitute(ndt::type("(int, real)"), typevars, true));
  EXPECT_EQ(ndt::type("(int32, 3 * real)"),
            ndt::substitute(ndt::type("(T, M * real)"), typevars, false));
  EXPECT_EQ(ndt::type("(int32, 3 * real)"),
            ndt::substitute(ndt::type("(T, M * real)"), typevars, true));

  EXPECT_EQ(
      ndt::type("(var * fixed * 4 * int32, 3 * real)"),
      ndt::substitute(ndt::type("(A... * T, M * real)"), typevars, false));
  EXPECT_EQ(ndt::type("(var * fixed * 4 * int32, 3 * real)"),
            ndt::substitute(ndt::type("(A... * T, M * real)"), typevars, true));
}

TEST(SubstituteTypeVars, Struct)
{
  map<nd::string, ndt::type> typevars;
  typevars["T"] = ndt::type("int32");
  typevars["M"] = ndt::type("3 * void");
  typevars["A"] =
      ndt::make_dim_fragment(3, ndt::type("var * fixed * 4 * void"));

  EXPECT_EQ(ndt::type("{x: int, y: real}"),
            ndt::substitute(ndt::type("{x: int, y: real}"), typevars, false));
  EXPECT_EQ(ndt::type("{x: int, y: real}"),
            ndt::substitute(ndt::type("{x: int, y: real}"), typevars, true));
  EXPECT_EQ(ndt::type("{x: int32, y: 3 * real}"),
            ndt::substitute(ndt::type("{x: T, y: M * real}"), typevars, false));
  EXPECT_EQ(ndt::type("{x: int32, y: 3 * real}"),
            ndt::substitute(ndt::type("{x: T, y: M * real}"), typevars, true));

  EXPECT_EQ(ndt::type("{x: var * fixed * 4 * int32, y: 3 * real}"),
            ndt::substitute(ndt::type("{x: A... * T, y: M * real}"), typevars,
                            false));
  EXPECT_EQ(
      ndt::type("{x: var * fixed * 4 * int32, y: 3 * real}"),
      ndt::substitute(ndt::type("{x: A... * T, y: M * real}"), typevars, true));
}

TEST(SubstituteTypeVars, FuncProto)
{
  map<nd::string, ndt::type> typevars;
  typevars["T"] = ndt::type("int32");
  typevars["M"] = ndt::type("3 * void");
  typevars["A"] = ndt::make_dim_fragment(3, ndt::type("var * 4 * 9 * void"));

  EXPECT_EQ(
      ndt::type("(int, real) -> complex"),
      ndt::substitute(ndt::type("(int, real) -> complex"), typevars, false));
  EXPECT_EQ(
      ndt::type("(int, real) -> complex"),
      ndt::substitute(ndt::type("(int, real) -> complex"), typevars, true));
  EXPECT_EQ(ndt::type("(int32, 3 * real) -> var * 4 * 9 * complex"),
            ndt::substitute(ndt::type("(T, M * real) -> A... * complex"),
                            typevars, false));
}

TEST(SubstituteShape, Simple)
{
  intptr_t shape[5] = {0, 1, 2, 3, 4};
  EXPECT_EQ(ndt::type("0 * int32"),
            ndt::substitute_shape(ndt::type("fixed * int32"), 1, shape));
  EXPECT_EQ(ndt::type("1 * 2 * T"),
            ndt::substitute_shape(ndt::type("fixed**2 * T"), 2, shape + 1));
  EXPECT_EQ(ndt::type("1 * var * 3 * T"),
            ndt::substitute_shape(ndt::type("fixed * var * fixed * T"), 3,
                                  shape + 1));
  EXPECT_EQ(
      ndt::type("1 * var * 3 * T"),
      ndt::substitute_shape(ndt::type("fixed * var * 3 * T"), 3, shape + 1));
}

TEST(SubstituteShape, Errors)
{
  intptr_t shape[5] = {0, 1, 2, 3, 4};
  // Too many dimensions
  EXPECT_THROW(ndt::substitute_shape(ndt::type("fixed * int32"), 2, shape),
               type_error);
  // Mismatched fixed dimension
  EXPECT_THROW(ndt::substitute_shape(ndt::type("10 * int32"), 1, shape + 1),
               type_error);
}
