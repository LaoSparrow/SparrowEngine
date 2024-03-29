#pragma once

namespace SparrowEngine::Utils {

    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &p) const {
            std::size_t seed = 0;
            seed ^= std::hash<T1>()(p.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<T2>()(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };

}