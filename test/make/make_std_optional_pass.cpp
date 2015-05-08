// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/make.hpp>

//  template <template <class ...> class M, class X>
//  auto make(X&& x);
//  template <class M, class X>
//  auto make(X&& x);
//  template <class M, class ...Args>
//  auto make(Args&& ...args);

#if defined VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

#define VIBOES_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#include <experimental/make.hpp>
#include <optional.hpp>
#include <boost/detail/lightweight_test.hpp>


template <class T>
struct explicit_t {
  explicit_t(T) {}
};

struct explicit_d {};

template <class T>
explicit_t<T> explicit_(T v){ return explicit_t<T>{v};}

struct E {
  explicit E() {}
  explicit E(int) {}
  E(explicit_d) {}
  E(explicit_t<int>) {}
};
struct I {
  I() {}
  I(int) {}
};

struct IIL {
  IIL(std::initializer_list<int>) {}
};

struct EIL {
  explicit EIL(std::initializer_list<int>) {}
};


void ff(I p = {}) {}
void f(I p = {}) {}
//void f(E p = {}); // compile error
void f(E p = explicit_d{});

void f2(I p = 1) {}
//void f2(E p = 1); // compile error
void f2(E p = explicit_(1));

void f3(IIL p = {}){}
//void f3(EIL p = {}){} // compile error

void f33(IIL p = {1, 2}){}
//void f3(EIL p = {1, 2}){} // compile error


void f4(I p){}
void f5(E p){}

void g() {
  f4({});
}

void h() {
  //f5({}); // compile error
  f5(explicit_d{});
}

I g1() {
  return {};
}

E h1() {
  //return {}; // compile error
  return explicit_d{};
}

I g2() {
  return {1};
}

E h2() {
  //return {1}; // compile error
  return explicit_(1);
}

namespace std {
  namespace experimental {

    // Holder specialization
    template <>
    struct optional<_t>  : meta::lift<optional> {};

#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
    namespace meta
    {
      // type_constructor customization
      template <class T>
      struct type_constructor<optional<T>> : id<optional<_t>> {};
    }
#endif

    nullopt_t none_custom(meta::type<optional<_t>>) { return nullopt; }
    nullopt_t none_custom(meta::template_class<optional>) { return nullopt; }
  }
}


struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  namespace stde = std::experimental;
  {
    stde::optional<int> x = stde::none<stde::optional>();
    BOOST_TEST(! x);
  }
  {
    stde::optional<int> x = stde::none<stde::optional<stde::_t>>();
    BOOST_TEST(! x);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    short v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    stde::optional<A> x = stde::make<stde::optional<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    stde::optional<int> x = stde::make<stde::optional<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
  {
    stde::optional<int> x = stde::make<stde::optional<int>>();
    BOOST_TEST(! x);
  }
  {
    int v=0;
    stde::optional<int&> x = stde::make<stde::optional<int&>>(std::ref(v));
    BOOST_TEST(&v == &x.value());
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
//  {
//    constexpr int v=0;
//    constexpr stde::optional<int> x = stde::make<stde::optional>(v);
//    BOOST_TEST(*x == 0);
//  }

  return ::boost::report_errors();
}
#endif
