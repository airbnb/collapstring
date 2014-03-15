#include <alloca.h>
#include <assert.h>
#include <ruby.h>

typedef enum {
        C_S_OUT,
        C_S_OUT_BACKSLASH,
        C_S_IN_SINGLE,
        C_S_IN_SINGLE_BACKSLASH,
        C_S_IN_DOUBLE,
        C_S_IN_DOUBLE_BACKSLASH,
} collapse_state;

static VALUE collapstring_collapse(VALUE self, VALUE src_val)
{
        Check_Type(src_val, T_STRING);

        const char *src = RSTRING_PTR(src_val);
        const size_t src_len = RSTRING_LEN(src_val);

        char *dst = alloca(src_len);
        size_t dst_idx = 0;
        collapse_state state = C_S_OUT;

        for (size_t src_idx = 0; src_idx < src_len; src_idx++) {
                const char cur = src[src_idx];
                switch (cur) {
                case '\"':
                        switch (state) {
                        case C_S_OUT:
                                dst[dst_idx++] = cur;
                                state = C_S_IN_DOUBLE;
                                break;
                        case C_S_OUT_BACKSLASH:
                        case C_S_IN_DOUBLE:
                                dst[dst_idx++] = cur;
                                state = C_S_OUT;
                                break;
                        case C_S_IN_DOUBLE_BACKSLASH:
                                state = C_S_IN_DOUBLE;
                                /* fallthrough */
                        case C_S_IN_SINGLE:
                                break;
                        case C_S_IN_SINGLE_BACKSLASH:
                                state = C_S_IN_SINGLE;
                                break;
                        }
                        break;
                case '\'':
                        switch (state) {
                        case C_S_OUT:
                                dst[dst_idx++] = cur;
                                state = C_S_IN_SINGLE;
                                break;
                        case C_S_OUT_BACKSLASH:
                        case C_S_IN_SINGLE:
                                dst[dst_idx++] = cur;
                                state = C_S_OUT;
                                break;
                        case C_S_IN_SINGLE_BACKSLASH:
                                state = C_S_IN_SINGLE;
                                break;
                        case C_S_IN_DOUBLE_BACKSLASH:
                                state = C_S_IN_DOUBLE;
                                /* fallthrough */
                        case C_S_IN_DOUBLE:
                                break;
                        }
                        break;
                case '\\':
                        switch (state) {
                        case C_S_IN_SINGLE:
                                state = C_S_IN_SINGLE_BACKSLASH;
                                break;
                        case C_S_IN_SINGLE_BACKSLASH:
                                state = C_S_IN_SINGLE;
                                break;
                        case C_S_IN_DOUBLE:
                                state = C_S_IN_DOUBLE_BACKSLASH;
                                break;
                        case C_S_IN_DOUBLE_BACKSLASH:
                                state = C_S_IN_DOUBLE;
                                break;
                        case C_S_OUT:
                                dst[dst_idx++] = cur;
                                state = C_S_OUT_BACKSLASH;
                                break;
                        case C_S_OUT_BACKSLASH:
                                dst[dst_idx++] = cur;
                                state = C_S_OUT;
                                break;
                        }
                        break;
                default:
                        switch (state) {
                        case C_S_IN_SINGLE_BACKSLASH:
                                state = C_S_IN_SINGLE;
                                break;
                        case C_S_IN_DOUBLE_BACKSLASH:
                                state = C_S_IN_DOUBLE;
                                break;
                        case C_S_OUT:
                                dst[dst_idx++] = cur;
                                break;
                        case C_S_OUT_BACKSLASH:
                                dst[dst_idx++] = cur;
                                state = C_S_OUT;
                                break;
                        case C_S_IN_SINGLE:
                        case C_S_IN_DOUBLE:
                                break;
                        }
                }
        }
        return rb_str_new(dst, dst_idx);
}

void Init_collapstring(void)
{
        VALUE module = rb_define_module("Collapstring");
        assert(module);
        rb_define_module_function(module, "collapse", collapstring_collapse, 1);
}
