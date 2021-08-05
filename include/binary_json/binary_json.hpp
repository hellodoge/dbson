#ifndef DBSON_BINARY_JSON_HPP
#define DBSON_BINARY_JSON_HPP

#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <iterator>
#include <boost/optional.hpp>
#include <boost/container/flat_map.hpp>

#include "binary_json/object.hpp"
#include "binary_json/serialize.hpp"

namespace binary_json {

    struct BinJSON {

        template <JSONField T>
        explicit BinJSON(T t) {
            this->obj = std::make_unique<object_t>(std::move(t));
            this->id = getDatatypeId<T>();
        }

        template <JSONField T>
        auto getValue() -> boost::optional<std::reference_wrapper<T>> {
            auto v = std::get_if<T>(this->obj.get());
            if (v != NULL)
                return boost::optional<std::reference_wrapper<T>>(*v);
            else
                return boost::none;
        }

        template<typename Writer> // std::output_iterator should be here
        size_t serialize(Writer w) {
            *w++ = static_cast<char>(id);
            size_t size = 0;
            if (auto v = std::get_if<integer>(this->obj.get())) {
                size = serialize_integer(w, *v) + 0;
            } else if (auto v = std::get_if<string>(this->obj.get())) {
                size = serialize_string(w, std::ref(*v));
            }
            return size + 1;
        }

    private:
        std::unique_ptr<object_t> obj;
        datatype_id id;
    };
}

#endif //DBSON_BINARY_JSON_HPP
