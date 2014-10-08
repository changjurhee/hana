/*!
@file
Defines `boost::hana::Printable::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PRINTABLE_LIST_MCD_HPP
#define BOOST_HANA_PRINTABLE_LIST_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/printable/mcd.hpp>
#include <boost/hana/string.hpp>

#include <string>


namespace boost { namespace hana {
    //! @todo Document and write an example.
    struct Printable::list_mcd : Printable::mcd {
        template <typename Xs>
        static auto print_impl(Xs&& xs) {
            std::string result = "(";
            auto ready = intersperse(BOOST_HANA_STRING(", "), fmap(detail::std::forward<Xs>(xs), print));
            for_each(detail::std::move(ready), [&result](auto&& x) {
                result += detail::std::forward<decltype(x)>(x);
            });
            result += ")";
            return result;
        }
    };

    //! @todo Document and write an example.
    template <typename L>
    struct Printable::instance<L, when<is_a<List, L>()>>
        : Printable::list_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRINTABLE_LIST_MCD_HPP
