/**
 * @file axis.h
 * @brief Заголовочный файл, определяющий перечисление осей и операций для
 * трансформаций.
 *
 * Этот файл содержит перечисление `Axis`, которое используется для указания
 * различных осей (X, Y, Z), а также для обозначения операций вращения
 * (X_Rotate, Y_Rotate, Z_Rotate) и масштабирования (Scale). Перечисление
 * позволяет однозначно идентифицировать ось или операцию, с которой необходимо
 * провести трансформацию.
 */

#ifndef AXIS_H_
#define AXIS_H_

namespace s21 {

/**
 * @enum Axis
 * @brief Перечисление, представляющее оси и операции трансформаций.
 *
 * Перечисление `Axis` включает оси X, Y, Z, а также операции вращения вокруг
 * этих осей (X_Rotate, Y_Rotate, Z_Rotate) и операцию масштабирования (Scale).
 * Это перечисление используется для указания, какая ось или операция будет
 * применяться в процессе трансформаций объектов.
 */
enum class Axis { X, Y, Z, X_Rotate, Y_Rotate, Z_Rotate, Scale };
}  // namespace s21

#endif  // AXIS_H
