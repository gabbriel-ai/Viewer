/**
 * @file factory.h
 * @brief Заголовочный файл для классов и структур, связанных с реализацией
 * фабричного метода
 */
#ifndef FACTORY_H
#define FACTORY_H

#include <vector>

#include "../../libs/s21_matrix_oop.h"

/**
 * @struct Delta
 * @brief Структура для представления изменения координат
 *
 * Эта структура используется для хранения изменений в координатах X, Y и Z.
 */
struct Delta {
  float x;
  float y;
  float z;
};

/**
 * @struct TransformParametrs
 * @brief Структура для хранения параметров трансформации
 *
 * Эта структура содержит три поля типа Delta для представления масштабирования,
 * перемещения и поворота объекта.
 */
struct TransformParametrs {
  Delta scale;
  Delta move;
  Delta rotation;
};

namespace s21 {

/**
 * @class TransformMatrix
 * @brief Базовый класс для матриц трансформации
 *
 * Этот класс является базовой абстракцией для различных типов трансформаций.
 * Он наследуется другими классами для конкретных типов преобразований.
 */
class TransformMatrix : public s21::Matrix {
 private:
 public:
  /**
   * @brief Конструктор по умолчанию
   */
  TransformMatrix() : s21::Matrix(4, 4) { this->SetIdentityMatrix(); };

  /**
   * @brief Деструктор
   */
  virtual ~TransformMatrix() {};

  /**
   * @brief Абстрактный метод для расчета матрицы трансформации
   * @param delta Параметры трансформации
   */
  virtual void SetTransformMatrix(TransformParametrs delta) = 0;

  /**
   * @brief Устанавливает единичную матрицу
   */
  void SetIdentityMatrix();

  /**
   * @brief Проверяет отличны ли значения от нуля
   * @param delta Объект для проверки
   * @return true, если отличны от нуля, false в противном случае
   */
  static bool IsDelta(Delta delta);

  /**
   * @brief Проверяет, является ли текущая матрица единичной
   * @return true, если матрица единична, false в противном случае
   */
  bool IsIdentityMatrix();
};

/**
 * @class MoveTransformMatrix
 * @brief Класс для создания матрицы перемещения
 */
class MoveTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~MoveTransformMatrix() override {};

  /**
   * @brief Устанавливает матрицу перемещения на основе параметров трансформации
   * @param delta Параметры трансформации с полем move, содержащим координаты
   * перемещения
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class ScaleTransformMatrix
 * @brief Класс для создания матрицы масштабирования
 */
class ScaleTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~ScaleTransformMatrix() override {};

  /**
   * @brief Устанавливает матрицу масштабирования на основе параметров
   * трансформации
   * @param delta Параметры трансформации с полем scale, содержащим коэффициенты
   * масштабирования
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class RotationXTransformMatrix
 * @brief Класс для создания матрицы поворота вокруг оси X
 */
class RotationXTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~RotationXTransformMatrix() override {};

  /**
   * @brief Устанавливает матрицу поворота вокруг оси X на основе параметров
   * трансформации
   * @param delta Параметры трансформации с полем rotation.x, содержащим угол
   * поворота
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class RotationYTransformMatrix
 * @brief Класс для создания матрицы поворота вокруг оси Y
 */
class RotationYTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~RotationYTransformMatrix() override {};

  /**
   * @brief Устанавливает матрицу поворота вокруг оси Y на основе параметров
   * трансформации
   * @param delta Параметры трансформации с полем rotation.y, содержащим угол
   * поворота
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class RotationZTransformMatrix
 * @brief Класс для создания матрицы поворота вокруг оси Z
 */
class RotationZTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~RotationZTransformMatrix() override {};
  /**
   * @brief Устанавливает матрицу поворота вокруг оси Z на основе параметров
   * трансформации
   * @param delta Параметры трансформации с полем rotation.z, содержащим угол
   * поворота
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class RotationTransformMatrix
 * @brief Класс для создания общей матрицы поворота
 */
class RotationTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~RotationTransformMatrix() override {};
  /**
   * @brief Устанавливает общую матрицу поворота на основе параметров
   * трансформации
   * @param delta Параметры трансформации с полями rotation.x, rotation.y и
   * rotation.z, содержащими углы поворота по осям X, Y и Z соответственно
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class GeneralTransformMatrix
 * @brief Класс для создания общей матрицы трансформации
 */
class GeneralTransformMatrix : public TransformMatrix {
 public:
  using TransformMatrix::TransformMatrix;
  ~GeneralTransformMatrix() override {};
  /**
   * @brief Устанавливает общую матрицу трансформации на основе параметров
   * трансформации
   * @param delta Параметры трансформации с полями scale, move и rotation,
   *              содержащими коэффициенты масштабирования, координаты
   * перемещения и углы поворота соответственно
   */
  void SetTransformMatrix(TransformParametrs delta) override;
};

/**
 * @class MatrixBuilder
 * @brief Интерфейс для создания матриц трансформации
 *
 * Этот интерфейс определяет метод FactoryMethod(), который должен быть
 * реализован в подклассах для создания конкретных типов матриц трансформации.
 */
class MatrixBuilder {
 public:
  /**
   * @brief Абстрактный метод для создания матрицы трансформации
   * @return Указатель на объект TransformMatrix
   */
  virtual TransformMatrix *FactoryMethod() = 0;

