//
// Copyright (C) 2011-14 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <complex>
#include <iostream>
#include <stdexcept>
#include "inc_gtest.hpp"

#include <dynd/type.hpp>
#include <dynd/types/ndarrayarg_type.hpp>

using namespace std;
using namespace dynd;

TEST(Type, NDTTypeObject) {
  // The ndt::type just contains one base_type *
  EXPECT_EQ(sizeof(base_type *), sizeof(ndt::type));
}

TEST(Type, BasicConstructor) {
    ndt::type d;

    // Default-constructed type properties
    EXPECT_EQ(uninitialized_type_id, d.get_type_id());
    EXPECT_EQ(void_kind, d.get_kind());
    EXPECT_EQ(1u, d.get_data_alignment());
    EXPECT_EQ(0u, d.get_data_size());
    EXPECT_TRUE(d.is_builtin());

    // void type
    d = ndt::type(void_type_id);
    EXPECT_EQ(void_type_id, d.get_type_id());
    EXPECT_EQ(void_kind, d.get_kind());
    EXPECT_EQ(1u, d.get_data_alignment());
    EXPECT_EQ(0u, d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // bool type
    d = ndt::type(bool_type_id);
    EXPECT_EQ(bool_type_id, d.get_type_id());
    EXPECT_EQ(bool_kind, d.get_kind());
    EXPECT_EQ(1u, d.get_data_alignment());
    EXPECT_EQ(1u, d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // int8 type
    d = ndt::type(int8_type_id);
    EXPECT_EQ(int8_type_id, d.get_type_id());
    EXPECT_EQ(int_kind, d.get_kind());
    EXPECT_EQ(1u, d.get_data_alignment());
    EXPECT_EQ(1u, d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // int16 type
    d = ndt::type(int16_type_id);
    EXPECT_EQ(int_kind, d.get_kind());
    EXPECT_EQ(2u, d.get_data_alignment());
    EXPECT_EQ(2u, d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // int32 type
    d = ndt::type(int32_type_id);
    EXPECT_EQ(int32_type_id, d.get_type_id());
    EXPECT_EQ(int_kind, d.get_kind());
    EXPECT_EQ(4u, d.get_data_alignment());
    EXPECT_EQ(4u, d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // int
    d = ndt::make_type<int>();
    EXPECT_EQ(int32_type_id, d.get_type_id());
    EXPECT_EQ(int_kind, d.get_kind());
    EXPECT_EQ(sizeof(int), d.get_data_alignment());
    EXPECT_EQ(sizeof(int), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // long
    d = ndt::make_type<long>();
    EXPECT_EQ(int_kind, d.get_kind());
    EXPECT_EQ(sizeof(long), d.get_data_alignment());
    EXPECT_EQ(sizeof(long), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // long long
    d = ndt::make_type<long long>();
    EXPECT_EQ(int64_type_id, d.get_type_id());
    EXPECT_EQ(int_kind, d.get_kind());
    EXPECT_EQ((size_t)scalar_align_of<long long>::value, d.get_data_alignment());
    EXPECT_EQ(sizeof(long long), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // unsigned int
    d = ndt::make_type<unsigned int>();
    EXPECT_EQ(uint32_type_id, d.get_type_id());
    EXPECT_EQ(uint_kind, d.get_kind());
    EXPECT_EQ(sizeof(unsigned int), d.get_data_alignment());
    EXPECT_EQ(sizeof(unsigned int), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // unsigned long
    d = ndt::make_type<unsigned long>();
    EXPECT_EQ(uint_kind, d.get_kind());
    EXPECT_EQ(sizeof(unsigned long), d.get_data_alignment());
    EXPECT_EQ(sizeof(unsigned long), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // unsigned long long
    d = ndt::make_type<unsigned long long>();
    EXPECT_EQ(uint64_type_id, d.get_type_id());
    EXPECT_EQ(uint_kind, d.get_kind());
    EXPECT_EQ((size_t)scalar_align_of<unsigned long long>::value, d.get_data_alignment());
    EXPECT_EQ(sizeof(unsigned long long), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // float
    d = ndt::make_type<float>();
    EXPECT_EQ(float32_type_id, d.get_type_id());
    EXPECT_EQ(real_kind, d.get_kind());
    EXPECT_EQ(sizeof(float), d.get_data_alignment());
    EXPECT_EQ(sizeof(float), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));

    // double
    d = ndt::make_type<double>();
    EXPECT_EQ(float64_type_id, d.get_type_id());
    EXPECT_EQ(real_kind, d.get_kind());
    EXPECT_EQ((size_t)scalar_align_of<double>::value, d.get_data_alignment());
    EXPECT_EQ(sizeof(double), d.get_data_size());
    EXPECT_TRUE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));
}

TEST(Type, NDArrayArg) {
    // Minimal test of the ndarrayarg type
    ndt::type d = ndt::make_ndarrayarg();
    EXPECT_EQ(ndarrayarg_type_id, d.get_type_id());
    EXPECT_EQ(dynamic_kind, d.get_kind());
    EXPECT_EQ((size_t)scalar_align_of<void *>::value, d.get_data_alignment());
    EXPECT_EQ(sizeof(void *), d.get_data_size());
    EXPECT_FALSE(d.is_builtin());
    // Roundtripping through a string
    EXPECT_EQ(d, ndt::type(d.str()));
}
