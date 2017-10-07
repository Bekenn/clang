// RUN: %clang_cc1 -std=c++2a -ffunc-parm-packs -fsyntax-only -verify %s

// Declarations used by the examples in P1219
namespace std {
  using size_t = decltype(sizeof(0));

  template <class E> class initializer_list {
  public:
    using value_type = E;
    using reference = const E &;
    using const_reference = const E &;
    using size_type = size_t;
    using iterator = const E *;
    using const_iterator = const E *;
    constexpr initializer_list() noexcept;
    constexpr size_t size() const noexcept;    // number of elements
    constexpr const E *begin() const noexcept; // first element
    constexpr const E *end() const noexcept;   // one past the last element
  };
  // 16.10.4, initializer list range access
  template <class E> constexpr const E *begin(initializer_list<E> il) noexcept;
  template <class E> constexpr const E *end(initializer_list<E> il) noexcept;

  template <bool b, class T, class F> struct conditional { using type = F; };

  template <class T, class F> struct conditional<true, T, F> { using type = T; };

  template <bool b, class T, class F>
  using conditional_t = typename conditional<b, T, F>::type;

  template <class... T> struct common_type;

  template <class T> struct common_type<T> { using type = T; };

  template <class T1, class T2, class... Ts> struct common_type<T1, T2, Ts...> {
    using type = decltype(true ? T1() : typename common_type<T2, Ts...>::type());
  };

  template <class... T> using common_type_t = typename common_type<T...>::type;

  template <class T> struct is_void { static constexpr bool value = false; };
  template <> struct is_void<void> { static constexpr bool value = true; };

  template <class T> inline constexpr bool is_void_v = is_void<T>::value;

  template <class T> struct decay { using type = T; };

  template <class T> struct decay<const T> { using type = T; };

  template <class T> struct decay<volatile T> { using type = T; };

  template <class T> struct decay<const volatile T> { using type = T; };

  template <class T> struct decay<T &> { using type = typename decay<T>::type; };

  template <class R, class... Args> struct decay<R(Args...)> {
    using type = R (*)(Args...);
  };

  template <class T, size_t N> struct decay<T[N]> { using type = T *; };

  template <class T> struct decay<T[]> { using type = T *; };

  template <class T> using decay_t = typename decay<T>::type;

  template <class T, size_t N> struct array;
} // namespace std

using namespace std;

namespace A {
  template <class Type> void f(Type... vs);

  template <class T> T min(T v1, T... vs) {
    return (v1, ..., (v1 = vs < v1 ? vs : v1));
  }
} // namespace A

namespace B {
  template <class T> constexpr T min(T v1, T... vs);

  template <class Compare, class T> constexpr T min(Compare comp, T v1, T... vs);
} // namespace B

namespace C {
  template <class T>
  constexpr auto make_array(T &&... t) -> array<decay_t<T>, sizeof...(t)>;

  template <class T> class MyContainer {
  public:
    template <> MyContainer(const T &... elems);
    // ...
  };

  MyContainer<int> cont = {5, 10};
} // namespace C

namespace D {
  template <class T> class MyContainer {
  public:
    template <> MyContainer(const T &... elems);
    explicit MyContainer(const T &min, const T &max); // expected-note {{declared here}}
    // ...
  };

  MyContainer<int> cont = {5, 10}; // expected-error {{constructor is explicit}}
} // namespace D

namespace E {
  auto a = [](int... v) { return (1 * ... * v); };
} // namespace Q

namespace F {
  template <class T> void foo(T... v);
  template <> void foo(int... v);
  template <> void foo(int a);
  template <> void foo<>(int a, int b);
  template <> void foo(float a);
  template <> void foo<int>(int a, int b);
  void bar(int... v);
  template <> void baz(); // expected-error {{no function template matches}}
} // namespace F

namespace G {
  int k = [](int... i) { return (0 + ... + i); }(3, 4);
} // namespace G

namespace H {
  template <> bool all(bool... args) { return (... && args); }
  bool b = all(true, true, true, false);
} // namespace H

