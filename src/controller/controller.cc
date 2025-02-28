/**
 * @file controller.cc
 * @brief Реализация методов класса Controller.
 *
 * В этом файле находятся реализации методов класса Controller.
 */
#include "controller.h"

#include "../view/view.h"

namespace s21 {

s21::Controller::Controller(Model* model, View* view)
    : model_(model), view_(view) {
  connect(view_, &View::filePathSelected, this,
          [this](const QString& filePath) {
            this->LoadModel(filePath.toStdString());
          });
  connect(view, &View::moveChanged, this, &Controller::OnMoveChanged);
  connect(view, &View::rotateChanged, this, &Controller::OnRotateChanged);
  connect(view, &View::scaleChanged, this, &Controller::OnScaleChanged);
}

s21::Controller::~Controller() {}

void s21::Controller::LoadModel(const std::string& path) {
  view_->resetSliders();
  auto [success, error_message] = model_->LoadFile(path);
  if (!success) {
    view_->ShowError("Failed to load model: " + error_message);
    return;
  }
  delta_ = {};
  const auto& vertices = model_->GetVertices();
  const auto& faces = model_->GetFaces();
  view_->getModelRenderWidget()->setModelData(vertices, faces);
}

void s21::Controller::UpdateModel() {
  model_->Transform(delta_);
  delta_ = {};
  const auto& vertices = model_->GetVertices();
  const auto& faces = model_->GetFaces();
  view_->getModelRenderWidget()->setModelData(vertices, faces);
}

void s21::Controller::OnMoveChanged(float value, Axis axis) {
  switch (axis) {
    case Axis::X:
      delta_.move.x = value;
      break;
    case Axis::Y:
      delta_.move.y = value;
      break;
    case Axis::Z:
      delta_.move.z = value;
      break;
    default:
      break;
  }
  UpdateModel();
}

void s21::Controller::OnRotateChanged(float value, Axis axis) {
  // Преобразование из градусов в радианы
  float radians = value * M_PI / 180.0;
  switch (axis) {
    case Axis::X_Rotate:
      delta_.rotation.x = radians;
      break;
    case Axis::Y_Rotate:
      delta_.rotation.y = radians;
      break;
    case Axis::Z_Rotate:
      delta_.rotation.z = radians;
      break;
    default:
      break;
  }
  UpdateModel();
}

void s21::Controller::OnScaleChanged(float value) {
  delta_.scale.x = delta_.scale.y = delta_.scale.z = value;
  UpdateModel();
}

}  // namespace s21
