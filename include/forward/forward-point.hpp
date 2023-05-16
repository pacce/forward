#ifndef FORWARD_POINT_HPP__
#define FORWARD_POINT_HPP__

#include <geometry/geometry.hpp>

namespace forward {
    template <typename T> using Point       = geometry::d3::Point<T>;
    template <typename T> using Points      = std::vector<Point<T>>;

    template <typename T> using Normal      = Point<T>;
    template <typename T> using Normals     = Points<T>;

    template <typename T> using Centroid    = Point<T>;
    template <typename T> using Centroids   = Points<T>;
} // namespace forward

#endif // FORWARD_POINT_HPP__
