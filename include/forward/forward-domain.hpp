#ifndef FORWARD_DOMAIN_HPP__
#define FORWARD_DOMAIN_HPP__

#include <dimension/dimension.hpp>
#include <mesh/mesh.hpp>
#include <memory>

#include "forward-electrode.hpp"
#include "forward-element.hpp"
#include "forward-point.hpp"

namespace forward {
    template <typename T>
    class Domain {
        public:
            Domain(const Points<T>& nodes, const Elements& elements)
                : elements_(elements)
            {
                nodes_ = std::make_shared<Points<T>>();
                nodes_->reserve(nodes.size());
                std::copy(nodes.begin(), nodes.end(), nodes_->begin());
            }

            forward::Centroids<T>
            centroids() const {
                forward::Centroids<T> cs;
                cs.reserve(elements_.size());

                forward::element::Centroid<T> visitor(nodes_);
                for (const Element& element : elements_) {
                    cs.push_back(std::visit(visitor, element));
                }

                return cs;
            }
        private:
            std::shared_ptr<Points<T>>  nodes_;
            Elements                    elements_;
    };
} // namespace forward

#endif // FORWARD_DOMAIN_HPP__
