/*!
@file
Defines the instance of `boost::hana::Printable` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_PRINTABLE_HPP
#define BOOST_HANA_STRING_PRINTABLE_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/printable/mcd.hpp>
#include <boost/hana/string/string.hpp>

#include <string>


namespace boost { namespace hana {
    //! @todo Document and write an example.
    template <>
    struct Printable::instance<String> : Printable::mcd {
        template <typename Str>
        static auto print_impl(Str&& str) {
            return '"' + std::string{value(detail::std::forward<Str>(str))} + '"';
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_PRINTABLE_HPP
