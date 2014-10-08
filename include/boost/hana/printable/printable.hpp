/*!
@file
Forward declares `boost::hana::Printable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PRINTABLE_PRINTABLE_HPP
#define BOOST_HANA_PRINTABLE_PRINTABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! `Printable` represents data structures that can be represented as
    //! a string.
    //!
    //! ### Laws
    //! The representation must be such that
    //! @code
    //! 	print(x) == print(y) if and x == y
    //! @endcode
    //!
    //! This ensures that the printed representation of a data structure
    //! contains the same information as the data structure itself, and
    //! could be used in theory to recreate the original data structure.
    struct Printable {
        BOOST_HANA_TYPECLASS(Printable);
        struct mcd;
        struct integral_constant_mcd;
        struct list_mcd;
    };

    //!
    //! @relates Printable
    //!
    //! ### Example
    //! @snippet example/printable.cpp print
    constexpr auto print = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& printable) -> decltype(auto) {
        return Printable::instance<
            datatype_t<decltype(printable)>
        >::print_impl(detail::std::forward<decltype(printable)>(printable));
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRINTABLE_PRINTABLE_HPP
