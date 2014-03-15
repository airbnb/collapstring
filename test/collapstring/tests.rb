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
end
