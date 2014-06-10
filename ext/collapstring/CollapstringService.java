import org.jruby.Ruby;
import org.jruby.RubyModule;
import org.jruby.RubyString;
import org.jruby.anno.JRubyMethod;
import org.jruby.anno.JRubyModule;
import org.jruby.runtime.builtin.IRubyObject;
import org.jruby.runtime.ThreadContext;
import org.jruby.runtime.load.BasicLibraryService;
import org.jruby.util.ByteList;

import java.io.IOException;
import java.util.Random;

@JRubyModule(name = "Collapstring")
public class CollapstringService implements BasicLibraryService {
    @Override
    public boolean basicLoad(Ruby ruby) throws IOException {
        final RubyModule module = ruby.defineModule("Collapstring");
        module.defineAnnotatedMethods(CollapstringService.class);
        return true;
    }

    @JRubyMethod(name = "collapse!", required = 1, module = true)
    public static IRubyObject collapse_bang(IRubyObject self, IRubyObject src) {
        final RubyString srcString = src.convertToString();
        srcString.modify();
        collapseBytes(srcString.getByteList());
        return src;
    }

    @JRubyMethod(name = "collapse", required = 1, module = true)
    public static IRubyObject collapse(IRubyObject self, IRubyObject src) {
        final IRubyObject res = src.dup();
        collapse_bang(self, res);
        return res;
    }

    @JRubyMethod(name = "fuzz", required = 1, module = true)
    public static IRubyObject fuzz(IRubyObject self, IRubyObject length) {
        final Random rnd = new Random();
        final long llen = length.convertToInteger().getLongValue();
        final int len = (int) llen;
        final byte[] bytes = new byte[len];
        for (int i = 0; i < len; i++) {
            switch (rnd.nextInt(4)) {
                case 0:
                    bytes[i] = '\'';
                    break;
                case 1:
                    bytes[i] = '\"';
                    break;
                case 2:
                    bytes[i] = '\\';
                    break;
                case 3:
                    bytes[i] = ' ';
                    break;
            }
        }
        return RubyString.newString(self.getRuntime(), bytes);
    }

    private static void collapseBytes(ByteList bytes) {
        final int len = bytes.length();
        State state = State.OUT;
        for (int src_idx = 0, dst_idx = 0; src_idx < len; src_idx++) {
            final int cur = bytes.get(src_idx);

            switch (cur) {
                case '\"':
                    switch (state) {
                        case OUT:
                            bytes.set(dst_idx++, cur);
                            state = State.IN_DOUBLE;
                            break;
                        case OUT_BACKSLASH:
                        case IN_DOUBLE:
                            bytes.set(dst_idx++, cur);
                            state = State.OUT;
                            break;
                        case IN_DOUBLE_BACKSLASH:
                            state = State.IN_DOUBLE;
                 /* fallthrough */
                        case IN_SINGLE:
                            break;
                        case IN_SINGLE_BACKSLASH:
                            state = State.IN_SINGLE;
                            break;
                    }
                    break;
                case '\'':
                    switch (state) {
                        case OUT:
                            bytes.set(dst_idx++, cur);
                            state = State.IN_SINGLE;
                            break;
                        case OUT_BACKSLASH:
                        case IN_SINGLE:
                            bytes.set(dst_idx++, cur);
                            state = State.OUT;
                            break;
                        case IN_SINGLE_BACKSLASH:
                            state = State.IN_SINGLE;
                            break;
                        case IN_DOUBLE_BACKSLASH:
                            state = State.IN_DOUBLE;
                 /* fallthrough */
                        case IN_DOUBLE:
                            break;
                    }
                    break;
                case '\\':
                    switch (state) {
                        case IN_SINGLE:
                            state = State.IN_SINGLE_BACKSLASH;
                            break;
                        case IN_SINGLE_BACKSLASH:
                            state = State.IN_SINGLE;
                            break;
                        case IN_DOUBLE:
                            state = State.IN_DOUBLE_BACKSLASH;
                            break;
                        case IN_DOUBLE_BACKSLASH:
                            state = State.IN_DOUBLE;
                            break;
                        case OUT:
                            bytes.set(dst_idx++, cur);
                            state = State.OUT_BACKSLASH;
                            break;
                        case OUT_BACKSLASH:
                            bytes.set(dst_idx++, cur);
                            state = State.OUT;
                            break;
                    }
                    break;
                default:
                    switch (state) {
                        case IN_SINGLE_BACKSLASH:
                            state = State.IN_SINGLE;
                            break;
                        case IN_DOUBLE_BACKSLASH:
                            state = State.IN_DOUBLE;
                            break;
                        case OUT:
                            bytes.set(dst_idx++, cur);
                            break;
                        case OUT_BACKSLASH:
                            bytes.set(dst_idx++, cur);
                            state = State.OUT;
                            break;
                        case IN_SINGLE:
                        case IN_DOUBLE:
                            break;
                    }
            }
            bytes.setRealSize(dst_idx);
        }
    }

    private static enum State {
        OUT,
        OUT_BACKSLASH,
        IN_SINGLE,
        IN_SINGLE_BACKSLASH,
        IN_DOUBLE,
        IN_DOUBLE_BACKSLASH
    }
}
