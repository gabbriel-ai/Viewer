/**
 * @file matrix_oop.cc
 * @brief Реализация класса Matrix
 */
#include "s21_matrix_oop.h"

using namespace s21;

Matrix::Matrix() {
  this->rows_ = 1;
  this->cols_ = 1;
  this->matrix_ = new double *[1];
  for (int i = 0; i < 1; i++) {
    this->matrix_[i] = new double[1];
  }
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 1; j++) {
      this->matrix_[i][j] = 0;
    }
  }
}

Matrix::Matrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("ERROR");
  }
  this->rows_ = rows;
  this->cols_ = cols;
  this->matrix_ = new double *[rows];
  for (int i = 0; i < rows; i++) {
    this->matrix_[i] = new double[cols];
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      this->matrix_[i][j] = 0;
    }
  }
}

Matrix::~Matrix() {
  for (int i = 0; i < this->rows_; i++) {
    delete[] this->matrix_[i];
  }
  delete[] this->matrix_;
}

Matrix::Matrix(const Matrix &other) {
  this->rows_ = other.rows_;
  this->cols_ = other.cols_;
  this->matrix_ = new double *[other.rows_];
  for (int i = 0; i < other.rows_; i++) {
    this->matrix_[i] = new double[other.cols_];
  }
  for (int i = 0; i < other.rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      this->matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

Matrix::Matrix(Matrix &&other) {
  this->rows_ = other.rows_;
  this->cols_ = other.cols_;
  this->matrix_ = other.matrix_;
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

Matrix &Matrix::operator=(const Matrix &other) {
  // if (other.matrix_ != this->matrix_) {
  for (int i = 0; i < this->rows_; i++) {
    delete[] this->matrix_[i];
  }
  delete[] this->matrix_;
  //}
  this->rows_ = other.rows_;
  this->cols_ = other.cols_;
  this->matrix_ = new double *[other.rows_];
  for (int i = 0; i < other.rows_; i++) {
    this->matrix_[i] = new double[other.cols_];
  }
  for (int i = 0; i < other.rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      this->matrix_[i][j] = other.matrix_[i][j];
    }
  }
  return *this;
}

bool Matrix::operator==(const Matrix &other) { return EqMatrix(other); }

Matrix Matrix::operator+(const Matrix &other) {
  Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

Matrix Matrix::operator-(const Matrix &other) {
  Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

Matrix Matrix::operator*(const Matrix &other) {
  Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

Matrix Matrix::operator*(double number) const {
  Matrix result(*this);
  result.MulNumber(number);
  return result;
}

Matrix &Matrix::operator+=(const Matrix &other) {
  this->SumMatrix(other);
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
  this->SubMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
  this->MulMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const double number) {
  this->MulNumber(number);
  return *this;
}

void Matrix::SumMatrix(const Matrix &other) {
  if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
    throw std::range_error("ERROR");
  }
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      this->matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void Matrix::SubMatrix(const Matrix &other) {
  if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
    throw std::range_error("ERROR");
  }
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      this->matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void Matrix::MulNumber(const double number) {
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      this->matrix_[i][j] *= number;
    }
  }
}

void Matrix::MulMatrix(const Matrix &other) {
  if (this->cols_ != other.rows_) {
    throw std::range_error("ERROR");
  }

  Matrix result(this->rows_, other.cols_);
  for (int k = 0; k < this->cols_; k++) {
    for (int i = 0; i < this->rows_; i++) {
      for (int j = 0; j < other.cols_; j++) {
        result.matrix_[i][j] += this->matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = result;
}

bool Matrix::EqMatrix(const Matrix &other) {
  bool status = true;
  int flag_eq = 0;
  if (this->rows_ == other.rows_ && this->cols_ == other.cols_) {
    for (int i = 0; i < this->rows_ && flag_eq == 0; i++) {
      for (int j = 0; j < this->cols_ && flag_eq == 0; j++) {
        if (fabs(this->matrix_[i][j] - other.matrix_[i][j]) >= 1e-7) {
          flag_eq = 1;
        }
      }
    }
  } else
    status = false;
  if (flag_eq == 1) status = false;

  return status;
}

double &Matrix::operator()(int i, int j) const {
  if (i >= this->rows_ || j >= this->cols_ || i < 0 || j < 0) {
    throw std::out_of_range("ERROR");
  }
  return this->matrix_[i][j];
}

Matrix Matrix::Transpose() {
  Matrix result(this->cols_, this->rows_);
  for (int i = 0; i < this->cols_; i++) {
    for (int j = 0; j < this->rows_; j++) {
      result.matrix_[i][j] = this->matrix_[j][i];
    }
  }

  return result;
}

Matrix Matrix::CalcComplements() {
  if (this->cols_ != this->rows_) {
    throw std::range_error("ERROR");
  }
  Matrix M(this->rows_ - 1, this->cols_ - 1);
  Matrix result(this->rows_, this->cols_);
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      M = this->CreateMinor(i, j);

      result.matrix_[i][j] = M.Determinant() * powf(-1.0, i + j);
    }
  }
  return result;
}

double Matrix::Determinant() {
  if (this->cols_ != this->rows_) {
    throw std::range_error("ERROR");
  }
  double result = 1;
  double tmp = 0.0;
  double sign = 1.0;
  Matrix TMP(this->rows_, this->cols_);
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      TMP.matrix_[i][j] = this->matrix_[i][j];
    }
  }
  for (int k = 0; k < this->cols_ - 1; k++) {
    TMP.RawRearrange(k, &sign);
    for (int i = 0; i < this->rows_; i++) {
      tmp = TMP.matrix_[i][k] / TMP.matrix_[k][k];

      for (int j = 0; j < TMP.cols_; j++) {
        if (i > k && j >= k) {
          TMP.matrix_[i][j] = TMP.matrix_[i][j] - tmp * TMP.matrix_[k][j];
        }
      }
    }
  }

  for (int i = 0; i < TMP.cols_; i++) {
    result *= TMP.matrix_[i][i];
  }

  if (result != result)
    result = 0;
  else
    result *= sign;

  if (fabs(result) < 1e-6) result = 0.0;
  return result;
}

Matrix Matrix::CreateMinor(const int row_i, const int column_j) {
  Matrix result(this->rows_ - 1, this->cols_ - 1);

  for (int i = 0; i < this->rows_ - 1; i++) {
    for (int j = 0; j < this->cols_ - 1; j++) {
      if (i < row_i && j < column_j) {
        result.matrix_[i][j] = this->matrix_[i][j];
      } else if (i >= row_i && j < column_j) {
        result.matrix_[i][j] = this->matrix_[i + 1][j];
      } else if (i < row_i && j >= column_j) {
        result.matrix_[i][j] = this->matrix_[i][j + 1];

      } else if (i >= row_i && j >= column_j) {
        result.matrix_[i][j] = this->matrix_[i + 1][j + 1];
      }
    }
  }
  return result;
}

Matrix Matrix::InverseMatrix() {
  double determinant = 0.0;
  determinant = this->Determinant();
  if (determinant == 0.0) {
    throw std::invalid_argument("ERROR");
  }
  Matrix result(this->rows_, this->cols_);
  if (this->rows_ == this->cols_ && this->rows_ == 1) {
    if (fabs(this->matrix_[0][0]) >= 1e-7) {
      result.matrix_[0][0] = 1.0 / this->matrix_[0][0];
    }

  } else if (fabs(determinant) >= 1e-7 && this->rows_ == this->cols_) {
    Matrix TMP = this->CalcComplements();
    result = TMP.Transpose();
    result.MulNumber(1.0 / determinant);
  }

  return result;
}

void Matrix::RawRearrange(int k, double *sign) {
  int i = 0;
  double max_val = fabs(this->matrix_[k][k]);
  int raw_i = k;
  for (i = k; i < this->rows_; i++) {
    if (fabs(this->matrix_[i][k]) > fabs(max_val)) {
      max_val = fabs(this->matrix_[i][k]);
      raw_i = i;
    }
  }
  if (max_val != fabs(this->matrix_[k][k])) {
    double *ptr = this->matrix_[k];
    this->matrix_[k] = this->matrix_[raw_i];
    this->matrix_[raw_i] = ptr;
    if (*sign == 1.0)
      *sign = -1.0;
    else
      *sign = 1.0;
  }
}

void Matrix::SetRows(int rows) {
  if (rows <= 0) {
    throw std::invalid_argument("ERROR");
  }
  if (rows < this->rows_) {
    for (int i = rows; i < this->rows_; i++) {
      delete[] this->matrix_[i];
    }
    this->rows_ = rows;
  } else if (rows > this->rows_) {
    Matrix TMP(rows, this->cols_);
    for (int i = 0; i < this->rows_; i++) {
      for (int j = 0; j < this->cols_; j++) {
        TMP.matrix_[i][j] = this->matrix_[i][j];
      }
    }
    *this = TMP;
  }
}

void Matrix::SetCols(int cols) {
  if (cols <= 0) {
    throw std::invalid_argument("ERROR");
  }
  if (cols != this->cols_) {
    Matrix TMP(this->rows_, cols);
    for (int i = 0; i < this->rows_; i++) {
      for (int j = 0; j < cols; j++) {
        TMP.matrix_[i][j] = this->matrix_[i][j];
      }
    }
    *this = TMP;
  }
}

int Matrix::GetRows() const { return this->rows_; }
int Matrix::GetCols() const { return this->cols_; }
