void test_parenthesize_handles_lines_with_initial_parens() {
  IndentSensitiveStream cs(stream("(a b c)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_skips_indent_tokens() {
  IndentSensitiveStream cs(stream("  (a\tb c)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_skips_outdent_tokens() {
  IndentSensitiveStream cs(stream("(a b c\n  bc\n    def\n  gh)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "bc"); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "gh"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_preserves_following_indent() {
  IndentSensitiveStream cs(stream("a\n  b"));
  nextExpr(cs);
  checkEq(cs.fd.peek(), ' ');
}

void test_parenthesize_handles_fully_parenthesized_expressions_regardless_of_indent() {
  IndentSensitiveStream cs(stream("(a b c\n  (def gh)\n    (i j k)\n  lm\n\n\n    (no p))"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "gh"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "i"); ++p;
  checkEq(*p, "j"); ++p;
  checkEq(*p, "k"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "lm"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "no"); ++p;
  checkEq(*p, "p"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_single_word_lines() {
  IndentSensitiveStream cs(stream("a"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "a"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_single_word_lines2() {
  IndentSensitiveStream cs(stream("a  \nb\nc"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "a"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "b"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "c"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_words_on_single_line() {
  IndentSensitiveStream cs(stream("a b c  "));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_words_on_accidentally_indented_line() {
  IndentSensitiveStream cs(stream(" a b c"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_quoted_words() {
  IndentSensitiveStream cs(stream(",a b c"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, ","); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_quoted_words2() {
  IndentSensitiveStream cs(stream(",@a b c"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, ",@"); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_quoted_words3() {
  IndentSensitiveStream cs(stream("'a b c"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "'"); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_nested_quoted_words() {
  IndentSensitiveStream cs(stream("a b\n  'c\n  ,d\n  @e"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "'"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ","); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "@"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_quoted_groups() {
  IndentSensitiveStream cs(stream(",(a b c)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, ","); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_quoted_groups2() {
  IndentSensitiveStream cs(stream(",@(a b c)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, ",@"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_quoted_groups3() {
  IndentSensitiveStream cs(stream(",,(a b c)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, ","); ++p;
  checkEq(*p, ","); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_words_on_single_indented_line() {
  IndentSensitiveStream cs(stream("    a b c\n  34"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p=tokens.begin();
  checkEq(*p, "34"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_words_on_each_line_without_indent() {
  IndentSensitiveStream cs(stream("a b c  \nd ef"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p=tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "ef"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_across_indent() {
  IndentSensitiveStream cs(stream("a b c  \n  d ef"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "ef"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_across_indent2() {
  IndentSensitiveStream cs(stream("a b c  \n  (d ef)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "ef"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_across_indent3() {
  IndentSensitiveStream cs(stream("a b c  \n  (d ef)\n\n  g"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "ef"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "g"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_nested_indents() {
  IndentSensitiveStream cs(stream("a b c\n  d e\n    f\ny"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, "f"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "y"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_handles_quotes_and_comments() {
  IndentSensitiveStream cs(stream("a b c  \n  '(d ef)\n\n  g #abc"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "'"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "ef"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "g"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_before_outdents() {
  IndentSensitiveStream cs(stream("a b c  \n    '(d ef)\n\n  g #abc"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "'"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "ef"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "g"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_before_outdents2() {
  IndentSensitiveStream cs(stream("def foo\n    a b c\n  d e\nnewdef"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "foo"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "newdef"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_before_too_much_outdent() {
  IndentSensitiveStream cs(stream("  a a\n    a\ny"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "y"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_groups_across_comments() {
  IndentSensitiveStream cs(stream("def foo\n#a b c\n  d e\nnew"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "foo"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "new"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_does_not_group_inside_parens() {
  IndentSensitiveStream cs(stream("(def foo\n    #a b c\n  d e)\nnew"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "foo"); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "new"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_does_not_group_inside_parens2() {
  IndentSensitiveStream cs(stream("`(def foo\n    #a b c\n  d e)\nnew"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "`"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "foo"); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "new"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_does_not_group_inside_parens3() {
  IndentSensitiveStream cs(stream("  (a b c\n    d e)"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_does_not_group_inside_arglists() {
  IndentSensitiveStream cs(stream("def foo(a (b)\n    c d)\n  d e\nnew"));
  list<Token> tokens = nextExpr(cs);
  list<Token>::iterator p = tokens.begin();
  checkEq(*p, "("); ++p;
  checkEq(*p, "def"); ++p;
  checkEq(*p, "foo"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "a"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "b"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "c"); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, "("); ++p;
  checkEq(*p, "d"); ++p;
  checkEq(*p, "e"); ++p;
  checkEq(*p, ")"); ++p;
  checkEq(*p, ")"); ++p;
  check(p == tokens.end());
  tokens = nextExpr(cs); p = tokens.begin();
  checkEq(*p, "new"); ++p;
  check(p == tokens.end());
  check(cs.eof());
}

void test_parenthesize_passes_through_unbalanced_open_paren() {
  IndentSensitiveStream cs(stream("("));
  list<Token> tokens = nextExpr(cs);
  checkEq(tokens.size(), 1);
  checkEq(tokens.front(), "(");
  check(cs.eof());
}

void test_parenthesize_errors_on_unbalanced_closed_paren() {
  IndentSensitiveStream cs(stream(")"));
  list<Token> tokens = nextExpr(cs);
  check(raiseCount > 0);   raiseCount=0;
  check(cs.eof());
}
