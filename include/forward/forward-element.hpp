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

    template <typename T>
    struct Normal {
        std::shared_ptr<Points<T>> nodes;

        forward::Normal<T>
        operator()(const Triangle& triangle) const {
            const Point<T>& p0 = (*nodes)[triangle.nodes[0]];
            const Point<T>& p1 = (*nodes)[triangle.nodes[1]];
            const Point<T>& p2 = (*nodes)[triangle.nodes[2]];

            Point<T> u = (p1 - p0);
            Point<T> v = (p2 - p0);
            return u.cross(v).normalized();
        }
    };
} // namespace element
} // namespace forward

#endif // FORWARD_ELEMENT_HPP__
