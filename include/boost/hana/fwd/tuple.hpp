/*!
@file
Forward declares `boost::hana::Tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TUPLE_HPP
#define BOOST_HANA_FWD_TUPLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/static_constexpr.hpp>
#include <boost/hana/fwd/core/make.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! General purpose index-based heterogeneous sequence with a fixed length.
    //!
    //! `Tuple` is the bread and butter for static metaprogramming.
    //! Conceptually, it is exactly the same as a `std::tuple`; it is just
    //! a sequence able of holding objects of different types and whose size
    //! is fixed at compile-time. However, Hana's tuple provides much more
    //! functionality than its `std` counterpart, and it is also much more
    //! efficient than all standard library implementations tested so far.
    //!
    //! `Tuple`s are index-based sequences. If you need an associative
    //! sequence with a key-based access, then you should consider the
    //! `Map` and `Set` data types instead.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! `Sequence`, and all its superclasses
    //!
    //!
    //! Provided operators
    //! ------------------
    //! For convenience, operators for methods of the following concepts can
    //! be used with `Tuple`: `Comparable`, `Orderable`, `Monad`, `Iterable`.
    struct Tuple { };

    template <typename ...Xs>
    struct _tuple;

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Create a `Tuple` containing the given objects.
    //! @relates Tuple
    //!
    //! This is analoguous to `std::make_tuple` for creating Hana tuples.
    //! The elements are held by value inside the resulting tuple, and hence
    //! they are copied or moved in.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/tuple.cpp make<Tuple>
    template <>
    constexpr auto make<Tuple> = [](auto&& ...xs) {
        return _tuple<decayed(decltype(xs))...>{forwarded(xs)...};
    };
#endif

    //! Alias to `make<Tuple>`; provided for convenience.
    //! @relates Tuple
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto make_tuple = make<Tuple>;
#else
    namespace {
        constexpr auto const& make_tuple = make<Tuple>;
    }
#endif

    //! Create a `Tuple` specialized for holding `Type`s.
    //! @relates Tuple
    //!
    //! This is functionally equivalent to `make<Tuple>(type<T>...)`, except
    //! that using `tuple_t` allows the library to perform some compile-time
    //! optimizations. Also note that the type of the objects returned by
    //! `tuple_t` and an equivalent call to `make<Tuple>` may differ.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/tuple.cpp tuple_t
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename ...T>
    constexpr unspecified-type tuple_t{};
#else
    template <typename ...T>
    struct _tuple_t;

    namespace gcc_wknd {
        template <typename ...T>
        constexpr typename _tuple_t<T...>::_ make_tuple_t() { return {}; }
    }

    namespace {
        template <typename ...T>
        constexpr auto const& tuple_t =
                        detail::static_constexpr<typename _tuple_t<T...>::_>;
    }
#endif

    //! Create a `Tuple` specialized for holding `IntegralConstant`s.
    //! @relates Tuple
    //!
    //! This is functionally equivalent to `make<Tuple>(integral_constant<T, v>...)`,
    //! except that using `tuple_c` allows the library to perform some
    //! compile-time optimizations. Also note that the type of the objects
    //! returned by `tuple_c` and an equivalent call to `make<Tuple>` may differ.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/tuple.cpp tuple_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T, T ...v>
    constexpr unspecified-type tuple_c{};
#else
    template <typename T, T ...v>
    struct _tuple_c;

    namespace {
        template <typename T, T ...v>
        constexpr auto const& tuple_c = detail::static_constexpr<_tuple_c<T, v...>>;
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TUPLE_HPP
