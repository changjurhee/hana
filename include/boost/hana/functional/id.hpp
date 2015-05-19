/*!
@file
Defines `boost::hana::id`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_ID_HPP
#define BOOST_HANA_FUNCTIONAL_ID_HPP

#include <boost/hana/detail/static_constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! The identity function -- returns its argument unchanged.
    //!
    //! ### Example
    //! @snippet example/functional.cpp id
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto id = [](auto&& x) -> decltype(auto) {
        return forwarded(x);
    };
#else
    struct _id {
        template <typename T>
        constexpr T operator()(T&& t) const {
            return static_cast<T&&>(t);
        }
    };

    namespace {
        constexpr auto const& id = detail::static_constexpr<_id>;
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ID_HPP
