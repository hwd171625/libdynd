//
// Copyright (C) 2011-14 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <dynd/types/base_bytes_type.hpp>

using namespace std;
using namespace dynd;


base_bytes_type::~base_bytes_type()
{
}

size_t base_bytes_type::get_iterdata_size(intptr_t DYND_UNUSED(ndim)) const
{
    return 0;
}
