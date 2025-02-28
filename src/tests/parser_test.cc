#include "../model/parser/parser.h"

#include <gtest/gtest.h>

TEST(ParserTest, CubeObject) {
  std::vector<unsigned int> expected_faces{
      4, 2, 2, 0, 0, 4, 2, 7, 7, 3, 3, 2, 6, 5, 5, 7, 7, 6, 1, 7, 7, 5, 5, 1,
      0, 3, 3, 1, 1, 0, 4, 1, 1, 5, 5, 4, 4, 6, 6, 2, 2, 4, 2, 6, 6, 7, 7, 2,
      6, 4, 4, 5, 5, 6, 1, 3, 3, 7, 7, 1, 0, 2, 2, 3, 3, 0, 4, 0, 0, 1, 1, 4};
  std::vector<float> expected_vertices{1,  1,  -1, 1, -1, -1, 1,  1,
                                       1,  1,  -1, 1, -1, 1,  -1, -1,
                                       -1, -1, -1, 1, 1,  -1, -1, 1};

  s21::Parser parser{};
  parser.LoadFile("tests/files/cube.obj");
  s21::ObjectData data = parser.GetData();
  EXPECT_EQ(data.faces.size(), expected_faces.size());
  EXPECT_EQ(data.vertices.size(), expected_vertices.size());
  for (size_t i = 0; i < expected_faces.size(); i++) {
    EXPECT_EQ(expected_faces[i], data.faces.at(i));
  }
  for (size_t i = 0; i < expected_vertices.size(); i++) {
    EXPECT_EQ(expected_vertices[i], data.vertices.at(i));
  }
}

TEST(ParserTest, CubeWithTextures) {
  std::vector<unsigned int> expected_faces{
      1, 2, 2, 3, 3, 1, 7, 6, 6, 5, 5, 7, 4, 5, 5, 1, 1, 4, 5, 6, 6, 2, 2, 5,
      2, 6, 6, 7, 7, 2, 0, 3, 3, 7, 7, 0, 0, 1, 1, 3, 3, 0, 4, 7, 7, 5, 5, 4,
      0, 4, 4, 1, 1, 0, 1, 5, 5, 2, 2, 1, 3, 2, 2, 7, 7, 3, 4, 0, 0, 7, 7, 4};

  size_t expected_size_vertex = 24;

  s21::Parser parser;

  parser.LoadFile("tests/files/cube_with_textures.obj");
  s21::ObjectData data = parser.GetData();
  EXPECT_EQ(data.faces.size(), expected_faces.size());

  EXPECT_EQ(data.vertices.size(), expected_size_vertex);

  for (size_t i = 0; i < expected_faces.size(); i++) {
    EXPECT_EQ(expected_faces[i], data.faces[i]);
  }
}

TEST(ParserTest, FaceCheck) {
  std::vector<unsigned int> expected_faces{0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2, 0,
                                           0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2, 0};

  s21::Parser parser;

  parser.LoadFile("tests/files/faces.obj");
  s21::ObjectData data = parser.GetData();
  EXPECT_EQ(data.faces.size(), expected_faces.size());

  for (size_t i = 0; i < expected_faces.size(); i++) {
    EXPECT_EQ(expected_faces[i], data.faces[i]);
  }
}

TEST(ParserTest, MultipleSpaces) {
  std::vector<unsigned int> expected_faces{
      0, 6, 6, 4, 4, 0, 0, 2, 2, 6, 6, 0, 0, 3, 3, 2, 2, 0, 0, 1,
      1, 3, 3, 0, 2, 7, 7, 6, 6, 2, 2, 3, 3, 7, 7, 2, 4, 6, 6, 7,
      7, 4, 4, 7, 7, 5, 5, 4, 0, 4, 4, 5, 5, 0, 0, 5, 5, 1, 1, 0};
  s21::Parser parser;

  parser.LoadFile("tests/files/cube_2.obj");
  s21::ObjectData data = parser.GetData();
  EXPECT_EQ(data.faces.size(), expected_faces.size());
  for (size_t i = 0; i < expected_faces.size(); i++) {
    EXPECT_EQ(expected_faces[i], data.faces[i]);
  }
}

TEST(ParserTest, OneFaceInLine) {
  size_t expected_size{40};
  s21::Parser parser;

  parser.LoadFile("tests/files/pyramid.obj");
  s21::ObjectData data = parser.GetData();
  EXPECT_EQ(data.faces.size(), expected_size);
}

TEST(ParserTest, NegativeFaces) {
  std::vector<unsigned int> expected_faces{3, 2, 2, 1, 1, 3};

  s21::Parser parser;

  parser.LoadFile("tests/files/negative_faces.obj");
  s21::ObjectData data = parser.GetData();

  for (size_t i = 0; i < expected_faces.size(); i++) {
    EXPECT_EQ(expected_faces[i], data.faces[i]);
  }
}

TEST(ParserTest, InvalidFile) {
  EXPECT_THROW(
      {
        try {
          s21::Parser parser;
          parser.LoadFile("tests/files/invalid_file.obj");
        } catch (std::exception const& exception) {
          throw;
        }
      },
      std::exception);
}