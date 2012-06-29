//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <stdint.h>
#include "inc_gtest.hpp"

#include "dnd/ndarray.hpp"
#include "dnd/exceptions.hpp"

using namespace std;
using namespace dnd;

TEST(NDArrayIndex, BasicInteger) {
    int i0[3][2] = {{1,2},{3,4},{5,6}};
    ndarray a = i0;
    ndarray b;

    // Indexing in two steps
    b = a(0);
    EXPECT_EQ(1, b(0).as<int>());
    EXPECT_EQ(2, b(1).as<int>());
    b = a(1);
    EXPECT_EQ(3, b(0).as<int>());
    EXPECT_EQ(4, b(1).as<int>());
    b = a(2);
    EXPECT_EQ(5, b(0).as<int>());
    EXPECT_EQ(6, b(1).as<int>());
    // Python style negative index
    EXPECT_EQ(5, b(-2).as<int>());
    EXPECT_EQ(6, b(-1).as<int>());
    EXPECT_THROW(b(-3), index_out_of_bounds);
    EXPECT_THROW(b(2), index_out_of_bounds);
    EXPECT_THROW(b(0,0), too_many_indices);

    // Indexing in one step
    EXPECT_EQ(1, a(0,0).as<int>());
    EXPECT_EQ(2, a(0,1).as<int>());
    EXPECT_EQ(3, a(1,0).as<int>());
    EXPECT_EQ(4, a(1,1).as<int>());
    EXPECT_EQ(5, a(2,0).as<int>());
    EXPECT_EQ(6, a(2,1).as<int>());
    // Indexing with negative Python style
    EXPECT_EQ(1, a(-3,-2).as<int>());
    EXPECT_EQ(2, a(-3,-1).as<int>());
    EXPECT_EQ(3, a(-2,-2).as<int>());
    EXPECT_EQ(4, a(-2,-1).as<int>());
    EXPECT_EQ(5, a(-1,-2).as<int>());
    EXPECT_EQ(6, a(-1,-1).as<int>());
    EXPECT_THROW(a(-4,0), index_out_of_bounds);
    EXPECT_THROW(a(3,0), index_out_of_bounds);
    EXPECT_THROW(a(0,-3), index_out_of_bounds);
    EXPECT_THROW(a(0,2), index_out_of_bounds);
    EXPECT_THROW(a(0,0,0), too_many_indices);

    int i1[2][2][2] = {{{1,2}, {3,4}}, {{5,6}, {7,8}}};
    a = i1;

    // Indexing in two steps
    b = a(0,0);
    EXPECT_EQ(1, b(0).as<int>());
    EXPECT_EQ(2, b(1).as<int>());
    b = a(0,1);
    EXPECT_EQ(3, b(0).as<int>());
    EXPECT_EQ(4, b(1).as<int>());
    b = a(1,0);
    EXPECT_EQ(5, b(0).as<int>());
    EXPECT_EQ(6, b(1).as<int>());
    b = a(1,1);
    EXPECT_EQ(7, b(0).as<int>());
    EXPECT_EQ(8, b(1).as<int>());

    // Indexing in one step
    EXPECT_EQ(1, a(0,0,0).as<int>());
    EXPECT_EQ(2, a(0,0,1).as<int>());
    EXPECT_EQ(3, a(0,1,0).as<int>());
    EXPECT_EQ(4, a(0,1,1).as<int>());
    EXPECT_EQ(5, a(1,0,0).as<int>());
    EXPECT_EQ(6, a(1,0,1).as<int>());
    EXPECT_EQ(7, a(1,1,0).as<int>());
    EXPECT_EQ(8, a(1,1,1).as<int>());
    // Indexing with negative Python style
    EXPECT_EQ(1, a(-2,-2,-2).as<int>());
    EXPECT_EQ(2, a(-2,-2,-1).as<int>());
    EXPECT_EQ(3, a(-2,-1,-2).as<int>());
    EXPECT_EQ(4, a(-2,-1,-1).as<int>());
    EXPECT_EQ(5, a(-1,-2,-2).as<int>());
    EXPECT_EQ(6, a(-1,-2,-1).as<int>());
    EXPECT_EQ(7, a(-1,-1,-2).as<int>());
    EXPECT_EQ(8, a(-1,-1,-1).as<int>());
    // Out of bounds
    EXPECT_THROW(a(-3,0,0), index_out_of_bounds);
    EXPECT_THROW(a(2,0,0), index_out_of_bounds);
    EXPECT_THROW(a(0,-3,0), index_out_of_bounds);
    EXPECT_THROW(a(0,2,0), index_out_of_bounds);
    EXPECT_THROW(a(0,0,-3), index_out_of_bounds);
    EXPECT_THROW(a(0,0,2), index_out_of_bounds);
    EXPECT_THROW(a(0,0,0,0), too_many_indices);
}

