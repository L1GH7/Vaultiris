require "test_helper"

class LegacyRailsCVE20205267PatchTest < Minitest::Test
  include ActionView::Helpers::JavaScriptHelper
  def test_version_number
    refute_nil ::LegacyRailsCVE20205267Patch::VERSION
  end

  def test_escape_javascript
    assert_equal "", escape_javascript(nil)
    assert_equal "123", escape_javascript(123)
    assert_equal "en", escape_javascript(:en)
    assert_equal "false", escape_javascript(false)
    assert_equal "true", escape_javascript(true)
    assert_equal %(This \\"thing\\" is really\\n netos\\'), escape_javascript(%(This "thing" is really\n netos'))
    assert_equal %(backslash\\\\test), escape_javascript(%(backslash\\test))
    assert_equal %(dont <\\/close> tags), escape_javascript(%(dont </close> tags))
    assert_equal %(unicode &#x2028; newline), escape_javascript((+%(unicode \342\200\250 newline)).force_encoding(Encoding::UTF_8).encode!)
    assert_equal %(unicode &#x2029; newline), escape_javascript((+%(unicode \342\200\251 newline)).force_encoding(Encoding::UTF_8).encode!)

    assert_equal %(dont <\\/close> tags), j(%(dont </close> tags))
  end

  def test_escape_javascript
    assert_equal "", escape_javascript(nil)
    assert_equal "123", escape_javascript(123)
    assert_equal "en", escape_javascript(:en)
    assert_equal "false", escape_javascript(false)
    assert_equal "true", escape_javascript(true)
    assert_equal %(This \\"thing\\" is really\\n netos\\'), escape_javascript(%(This "thing" is really\n netos'))
    assert_equal %(backslash\\\\test), escape_javascript(%(backslash\\test))
    assert_equal %(dont <\\/close> tags), escape_javascript(%(dont </close> tags))
    assert_equal %(dont <\\/close> tags), j(%(dont </close> tags))
  end

  def test_escape_backtick
    assert_equal "\\`", escape_javascript("`")
    assert_equal "\\`", j("`")
  end

  def test_escape_dollar_sign
    assert_equal "\\$", escape_javascript("$")
    assert_equal "\\$", j("$")
  end
end
