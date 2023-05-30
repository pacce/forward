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
            nodes_->emplace_back(0.5f, 0.5f, 0.0f);

            elements_.push_back(forward::element::Triangle{{0, 1, 3}});
            elements_.push_back(forward::element::Triangle{{1, 2, 3}});
        }

        std::shared_ptr<forward::Points<float>> nodes_;
        forward::Elements                       elements_;
};

TEST_F(Mesh, Centroid) {
    forward::Centroids<float> expected  = {
          forward::Centroid<float>(0.333333f, 0.333333f, 0.0f)
        , forward::Centroid<float>(0.666666f, 0.666666f, 0.0f)
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

int
main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
