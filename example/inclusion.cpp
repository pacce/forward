#include <boost/program_options.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <filesystem>
#include <forward/forward.hpp>
#include <iostream>
#include <optional>

namespace program = boost::program_options;

const std::string DOMAIN        = "domain";
const std::string CENTER        = "center";
const std::string ELECTRODES    = "electrodes";

namespace converter {
    template <typename T>
    forward::Points<T>
    nodes(const mesh::Mesh<T>& mesh) {
        forward::Points<float> ns;

        ns.reserve(mesh.nodes.size());
        for (const auto& [_, node] : mesh.nodes) { ns.push_back(node); }

        return ns;
    }

    template <typename T>
    forward::Elements
    elements(const mesh::Mesh<T>& mesh) {
        if (not mesh.physical) { throw std::runtime_error("could not find physical entries"); }

        std::optional<mesh::physical::Tag> tag;

        mesh::physical::Entities entities = *mesh.physical;
        for (const mesh::physical::Entity& entity : entities) {
            if (entity.name == DOMAIN) { tag = entity.tag; } 
        }
        if (not tag) { throw std::runtime_error("could not find domain tag"); }

        forward::Elements es;
        for (const auto& [_, e] : mesh.elements) {
            using mesh::element::Type;
            if (e.tags.at(0) == tag) {
                forward::element::Triangle triangle;
                for (const mesh::node::Number& node: e.nodes) {
                    triangle.nodes.push_back(node - 1);
                }
                es.push_back(triangle);
            }
        }
        return es;
    }

    template <typename T>
    forward::Domain<T>
    domain(const mesh::Mesh<T>& mesh) {
        forward::Points<float> ns   = converter::nodes<float>(mesh);
        forward::Elements es        = converter::elements<float>(mesh);

        return forward::Domain<T>(ns, es);
    }
} // namespace converter

int
main(int argc, char** argv) {
    try {
        program::options_description description("options");
        description.add_options()
            ( "help,h", "shows help")
            ( "mesh", program::value<std::filesystem::path>(), "gmsh mesh")
            ;

        program::positional_options_description positional;
        positional.add("mesh", -1);

        program::command_line_parser parser{argc, argv};
        parser.options(description).positional(positional);

        program::parsed_options parsed = parser.run();

        program::variables_map vm;
        program::store(parsed, vm);
        program::notify(vm);

        if (vm.count("help")) {
            std::cout << description << std::endl;
        } else if (vm.count("mesh")) {
            std::filesystem::path path      = vm["mesh"].as<std::filesystem::path>();
            forward::Domain<float> domain   = converter::domain<float>(mesh::decode<float>(path));

            std::size_t nodes       = domain.nodes().size();
            std::size_t elements    = domain.elements().size();

            std::vector<float> zs(elements, 1.0f);
            std::map<forward::element::Pair, float> vs = domain.stiffness(zs);

            Eigen::SparseMatrix<float> ks(nodes, nodes);
            ks.reserve(vs.size());
            for (const auto& [k, v] : vs) { ks.insert(k.first, k.second) = v; }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
