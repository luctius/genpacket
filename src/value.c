#include "value.h"

extern inline bool v_is_int(struct value v);
extern inline bool v_is_float(struct value v);
extern inline bool v_is_signed(struct value v);
extern inline bool v_is_unsigned(struct value v);

extern inline int64_t v_get_i(struct value v);
extern inline uint64_t v_get_u(struct value v);
extern inline double v_get_f(struct value v);

extern inline struct value v_set_i(int64_t i);
extern inline struct value v_set_u(uint64_t u);
extern inline struct value v_set_f(double f);
