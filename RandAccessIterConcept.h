#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <boost/concept_check.hpp>

template <class T>
struct MyDefaultConstructible {
    BOOST_CONCEPT_USAGE(MyDefaultConstructible) {
        T u1;
        T u2{};
        T();
        T{};
    }
};

template <class T>
struct MyEqualityComparable {
    BOOST_CONCEPT_USAGE(MyEqualityComparable) {
        bool result = a == b;
    }

private:
    T a;
    const T b;
};

template <class It, class = std::enable_if_t<std::is_copy_constructible<It>::value &&
                                             std::is_copy_assignable<It>::value &&
                                             std::is_destructible<It>::value &&
                                             std::is_swappable<It>::value>>
struct MyIterator {

    using value_type        = typename std::iterator_traits<It>::value_type;
    using difference_type   = typename std::iterator_traits<It>::difference_type;
    using reference         = typename std::iterator_traits<It>::reference;
    using pointer           = typename std::iterator_traits<It>::pointer;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;

    BOOST_CONCEPT_USAGE(MyIterator) {
        *r;
        It& i = ++r;
    }

private:
    It r;
};

template <class It>
struct MyInputIterator : public MyIterator<It>,
                         public MyEqualityComparable<It> {
    using value_type        = typename std::iterator_traits<It>::value_type;
    using difference_type   = typename std::iterator_traits<It>::difference_type;
    using reference         = typename std::iterator_traits<It>::reference;
    using pointer           = typename std::iterator_traits<It>::pointer;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;

    BOOST_CONCEPT_USAGE(MyInputIterator) {
        auto result = i != j;
        static_assert(std::is_convertible<decltype(result), bool>::value, "Result can't be converted to bool");

        (i != j) == !(i == j);

        auto r = *i;
        static_assert(std::is_same<decltype(r), reference>::value, "Type mismatch");
        static_assert(std::is_convertible<decltype(r), value_type>::value, "Can't be converted");

        It& it = ++i;
        (void)i++;
        (void)++i;
        auto v = *i++;
        static_assert(std::is_convertible<decltype(v), value_type>::value, "Can't be converted");
    }

private:
    It i;
    const It j;
};

template <class It, class = std::enable_if_t<std::is_default_constructible<It>::value>>
struct MyForwardIterator : public MyInputIterator<It> {
    using value_type        = typename std::iterator_traits<It>::value_type;
    using difference_type   = typename std::iterator_traits<It>::difference_type;
    using reference         = typename std::iterator_traits<It>::reference;
    using pointer           = typename std::iterator_traits<It>::pointer;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;

    static_assert(std::is_same<reference, const value_type&>::value, "Types are not the same");

    BOOST_CONCEPT_USAGE(MyForwardIterator) {
        It ret1 = i++;
        reference ret2 = *i++;
    }

private:
    It i;
};

template <class It>
struct MyBidirectionalIterator : public MyForwardIterator<It> {
    using value_type        = typename std::iterator_traits<It>::value_type;
    using difference_type   = typename std::iterator_traits<It>::difference_type;
    using reference         = typename std::iterator_traits<It>::reference;
    using pointer           = typename std::iterator_traits<It>::pointer;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;

    BOOST_CONCEPT_USAGE(MyBidirectionalIterator) {
        It& ret1 = --a;
        auto ret2 = a--;
        static_assert(std::is_convertible<decltype(ret2), const It&>::value, "Can't be converted");

        reference ret3 = *a--;
    }

private:
    It a;
};

template <class It>
struct MyRandomAccessIterator : public MyBidirectionalIterator<It> {
    using value_type        = typename std::iterator_traits<It>::value_type;
    using difference_type   = typename std::iterator_traits<It>::difference_type;
    using reference         = typename std::iterator_traits<It>::reference;
    using pointer           = typename std::iterator_traits<It>::pointer;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;

    BOOST_CONCEPT_USAGE(MyRandomAccessIterator) {
        It& ret1 = i += n;
        It ret2 = a + n;
        It ret3 = n + a;
        It& ret4 = r -= n;
        It ret5 = i - n;
        difference_type ret6 = b - a;
        auto ret7 = i[n];
        static_assert(std::is_convertible<decltype(ret7), reference>::value, "Can't be converted");

        bool ret8 = a < b;
        bool ret9 = a > b;
        bool ret10 = a >= b;
        bool ret11 = a <= b;
    }

private:
    It i;
    const It a, b;
    It& r;
    difference_type n;
};