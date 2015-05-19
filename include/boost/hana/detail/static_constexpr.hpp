/*!
@file
Defines `boost::hana::detail::static_constexpr`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STATIC_CONSTEXPR_HPP
#define BOOST_HANA_DETAIL_STATIC_CONSTEXPR_HPP

namespace boost { namespace hana { namespace detail {
    template <typename T>
    struct _static_constexpr {
        static constexpr T value{};
    };

    template <typename T>
    constexpr T _static_constexpr<T>::value;

    //! @ingroup group-details
    //! Workaround to avoid ODR violations when declaring `static constexpr`
    //! objects.
    //!
    //! See [this paper][1] for a detailed explanation of how to use this
    //! utility.
    //!
    //! [1]: http://ericniebler.github.io/std/wg21/D4381.html
    template <typename T>
    constexpr auto const& static_constexpr = _static_constexpr<T>::value;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_STATIC_CONSTEXPR_HPP
