#ifndef DBSON_OBJECT_HPP
#define DBSON_OBJECT_HPP

#include <boost/none.hpp>
#include <boost/variant.hpp>
#include <boost/container/flat_map.hpp>

namespace binary_json {

    using boost::container::flat_map;

    using integer = int32_t;
    using real = double;
    using string = std::string;
    using none = boost::none_t;

    typedef boost::make_recursive_variant<
            integer, real, string,
            std::vector<boost::recursive_variant_>,
            flat_map<string, boost::recursive_variant_>,
            none
    >::type object_t;

    using array = std::vector<object_t>;
    using assoc = flat_map<string, object_t>;

    enum datatype_id {
        Integer = 'i',
        Real = 'r',
        String = 's',
        Array = 'a',
        Assoc = 'o',
        None = 'n'
    };

    template<typename T>
    concept JSONField =
        std::is_same<T, integer>::value ||
        std::is_same<T, real>::value ||
        std::is_same<T, string>::value ||
        std::is_same<T, array>::value ||
        std::is_same<T, assoc>::value ||
        std::is_same<T, none>::value;
}

#endif //DBSON_OBJECT_HPP
