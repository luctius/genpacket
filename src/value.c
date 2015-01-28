#include "value.h"

extern inline bool v_is_int(struct value v);
extern inline bool v_is_float(struct value v);
extern inline bool v_is_signed(struct value v);
extern inline bool v_is_unsigned(struct value v);
extern inline bool v_is_char(struct value v);

extern inline int64_t v_get_i(struct value v);
extern inline uint64_t v_get_u(struct value v);
extern inline double v_get_f(struct value v);
extern inline char v_get_c(struct value v);

extern inline struct value v_set_i(int64_t i);
extern inline struct value v_set_u(uint64_t u);
extern inline struct value v_set_f(double f);

char *v_to_str(struct value v) {
    static char bfr[20];
    switch (v.ft) {
        case FT_SIGNED:
            sprintf(bfr, "%" PRIi64 "", v.i);
            break;
        case FT_UNSIGNED:
            sprintf(bfr, "%" PRIu64 "", v.u);
            break;
        case FT_FLOAT:
            sprintf(bfr, "%f", v.d);
            break;
        case FT_CHAR:
            sprintf(bfr, "%c", v.c);
            break;
        default: sprintf(bfr, "default"); break;
    }
    return bfr;
}

inline struct value v_convert(struct value v, enum field_type target_ft) {
    struct value vn;
    vn.ft = target_ft;

    switch(target_ft) {
        default:
            vn.ft = FT_SIGNED;
        case FT_SIGNED: 
            switch(v.ft) {
                default:
                case FT_SIGNED: vn.i = v.i; break;
                case FT_UNSIGNED: vn.i = v.u; break;
                case FT_FLOAT: vn.i = v.d; break;
            } break;
        case FT_UNSIGNED:
            switch(v.ft) {
                default:
                case FT_SIGNED: vn.u = v.i; break;
                case FT_UNSIGNED: vn.u = v.u; break;
                case FT_FLOAT: vn.u = v.d; break;
            } break;
        case FT_FLOAT:
            switch(v.ft) {
                default:
                case FT_SIGNED: vn.d = v.i; break;
                case FT_UNSIGNED: vn.d = v.u; break;
                case FT_FLOAT: vn.d = v.d; break;
            } break;
        case FT_CHAR:
            switch(v.ft) {
                default:
                case FT_SIGNED: vn.c = v.i; break;
                case FT_UNSIGNED: vn.c = v.u; break;
                case FT_FLOAT: vn.c = v.d; break;
            } break;
    }
    return vn;
}

