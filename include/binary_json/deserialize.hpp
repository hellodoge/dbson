#ifndef DBSON_DESERIALIZE_HPP
#define DBSON_DESERIALIZE_HPP

#include <arpa/inet.h>
#include <byteswap.h>

#include "binary_json/object.hpp"
#include "exceptions/eof_error.hpp"

namespace binary_json {

    template <typename To, typename Reader, typename ReaderEnd>
    To read_uint(Reader &r, ReaderEnd end) noexcept(false) {
        To value;
        char *buffer = reinterpret_cast<char *>(&value);
        for (size_t i = 0; i < sizeof(To); i++) {
            if (r == end)
                throw eof_error{};
            buffer[i] = *r++;
        }
        return value;
    }

    template <typename Reader, typename ReaderEnd>
    size_t deserialize_len(Reader &r, ReaderEnd end) noexcept(false) {
        if (r == end)
            throw eof_error{};
        size_t size = static_cast<size_t>(*r++);
        if (size > sizeof(size_t))
            throw std::length_error("deserializer: int len is too big");
        if (size == sizeof(uint16_t)) {
            uint16_t value_u16 = read_uint<uint16_t>(r, end);
            value_u16 = ntohs(value_u16);
            return static_cast<size_t>(value_u16);
        }
        if (size == sizeof(uint32_t)) {
            uint32_t value_u32 = read_uint<uint32_t>(r, end);
            value_u32 = ntohl(value_u32);
            return static_cast<size_t>(value_u32);
        }
        if (size == sizeof(uint64_t)) {
            uint64_t value_u64 = read_uint<uint64_t>(r, end);
            if (1 != htonl(1))
                value_u64 = bswap_64(value_u64);
            return static_cast<size_t>(value_u64);
        }
        throw std::logic_error("deserializer: unsupported int len");
    }

    template <typename Reader, typename ReaderEnd>
    integer deserialize_integer(Reader &r, ReaderEnd end) noexcept(false) {
        uint32_t deserialized = read_uint<uint32_t>(r, end);
        deserialized = ntohl(deserialized);
        return static_cast<integer>(deserialized);
    }

    template <typename Reader, typename ReaderEnd>
    string deserialize_string(Reader &r, ReaderEnd end) noexcept(false) {
        const size_t str_size = deserialize_len(r, end);
        string s{};
        s.reserve(str_size);
        for (size_t i = 0; i < str_size; i++) {
            if (r == end)
                throw eof_error{};
            s.push_back(*r++);
        }
        return s;
    }

    template <typename Reader, typename ReaderEnd>
    object_t deserialize_object(Reader &r, ReaderEnd end) noexcept(false);

    template <typename Reader, typename ReaderEnd>
    array deserialize_array(Reader &r, ReaderEnd end) noexcept(false) {
        const size_t arr_len = deserialize_len(r, end);
        array arr{};
        arr.reserve(arr_len);
        for (size_t i = 0; i < arr_len; i++)
            arr.push_back(deserialize_object(r, end));
        return arr;
    }

    template <typename Reader, typename ReaderEnd>
    object_t deserialize_object(Reader &r, ReaderEnd end) noexcept(false) {
        if (r == end)
            throw eof_error{};
        datatype_id type = static_cast<datatype_id>(*r++);
        switch (type) {
            case Integer:
                return deserialize_integer(r, end);
            case Real:
                break;
            case String:
                return deserialize_string(r, end);
            case Array:
                return deserialize_array(r, end);
            case Assoc:
                break;
            case None:
                break;
        }
        throw std::domain_error("unknown type");
    }

}

#endif //DBSON_DESERIALIZE_HPP