namespace I {
  template <> int sum(int... n) { return (0 + ... + n); }

  void h(int x, int y, int z) {
    int i = sum(x, y, z);
  }
} // namespace I

namespace J {
  template <class T> void f(T... v);
  auto pfn = &f<int>;   // expected-error {{overloaded function type}}

  template <class T> struct S {};
  using bad = S<int...>; // expected-error {{does not contain any unexpanded parameter packs}}

  using bad1_fn = void(int...); // expected-error {{function declaration}}
  template <>         // expected-error {{extraneous 'template<>' in declaration}}
  using bad2_fn =
      void(int...);   // expected-error {{function declaration}}

  template <> void pfnlist(void... arg(int)); // ok
  int (*p)(int...); // expected-error {{function declaration}}
  int (&r)(int...); // expected-error {{function declaration}} expected-error {{requires an initializer}}

  template <> int vartempl = 5; // expected-error {{extraneous 'template<>' in declaration}}
} // namespace J

namespace K {
  template <class T>
  struct S {
    S(int);
    S(T);
  };
  template <> S(int) -> S<double>;  // expected-error {{cannot be explicitly specialized}}
}

template <> constexpr bool all(bool... vs) { return (... && vs); }
static_assert(all());
static_assert(!all(false));
static_assert(all(true));
static_assert(!all(false, true));
static_assert(all(true, true, true, true));

template <> constexpr bool any(bool... vs) { return (... || vs); }
static_assert(!any());
static_assert(!any(false));
static_assert(any(true));
static_assert(any(false, true));
static_assert(!any(false, false, false, false));

template <> constexpr int f(int...) { return 0; }
template <> constexpr int f(int) { return 1; }
template <> constexpr int f<>(int, int) { return 2; }
static_assert(f() == 0);
static_assert(f(1) == 1);
static_assert(f(1, 2) == 2);
static_assert(f(1, 2, 3) == 0); // expected-note {{required here}}
template <> constexpr int f(int, int, int) { // expected-error {{after instantiation}}
  return 3;
}
template <> constexpr int f<>(int, int, int, int...); // expected-error {{partial specialization is not allowed}}

template <> constexpr size_t count_ints(int... vs) { return sizeof...(vs); }
static_assert(count_ints() == 0);
static_assert(count_ints(0) == 1);
static_assert(count_ints(2, 3, 5, 7, 11) == 5);

template <auto n> struct constant { static constexpr auto value = n; };
template <> auto has_five_ints(int... vs) -> constant<sizeof...(vs) == 5>;
static_assert(decltype(has_five_ints(0, 1, 2, 3, 4))::value);
static_assert(!decltype(has_five_ints(0, 1, 2))::value);

template <> void noexcept_if_five_ints(int... vs) noexcept(sizeof...(vs) == 5);
static_assert(noexcept(noexcept_if_five_ints(0, 1, 2, 3, 4)));
static_assert(!noexcept(noexcept_if_five_ints(0, 1, 2)));

template <class T>
void dependent_noexcept(T... vs) noexcept(sizeof...(vs) == 5);
template <class T> struct dependent_noexcept_tester {
  friend void dependent_noexcept<>(T, T, T, T, T) noexcept(true);
  static_assert(noexcept(dependent_noexcept(T(), T(), T(), T(), T())));
};
template struct dependent_noexcept_tester<int>;

template <class T> struct S1 {
  void f(int...);
};

template <class T> struct S2 {
  template <> void g(int... vs) noexcept(sizeof...(vs) == 2);
};
template <> template <> void S2<char>::g(int a, int b) noexcept {}

template <class T> void example(T);
void (*pexample)(int) = &example;

int in_lambda() {
  auto test = [](int... a) { return sizeof...(a); };
  return test(1, 2, 3);
}

template <>
void overload(int...); // expected-note {{candidate function template}}
template <>
void overload(short...); // expected-note {{candidate function template}}
void test_overload() {
  overload();      // expected-error {{call to 'overload' is ambiguous}}
}
