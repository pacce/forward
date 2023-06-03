#ifndef FORWARD_ELEMENT_HPP__
#define FORWARD_ELEMENT_HPP__

#include <cmath>
#include <cstddef>
#include <map>
#include <memory>
#include <variant>
#include <vector>

#include "forward-point.hpp"

namespace forward {
namespace element {
    using Index = std::size_t;
    using Pair  = std::pair<Index, Index>;

    struct Triangle {
        std::vector<Index> nodes;

        template <typename T>
        forward::Centroid<T>
        centroid(const forward::Points<T>& ps) const {
            forward::Centroid<T> c;
            T scale = (1.0 / 3.0);

            for (const Index& node : nodes) {
                c += (ps[node] * scale);
            }
            return c;
        }

        template <typename T>
        forward::Normal<T>
        normal(const forward::Points<T>& ps) const {
            const Point<T>& p0 = ps[nodes[0]];
            const Point<T>& p1 = ps[nodes[1]];
            const Point<T>& p2 = ps[nodes[2]];

            Point<T> u = (p1 - p0);
            Point<T> v = (p2 - p0);

            return u.cross(v);
        }

        template <typename T>
        T
        area(const forward::Points<T>& ps) const {
            return this->normal(ps).norm() * 0.5;
        }

        template <typename T>
        std::map<Pair, T>
        stiffness(const forward::Points<T>& ps, const T resistivity, const T thickness = 1.0) const {
            const Point<T>& p0 = ps[nodes[0]];
            const Point<T>& p1 = ps[nodes[1]];
            const Point<T>& p2 = ps[nodes[2]];

            std::vector<T> bs = {
                  p1.y() - p2.y()
                , p2.y() - p0.y()
                , p0.y() - p1.y()
            };
            std::vector<T> gs = {
                  p2.x() - p1.x()
                , p0.x() - p2.x()
                , p1.x() - p0.x()
            };
            T scalar = thickness / (4.0 * this->area(ps) * resistivity);

            std::map<Pair, T> vs;
            for (std::size_t i = 0; i < 3; i++) {
            for (std::size_t j = 0; j < 3; j++) {
                vs.emplace(Pair{nodes[i], nodes[j]}, scalar * (bs[i] * bs[j] + gs[i] * gs[j]));
            }
            }
            return vs;
        }
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
        operator()(const Triangle& triangle) const { return triangle.centroid(*nodes); }
    };

    template <typename T>
    struct Normal {
        std::shared_ptr<Points<T>> nodes;

        forward::Normal<T>
        operator()(const Triangle& triangle) const { return triangle.normal(*nodes); }
    };

    template <typename T>
    struct Dimension {
        std::shared_ptr<Points<T>> nodes;

        T
        operator()(const Triangle& triangle) const { return triangle.area(*nodes); }
    };

    template <typename T>
    struct Stiffness {
        std::shared_ptr<Points<T>>  nodes;
        T resistivity;
        T thickness;

        std::map<std::pair<Index, Index>, T>
        operator()(const Triangle& triangle) const {
            return triangle.stiffness(*nodes, resistivity, thickness);
        }
    };
} // namespace element
} // namespace forward

#endif // FORWARD_ELEMENT_HPP__
