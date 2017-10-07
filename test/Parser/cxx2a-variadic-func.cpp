// RUN: %clang_cc1 -std=c++2a -ffunc-parm-packs -fsyntax-only -verify %s

// Require a comma before C-style variadic ellipsis
int f1(int...);     // expected-error {{can only appear in function templates}}
int f2(int n...);   // expected-error {{comma required}}
int f3(int, ...);
int f4(...);

// Allow homogeneous function parameter packs in templates
template <class T> int f5(T...);
template <> int f6(int...);

// Homogeneous function parameter pack must be the last parameter
template <class T> int f6(T..., int);   // expected-error {{must be the last function parameter}}

// Homogeneous function parameter pack cannot have a default argument
template <> int f7(int... vs = 5);      // expected-error {{cannot have a default argument}}

// Disambiguate between templates taking no arguments and explicit specializations
template <> int good(int...);           // template
template <> int good<>(int, int);       // explicit specialization
template <> int good(int, int, int);    // explicit specialization (deduced)
template <> int bad(int, int);          // expected-error {{no function template matches}}
