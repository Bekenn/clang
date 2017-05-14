// RUN: %clang_cc1 -std=c++1z -ffunc-parm-packs -fsyntax-only -verify %s

int f1(int...);     // expected-error{{does not contain any unexpanded parameter packs}}
int f2(int n...);   // expected-error{{comma required}}
int f3(int, ...);
int f4(...);
