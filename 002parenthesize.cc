//// insert explicit parens based on indentation

                                  bool endOfInput(istream& in) {
                                    if (in.eof()) return true;
                                    if (!interactive) return false;
                                    // in interactive mode two <Enter>s are like eof
                                    bool ans = false;
                                    char c = in.get();
                                    if (c == '\n' && !in.eof() && in.peek() == '\n')
                                      ans = true;
                                    in.putback(c);
                                    return ans;
                                  }

list<Token> nextLine(CodeStream& c) {
  list<Token> result;
  if (endOfInput(c.fd)) return result;

  if (c.currIndent == -1)
    result.push_back(Token::indent(c.currIndent=indent(c.fd)));
  else
    result.push_back(Token::indent(c.currIndent));

  do { result.push_back(nextToken(c)); }
  while (!endOfInput(c.fd) && !result.back().isIndent());
  return result;
}



                                  bool isParen(Token x) {
                                    return x == "(" || x == ")";
                                  }

                                  bool isQuoteOrUnquote(Token x) {
                                    return x == "'" || x == "`" || x == "," || x == ",@" || x == "@";
                                  }

                                  long numWordsInLine(list<Token> line) {
                                    long numWords = 0;
                                    for (list<Token>::iterator p = line.begin(); p != line.end(); ++p)
                                      if (!p->isIndent() && !isParen(*p)
                                          && !isQuoteOrUnquote(*p))
                                        ++numWords;
                                    return numWords;
                                  }

                                  void add(list<Token>& l, Token x) {
                                    if (!x.isIndent())
                                      l.push_back(x);
                                  }

                                  list<Token>::iterator firstNonQuote(list<Token>& line) {
                                    for (list<Token>::iterator p = line.begin(); p != line.end(); ++p) {
                                      if (!p->isIndent() && !isQuoteOrUnquote(*p))
                                        return p;
                                    }
                                    return line.end();
                                  }

                                  bool noParenAtStart(list<Token> line) {
                                    list<Token>::iterator p = firstNonQuote(line);
                                    return p != line.end() && *p != "(";
                                  }

list<Token> nextExpr(CodeStream& c) {
  list<Token> result;
  stack<long> explicitParenStack; // parens in the original
  stack<long> implicitParenStack; // parens we inserted
  for (list<Token> line = nextLine(c); !line.empty(); line=nextLine(c)) {
    long thisLineIndent=line.front().indentLevel, nextLineIndent=line.back().indentLevel;

    // open an implicit paren if necessary
    if (explicitParenStack.empty() && numWordsInLine(line) > 1 && noParenAtStart(line)) {
      add(result, Token::of("("));
      implicitParenStack.push(thisLineIndent);
    }

    // copy line tokens
    for (list<Token>::iterator q = line.begin(); q != line.end(); ++q) {
      add(result, *q);

      if (*q == "(")
        explicitParenStack.push(q->indentLevel);
      if (*q == ")") {
        if (explicitParenStack.empty())
          RAISE << "Unbalanced )" << endl << DIE;
        explicitParenStack.pop();
      }
    }

    // close all possible implicit parens
    while (!implicitParenStack.empty() && implicitParenStack.top() >= nextLineIndent) {
      add(result, Token::of(")"));
      implicitParenStack.pop();
    }

    if (implicitParenStack.empty() && explicitParenStack.empty()) {
      if (!c.fd.eof())
        // Clean up indent state for the next call.
        for (int i = 0; i < nextLineIndent; ++i)
          c.fd.putback(' ');
      break;
    }
  }

  for (unsigned long i=0; i < implicitParenStack.size(); ++i)
    result.push_back(Token::of(")"));
  return result;
}
