/*!
@file
Defines the instance of `boost::hana::Printable` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_PRINTABLE_HPP
#define BOOST_HANA_FOREIGN_PRINTABLE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/printable/mcd.hpp>
#include <boost/hana/printable/printable.hpp>

#include <sstream>


namespace boost { namespace hana {
    //! Instance of `Printable` for foreign but `OutputStreamable` objects.
    //!
    //! Any foreign object that can be written to a standard stream using `<<`
    //! is automatically `Printable` by using that representation.
    template <typename T>
    struct Printable::instance<T,
        when<is_valid<decltype((void)(*(std::ostringstream*)0 << *(T*)0))>>
    >
        : Printable::mcd
    {
        template <typename X>
        static decltype(auto) print_impl(X&& x) {
            std::ostringstream os;
            os << detail::std::forward<X>(x);
            return os.str();
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_PRINTABLE_HPP
