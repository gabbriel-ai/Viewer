/**
 * @file parser.cc
 * @brief Реализация класса парсера, который загружает и обрабатывает данные
 * объекта из файла.
 */

#include "parser.h"

namespace s21 {

void Parser::LoadFile(const std::string& path) {
  std::vector<unsigned int> last_faces{std::move(data_.faces)};
  std::vector<float> last_vertices{std::move(data_.vertices)};
  data_.faces.clear();
  data_.vertices.clear();
  try {
    ReadData(path);
    ValidationData();
  } catch (const std::exception& exception) {
    data_.faces = std::move(last_faces);
    data_.vertices = std::move(last_vertices);
    throw exception;
  }
}

const ObjectData& Parser::GetData() { return data_; }

void Parser::ReadData(const std::string& path) {
  std::ifstream source;
  source.open(path, std::ios::out);
  if (source.is_open()) {
    ReserveSpace(source);

    std::string line;

    while (std::getline(source >> std::ws, line)) {
      if (line.empty() || line[0] == '#') continue;
      if (line.at(0) == 'v') ParseVertex(line);
      if (line.at(0) == 'f') ParseFaces(line);
    }

  } else {
    throw std::logic_error{"Can't open file"};
  }
};

void Parser::ReserveSpace(std::ifstream& in) {
  unsigned int vertex_count = 0, face_count = 0;
  std::string line;
  while (std::getline(in >> std::ws, line)) {
    if (line.empty() || line[0] == '#') continue;
    if (line.at(0) == 'v') {
      if (line.at(1) == ' ') vertex_count++;
    } else if (line.at(0) == 'f') {
      face_count++;
    }
  }
  data_.faces.reserve(face_count * 6);
  data_.vertices.reserve(vertex_count * 3);
  in.clear();
  in.seekg(0);
}

void Parser::ParseVertex(const std::string& line) {
  std::istringstream stream{line};
  char type;
  stream >> type;
  float vertex{};
  while (stream >> vertex) {
    data_.vertices.push_back(vertex);
  }
}

void Parser::ParseFaces(const std::string& line) {
  std::istringstream stream{line};
  char type;
  stream >> type;
  int first_face{}, face{};
  stream >> first_face;
  if (first_face < 0)
    first_face = (data_.vertices.size() / 3) + (first_face) + 1;
  data_.faces.push_back(first_face - 1);
  stream.ignore(256, ' ');
  while (stream >> face) {
    if (face < 0) face = (data_.vertices.size() / 3) + face + 1;
    data_.faces.push_back(face - 1);
    data_.faces.push_back(face - 1);
    stream.ignore(256, ' ');
  }
  data_.faces.push_back(first_face - 1);
}

void Parser::ValidationData() {
  size_t size_vertex = data_.vertices.size() / 3;
  for (unsigned int i : data_.faces) {
    if (i >= size_vertex) {
      throw std::logic_error("Index more then vertices size");
    }
  }
}

}  // namespace s21