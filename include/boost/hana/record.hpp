/*!
@file
Defines `boost::hana::Record`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_HPP
#define BOOST_HANA_RECORD_HPP

#include <boost/hana/fwd/record.hpp>

#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/searchable.hpp>

// provided instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //! Folding a `Record` `R` is equivalent to folding a list of its members,
    //! in the same order as they appear in `members<R>()`.
    template <typename R>
    struct foldl_impl<R, when<is_a<Record, R>{}>> {
        template <typename Udt, typename S, typename F>
        static constexpr decltype(auto) apply(Udt&& udt, S&& s, F&& f) {
            return foldl(members<R>(), detail::std::forward<S>(s),
                [&udt, f(detail::std::forward<F>(f))]
                (auto&& s, auto&& member) -> decltype(auto) {
                    return f(
                        detail::std::forward<decltype(s)>(s),
                        second(detail::std::forward<decltype(member)>(member))(
                            detail::std::forward<Udt>(udt)
                        )
                    );
                }
            );
        }
    };

    template <typename R>
    struct foldr_impl<R, when<is_a<Record, R>{}>> {
        template <typename Udt, typename S, typename F>
        static constexpr decltype(auto) apply(Udt&& udt, S&& s, F&& f) {
            return foldr(members<R>(), detail::std::forward<S>(s),
                [&udt, f(detail::std::forward<F>(f))]
                (auto&& member, auto&& s) -> decltype(auto) {
                    return f(
                        second(detail::std::forward<decltype(member)>(member))(
                            detail::std::forward<Udt>(udt)
                        ),
                        detail::std::forward<decltype(s)>(s)
                    );
                }
            );
        }
    };

    //! Two `Records` of the same data type `R` are equal if and only if
    //! all their members are equal. The members are compared in the
    //! same order as they appear in `members<R>()`.
    template <typename R>
    struct equal_impl<R, R, when<is_a<Record, R>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return all(members<R>(), [&x, &y](auto&& member) -> decltype(auto) {
                auto accessor = second(detail::std::forward<decltype(member)>(member));
                return equal(accessor(x), accessor(y));
            });
        }
    };

    //! Minimal complete definition: `Record`
    //!
    //! Searching a `Record` `r` is equivalent to searching `to<Map>(r)`.
    template <typename R>
    struct find_impl<R, when<is_a<Record, R>{}>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X&& x, Pred&& pred) {
            return transform(
                find(members<R>(), [&pred](auto&& member) -> decltype(auto) {
                    return pred(first(detail::std::forward<decltype(member)>(member)));
                }),
                [&x](auto&& member) -> decltype(auto) {
                    return second(detail::std::forward<decltype(member)>(member))(
                        detail::std::forward<X>(x)
                    );
                }
            );
        }
    };

    template <typename R>
    struct any_impl<R, when<is_a<Record, R>{}>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X const&, Pred&& pred) {
            return any(members<R>(), [&pred](auto&& member) -> decltype(auto) {
                return pred(first(detail::std::forward<decltype(member)>(member)));
            });
        }
    };

    template <typename R>
    struct members_impl<R, when_valid<typename R::hana::members_impl>>
        : R::hana::members_impl
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RECORD_HPP
