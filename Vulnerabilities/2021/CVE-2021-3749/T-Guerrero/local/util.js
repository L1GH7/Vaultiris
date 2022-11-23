export function IsWhiteSpaceOrLineTerminator(charCode) {
  // 0x0020 - SPACE (Intentionally out of order to fast path a commmon case)
  if (charCode == 0x0020) {
    return true;
  }

  // Common Non-whitespace characters from (0x000E, 0x00A0)
  if (((charCode) - 0x000E) < 0x0092) {
    return false;
  }

  // 0x0009 - HORIZONTAL TAB
  if (charCode < 0x0009) {
    return false;
  }
  // 0x000A - LINE FEED OR NEW LINE
  // 0x000B - VERTICAL TAB
  // 0x000C - FORMFEED
  // 0x000D - HORIZONTAL TAB
  if (charCode <= 0x000D) {
    return true;
  }

  // 0x00A0 - NO-BREAK SPACE
  if (charCode == 0x00A0) {
    return true;
  }

  // 0x1680 - Ogham Space Mark
  if (charCode == 0x1680) {
    return true;
  }

  // 0x2000 - EN QUAD
  if (charCode < 0x2000) {
    return false;
  }
  // 0x2001 - EM QUAD
  // 0x2002 - EN SPACE
  // 0x2003 - EM SPACE
  // 0x2004 - THREE-PER-EM SPACE
  // 0x2005 - FOUR-PER-EM SPACE
  // 0x2006 - SIX-PER-EM SPACE
  // 0x2007 - FIGURE SPACE
  // 0x2008 - PUNCTUATION SPACE
  // 0x2009 - THIN SPACE
  // 0x200A - HAIR SPACE
  if (charCode <= 0x200A) {
    return true;
  }

  // 0x2028 - LINE SEPARATOR
  if (charCode == 0x2028) {
    return true;
  }
  // 0x2029 - PARAGRAPH SEPARATOR
  if (charCode == 0x2029) {
    return true;
  }
  // 0x202F - NARROW NO-BREAK SPACE
  if (charCode == 0x202F) {
    return true;
  }
  // 0x205F - MEDIUM MATHEMATICAL SPACE
  if (charCode == 0x205F) {
    return true;
  }
  // 0xFEFF - BYTE ORDER MARK
  if (charCode == 0xFEFF) {
    return true;
  }
  // 0x3000 - IDEOGRAPHIC SPACE
  if (charCode == 0x3000) {
    return true;
  }

  return false;
}