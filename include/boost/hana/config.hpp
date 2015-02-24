/*!
@file
Defines configuration macros used throughout the library.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONFIG_HPP
#define BOOST_HANA_CONFIG_HPP

//////////////////////////////////////////////////////////////////////////////
// Compiler-independent options
//////////////////////////////////////////////////////////////////////////////

#if defined(BOOST_HANA_DOXYGEN_INVOKED) || \
    (defined(NDEBUG) && !defined(BOOST_HANA_CONFIG_DISABLE_PRECONDITIONS))
    //! @ingroup group-config
    //! Disables the checking of preconditions in tag-dispatched methods.
    //!
    //! Some tag-dispatched methods have preconditions that can be checked
    //! reasonably easily. When this is the case, these preconditions are
    //! checked automatically when the method is used. When this macro is
    //! defined, the checks are not performed.
    //!
    //! This macro is defined automatically when `NDEBUG` is defined. It can
    //! also be defined by users before including this header or defined on
    //! the command line.
    //!
    //! @note
    //! This does not disable the `BOOST_HANA_*_ASSERT` macro & friends.
    //! See @ref BOOST_HANA_CONFIG_DISABLE_ASSERTIONS for this.
#   define BOOST_HANA_CONFIG_DISABLE_PRECONDITIONS
#endif

#if defined(BOOST_HANA_DOXYGEN_INVOKED) || \
    (defined(NDEBUG) && !defined(BOOST_HANA_CONFIG_DISABLE_ASSERTIONS))
    //! @ingroup group-config
    //! Disables the `BOOST_HANA_*_ASSERT` macro & friends.
    //!
    //! When this macro is defined, the `BOOST_HANA_*_ASSERT` macro & friends
    //! are disabled, i.e. they expand to nothing.
    //!
    //! This macro is defined automatically when `NDEBUG` is defined. It can
    //! also be defined by users before including this header or defined on
    //! the command line.
    //!
    //! @note
    //! This macro has the side effect of disabling the checking of most
    //! preconditions in tag-dispatched methods, because those preconditions
    //! usually use the `BOOST_HANA_*_ASSERT` macros to do their job. However,
    //! if one wants to disable the checking of preconditions specifically,
    //! the @ref BOOST_HANA_CONFIG_DISABLE_PRECONDITIONS macro should be used
    //! instead.
#   define BOOST_HANA_CONFIG_DISABLE_ASSERTIONS
#endif


//////////////////////////////////////////////////////////////////////////////
// Compiler-dependent options
//////////////////////////////////////////////////////////////////////////////

#if defined(BOOST_HANA_DOXYGEN_INVOKED)

    //! @ingroup group-config
    //! Enables some optimizations based on C++1z fold-expressions.
#   define BOOST_HANA_CONFIG_HAS_CXX1Z_FOLD_EXPRESSIONS

    //! @ingroup group-config
    //! Enables some constructs requiring `constexpr` lambdas, which are not
    //! in the language (yet).
#   define BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA

    //! @ingroup group-config
    //! Whether C++14 variable templates are supported by the compiler.
    //!
    //! This essentially affects some tag-dispatched methods which are
    //! regular functions instead of function objects when variable templates
    //! are not supported.
#   define BOOST_HANA_CONFIG_HAS_VARIABLE_TEMPLATES

    //! @ingroup group-config
    //! Whether C++14 relaxed constexpr functions are supported by the
    //! compiler.
    //!
    //! This essentially affects the implementation of some constexpr
    //! functions, which will use recursion instead of iteration if
    //! relaxed constexpr is not supported. Recursion tends to be
    //! slower at compile-time than iteration and it is also limited
    //! to the recursive `constexpr` depth of the compiler.
#   define BOOST_HANA_CONFIG_HAS_RELAXED_CONSTEXPR

    //! @ingroup group-config
    //! Whether non-static constexpr member functions are automatically marked
    //! as `const`.
    //!
    //! In C++11, a non-static constexpr member function is automatically
    //! marked as `const`. In C++14, this is not the case. This macro
    //! essentially influences which overloads are provided for non-static
    //! constexpr member functions. In particular, when such a function is
    //! automatically marked as `const`, we can't provide both overloads:
    //! @code
    //!     constexpr ... member_function(...) const&
    //!     constexpr ... member_function(...) &
    //! @endcode
    //! since they are actually the same.
#   define BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST


#elif defined(__clang__)

#   if __has_feature(cxx_variable_templates) || __has_extension(cxx_variable_templates)
#       define BOOST_HANA_CONFIG_HAS_VARIABLE_TEMPLATES
#   endif

#   if __has_feature(cxx_relaxed_constexpr) || __has_extension(cxx_relaxed_constexpr)
#       define BOOST_HANA_CONFIG_HAS_RELAXED_CONSTEXPR
#   endif


#elif defined(__GNUC__)

#   define BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST


#else

#   warning "Your compiler is not officially supportd by Hana or it was not detected properly. Good luck."

#endif


#ifdef BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA
    //! @ingroup group-details
    //! Expands to `constexpr` if constexpr lambdas are supported and
    //! to nothing otherwise.
#   define BOOST_HANA_CONSTEXPR_LAMBDA constexpr
#else
#   define BOOST_HANA_CONSTEXPR_LAMBDA /* nothing */
#endif

#endif // !BOOST_HANA_CONFIG_HPP
