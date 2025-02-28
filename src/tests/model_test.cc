#include "../model/model.h"

#include <gtest/gtest.h>

using namespace s21;

TEST(ModelTest, GetVertices) {
  s21::Model model_;
  model_.LoadFile("tests/files/cube.obj");
  const auto& vertices = model_.GetVertices();
  std::vector<float> expected_vertices{0.5f,  0.5f, -0.5f, 0.5f,  -0.5f, -0.5f,
                                       0.5f,  0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,
                                       -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
                                       -0.5f, 0.5f, 0.5f,  -0.5f, -0.5f, 0.5f};
  EXPECT_EQ(vertices.size(), expected_vertices.size());
  for (size_t i = 0; i < expected_vertices.size(); ++i) {
    EXPECT_FLOAT_EQ(vertices[i], expected_vertices[i]);
  }
}

TEST(ModelTest, GetFaces) {
  s21::Model model_;
  model_.LoadFile("tests/files/cube.obj");
  const auto& faces = model_.GetFaces();
  std::vector<unsigned int> expected_faces{
      4, 2, 2, 0, 0, 4, 2, 7, 7, 3, 3, 2, 6, 5, 5, 7, 7, 6, 1, 7, 7, 5, 5, 1,
      0, 3, 3, 1, 1, 0, 4, 1, 1, 5, 5, 4, 4, 6, 6, 2, 2, 4, 2, 6, 6, 7, 7, 2,
      6, 4, 4, 5, 5, 6, 1, 3, 3, 7, 7, 1, 0, 2, 2, 3, 3, 0, 4, 0, 0, 1, 1, 4};
  EXPECT_EQ(faces.size(), expected_faces.size());
  for (size_t i = 0; i < expected_faces.size(); ++i) {
    EXPECT_EQ(faces[i], expected_faces[i]);
  }
}

TEST(ModelTest, CalculateBoundingBox) {
  s21::Model model_;
  model_.LoadFile("tests/files/cube_2.obj");
  float min_x, min_y, min_z, max_x, max_y, max_z;
  model_.CalculateBoundingBox(min_x, min_y, min_z, max_x, max_y, max_z);
  EXPECT_FLOAT_EQ(min_x, -0.5f);
  EXPECT_FLOAT_EQ(min_y, -0.5f);
  EXPECT_FLOAT_EQ(min_z, -0.5f);
  EXPECT_FLOAT_EQ(max_x, 0.5f);
  EXPECT_FLOAT_EQ(max_y, 0.5f);
  EXPECT_FLOAT_EQ(max_z, 0.5f);
}

TEST(ModelTest, ResetTransform) {
  s21::Model model_;
  model_.LoadFile("tests/files/cube_2.obj");
  const auto& vertices_before = model_.GetVertices();
  TransformParametrs delta = {
      {0.0f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}};
  model_.Transform(delta);
  model_.ResetTransform();
  const auto& vertices_after = model_.GetVertices();
  EXPECT_EQ(vertices_before, vertices_after)
      << "Vertices before and after reset are different!";
}

TEST(ModelTest, Transform) {
  s21::Model model_;
  model_.LoadFile("tests/files/cube_2.obj");
  TransformParametrs delta = {
      {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
  model_.Transform(delta);
  const auto& vertices_after = model_.GetVertices();
  EXPECT_NE(vertices_after[0], 1.0f);
}

TEST(ModelTest, InvalidFile) {
  s21::Model model_;
  auto result = model_.LoadFile("tests/files/invalid_file.obj");
  EXPECT_FALSE(result.first);
  EXPECT_NE(result.second, "");
}