#ifndef FORWARD_INCLUSION_HPP__
#define FORWARD_INCLUSION_HPP__

#include <dimension/dimension.hpp>
#include <geometry/geometry.hpp>

namespace forward {
namespace inclusion {
    template <typename T>
    class Circular {
        public:
            Circular(const geometry::d2::Point<T>& center, const dimension::radius& radius) 
                : center_(center)
                , radius_(radius)
            {}

            bool
            contains(const geometry::d2::Point<T>& p) {
                return (p - center_).norm() < radius_;
            }
        private:
            geometry::d2::Point<T>  center_;
            dimension::radius       radius_;
    };
} // namespace inclusion
} // namespace forward

#endif // FORWARD_INCLUSION_HPP__
