#ifndef DBSON_OBJECT_HPP
#define DBSON_OBJECT_HPP

#include <boost/variant.hpp>
#include <boost/container/flat_map.hpp>

namespace binary_json {

    using boost::container::flat_map;

    using integer = int32_t;
    using real = double;
    using string = std::string;

    typedef boost::make_recursive_variant<
            integer, real, string,
            std::vector<boost::recursive_variant_>,
            flat_map<string, boost::recursive_variant_>
    >::type object_t;

    using array = std::vector<object_t>;
    using map = flat_map<string, object_t>;

    enum datatype_id {
        Integer = 'i',
        Real = 'r',
        String = 's',
        Array = 'a',
        Object = 'o',
        None = 'n'
    };

    template<typename T>
    concept JSONField =
        std::is_same<T, integer>::value ||
        std::is_same<T, real>::value ||
        std::is_same<T, string>::value ||
        std::is_same<T, array>::value ||
        std::is_same<T, map>::value;
}

#endif //DBSON_OBJECT_HPP
