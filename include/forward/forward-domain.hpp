#ifndef FORWARD_DOMAIN_HPP__
#define FORWARD_DOMAIN_HPP__

#include <mesh/mesh.hpp>
#include <memory>

#include "forward-element.hpp"
#include "forward-point.hpp"

namespace forward {
    template <typename T>
    class Domain {
        public:
            Domain(const std::filesystem::path& path) : Domain(mesh::decode<T>(path)) {}
            ~Domain() {}

            Domain(const mesh::Mesh<T>& mesh) {
                nodes_ = std::make_shared<Points<T>>();
                nodes_->reserve(mesh.nodes.size());
                for (const auto& [_, node] : mesh.nodes) { nodes_->push_back(node); }

                for (const auto& [_, e] : mesh.elements) {
                    using mesh::element::Type;

                    if (e.type == Type::TRIANGLE3) {
                        element::Triangle triangle;
                        for (const mesh::node::Number& node : e.nodes) {
                            triangle.nodes.push_back(node - 1);
                        }
                        elements_.push_back(triangle);
                    }
                }
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
