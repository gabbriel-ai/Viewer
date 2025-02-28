/**
 * @file affinetransform.h
 * @brief Заголовочный файл для класса AffineTransform
 *
 * Этот файл содержит определение класса AffineTransform, который предоставляет
 * функциональность для выполнения афинных преобразований. Класс включает методы
 * для работы с матрицами преобразований, сдвигами и применением трансформаций
 * к вершинам 3D-моделей. Он использует параметры перемещения и трансформации
 * для изменения положения, масштаба и ориентации объектов в 3D-пространстве.
 *
 */

#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

#include <vector>

#include "factory.h"

namespace s21 {

/**
 * @class AffineTransform
 * @brief Класс для выполнения афинных преобразований
 *
 * Этот класс реализует методы для трансформации вершин с использованием матрицы
 * преобразования и параметров трансформации.
 */
class AffineTransform {
 private:
  GeneralTransformMatrix transform_matrix_;  ///< Матрица преобразования
  std::vector<float> *vertices_;             ///< Указатель на вектор вершин
  Delta translation_;                        ///< Параметры перемещения

  /**
   * @brief Приватный метод для перевода в глобальную систему координат
   */
  void TranslateInGlobal();

  /**
   * @brief Приватный метод для перевода в локальную систему координат
   */
  void TranslateInLocal();

  /**
   * @brief Устанавливает параметры перемещения
   * @param delta Параметры перемещения
   */
  void SetTranslation(Delta &delta);

  /**
   * @brief Приватный метод для трансформации вершин
   * @param delta Параметры трансформации
   */
  void PrivateTransformVertices(TransformParametrs &delta);

  /**
   * @brief Проверяет была ли фигура сдвинута от начала координат
   * @return true, если фигура сдвинута от начала координат, false в противном
   * случае
   */
  bool IsTranslation();

 public:
  /**
   * @brief Конструктор по умолчанию
   */
  AffineTransform();

  /**
   * @brief Добавляет вектор вершин для трансформации
   * @param vertices Указатель на вектор вершин
   */
  void AddVertices(std::vector<float> *vertices);

  /**
   * @brief Трансформирует вектор вершин с учетом заданных параметров
   * трансформации
   * @param delta Параметры трансформации
   */
  void TransformVertices(TransformParametrs &delta);

  /**
   * @brief Получает указатель на вектор вершин
   * @return Указатель на вектор вершин
   */
  std::vector<float> *GetVertices();
};

}  // namespace s21

#endif  // AFFINETRANSFORM_H
