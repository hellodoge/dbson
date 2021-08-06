#ifndef DBSON_BSWAP_HPP
#define DBSON_BSWAP_HPP

namespace util {
    template <typename T>
    T bswap(T x) {
        char *s = reinterpret_cast<char *>(&x);
        char *e = s + sizeof(T) - 1;
        while (s < e) {
            char tmp = *s;
            *s = *e;
            *e = tmp;
            *s++, *e--;
        }
        return x;
    }
}

#endif //DBSON_BSWAP_HPP
