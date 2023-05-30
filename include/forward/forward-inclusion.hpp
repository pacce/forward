#ifndef FORWARD_INCLUSION_HPP__
#define FORWARD_INCLUSION_HPP__

#include <dimension/dimension.hpp>

#include "forward-point.hpp"

namespace forward {
namespace inclusion {
    template <typename T>
    class Circular {
        public:
            Circular(const Point<T>& center, const Normal<T>& n, const dimension::radius& radius)
                : center_(center)
                , normal_(n.normalized())
                , radius_(radius)
            {}

            bool
            contains(const Point<T>& q) {
                Point<T> p = this->closest(q);
                return (p - center_).norm() < static_cast<T>(radius_);
            }
        private:
            Point<T>
            closest(const Point<T>& q) {
                return q - ((q - center_).dot(normal_) * normal_);
            }

            Point<T>            center_;
            Normal<T>           normal_;
            dimension::radius   radius_;
    };
} // namespace inclusion
} // namespace forward

#endif // FORWARD_INCLUSION_HPP__
