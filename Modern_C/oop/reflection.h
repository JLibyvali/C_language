#ifndef REFLECTION__H
#define REFLECTION__H
#include "debug.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief Using specific file section to implement dynamic reflection at runtime
 */

typedef struct
{
    const char *m_name;
    void (*m_fn)(void *const _param);
} reflect_t;

typedef void (*reflect_cb_t)(void *const _param);

#define REFLECTION_SET __attribute__((section("reflection_custom"), aligned(8)))

static void reflect_cb_typecheck(reflect_cb_t _fn) { return; }

#define REFLECTION_REG(_name, _fn)                                   \
    do                                                               \
    {                                                                \
        reflect_cb_typecheck(_fn);                                   \
        static reflect_t _name##_reg REFLECTION_SET = {#_name, _fn}; \
    }                                                                \
    while (0)

// Reflection Section boundary, providing by linker.
extern const reflect_t __start_reflection_custom[];
extern const reflect_t __stop_reflection_custom[];

static void            invoke_ref(const char *_name, void *const _param)
{

    reflect_t *cb = __start_reflection_custom;
    for (cb; cb < __stop_reflection_custom; ++cb)
    {
        if (!strcmp(cb->m_name, _name))
        {
            cb->m_fn(_param);
            return;
        }
    }
    printf(RED "Haven't set reflection callback.\n" NONE);
    return;
}

#endif