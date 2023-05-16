#ifndef FORWARD_DOMAIN_HPP__
#define FORWARD_DOMAIN_HPP__

#include <mesh/mesh.hpp>
#include <memory>

#include "forward-electrode.hpp"
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

                if (not mesh.physical) {
                    throw std::runtime_error("could not find physical entries");
                }

                const std::string DOMAIN        = "domain";
                const std::string CENTER        = "center";
                const std::string ELECTRODES    = "electrodes";

                std::map<std::string, mesh::physical::Tag> tags = Domain::tags(*mesh.physical);
                for (const auto& [_, e] : mesh.elements) {
                    using mesh::element::Type;

                    const mesh::element::Tag tag = e.tags.at(0);

                    if (tag == tags[DOMAIN]) {
                        element::Triangle triangle;
                        for (const mesh::node::Number& node : e.nodes) {
                            triangle.nodes.push_back(node - 1);
                        }
                        elements_.push_back(triangle);
                    } else if (tag == tags[CENTER]) {
                        center_ = nodes_->at(e.nodes[0] - 1);
                    } else if (tag == tags[ELECTRODES]) {
                        electrode::Point point(e.nodes[0] - 1);
                        electrodes_.push_back(point);
                    } else {
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

            forward::Centroid<T>
            center() const {
                return center_;
            }
        private:
            static std::map<std::string, mesh::physical::Tag>
            tags(const mesh::physical::Entities& entities) {
                std::map<std::string, mesh::physical::Tag> xs;
                for (const mesh::physical::Entity& entity : entities) {
                    xs[entity.name] = entity.tag;
                }
                return xs;
            }

            std::shared_ptr<Points<T>>  nodes_;

            Centroid<T> center_;

            Electrodes  electrodes_;
            Elements    elements_;
    };
} // namespace forward

#endif // FORWARD_DOMAIN_HPP__
