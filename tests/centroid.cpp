#include <forward/forward.hpp>
#include <mesh/mesh.hpp>
#include <gtest/gtest.h>

TEST(Centroid, Simple) {
    mesh::node::Map<float> nodes = {
          {1, mesh::Node<float>(0.0f, 0.0f, 0.0f)}
        , {2, mesh::Node<float>(1.0f, 0.0f, 0.0f)}
        , {3, mesh::Node<float>(1.0f, 1.0f, 0.0f)}
        , {4, mesh::Node<float>(0.0f, 1.0f, 0.0f)}
        , {5, mesh::Node<float>(0.5f, 0.5f, 0.0f)}
    };
    mesh::element::Map elements = {
          {1, {mesh::element::Type::TRIANGLE3, {1}, {1, 2, 4}}}
        , {2, {mesh::element::Type::TRIANGLE3, {1}, {2, 3, 4}}}
        , {3, {mesh::element::Type::POINT1, {2}, {5}}}
    };
    mesh::physical::Entities entities   = {
          mesh::physical::Entity{2, 1, "domain"}
        , mesh::physical::Entity{0, 2, "center"}
    };
    mesh::Mesh<float> mesh      = {{2.2, 0, 8}, entities, nodes, elements};

    forward::Domain<float> domain(mesh);
    forward::Centroids<float> expected  = {
          forward::Centroid<float>((1.0f / 3.0f), (1.0f / 3.0f), 0.0f)
        , forward::Centroid<float>((2.0f / 3.0f), (2.0f / 3.0f), 0.0f)
    };
    forward::Centroids<float> actual    = domain.centroids();

    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < elements.size(); i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

int
main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
