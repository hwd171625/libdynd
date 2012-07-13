//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DND__UNARY_KERNEL_ADAPTER_CODEGEN_HPP_
#define _DND__UNARY_KERNEL_ADAPTER_CODEGEN_HPP_

#include <dnd/dtype.hpp>
#include <dnd/kernels/kernel_instance.hpp>
#include <dnd/memblock/memory_block.hpp>
#include <stdint.h>

namespace dnd {

/**
 * Gets a kernel for adapting a unary function pointer of the given
 * prototype. This function assumes CDECL calling convention where
 * there are multiple conventions, or the system calling convention
 * if there is just one.
 *
 * @param exec_memblock  An executable_memory_block where memory for the
 *                       code generation is used.
 * @param restype        The return type of the function.
 * @param arg0type       The type of the function's first parameter.
 */
unary_operation_t get_unary_function_adapter(const memory_block_ptr& exec_memblock, const dtype& restype,
                    const dtype& arg0type);

} // namespace dnd

#endif // _DND__UNARY_KERNEL_ADAPTER_CODEGEN_HPP_