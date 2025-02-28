#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <random>

#include "../model/affine_transform/affinetransform.h"
using namespace s21;

TEST(MatrixBuilderTest, CreateIdentityMatrix) {
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();

  float expected[4][4] = {
      {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_FLOAT_EQ((*g_matrix)(i, j), expected[i][j]);
    }
  }
  ASSERT_TRUE(g_matrix->IsIdentityMatrix());
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, CreateScaleMatrix4x4) {
  TransformParametrs delta = {{2, 2, 2}, {0, 0, 0}, {0, 0, 0}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);

  float expected[4][4] = {
      {2, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 2, 0}, {0, 0, 0, 1}};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_EQ((*g_matrix)(i, j), expected[i][j]);
    }
  }
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, CreateMoveMatrix4x4) {
  TransformParametrs delta = {{0, 0, 0}, {1, 2, 3}, {0, 0, 0}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);

  float expected[4][4] = {
      {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {1, 2, 3, 1}};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_EQ((*g_matrix)(i, j), expected[i][j]);
    }
  }
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, CreateRotationMatrix_x) {
  TransformParametrs delta = {{0, 0, 0}, {0, 0, 0}, {M_PI / 4, 0, 0}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);

  float expected[4][4] = {{1, 0, 0, 0},
                          {0, 0.707107, 0.707107, 0},
                          {0, -0.707107, 0.707107, 0},
                          {0, 0, 0, 1}};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_NEAR((*g_matrix)(i, j), expected[i][j], 1e-3);
    }
  }
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, CreateRotationMatrix_y) {
  TransformParametrs delta = {{0, 0, 0}, {0, 0, 0}, {0, M_PI / 4, 0}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);

  float expected[4][4] = {{0.707107, 0, -0.707107, 0},
                          {0, 1, 0, 0},
                          {0.707107, 0, 0.707107, 0},
                          {0, 0, 0, 1}};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_NEAR((*g_matrix)(i, j), expected[i][j], 1e-3);
    }
  }
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, CreateRotationMatrix_z) {
  TransformParametrs delta = {{0, 0, 0}, {0, 0, 0}, {0, 0, M_PI / 4}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);

  float expected[4][4] = {{0.707107, 0.707107, 0, 0},
                          {-0.707107, 0.707107, 0, 0},
                          {0, 0, 1, 0},
                          {0, 0, 0, 1}};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_NEAR((*g_matrix)(i, j), expected[i][j], 1e-3);
    }
  }
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, CreateTransformMatrix4x4) {
  TransformParametrs delta = {{1, 2, 3}, {5, 6, 7}, {M_PI / 2, M_PI / 3, 0}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);

  float expected[4][4] = {
      {0.5, 0, -0.866025, 0}, {1.73205, 0, 1, 0}, {0, -3, 0, 0}, {5, 6, 7, 1}};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ASSERT_NEAR((*g_matrix)(i, j), expected[i][j], 1e-3);
    }
  }
  delete g_matrix;
  delete creator;
}

TEST(MatrixBuilderTest, IsDelta4x4) {
  Delta delta1 = {1, 2, 3};
  Delta delta2 = {0, 0, 0};

  ASSERT_TRUE(TransformMatrix::IsDelta(delta1));
  ASSERT_FALSE(TransformMatrix::IsDelta(delta2));
}

TEST(AffineTransformTest, ConstructorInvalidInput0) {
  std::vector<float> *vertices = new std::vector<float>();
  AffineTransform aff_tr;

  EXPECT_THROW(aff_tr.AddVertices(vertices), std::invalid_argument);

  delete vertices;
}

TEST(AffineTransformTest, ConstructorInvalidInput1) {
  AffineTransform aff_tr;

  EXPECT_THROW(aff_tr.AddVertices(nullptr), std::invalid_argument);
}

TEST(AffineTransformTest, ConstructorInvalidInput2) {
  std::vector<float> *vertices = new std::vector<float>();
  (*vertices).push_back(1);
  AffineTransform aff_tr;

  EXPECT_THROW(aff_tr.AddVertices(vertices), std::invalid_argument);

  delete vertices;
}

TEST(AffineTransformTest, MlnVertices) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<> dis(0.0f, 100.0f);

  std::vector<float> millionFloats(1000011);

  auto start = std::chrono::high_resolution_clock::now();

  for (float &val : millionFloats) {
    val = dis(gen);
  }
  TransformParametrs delta = {{1, 2, 3}, {5, 6, 7}, {M_PI / 2, M_PI / 3, 0}};

  AffineTransform aff_tr;
  aff_tr.AddVertices(&millionFloats);
  aff_tr.TransformVertices(delta);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  ASSERT_LT(duration.count(), 500);
}

