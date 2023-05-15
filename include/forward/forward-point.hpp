#ifndef FORWARD_POINT_HPP__
#define FORWARD_POINT_HPP__

#include <geometry/geometry.hpp>

namespace forward {
    template <typename T> using Point       = geometry::d3::Point<T>;
    template <typename T> using Points      = std::vector<Point<T>>;

    template <typename T> using Centroid    = geometry::d3::Point<T>;
    template <typename T> using Centroids   = std::vector<Centroid<T>>;
} // namespace forward

#endif // FORWARD_POINT_HPP__
