/*!
@file
Defines `boost::hana::Printable::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PRINTABLE_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_PRINTABLE_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foreign/printable.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>
#include <boost/hana/printable/mcd.hpp>


namespace boost { namespace hana {
    //! @todo Document and write an example.
    struct Printable::integral_constant_mcd : Printable::mcd {
        template <typename N>
        static auto print_impl(N const& n) {
            return print(value(n));
        }
    };

    //! @todo Document and write an example.
    template <typename I>
    struct Printable::instance<I, when<is_an<IntegralConstant, I>()>>
        : Printable::integral_constant_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRINTABLE_INTEGRAL_CONSTANT_MCD_HPP