TEST(AffineTransformTest, ValuesOfVertices) {
  std::vector<float> vertices(3);

  vertices[0] = 1;
  vertices[1] = 1;
  vertices[2] = 1;
  TransformParametrs delta = {{1, 2, 3}, {5, 6, 7}, {M_PI / 2, M_PI / 3, 0}};

  AffineTransform aff_tr;
  aff_tr.AddVertices(&vertices);
  aff_tr.TransformVertices(delta);

  ASSERT_NEAR(vertices[0], 7.23205, 1e-3);

  ASSERT_NEAR(vertices[1], 3, 1e-3);
  ASSERT_NEAR(vertices[2], 7.13399, 1e-3);
}

TEST(AffineTransformTest, ValuesOfVerticesWithoutTransform) {
  std::vector<float> vertices(3);

  vertices[0] = 1;
  vertices[1] = 1;
  vertices[2] = 1;
  TransformParametrs delta = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

  AffineTransform aff_tr;
  aff_tr.AddVertices(&vertices);
  aff_tr.TransformVertices(delta);

  ASSERT_NEAR(vertices[0], 1, 1e-3);

  ASSERT_NEAR(vertices[1], 1, 1e-3);
  ASSERT_NEAR(vertices[2], 1, 1e-3);
}

TEST(AffineTransformTest, ValuesOfVertices_Scale) {
  std::vector<float> vertices(3);

  vertices[0] = 1;
  vertices[1] = 1;
  vertices[2] = 1;
  TransformParametrs delta = {{2, 2, 2}, {0, 0, 0}, {0, 0, 0}};

  AffineTransform aff_tr;
  aff_tr.AddVertices(&vertices);
  aff_tr.TransformVertices(delta);

  ASSERT_NEAR(vertices[0], 2, 1e-3);

  ASSERT_NEAR(vertices[1], 2, 1e-3);
  ASSERT_NEAR(vertices[2], 2, 1e-3);
}

TEST(AffineTransformTest, ValuesOfVertices_Move) {
  std::vector<float> vertices(3);

  vertices[0] = 1;
  vertices[1] = 1;
  vertices[2] = 1;
  TransformParametrs delta = {{0, 0, 0}, {100, 100, 0}, {0, 0, 0}};

  AffineTransform aff_tr;
  aff_tr.AddVertices(&vertices);
  aff_tr.TransformVertices(delta);

  ASSERT_NEAR(vertices[0], 101, 1e-3);

  ASSERT_NEAR(vertices[1], 101, 1e-3);
  ASSERT_NEAR(vertices[2], 1, 1e-3);
}

TEST(AffineTransformTest, ValuesOfVertices_Rotation) {
  std::vector<float> vertices(3);

  vertices[0] = 1;
  vertices[1] = 1;
  vertices[2] = 1;
  TransformParametrs delta = {
      {0, 0, 0}, {0, 0, 0}, {M_PI / 2, M_PI / 3, M_PI / 4}};

  AffineTransform aff_tr;
  aff_tr.AddVertices(&vertices);
  aff_tr.TransformVertices(delta);

  ASSERT_NEAR(vertices[0], 1.673032, 1e-3);

  ASSERT_NEAR(vertices[1], 0.258825, 1e-3);
  ASSERT_NEAR(vertices[2], -0.36602, 1e-3);
}

TEST(AffineTransformTest, LocalMove) {
  std::vector<float> vertices = {-1, 1, 0, 1, 1, 0, 1, -1, 0, -1, -1, 0};

  TransformParametrs delta1 = {{2, 2, 2}, {10, 0, 0}, {0, M_PI, 0}};
  TransformParametrs delta2 = {{2, 2, 2}, {0, 0, 0}, {0, M_PI, 0}};
  AffineTransform aff_tr;
  aff_tr.AddVertices(&vertices);
  aff_tr.TransformVertices(delta1);
  std::vector<float> expected = {6, 4, 0, 14, 4, 0, 14, -4, 0, 6, -4, 0};
  aff_tr.TransformVertices(delta2);

  for (size_t i = 0; i < vertices.size(); i++) {
    ASSERT_NEAR(vertices[i], expected[i], 1e-3);
  }
}