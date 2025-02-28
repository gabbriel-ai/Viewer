/**
 * @file factory.cc
 * @brief Реализация фабричных методов для создания и трансформации матриц
 *
 * Этот файл содержит реализацию методов для классов, связанных с афинными
 * преобразованиями, таких как перемещение, масштабирование и вращение в 3D
 * пространстве. Каждый класс матрицы преобразования использует паттерн
 * "Фабричный метод" для создания соответствующего объекта трансформации. Эти
 * объекты затем используются для задания и применения матриц преобразований к
 * вершинам 3D-объектов.
 */

#include "factory.h"

#include <iostream>

using namespace s21;

void TransformMatrix::SetIdentityMatrix() {
  matrix_[0][0] = 1;
  matrix_[1][1] = 1;
  matrix_[2][2] = 1;
  matrix_[3][3] = 1;
}
bool TransformMatrix::IsDelta(Delta delta) {
  return delta.x || delta.y || delta.z;
}

void MoveTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  matrix_[0][0] = 1;
  matrix_[1][1] = 1;
  matrix_[2][2] = 1;
  matrix_[3][3] = 1;
  matrix_[3][0] = delta.move.x;
  matrix_[3][1] = delta.move.y;
  matrix_[3][2] = delta.move.z;
}

void ScaleTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  matrix_[0][0] = delta.scale.x;
  matrix_[1][1] = delta.scale.y;
  matrix_[2][2] = delta.scale.z;
  matrix_[3][3] = 1;
}

void RotationXTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  matrix_[1][1] = cos(delta.rotation.x);
  matrix_[2][2] = cos(delta.rotation.x);
  matrix_[2][1] = -sin(delta.rotation.x);
  matrix_[1][2] = sin(delta.rotation.x);
}

void RotationYTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  matrix_[0][0] = cos(delta.rotation.y);
  matrix_[2][2] = cos(delta.rotation.y);
  matrix_[0][2] = -sin(delta.rotation.y);
  matrix_[2][0] = sin(delta.rotation.y);
}

void RotationZTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  matrix_[0][0] = cos(delta.rotation.z);
  matrix_[1][1] = cos(delta.rotation.z);
  matrix_[0][1] = sin(delta.rotation.z);
  matrix_[1][0] = -sin(delta.rotation.z);
}

void RotationTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  MatrixBuilder *creator1 = new RotationXMatrixBuilder();
  TransformMatrix *x_matrix = creator1->FactoryMethod();
  x_matrix->SetTransformMatrix(delta);
  this->MulMatrix(*x_matrix);
  delete x_matrix;
  delete creator1;

  MatrixBuilder *creator2 = new RotationYMatrixBuilder();
  TransformMatrix *y_matrix = creator2->FactoryMethod();
  y_matrix->SetTransformMatrix(delta);
  this->MulMatrix(*y_matrix);
  delete y_matrix;
  delete creator2;

  MatrixBuilder *creator3 = new RotationZMatrixBuilder();
  TransformMatrix *z_matrix = creator3->FactoryMethod();
  z_matrix->SetTransformMatrix(delta);
  this->MulMatrix(*z_matrix);
  delete z_matrix;
  delete creator3;
}

void GeneralTransformMatrix::SetTransformMatrix(TransformParametrs delta) {
  if (IsDelta(delta.scale)) {
    MatrixBuilder *creator = new ScaleMatrixBuilder();
    TransformMatrix *s_matrix = creator->FactoryMethod();
    s_matrix->SetTransformMatrix(delta);
    this->MulMatrix(*s_matrix);
    delete s_matrix;
    delete creator;
  }

  if (IsDelta(delta.rotation)) {
    MatrixBuilder *creator = new RotationMatrixBuilder();
    TransformMatrix *r_matrix = creator->FactoryMethod();
    r_matrix->SetTransformMatrix(delta);
    this->MulMatrix(*r_matrix);
    delete r_matrix;
    delete creator;
  }

  if (IsDelta(delta.move)) {
    MatrixBuilder *creator = new MoveMatrixBuilder();
    TransformMatrix *m_matrix = creator->FactoryMethod();
    m_matrix->SetTransformMatrix(delta);
    this->MulMatrix(*m_matrix);
    delete m_matrix;
    delete creator;
  }
}

TransformMatrix *MoveMatrixBuilder::FactoryMethod() {
  return new MoveTransformMatrix();
}

TransformMatrix *ScaleMatrixBuilder::FactoryMethod() {
  return new ScaleTransformMatrix();
}

TransformMatrix *RotationMatrixBuilder::FactoryMethod() {
  return new RotationTransformMatrix();
}

TransformMatrix *RotationXMatrixBuilder::FactoryMethod() {
  return new RotationXTransformMatrix();
}
TransformMatrix *RotationYMatrixBuilder::FactoryMethod() {
  return new RotationYTransformMatrix();
}
TransformMatrix *RotationZMatrixBuilder::FactoryMethod() {
  return new RotationZTransformMatrix();
}

TransformMatrix *GeneralMatrixBuilder::FactoryMethod() {
  return new GeneralTransformMatrix();
}

bool TransformMatrix::IsIdentityMatrix() {
  bool result = true;
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      if ((i == j && matrix_[i][j] == 1) || (i != j && matrix_[i][j] == 0))
        continue;
      else {
        result = false;
        return result;
      }
    }
  }
  return result;
}