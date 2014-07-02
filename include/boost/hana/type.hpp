/*!
@file
Defines `boost::hana::Type`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>

#include <initializer_list>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! C++ type represented as a constexpr object.
    //!
    //! ## Instance of
    //! `Comparable`, `Functor`, `Applicative`, `Monad`
    //!
    //!
    //! @todo
    //! - Completely figure out and document the category theoretical
    //!   foundation of this data type.
    //! - Verify `Monad` laws.
    //! - Instantiate `Applicative`.
    //! - Move self-notes for `Type`-related stuff to the (internal?)
    //!   documentation of `Type`.
    //! - Consider having a `.name()` method that would return the
    //!   (demangled?) `typeid(T).name()`.
    //! - Document `Functor` and `Monad` instances.
    //! - Use more lambdas once http://llvm.org/bugs/show_bug.cgi?id=20046
    //!   is fixed.
    struct Type { };

    namespace type_detail {
        template <typename T>
        struct make_type {
            struct type_ : operators::enable {
                using hana_datatype = Type;
                using type = T;

                template <typename U>
                constexpr auto operator()(std::initializer_list<U> ilist) const
                { return T(ilist); }

                template <typename ...Args>
                constexpr auto operator()(Args ...args) const
                { return T{args...}; }
            };
            using type = type_;
        };
    }

    //! Creates an object representing the C++ type `T`.
    //! @relates Type
    //! @hideinitializer
    //!
    //! `type<T>` is a function returning an object of type `T` constructed
    //! with the arguments passed to it:
    //! @snippet example/type/construct.cpp main
    //!
    //! `std::initializer_list` is supported too:
    //! @snippet example/type/initializer_list.cpp main
    //!
    //! `decltype(type<T>)` also has a nested alias to `T` named `type`.
    //! Hence, it can be used as a metafunction returning `T`:
    //! @snippet example/type/as_metafunction.cpp main
    //!
    //! @todo
    //! Should this fail or not? Currently it fails because
    //! "non-constant-expression cannot be narrowed from type 'double' to
    //! 'float' in initializer list"
    //! @code
    //!     type<float>(double{1.2})
    //! @endcode
    template <typename T>
    constexpr typename type_detail::make_type<T>::type type{};

    namespace type_detail {
        struct decltype_ {
            template <typename T>
            constexpr auto operator()(T) const
            { return type<T>; }
        };

        struct sizeof_ {
            template <typename T>
            constexpr auto operator()(T) const
            { return size_t<sizeof(typename T::type)>; }
        };
    }

    //! Returns the type of an object as a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/decltype.cpp main
    constexpr type_detail::decltype_ decltype_{};

    //! Returns the size of the C++ type represented by a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/sizeof.cpp main
    //!
    //! @todo
    //! Should we also support non-`Type`s? That could definitely be useful.
    constexpr type_detail::sizeof_ sizeof_{};

    namespace type_detail {
        template <template <typename ...> class f>
        struct template_ {
            template <typename ...Args>
            constexpr auto operator()(Args...) const
            { return type<f<typename Args::type...>>; }
        };

        template <template <typename ...> class f>
        struct metafunction {
            template <typename ...Args>
            constexpr auto operator()(Args...) const
            { return type<typename f<typename Args::type...>::type>; }
        };

        template <typename f>
        struct metafunction_class {
            template <typename ...Args>
            constexpr auto operator()(Args...) const
            { return type<typename f::template apply<typename Args::type...>::type>; }
        };

        template <template <typename ...> class f>
        struct trait {
            template <typename ...Args>
            constexpr auto operator()(Args...) const
            { return f<typename Args::type...>{}; }
        };

        template <template <typename ...> class f>
        struct trait_ {
            template <typename ...T>
            constexpr auto operator()(T ...t) const
            { return f<decltype(t)...>{}; }
        };
    }

    //! Lift a template to a function on `Type`s.
    //! @relates Type
    //!
    //! Specifically, `template_<f>` is a function on `Type`s satisfying
    //! @code
    //!     template_<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>>
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/type/template.cpp main
    template <template <typename ...> class f>
    constexpr type_detail::template_<f> template_{};

    //! Lift a metafunction to a function on `Type`s.
    //! @relates Type
    //!
    //! Specifically, `metafunction<f>` is a function on `Type`s satisfying
    //! @code
    //!     metafunction<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>::type>
    //! @endcode
    template <template <typename ...> class f>
    constexpr type_detail::metafunction<f> metafunction{};

    //! Lift a metafunction class to a function on `Type`s.
    //! @relates Type
    //!
    //! Specifically, `metafunction_class<f>` is a function on `Type`s
    //! satisfying
    //! @code
    //!     metafunction_class<f>(type<x1>, ..., type<xN>) == type<f::apply<x1, ..., xN>::type>
    //! @endcode
    template <typename f>
    constexpr type_detail::metafunction_class<f> metafunction_class{};

    //! Lift a metafunction to a function taking `Type`s and returning a
    //! default-constructed object.
    //! @relates Type
    //!
    //! Specifically, `trait<f>(t...)` is equivalent to `template_<f>(t...)()`.
    //! The principal use case for `trait` is to transform metafunctions
    //! inheriting from a meaningful base like `std::integral_constant`
    //! into functions returning e.g. an `Integral`.
    //!
    //! The word `trait` is used because a name was needed and the principal
    //! use case involves metafunctions from the standard that we also call
    //! type traits.
    //!
    //! ### Example
    //! @snippet example/type/trait.cpp liftable
    //!
    //! Note that not all metafunctions of the standard library can be lifted
    //! this way. For example, `std::aligned_storage` can't be lifted because
    //! it requires non-type template parameters. Since there is no uniform
    //! way of dealing with non-type template parameters, one must resort to
    //! using e.g. an inline lambda to "lift" those metafunctions. In practice,
    //! however, this should not be a problem.
    //!
    //! ### Example of a non-liftable metafunction
    //! @snippet example/type/trait.cpp nonliftable
    //!
    //! @note
    //! When using `trait` with metafunctions returning `std::integral_constant`s,
    //! don't forget to include the boost/hana/adapted/std_integral_constant.hpp
    //! header!
    template <template <typename ...> class f>
    constexpr type_detail::trait<f> trait{};

    //! Equivalent to `compose(trait<f>, decltype_)`; provided for convenience.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/trait_.cpp main
    template <template <typename ...> class f>
    constexpr type_detail::trait_<f> trait_{};

    //! @details
    //! Two `Type`s are equal if and only if they represent the same C++ type.
    //! Hence, equality is equivalent to the `std::is_same` type trait.
    //!
    //! @snippet example/type/comparable.cpp main
    template <>
    struct Comparable::instance<Type, Type> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto equal_impl(T, U)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(T, T)
        { return true_; }
    };

    template <>
    struct Functor::instance<Type> : Functor::fmap_mcd {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, T t)
        { return f(t); }
    };

    template <>
    struct Monad::instance<Type> : Monad::flatten_mcd {
        template <typename T>
        static constexpr auto flatten_impl(T)
        { return typename T::type{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
