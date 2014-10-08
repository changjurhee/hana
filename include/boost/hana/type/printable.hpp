/*!
@file
Defines the instance of `boost::hana::Printable` for `boost::hana::Type`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_PRINTABLE_HPP
#define BOOST_HANA_TYPE_PRINTABLE_HPP

#include <boost/hana/printable/mcd.hpp>
#include <boost/hana/type/type.hpp>

#include <cxxabi.h>
#include <memory>
#include <string>
#include <typeinfo>


namespace boost { namespace hana {
    //! Instance of `Printable` for `Type`s.
    //!
    //! @todo Document and write example.
    //!
    //! @snippet example/type.cpp printable
    template <>
    struct Printable::instance<Type> : Printable::mcd {
        static std::string demangle(char const* mangled) {
            int status;
            std::unique_ptr<char[], void(*)(void*)> result(
                abi::__cxa_demangle(mangled, 0, 0, &status), std::free
            );
            return result.get() ? result.get() : mangled;
        }

        template <typename T>
        static auto print_impl(T const&) {
            return "type<" + demangle(typeid(typename T::type).name()) + '>';
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_PRINTABLE_HPP
