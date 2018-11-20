//
//  amd64.h
//  File file is part of the "Coroutine" project and released under the MIT License.
//
//  Created by Samuel Williams on 10/5/2018.
//  Copyright, 2018, by Samuel Williams. All rights reserved.
//

#pragma once

#include <assert.h>
#include <string.h>

#if __cplusplus
extern "C" {
#endif

#define COROUTINE void

const size_t COROUTINE_REGISTERS = 0xb0 / 8;

// The fiber context (stack pointer).
typedef struct
{
	void **stack_pointer;
} coroutine_context;

// The initialization function.
typedef void(* coroutine_start)(coroutine_context *from, coroutine_context *self);

inline void coroutine_initialize(
	coroutine_context *context,
	coroutine_start start,
	void *stack_pointer,
	size_t stack_size
) {
	/* Force 16-byte alignment */
	context->stack_pointer = (void**)((uintptr_t)stack_pointer & ~0xF);

	if (!start) {
		assert(!context->stack_pointer);
		/* We are main coroutine for this thread */
		return;
	}

	context->stack_pointer -= COROUTINE_REGISTERS;
	memset(context->stack_pointer, 0, sizeof(void*) * COROUTINE_REGISTERS);

	context->stack_pointer[0xa0 / 8] = (void*)start;
}

coroutine_context * coroutine_transfer(coroutine_context * current, coroutine_context * target);

inline void coroutine_destroy(coroutine_context * context)
{
}

#if __cplusplus
}
#endif
