/*!
@file
Defines `boost::hana::Type` and `boost::hana::Metafunction`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/fwd/type.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _type
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T>
    struct _type<T>::_ : _type<T>, operators::adl {
        struct hana { using datatype = Type; };
        using type = T;

        constexpr _ operator+() const { return *this; }
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <>
        struct of<Type>
            : operators::of<Comparable>
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Type, Type> {
        template <typename T, typename U>
        static constexpr _bool<false> apply(T, U)
        { return {}; }

        template <typename T>
        static constexpr _bool<true> apply(T, T)
        { return {}; }
    };

    //! @cond
    template <typename T>
    constexpr auto _sizeof::operator()(T) const
    { return _size_t<sizeof(typename T::type)>{}; }
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
