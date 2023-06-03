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
                nodes_->resize(nodes.size());
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

            std::map<forward::element::Pair, T>
            stiffness(const std::vector<T>& resistivities) const {
                forward::element::Stiffness<T> visitor{nodes_, 1.0, 1.0};

                std::map<forward::element::Pair, T> vs;
                for (std::size_t i = 0; i < resistivities.size(); i++) {
                    visitor.resistivity = resistivities[i];
                    visitor.thickness   = 1.0;

                    std::map<forward::element::Pair, T> xs = std::visit(visitor, elements_[i]);
                    for (const auto& [k, x] : xs) { vs[k] += x; }
                }
                return vs;
            }

            const Elements&
            elements() const { return elements_; }

            const Points<T>&
            nodes() const { return *nodes_; }
        private:
            std::shared_ptr<Points<T>>  nodes_;
            Elements                    elements_;
    };
} // namespace forward

#endif // FORWARD_DOMAIN_HPP__
