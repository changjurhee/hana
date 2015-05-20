/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_SEQ_HPP
#define BOOST_HANA_TEST_TEST_SEQ_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    namespace test {
        struct Seq;

        template <typename Storage>
        struct seq_type : operators::Iterable_ops<seq_type<Storage>> {
            explicit constexpr seq_type(Storage s) : storage(s) { }
            Storage storage;
            struct hana { using datatype = Seq; };
        };

        struct _seq {
            template <typename ...Xs>
            constexpr decltype(auto) operator()(Xs ...xs) const {
                auto storage = make_tuple(xs...);
                return seq_type<decltype(storage)>(storage);
            }
        };
        constexpr _seq seq{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_FOLDABLE_FOLDS_MCD
    //  BOOST_HANA_TEST_FOLDABLE_UNPACK_MCD
    //  BOOST_HANA_TEST_FOLDABLE_ITERABLE_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_FOLDABLE_FOLDS_MCD
    template <>
    struct fold_right_impl<test::Seq> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) {
            return hana::fold.right(xs.storage, s, f);
        }
    };

    template <>
    struct fold_left_impl<test::Seq> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) {
            return hana::fold.left(xs.storage, s, f);
        }
    };
#elif defined(BOOST_HANA_TEST_FOLDABLE_UNPACK_MCD)
    template <>
    struct unpack_impl<test::Seq> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f)
        { return hana::unpack(xs.storage, f); }
    };
#else
    template <>
    struct fold_left_impl<test::Seq>
        : Iterable::fold_left_impl<test::Seq>
    { };
    template <>
    struct fold_left_nostate_impl<test::Seq>
        : Iterable::fold_left_nostate_impl<test::Seq>
    { };
    template <>
    struct fold_right_impl<test::Seq>
        : Iterable::fold_right_impl<test::Seq>
    { };
    template <>
    struct fold_right_nostate_impl<test::Seq>
        : Iterable::fold_right_nostate_impl<test::Seq>
    { };
#endif

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<test::Seq> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return hana::head(xs.storage);
        }
    };

    template <>
    struct tail_impl<test::Seq> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return hana::unpack(hana::tail(xs.storage), test::seq);
        }
    };

    template <>
    struct is_empty_impl<test::Seq> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return hana::is_empty(xs.storage);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<test::Seq> {
        template <typename X>
        static constexpr auto apply(X x)
        { return test::seq(x); }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_SEQUENCE_MONAD_PLUS_MCD
    //  BOOST_HANA_TEST_SEQUENCE_PREPEND_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_SEQUENCE_MONAD_PLUS_MCD
    template <>
    struct concat_impl<test::Seq> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs xs, Ys ys) {
            return hana::unpack(
                hana::concat(xs.storage, ys.storage),
                test::seq
            );
        }
    };
#else
    template <>
    struct prepend_impl<test::Seq> {
        template <typename X, typename Xs>
        static constexpr auto apply(X x, Xs xs) {
            return hana::unpack(hana::prepend(x, xs.storage), test::seq);
        }
    };
#endif

    template <>
    struct empty_impl<test::Seq> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply()
        { return test::seq(); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, test::Seq>
        : detail::std::true_type
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_SEQ_HPP
