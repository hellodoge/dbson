#ifndef DBSON_OBJECT_HPP
#define DBSON_OBJECT_HPP

namespace binary_json {

    class BinJSON;

    typedef int32_t integer;
    typedef double real;
    typedef std::string string;
    typedef std::vector<BinJSON> array;
    typedef boost::container::flat_map<std::string, BinJSON> map;

    typedef std::variant<
            integer,
            real,
            string,
            array,
            map
    > object_t;

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

    template<JSONField T>
    constexpr datatype_id getDatatypeId() {
        if (typeid(T) == typeid(integer))
            return Integer;
        if (typeid(T) == typeid(real))
            return Real;
        if (typeid(T) == typeid(string))
            return String;
        if (typeid(T) == typeid(array))
            return Array;
        if (typeid(T) == typeid(map))
            return Object;
        return None;
    }
}

#endif //DBSON_OBJECT_HPP
