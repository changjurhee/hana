/*!
@file
Defines the instance of `boost::hana::Printable` for `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_PRINTABLE_HPP
#define BOOST_HANA_MAP_PRINTABLE_HPP

#include <boost/hana/core/convert.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/printable/mcd.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/tuple/convert.hpp>
#include <boost/hana/tuple/functor.hpp>

#include <string>


namespace boost { namespace hana {
    //! Instance of `Printable` for `Map`s.
    //!
    //! @todo Document and write example.
    //!
    //! ### Example
    //! @snippet example/map.cpp printable
    template <>
    struct Printable::instance<Map> : Printable::mcd {
        template <typename M>
        static auto print_impl(M&& map) {
            std::string result = "{";
            auto pairs = fmap(to<Tuple>(detail::std::forward<M>(map)),
                [](auto&& pair) -> decltype(auto) {
                    return print(first(detail::std::forward<decltype(pair)>(pair)))
                            + "=>" +
                           print(second(detail::std::forward<decltype(pair)>(pair)));
                });
            auto ready = intersperse(", ", detail::std::move(pairs));
            for_each(detail::std::move(ready), [&result](auto&& elt) {
                result += detail::std::forward<decltype(elt)>(elt);
            });
            result += "}";
            return result;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_PRINTABLE_HPP
