//
// Copyright (C) 2011-14 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/kernels/expr_kernel_generator.hpp>
#include <dynd/types/date_type.hpp>

namespace dynd {

expr_kernel_generator *make_strftime_kernelgen(const std::string& format);
expr_kernel_generator *make_replace_kernelgen(int32_t year, int32_t month, int32_t day);

} // namespace dynd
