/*!
@file
Forward declares `boost::hana::eval`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_EVAL_FWD_HPP
#define BOOST_HANA_DETAIL_EVAL_FWD_HPP

#include <boost/hana/detail/static_constexpr.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/when.hpp>


namespace boost { namespace hana {
    template <typename T, typename = void>
    struct eval_impl;

    template <typename T, typename>
    struct eval_impl : eval_impl<T, when<true>> { };

    struct _eval {
        template <typename Expr>
        constexpr decltype(auto) operator()(Expr&& expr) const {
            return eval_impl<typename datatype<Expr>::type>::apply(
                static_cast<Expr&&>(expr)
            );
        }
    };

    namespace {
        constexpr auto const& eval = detail::static_constexpr<_eval>;
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_EVAL_FWD_HPP