  /**
   * @brief Деструктор
   */
  virtual ~MatrixBuilder() {}
};

/**
 * @class MoveMatrixBuilder
 * @brief Класс-строитель для создания матрицы перемещения
 *
 * Этот класс наследуется от MatrixBuilder и реализует метод FactoryMethod(),
 * который создает экземпляр конкретного класса-наследника TransformMatrix,
 * специализирующегося на матрице перемещения.
 */
class MoveMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания матрицы перемещения
   *
   * Этот метод создает и возвращает экземпляр класса MoveTransformMatrix,
   * который представляет собой матрицу перемещения.
   *
   * @return Указатель на новый экземпляр MoveTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;
  ~MoveMatrixBuilder() override {}
};

/**
 * @class ScaleMatrixBuilder
 * @brief Класс-строитель для создания матрицы масштабирования
 *
 * Реализует метод FactoryMethod() для создания экземпляра ScaleTransformMatrix.
 */
class ScaleMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания матрицы масштабирования
   *
   * Создает и возвращает экземпляр ScaleTransformMatrix.
   *
   * @return Указатель на новый экземпляр ScaleTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;

  ~ScaleMatrixBuilder() override {}
};

/**
 * @class RotationMatrixBuilder
 * @brief Класс-строитель для создания общей матрицы поворота
 *
 * Реализует метод FactoryMethod() для создания экземпляра
 * RotationTransformMatrix.
 */
class RotationMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания общей матрицы поворота
   *
   * Создает и возвращает экземпляр RotationTransformMatrix.
   *
   * @return Указатель на новый экземпляр RotationTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;

  ~RotationMatrixBuilder() override {}
};

/**
 * @class RotationXMatrixBuilder
 * @brief Класс-строитель для создания матрицы поворота вокруг оси X
 *
 * Реализует метод FactoryMethod() для создания экземпляра
 * RotationXTransformMatrix.
 */
class RotationXMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания матрицы поворота вокруг оси X
   *
   * Создает и возвращает экземпляр RotationXTransformMatrix.
   *
   * @return Указатель на новый экземпляр RotationXTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;

  ~RotationXMatrixBuilder() override {}
};

/**
 * @class RotationYMatrixBuilder
 * @brief Класс-строитель для создания матрицы поворота вокруг оси Y
 *
 * Реализует метод FactoryMethod() для создания экземпляра
 * RotationYTransformMatrix.
 */
class RotationYMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания матрицы поворота вокруг оси Y
   *
   * Создает и возвращает экземпляр RotationYTransformMatrix.
   *
   * @return Указатель на новый экземпляр RotationYTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;

  ~RotationYMatrixBuilder() override {}
};

/**
 * @class RotationZMatrixBuilder
 * @brief Класс-строитель для создания матрицы поворота вокруг оси Z
 *
 * Реализует метод FactoryMethod() для создания экземпляра
 * RotationZTransformMatrix.
 */
class RotationZMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания матрицы поворота вокруг оси Z
   *
   * Создает и возвращает экземпляр RotationZTransformMatrix.
   *
   * @return Указатель на новый экземпляр RotationZTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;

  ~RotationZMatrixBuilder() override {}
};

/**
 * @class GeneralMatrixBuilder
 * @brief Класс-строитель для создания общей матрицы трансформации
 *
 * Реализует метод FactoryMethod() для создания экземпляра
 * GeneralTransformMatrix.
 */
class GeneralMatrixBuilder : public MatrixBuilder {
 public:
  /**
   * @brief Метод-фабрика для создания общей матрицы трансформации
   *
   * Создает и возвращает экземпляр GeneralTransformMatrix.
   *
   * @return Указатель на новый экземпляр GeneralTransformMatrix
   */
  TransformMatrix *FactoryMethod() override;

  ~GeneralMatrixBuilder() override {}
};
}  // namespace s21

#endif  // FACTORY_H
