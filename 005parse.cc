//// construct parse tree out of tokens

// Currently ,@(list x) creates a flat list: ,@ ( list x )
// Equally valid to have it create a 2 level list: ,@ followed by ( list x )
// Would require changing build phase appropriately.

struct AstNode {
  Token atom;
  list<AstNode> elems;

  explicit AstNode(Token t) :atom(t) {}
  explicit AstNode(list<AstNode> l) :atom(eof()), elems(l) {}

  bool isAtom() const {
    return elems.empty();
  }
  bool isList() const {
    return !elems.empty();
  }
  bool isNil() const {
    return atom == "nil"
        || (elems.size() == 2 && elems.front() == "(" && elems.back() == ")");
  }

  bool operator==(const Token& x) const {
    return elems.empty() && atom == x.token;  // whitespace should be gone by now
  }
  bool operator==(const string& x) const {
    return elems.empty() && atom == x;
  }
  bool operator!=(const Token& x) const {
    return !(*this == x);
  }
  bool operator!=(const string& x) const {
    return !(*this == x);
  }
};

AstNode nextAstNode(IndentSensitiveStream& in) {
  list<Token> bufferedTokens = nextExpr(in);
  return nextAstNode(bufferedTokens);
}

AstNode nextAstNode(list<Token>& buffer) {
  Token curr = nextToken(buffer);
  if (curr != "(" && !isQuoteOrUnquote(curr))
    return AstNode(curr);

  list<AstNode> subform;
  subform.push_back(AstNode(curr));
  while (!eof(subform.back()) && isQuoteOrUnquote(subform.back()))
    subform.push_back(AstNode(nextToken(buffer)));

  if (subform.back() == "(") {
    while (!eof(subform.back()) && subform.back().atom != ")")
      subform.push_back(nextAstNode(buffer));
    if (eof(subform.back())) RAISE << "Unbalanced (" << endl << DIE;
  }

  return AstNode(subform);
}



// internals

Token nextToken(list<Token>& buffer) {
  if(buffer.empty()) return eof();
  Token result = buffer.front(); buffer.pop_front();
  return result;
}

Token eof() {
  return Token(0);
}

bool eof(AstNode n) {
  return n.atom.token == "" && n.elems.empty();
}

ostream& operator<<(ostream& os, AstNode x) {
  if (x.elems.empty()) return os << x.atom;
  bool prevWasOpen = true;
  for (list<AstNode>::iterator p = x.elems.begin(); p != x.elems.end(); ++p) {
    if (!(*p == ")" || prevWasOpen)) os << " ";
    prevWasOpen = (*p == "(" || *p == "'" || *p == "," || *p == ",@" || *p == "@");
    os << *p;
  }
  return os;
}



bool isQuoteOrUnquote(const AstNode& n) {
  return n.isAtom() && isQuoteOrUnquote(n.atom);
}
