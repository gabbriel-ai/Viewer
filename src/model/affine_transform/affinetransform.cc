/**
 * @file affinetransform.cc
 * @brief Реализация методов класса AffineTransform
 */

#include "affinetransform.h"
using namespace s21;

AffineTransform::AffineTransform() {
  vertices_ = nullptr;
  TransformParametrs delta = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      transform_matrix_(i, j) = (*g_matrix)(i, j);
    }
  }

  delete g_matrix;
  delete creator;
  translation_ = {0, 0, 0};
}

void AffineTransform::AddVertices(std::vector<float> *vertices) {
  if (!vertices || (*vertices).empty() || (*vertices).size() % 3 != 0) {
    throw std::invalid_argument("Invalid input parameters!\n");
  }
  if (!this->GetVertices()) {
    vertices_ = vertices;
  }
}

std::vector<float> *AffineTransform::GetVertices() { return vertices_; }

/*Delta AffineTransform::GetTranslation() {
    return translation_;
  }*/

void AffineTransform::SetTranslation(Delta &delta) {
  translation_.x += delta.x;
  translation_.y += delta.y;
  translation_.z += delta.z;
}

void AffineTransform::PrivateTransformVertices(TransformParametrs &delta) {
  MatrixBuilder *creator = new GeneralMatrixBuilder();
  TransformMatrix *g_matrix = creator->FactoryMethod();
  g_matrix->SetTransformMatrix(delta);
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      transform_matrix_(i, j) = (*g_matrix)(i, j);
    }
  }

  delete g_matrix;
  delete creator;
  if (!this->transform_matrix_.IsIdentityMatrix()) {
    for (size_t i = 0; i < (*vertices_).size() || i < 1; i += 3) {
      Matrix m_vert(1, 4);

      m_vert(0, 0) = (*vertices_)[i];
      m_vert(0, 1) = (*vertices_)[i + 1];
      m_vert(0, 2) = (*vertices_)[i + 2];
      m_vert(0, 3) = 1;
      m_vert.MulMatrix(transform_matrix_);

      (*vertices_)[i] = m_vert(0, 0);
      (*vertices_)[i + 1] = m_vert(0, 1);
      (*vertices_)[i + 2] = m_vert(0, 2);
    }
  }
}

void AffineTransform::TranslateInLocal() {
  if (IsTranslation()) {
    TransformParametrs delta = {
        {0, 0, 0},
        {-translation_.x, -translation_.y, -translation_.z},
        {0, 0, 0}};
    PrivateTransformVertices(delta);
  }
}

void AffineTransform::TranslateInGlobal() {
  if (IsTranslation()) {
    TransformParametrs delta = {
        {0, 0, 0}, {translation_.x, translation_.y, translation_.z}, {0, 0, 0}};
    PrivateTransformVertices(delta);
  }
}

void AffineTransform::TransformVertices(TransformParametrs &delta) {
  if (!vertices_) {
    throw std::invalid_argument("Add vertices!\n");
  }
  TranslateInLocal();

  PrivateTransformVertices(delta);

  TranslateInGlobal();
  SetTranslation(delta.move);
}

bool AffineTransform::IsTranslation() {
  return translation_.x || translation_.y || translation_.z;
}