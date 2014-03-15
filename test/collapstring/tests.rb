require 'collapstring'
require 'test/unit'

class TestCollapstring < Test::Unit::TestCase
  def test_arity
    assert_raise ArgumentError do
      Collapstring.collapse
    end

    assert_raise ArgumentError do
      Collapstring.collapse('foo', 'bar')
    end
  end

  def test_type_safety
    assert_raise TypeError do
      Collapstring.collapse 1
    end
  end

  def test_cases
    assert_equal %[], Collapstring.collapse(%[])

    assert_equal %[noop], Collapstring.collapse(%[noop])

    assert_equal %[''], Collapstring.collapse(%['foo'])
    assert_equal %[""], Collapstring.collapse(%["foo"])

    assert_equal %[single \\], Collapstring.collapse(%[single \\])
    assert_equal %[single '], Collapstring.collapse(%[single '])
    assert_equal %[single "], Collapstring.collapse(%[single "])

    assert_equal %[''], Collapstring.collapse(%['a\\'b'])
    assert_equal %[''], Collapstring.collapse(%['a\\"b'])
    assert_equal %[""], Collapstring.collapse(%["a\\'b"])
    assert_equal %[""], Collapstring.collapse(%["a\\'b"])

    assert_equal %[a\\b], Collapstring.collapse(%[a\\b])
    assert_equal %[a\\\\b], Collapstring.collapse(%[a\\\\b])

    assert_equal %[\\'], Collapstring.collapse(%[\\'])
    assert_equal %[\\"], Collapstring.collapse(%[\\"])

    assert_equal %[""], Collapstring.collapse(%["\\a"])
    assert_equal %[''], Collapstring.collapse(%['\\a'])

    assert_equal %[\\a '], Collapstring.collapse(%[\\a '])
    assert_equal %[\\a "], Collapstring.collapse(%[\\a "])
  end

  def test_long_str
    long_str = 'a' * 1_000_000
    assert_equal long_str, Collapstring.collapse!(long_str)
    assert_equal long_str, Collapstring.collapse(long_str)
  end

  def test_preserves_original_string
    original = 'foo "bar" baz'
    copy = original.dup
    assert_equal 'foo "" baz', Collapstring.collapse(copy)
    assert_equal original, copy
  end
end
