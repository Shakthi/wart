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
    os << (table*)c->car;
    if (cdr(c) != nil)
      os << "->" << cdr(c);
    return os;
  case COMPILED_FN:
    return os << "#compiled";
  default:
    cerr << "Can't print type " << c->type << '\n' << die();
    return os;
  }
}

ostream& operator<<(ostream& os, table* t) {
  os << "{";
  // order common keys deterministically for tracing tests
  if (t->value[sym_name]) os << t->value[sym_name] << ": ";
  if (t->value[sym_sig]) os << sym_sig << ", ";
  if (t->value[sym_body]) os << sym_body << ", ";
  if (t->value[sym_env]) os << sym_env << ", ";
  for (cell_map::iterator p = t->value.begin(); p != t->value.end(); ++p) {
    if (!p->second) continue;
    if (p->first == sym_name || p->first == sym_sig || p->first == sym_body || p->first == sym_env)
      continue;
    os << (cell*)p->first << ", ";
  }
  return os << "}";
}