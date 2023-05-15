#ifndef FORWARD_ELEMENT_HPP__
#define FORWARD_ELEMENT_HPP__

#include <cstddef>
#include <memory>
#include <variant>
#include <vector>

#include "forward-point.hpp"

namespace forward {
namespace element {
    using Index = std::size_t;

    struct Triangle {
        std::vector<Index> nodes;
    };
} // namespace element
    using Element   = std::variant<element::Triangle>;
    using Elements  = std::vector<Element>;
namespace element {
    struct Nodes {
        std::vector<Index>
        operator()(const Triangle& triangle) const { return triangle.nodes; }
    };

    template <typename T>
    struct Centroid {
        std::shared_ptr<Points<T>> nodes;

        forward::Centroid<T>
        operator()(const Triangle& triangle) const {
            forward::Centroid<T> c;
            T scale = (1.0 / 3.0);

            for (const Index& node : triangle.nodes) {
                c += ((*nodes)[node] * scale);
            }
            return c;
        }
    };
} // namespace element
} // namespace forward

#endif // FORWARD_ELEMENT_HPP__
