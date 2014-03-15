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

static VALUE collapstring_collapse_bang(VALUE self, VALUE src)
{
        Check_Type(src, T_STRING);

        char *str = RSTRING_PTR(src);
        const size_t len = RSTRING_LEN(src);

        size_t dst_idx = 0;
        collapse_state state = C_S_OUT;

        for (size_t src_idx = 0; src_idx < len; src_idx++) {
                const char cur = str[src_idx];
                switch (cur) {
                case '\"':
                        switch (state) {
                        case C_S_OUT:
                                str[dst_idx++] = cur;
                                state = C_S_IN_DOUBLE;
                                break;
                        case C_S_OUT_BACKSLASH:
                        case C_S_IN_DOUBLE:
                                str[dst_idx++] = cur;
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
                                str[dst_idx++] = cur;
                                state = C_S_IN_SINGLE;
                                break;
                        case C_S_OUT_BACKSLASH:
                        case C_S_IN_SINGLE:
                                str[dst_idx++] = cur;
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
                                str[dst_idx++] = cur;
                                state = C_S_OUT_BACKSLASH;
                                break;
                        case C_S_OUT_BACKSLASH:
                                str[dst_idx++] = cur;
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
                                str[dst_idx++] = cur;
                                break;
                        case C_S_OUT_BACKSLASH:
                                str[dst_idx++] = cur;
                                state = C_S_OUT;
                                break;
                        case C_S_IN_SINGLE:
                        case C_S_IN_DOUBLE:
                                break;
                        }
                }
        }

        return rb_str_resize(src, dst_idx);
}

static VALUE collapstring_collapse(VALUE self, VALUE src) {
        VALUE dst = rb_obj_clone(src);
        assert(dst);
        collapstring_collapse_bang(self, dst);
        return dst;
}

void Init_collapstring(void)
{
        VALUE module = rb_define_module("Collapstring");
        assert(module);
        rb_define_module_function(module, "collapse!", collapstring_collapse_bang, 1);
        rb_define_module_function(module, "collapse", collapstring_collapse, 1);
}
