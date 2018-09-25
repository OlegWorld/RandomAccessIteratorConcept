#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <boost/concept_check.hpp>

//template <class T>
//struct MyDefaultConstructible {
//    BOOST_CONCEPT_USAGE(MyDefaultConstructible) {
//        T u1;
//        T u2{};
//        T();
//        T{};
//    }
//};

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

        reference r = *i;
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

template <class It, class ValueT,
          class = std::enable_if_t<std::is_class<It>::value || std::is_pointer<It>::value>>
struct MyOutputIterator : public MyIterator<It> {
    using value_type        = typename std::iterator_traits<It>::value_type;
    using difference_type   = typename std::iterator_traits<It>::difference_type;
    using reference         = typename std::iterator_traits<It>::reference;
    using pointer           = typename std::iterator_traits<It>::pointer;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;

    BOOST_CONCEPT_USAGE(MyOutputIterator) {
        *r = o;
        It& res1 = ++r;
        auto res2 = r++;
        static_assert(std::is_convertible<decltype(res2), const It&>::value, "Can't be converted");
        *r++ = o;
    }

private:
    It r;
    ValueT o;
};
namespace internal {
    template <class It>
    struct MutableRef_t {
        using value_type        = typename std::iterator_traits<It>::value_type;
        using reference         = typename std::iterator_traits<It>::reference;

        static_assert(std::is_same<reference, value_type&>::value, "Reference type is not mutable");
    };

    template <class It>
    struct ConstRef_t {
        using value_type        = typename std::iterator_traits<It>::value_type;
        using reference         = typename std::iterator_traits<It>::reference;

        static_assert(std::is_same<reference, const value_type&>::value, "Reference type is not constant");
    };

    template <class It, class = std::enable_if_t<std::is_default_constructible<It>::value>>
    struct __MyForwardIteratorChecker : public MyInputIterator<It> {
        using value_type        = typename std::iterator_traits<It>::value_type;
        using difference_type   = typename std::iterator_traits<It>::difference_type;
        using reference         = typename std::iterator_traits<It>::reference;
        using pointer           = typename std::iterator_traits<It>::pointer;
        using iterator_category = typename std::iterator_traits<It>::iterator_category;

        BOOST_CONCEPT_USAGE(__MyForwardIteratorChecker) {
            It ret1 = i++;
            reference ret2 = *i++;
        }

    private:
        It i;
    };

    template <class It>
    struct __MyBidirectionalIteratorChecker : public __MyForwardIteratorChecker<It> {
        using value_type        = typename std::iterator_traits<It>::value_type;
        using difference_type   = typename std::iterator_traits<It>::difference_type;
        using reference         = typename std::iterator_traits<It>::reference;
        using pointer           = typename std::iterator_traits<It>::pointer;
        using iterator_category = typename std::iterator_traits<It>::iterator_category;

        BOOST_CONCEPT_USAGE(__MyBidirectionalIteratorChecker) {
            It& ret1 = --a;
            auto ret2 = a--;
            static_assert(std::is_convertible<decltype(ret2), const It&>::value, "Can't be converted");

            reference ret3 = *a--;
        }

    private:
        It a;
    };

    template <class It>
    struct __MyRandomAccessIteratorChecker : public __MyBidirectionalIteratorChecker<It> {
        using value_type        = typename std::iterator_traits<It>::value_type;
        using difference_type   = typename std::iterator_traits<It>::difference_type;
        using reference         = typename std::iterator_traits<It>::reference;
        using pointer           = typename std::iterator_traits<It>::pointer;
        using iterator_category = typename std::iterator_traits<It>::iterator_category;

        BOOST_CONCEPT_USAGE(__MyRandomAccessIteratorChecker) {
            It& ret1 = i += n;
            It ret2 = a + n;
            It ret3 = n + a;
            It& ret4 = r -= n;
            It ret5 = i - n;
            difference_type ret6 = b - a;
            reference ret7 = i[n];

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
}


template <class It>
struct MyForwardIterator : public internal::__MyForwardIteratorChecker<It>,
                           public internal::ConstRef_t<It>
{ };

template <class It>
struct MyMutableForwardIterator : public internal::__MyForwardIteratorChecker<It>,
                                  public MyOutputIterator<It, typename internal::__MyForwardIteratorChecker<It>::value_type>,
                                  public internal::MutableRef_t<It>
{ };

template <class It>
struct MyBidirectionalIterator : public internal::__MyBidirectionalIteratorChecker<It>,
                                 public internal::ConstRef_t<It>
{ };

template <class It>
struct MyMutableBidirectionalIterator : public internal::__MyBidirectionalIteratorChecker<It>,
                                        public MyOutputIterator<It, typename  internal::__MyBidirectionalIteratorChecker<It>::value_type>,
                                        public internal::MutableRef_t<It>
{ };


template <class It>
struct MyRandomAccessIterator : public internal::__MyRandomAccessIteratorChecker<It>,
                                public internal::ConstRef_t<It>
{ };

template <class It>
struct MyMutableRandomAccessIterator : public internal::__MyRandomAccessIteratorChecker<It>,
                                       public MyOutputIterator<It, typename  internal::__MyRandomAccessIteratorChecker<It>::value_type>,
                                       public internal::MutableRef_t<It>
{ };