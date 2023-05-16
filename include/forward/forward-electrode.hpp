#ifndef FORWARD_ELECTRODE_HPP__
#define FORWARD_ELECTRODE_HPP__

#include <cstddef>
#include <memory>
#include <variant>
#include <vector>

#include "forward-point.hpp"

namespace forward {
namespace electrode {
    using Index = std::size_t;

    struct Point {
        Index node;
    };
} // namespace electrode
    using Electrode     = std::variant<electrode::Point>;
    using Electrodes    = std::vector<Electrode>;
namespace electrode {
    struct Nodes {
        std::vector<Index>
        operator()(const Point& point) const { return {point.node}; }
    };

    template <typename T>
    struct Centroid {
        std::shared_ptr<Points<T>> nodes;

        forward::Centroid<T>
        operator()(const Point& point) const {
            return (*nodes)[point.node];
        }
    };
} // namespace electrode
} // namespace forward

#endif // FORWARD_ELECTRODE_HPP__
