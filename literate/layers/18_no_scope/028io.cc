//// compiled primitives for I/O

ostream& operator<<(ostream& os, cell* c) {
  if (c == NULL) return os << "NULLNULLNULL";
  if (c == nil) return os << "nil";
  switch(c->type) {
  case CONS:
    if (car(c) == sym_quote || car(c) == sym_backquote || car(c) == sym_unquote || car(c) == sym_splice || car(c) == sym_unquote_splice)
      return os << car(c) << cdr(c);
    os << "(" << car(c);
    for (cell* curr = cdr(c); curr != nil; curr = cdr(curr)) {
      if (is_cons(curr))
        os << " " << car(curr);
      else
        os << " ... " << curr;
    }
    return os << ")";
  case INTEGER:
    return os << to_int(c);
  case FLOAT:
    return os << to_float(c);
  case SYMBOL:
    return os << to_string(c);
  case STRING:
    return os << "\"" << to_string(c) << "\"";
  case TABLE:
    os << *(cell_map*)c->car;
    if (cdr(c) != nil)
      os << "->" << cdr(c);
    return os;
  default:
    cerr << "Can't print type " << c->type << '\n' << die();
    return os;
  }
}

ostream& operator<<(ostream& os, cell_map& t) {
  os << "{";
  if (t[sym_name]) os << t[sym_name] << ": ";
  for (cell_map::iterator p = t.begin(); p != t.end(); ++p) {
    if (p->second && p->first != sym_name)
      os << (cell*)p->first << ", ";
  }
  return os << "}";
}
