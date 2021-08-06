#ifndef DBSON_SERIALIZE_HPP
#define DBSON_SERIALIZE_HPP

#include <netinet/in.h>
#include <byteswap.h>
#include <numeric>

#include "binary_json/object.hpp"

namespace binary_json {

    template <typename From, typename Target, typename Writer>
    requires (
            std::is_same<Target, uint16_t>::value ||
            std::is_same<Target, uint32_t>::value ||
            std::is_same<Target, uint64_t>::value
    )
    size_t serialize_int(Writer w, From x) {
        Target network_order;
        if (std::is_same<Target, uint16_t>::value) {
            network_order = htons(static_cast<uint16_t>(x));
        } else if (std::is_same<Target, uint32_t>::value) {
            network_order = htonl(static_cast<uint32_t>(x));
        } else if (1 != htonl(1)) // htonll is not available so
            network_order = bswap_64(static_cast<uint64_t>(x));
        const char *bytes = reinterpret_cast<const char *>(&network_order);
        for (size_t i = 0; i < sizeof(Target); i++)
            *w++ = bytes[i];
        return sizeof(Target);
    }

    template <typename Writer>
    size_t serialize_len(Writer w, size_t len) {
        if (len <= std::numeric_limits<uint16_t>::max()) {
            *w++ = static_cast<char>(sizeof(uint16_t));
            return serialize_int<size_t, uint16_t>(w, len) + 1;
        }
        if (len <= std::numeric_limits<uint32_t>::max()) {
            *w++ = static_cast<char>(sizeof(uint32_t));
            return serialize_int<size_t, uint32_t>(w, len) + 1;
        }
        if (len <= std::numeric_limits<uint64_t>::max()) {
            *w++ = static_cast<char>(sizeof(uint64_t));
            return serialize_int<size_t, uint64_t>(w, len) + 1;
        }
        return 0;
    }

    template <typename Writer>
    size_t serialize_integer(Writer w, integer x) {
        *w++ = Integer;
        return serialize_int<integer, uint32_t>(w, x);
    }

    template <typename Writer>
    size_t serialize_string(Writer w, const string &s) {
        *w++ = String;
        const size_t str_size = s.length();
        const size_t len_size = serialize_len(w, str_size);
        const char *bytes = s.c_str();
        for (size_t i = 0; i < str_size; i++)
            *w++ = bytes[i];
        return len_size + str_size;
    }

    template <typename Writer>
    size_t serialize_object(Writer w, const object_t &obj);

    template <typename Writer>
    size_t serialize_array(Writer w, const array &arr) {
        *w++ = Array;
        const size_t arr_length = arr.size();
        size_t size = serialize_len(w, arr_length);
        for (auto&& el : arr) {
            size += serialize_object(w, el);
        }
        return size;
    }

    template <typename Writer>
    size_t serialize_object(Writer w, const object_t &obj) {
        auto visitor = [&](auto &v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same<T, integer>::value)
                return serialize_integer(w, v);
            if constexpr (std::is_same<T, string>::value)
                return serialize_string(w, v);
            if constexpr (std::is_same<T, array>::value)
                return serialize_array(w, v);
            return (size_t)0;
        };
        return boost::apply_visitor(visitor, obj);
    }
}

#endif //DBSON_SERIALIZE_HPP