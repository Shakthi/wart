void test_evalBindAll_handles_unquoted_param() {
  Cell* params = read("(x)");
  Cell* args = read("(a)");
  newBinding("a", newNum(3));
  evalBindAll(params, args);
  CHECK_EQ(lookup("x"), newNum(3));
}

void test_evalBindAll_binds_missing_params() {
  Cell* params = read("(x y)");
  Cell* args = read("(a)");
  newBinding("a", newNum(3));
  evalBindAll(params, args);
  CHECK_EQ(lookup("x"), newNum(3));
  CHECK_EQ(lookup("y"), nil);
}

void test_evalBindAll_handles_varargs_param() {
  Cell* params = read("x");
  Cell* args = read("(a b)");
  newBinding("a", newNum(3));
  newBinding("b", newNum(4));
  evalBindAll(params, args);
  // {x: (3 4)}
  CHECK_EQ(car(lookup("x")), newNum(3));
  CHECK_EQ(car(cdr(lookup("x"))), newNum(4));
  CHECK_EQ(cdr(cdr(lookup("x"))), nil);
}

void test_evalBindAll_handles_rest_param() {
  Cell* params = read("(x ... y)");
  Cell* args = read("(a b)");
  newBinding("a", newNum(3));
  newBinding("b", newNum(4));
  evalBindAll(params, args);
  // {x: 3, y: (4)}
  CHECK_EQ(lookup("x"), newNum(3));
  CHECK_EQ(car(lookup("y")), newNum(4));
  CHECK_EQ(cdr(lookup("y")), nil);
}

void test_evalBindAll_handles_destructured_params() {
  Cell* params = read("((a b))");
  Cell* args = read("((cons x (cons y)))");
  newBinding("x", newNum(3));
  newBinding("y", newNum(4));
  evalBindAll(params, args);
  // {a: 3, b: 4}
  CHECK_EQ(lookup("a"), newNum(3));
  CHECK_EQ(lookup("b"), newNum(4));
}



void test_nil_evals_to_itself() {
  Cell* expr = read("()");
  Cell* result = eval(expr);
  CHECK_EQ(result, nil);
  checkTraceContents("eval", "nil\n=> nil\n");
}

void test_num_evals_to_itself() {
  Cell* expr = read("34");
  Cell* result = eval(expr);
  CHECK_EQ(result, expr);
  checkTraceContents("eval", "34\nliteral: 34\n");
}

void test_colonsym_evals_to_itself() {
  Cell* expr = read(":abc");
  Cell* result = eval(expr);
  CHECK_EQ(result, expr);
  checkTraceContents("eval", ":abc\nkeyword sym: :abc\n");
}

void test_colon_evals() {
  Cell* expr = read(":");
  newBinding(":", newNum(34));
  Cell* result = eval(expr);
  CHECK_EQ(result, newNum(34));
  checkTraceContents("eval", ":\nsym: 34\n");
}

void test_string_evals_to_itself() {
  Cell* expr = read("\"ac bd\"");
  Cell* result = eval(expr);
  CHECK_EQ(result, expr);
  checkTraceContents("eval", "\"ac bd\"\nliteral: \"ac bd\"\n");
}

void test_sym_evals_to_value() {
  newBinding("a", newNum(34));
  Cell* expr = read("a");
  Cell* result = eval(expr);
  CHECK_EQ(result, newNum(34));
  checkTraceContents("eval", "a\nsym: 34\n");
}

void test_sym_evals_to_itself() {
  newBinding("a", newSym("a"));
  Cell* expr = read("a");
  Cell* result = eval(expr);
  CHECK_EQ(result, expr);
  checkTraceContents("eval", "a\nsym: a\n");
}

void test_eval_handles_quoted_atoms() {
  stringstream in("'a '34");
  while (!eof(in))
    eval(read(in));
  checkTraceContents("eval", "'a\nquote: a\n'34\nquote: 34\n");
}

void test_eval_handles_quoted_lists() {
  Cell* expr = read("'(a b)");
  eval(expr);
  checkTraceContents("eval", "'(a b)\nquote: (a b)\n");
}

void test_eval_handles_rest_params() {
  Cell* call = read("((fn (a b ... c) c) 1 2 3 4 5)");
  eval(call);
  checkTraceContents2("eval", 1, "((fn (a b ... c) c) 1 2 3 4 5)\n=> (3 4 5)\n");
}

void test_eval_handles_fn_calls() {
  Cell* call = read("((fn () 34))");
  eval(call);
  checkTraceContents2("eval", 1, "((fn nil 34))\n=> 34\n");
}

void test_eval_expands_syms_in_fn_bodies() {
  Cell* call = read("((fn () a))");
  newBinding("a", newNum(34));
  eval(call);
  checkTraceContents2("eval", 1, "((fn nil a))\n=> 34\n");
}

void test_eval_handles_assigned_fn_calls() {
  Cell* fn = read("(fn () 34)");
  Cell* f = eval(fn);
  newBinding("f", f);
  delete global_trace_stream, global_trace_stream = new TraceStream;
    Cell* call = read("(f)");
    eval(call);
    checkTraceContents2("eval", 1, "(f)\n=> 34\n");
}

void test_eval_handles_multiple_args() {
  Cell* fn = read("(fn (a b) b)");
  Cell* f = eval(fn);
  newBinding("f", f);
  delete global_trace_stream, global_trace_stream = new TraceStream;
  Cell* call = read("(f 1 2)");
  eval(call);
  checkTraceContents2("eval", 1, "(f 1 2)\n=> 2\n");
}

void test_eval_handles_multiple_body_exprs() {
  Cell* fn = read("(fn () 1 2)");
  Cell* f = eval(fn);
  newBinding("f", f);
  delete global_trace_stream, global_trace_stream = new TraceStream;
  Cell* call = read("(f)");
  eval(call);
  checkTraceContents2("eval", 1, "(f)\n=> 2\n");
}

void test_eval_handles_vararg_param() {
  Cell* call = read("((fn args args) 1)");
  eval(call);
  checkTraceContents2("eval", 1, "((fn args args) 1)\n=> (1)\n");
}

void test_eval_evals_args() {
  Cell* call = read("((fn (f) (f)) (fn () 34))");
  eval(call);
  checkTraceContents2("eval", 1, "((fn (f) (f)) (fn nil 34))\n=> 34\n");
}

void test_eval_handles_destructured_params() {
  Cell* call = read("((fn ((a b)) b) '(1 2))");
  eval(call);
  checkTraceContents2("eval", 1, "((fn ((a b)) b) '(1 2))\n=> 2\n");
}