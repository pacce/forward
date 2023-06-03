#include <forward/forward.hpp>
#include <mesh/mesh.hpp>
#include <gtest/gtest.h>

class Mesh : public ::testing::Test {
    protected:
        void SetUp() override {
            nodes_ = std::make_shared<forward::Points<float>>();
            nodes_->emplace_back(0.0f, 0.0f, 0.0f);
            nodes_->emplace_back(1.0f, 0.0f, 0.0f);
            nodes_->emplace_back(1.0f, 1.0f, 0.0f);
            nodes_->emplace_back(0.0f, 1.0f, 0.0f);

            elements_.push_back(forward::element::Triangle{{0, 1, 2}});
            elements_.push_back(forward::element::Triangle{{2, 3, 0}});
        }

        std::shared_ptr<forward::Points<float>> nodes_;
        forward::Elements                       elements_;
};

TEST_F(Mesh, Centroid) {
    forward::Centroids<float> expected  = {
          forward::Centroid<float>(0.666666f, 0.333333f, 0.0f)
        , forward::Centroid<float>(0.333333f, 0.666666f, 0.0f)
    };

    forward::Centroids<float> actual;
    actual.reserve(elements_.size());

    forward::element::Centroid<float> visitor{nodes_};
    for (const forward::Element& element : elements_) {
        actual.push_back(std::visit(visitor, element));
    }

    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(Mesh, Dimension) {
    std::vector<float> expected = { 0.5f, 0.5f };

    std::vector<float> actual;
    actual.reserve(elements_.size());

    forward::element::Dimension<float> visitor{nodes_};
    for (const forward::Element& element : elements_) {
        actual.push_back(std::visit(visitor, element));
    }

    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST_F(Mesh, Normal) {
    forward::Normals<float> expected  = {
          forward::Normal<float>(0.0f, 0.0f, 1.0f)
        , forward::Normal<float>(0.0f, 0.0f, 1.0f)
    };

    forward::Normals<float> actual;
    actual.reserve(elements_.size());

    forward::element::Normal<float> visitor{nodes_};
    for (const forward::Element& element : elements_) {
        actual.push_back(std::visit(visitor, element));
    }

    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST(Stiffness, Triangles) {
    std::shared_ptr<forward::Points<float>> nodes;
    forward::Elements                       elements;

    nodes = std::make_shared<forward::Points<float>>();

    nodes->emplace_back(-9.55104481e-01, 8.01838857e-04, 0.00000000e+00);
    nodes->emplace_back(-9.98841717e-01, 4.81167200e-02, 0.00000000e+00);
    nodes->emplace_back(-1.00000000e+00, 0.00000000e+00, 0.00000000e+00);
    elements.push_back(forward::element::Triangle{{0, 1, 2}});

    std::map<forward::element::Pair, float> expected = {
          {{0, 0},  0.53641557}
        , {{0, 1}, -0.02097539}
        , {{0, 2}, -0.51544031}
        , {{1, 0}, -0.02097539}
        , {{1, 1},  0.46687676}
        , {{1, 2}, -0.44590151}
        , {{2, 0}, -0.51544031}
        , {{2, 1}, -0.44590151}
        , {{2, 2},  0.96134182}
    };

    forward::element::Stiffness<float> visitor{nodes, 1.0f, 1.0f};
    for (const forward::Element& element : elements) {
        std::map<forward::element::Pair, float> actual = std::visit(visitor, element);
        for (const auto& [key, value] : expected) {
            ASSERT_TRUE(actual.contains(key));
            EXPECT_FLOAT_EQ(value, actual[key]);
        }
    }
}

int
main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
