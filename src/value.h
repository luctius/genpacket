#ifndef VALUE_H
#define VALUE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

enum field_type {
    FT_SIGNED,
    FT_UNSIGNED,
    FT_FLOAT,
    FT_CHAR,
};

struct value {
    enum field_type ft;
    union {
        int64_t i;
        uint64_t u;
        double d;
        char c;
    };
};

inline bool v_is_int(struct value v)      { return (v.ft == FT_SIGNED || v.ft == FT_UNSIGNED); }
inline bool v_is_float(struct value v)    { return (v.ft == FT_FLOAT); }
inline bool v_is_signed(struct value v)   { return (v.ft == FT_SIGNED || v.ft == FT_CHAR); }
inline bool v_is_unsigned(struct value v) { return (v.ft == FT_UNSIGNED); }
inline bool v_is_char(struct value v) { return (v.ft == FT_CHAR); }

inline int64_t v_get_i(struct value v) { return v.i; }
inline uint64_t v_get_u(struct value v) { return v.u; }
inline double v_get_f(struct value v) { return v.d; }
inline char v_get_c(struct value v) { return v.c; }

inline struct value v_set_i(int64_t i) { struct value v = { .ft = FT_SIGNED, .i = i, }; return v; }
inline struct value v_set_u(uint64_t u) { struct value v = { .ft = FT_UNSIGNED, .u = u, }; return v; }
inline struct value v_set_f(double f) { struct value v = { .ft = FT_FLOAT, .d = f, }; return v; }
inline struct value v_set_c(char c) { struct value v = { .ft = FT_CHAR, .c = c, }; return v; }

struct value v_convert(struct value v, enum field_type target_ft);
char *v_to_str(struct value v);

#endif /* VALUE_H */
