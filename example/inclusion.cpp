#include <boost/program_options.hpp>
#include <filesystem>
#include <forward/forward.hpp>
#include <iostream>

namespace program = boost::program_options;

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
            std::filesystem::path path = vm["mesh"].as<std::filesystem::path>();
            forward::Domain<float> domain(path);
            forward::Centroids<float> cs = domain.centroids();
            for (const forward::Centroid<float>& c : cs) {
                std::cout << c << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