TEST(NDArrayIndex, SimpleOneDimensionalRange) {
    int i0[] = {1,2,3,4,5,6};
    ndarray a = i0, b;

    // full range
    b = a(irange());
    EXPECT_EQ(6, b.get_shape()[0]);
    EXPECT_EQ(1, b(0).as<int>());
    EXPECT_EQ(2, b(1).as<int>());
    EXPECT_EQ(3, b(2).as<int>());
    EXPECT_EQ(4, b(3).as<int>());
    EXPECT_EQ(5, b(4).as<int>());
    EXPECT_EQ(6, b(5).as<int>());

    // selected range
    b = a(1 <= irange() < 3);
    EXPECT_EQ(2, b.get_shape()[0]);
    EXPECT_EQ(2, b(0).as<int>());
    EXPECT_EQ(3, b(1).as<int>());

    // lower-bound only
    b = a(3 <= irange());
    EXPECT_EQ(3, b.get_shape()[0]);
    EXPECT_EQ(4, b(0).as<int>());
    EXPECT_EQ(5, b(1).as<int>());
    EXPECT_EQ(6, b(2).as<int>());

    // upper-bound only
    b = a(irange() < 3);
    EXPECT_EQ(3, b.get_shape()[0]);
    EXPECT_EQ(1, b(0).as<int>());
    EXPECT_EQ(2, b(1).as<int>());
    EXPECT_EQ(3, b(2).as<int>());
}

TEST(NDArrayIndex, SteppedOneDimensionalRange) {
    int i0[] = {1,2,3,4,5,6};
    ndarray a = i0, b;

    // different step
    b = a(irange() / 2);
    EXPECT_EQ(3, b.get_shape()[0]);
    EXPECT_EQ(1, b(0).as<int>());
    EXPECT_EQ(3, b(1).as<int>());
    EXPECT_EQ(5, b(2).as<int>());

    // full reversed range
    b = a(irange() / -1);
    EXPECT_EQ(6, b.get_shape()[0]);
    EXPECT_EQ(6, b(0).as<int>());
    EXPECT_EQ(5, b(1).as<int>());
    EXPECT_EQ(4, b(2).as<int>());
    EXPECT_EQ(3, b(3).as<int>());
    EXPECT_EQ(2, b(4).as<int>());
    EXPECT_EQ(1, b(5).as<int>());

    // partial reversed range
    b = a(3 >= irange() / -1 >= 0);
    EXPECT_EQ(4, b.get_shape()[0]);
    EXPECT_EQ(4, b(0).as<int>());
    EXPECT_EQ(3, b(1).as<int>());
    EXPECT_EQ(2, b(2).as<int>());
    EXPECT_EQ(1, b(3).as<int>());

    // reversed range with different step
    b = a(irange() / -3);
    EXPECT_EQ(2, b.get_shape()[0]);
    EXPECT_EQ(6, b(0).as<int>());
    EXPECT_EQ(3, b(1).as<int>());

    // partial reversed range with different step
    b = a(2 >= irange() / -2);
    EXPECT_EQ(2, b.get_shape()[0]);
    EXPECT_EQ(3, b(0).as<int>());
    EXPECT_EQ(1, b(1).as<int>());

    // empty range
    b = a(2 <= irange() < 2);
    EXPECT_EQ(0, b.get_shape()[0]);

    // applying two ranges, one after another
    b = a(1 <= irange() <= 5)(irange() / -2);
    EXPECT_EQ(3, b.get_shape()[0]);
    EXPECT_EQ(6, b(0).as<int>());
    EXPECT_EQ(4, b(1).as<int>());
    EXPECT_EQ(2, b(2).as<int>());
}

TEST(NDArrayIndex, ExceptionsOneDimensionalRange) {
    int i0[] = {1,2,3,4,5,6};
    ndarray a = i0, b;

    // exceptions for out-of-bounds ranges
    EXPECT_THROW(a(-7 <= irange()), irange_out_of_bounds);
    EXPECT_THROW(a(0 <= irange() < 7), irange_out_of_bounds);
    EXPECT_THROW(a(0 <= irange() <= 6), irange_out_of_bounds);
    EXPECT_THROW(a(0,irange()), too_many_indices);
    EXPECT_THROW(a(0)(irange()), too_many_indices);
}
