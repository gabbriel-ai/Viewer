/**
 * @file model.cc
 * @brief Реализация методов класса Model.
 */
#include "model.h"

#include <algorithm>

using namespace s21;

s21::Model::Model() : parser_(), affine_transform_() {}

s21::Model::~Model() {}

std::pair<bool, std::string> s21::Model::LoadFile(const std::string &path) {
  try {
    parser_.LoadFile(path);
    object_data_ = parser_.GetData();
    affine_transform_.AddVertices(&object_data_.vertices);
    ResetTransform();
    return {true, ""};
  } catch (const std::exception &e) {
    std::cerr << "Error while loading file: " << e.what() << std::endl;
    return {false, e.what()};
  }
}

const std::vector<float> &s21::Model::GetVertices() const {
  return object_data_.vertices;
}

const std::vector<unsigned int> &s21::Model::GetFaces() const {
  return object_data_.faces;
}

void s21::Model::Transform(TransformParametrs &delta) {
  affine_transform_.TransformVertices(delta);
}

void s21::Model::CalculateBoundingBox(float &min_x, float &min_y, float &min_z,
                                      float &max_x, float &max_y,
                                      float &max_z) {
  if (object_data_.vertices.empty()) {
    throw std::invalid_argument("Vertices array is empty!");
  }
  min_x = min_y = min_z = std::numeric_limits<float>::max();
  max_x = max_y = max_z = std::numeric_limits<float>::lowest();

  for (size_t i = 0; i < object_data_.vertices.size(); i += 3) {
    float x = object_data_.vertices[i];
    float y = object_data_.vertices[i + 1];
    float z = object_data_.vertices[i + 2];
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    min_z = std::min(min_z, z);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
    max_z = std::max(max_z, z);
  }
}

void s21::Model::ResetTransform() {
  if (object_data_.vertices.empty()) {
    throw std::invalid_argument("Vertices array is empty!");
  }
  float min_x, min_y, min_z, max_x, max_y, max_z;
  CalculateBoundingBox(min_x, min_y, min_z, max_x, max_y, max_z);
  float center_x = min_x + (max_x - min_x) / 2.0f;
  float center_y = min_y + (max_y - min_y) / 2.0f;
  float center_z = min_z + (max_z - min_z) / 2.0f;

  float size_x = max_x - min_x;
  float size_y = max_y - min_y;
  float size_z = max_z - min_z;
  float max_size = std::max({size_x, size_y, size_z});
  if (max_size == 0) max_size = 1.0f;

  for (size_t i = 0; i < object_data_.vertices.size(); i += 3) {
    object_data_.vertices[i] -= center_x;
    object_data_.vertices[i + 1] -= center_y;
    object_data_.vertices[i + 2] -= center_z;
    object_data_.vertices[i] /= max_size;
    object_data_.vertices[i + 1] /= max_size;
    object_data_.vertices[i + 2] /= max_size;
  }
  current_state_ = {{1, 1, 1}, {0, 0, 0}, {0, 0, 0}};
}
