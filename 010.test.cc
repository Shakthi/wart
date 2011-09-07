void test_cons_works() {
  Cell* call = wartRead(stream(L"cons 1 2")).front();
  Cell* result = eval(call);
  check_eq(car(result), newNum(1));
  check_eq(cdr(result), newNum(2));
  rmref(result);
  rmref(call);
}

void test_if_sees_args_in_then_and_else() {
  Cell* lambda = wartRead(stream(L"(lambda(x) (if 34 x))")).front();
  Cell* f = eval(lambda);
  newDynamicScope(L"f", f);
  Cell* call = wartRead(stream(L"(f 35)")).front();
  Cell* result = eval(call);
  check_eq(result, newNum(35));
  rmref(result);
  rmref(call);
  endDynamicScope(L"f");
  rmref(f);
  rmref(lambda);
}

void test_sym_works_with_one_arg() {
  Cell* call = wartRead(stream(L"(sym \"abc\")")).front();
  Cell* result = eval(call);
  check_eq(result, newSym(L"abc"));
  rmref(result);
  rmref(call);
}

void test_sym_works_with_multiple_args() {
  Cell* call = wartRead(stream(L"(sym \"abc\" 42 'def)")).front();
  Cell* result = eval(call);
  check_eq(result, newSym(L"abc42def"));
  rmref(result);
  rmref(call);
}
