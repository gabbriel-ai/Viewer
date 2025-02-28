/**
 * @file openGLfuncs.cc
 * @brief Реализация методов отрисовки и инициализации OpenGL для класса
 * ModelRender.
 *
 * Этот файл содержит реализацию методов, связанных с рендерингом 3D-моделей с
 * использованием OpenGL. Включает в себя инициализацию OpenGL,
 * а также отрисовку рёбер и точек модели. Методы используют настройки, такие
 * как цвет фона, тип и толщина линий, размер и форма вершин.
 *
 * Все функции взаимодействуют с OpenGL для отображения 3D-графики в окне
 * приложения.
 */

#include "view.h"

namespace s21 {

s21::ModelRender::ModelRender(QWidget* parent) : QOpenGLWidget(parent) {
  loadSettings();
}

s21::ModelRender::~ModelRender() {
  makeCurrent();
  doneCurrent();
}

void s21::ModelRender::setModelData(const std::vector<float>& vertices,
                                    const std::vector<unsigned int>& faces) {
  vertices_ = vertices;
  faces_ = faces;
  update();
}

std::vector<float> s21::ModelRender::GetVertices() { return vertices_; }

std::vector<unsigned int> s21::ModelRender::GetFaces() { return faces_; }

void s21::ModelRender::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(settings_.bg_color.redF(), settings_.bg_color.greenF(),
               settings_.bg_color.blueF(), 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void s21::ModelRender::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void s21::ModelRender::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  int winWidth = width();
  int winHeight = height();
  if (settings_.is_parallel_projection) {
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
  } else {
    float aspect = static_cast<float>(winWidth) / winHeight;
    float fov = 60.0f * M_PI / 180.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;
    float top = near_plane * tan(fov / 2.0f);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;
    glFrustum(left, right, bottom, top, near_plane, far_plane);
    glTranslatef(0.0f, 0.0f, -2.0f);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (!vertices_.empty() && !faces_.empty()) {
    BuildLines();
    BuildPoints();
  }
}

void s21::ModelRender::BuildLines() {
  glLineWidth(settings_.edges_size);
  if (settings_.line_type == 1) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }
  glColor3f(settings_.edges_color.redF(), settings_.edges_color.greenF(),
            settings_.edges_color.blueF());
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_.data());
  glDrawElements(GL_LINES, static_cast<GLsizei>(faces_.size()), GL_UNSIGNED_INT,
                 faces_.data());
  glDisableClientState(GL_VERTEX_ARRAY);
}

void s21::ModelRender::BuildPoints() {
  glPointSize(settings_.vertex_size);
  glColor3f(settings_.vertex_color.redF(), settings_.vertex_color.greenF(),
            settings_.vertex_color.blueF());
  if (settings_.vertex_shape == 0) {
    return;
  }
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_.data());
  if (settings_.vertex_shape == 1) {
    glEnable(GL_POINT_SMOOTH);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices_.size() / 3));
    glDisable(GL_POINT_SMOOTH);
  } else if (settings_.vertex_shape == 2) {
    glDisable(GL_POINT_SMOOTH);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices_.size() / 3));
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void s21::ModelRender::setBackgroundColor(const QColor& color) {
  if (color != settings_.bg_color) {
    settings_.bg_color = color;
    makeCurrent();
    glClearColor(color.redF(), color.greenF(), color.blueF(), 1.0f);
    doneCurrent();
    saveSettings();
    update();
  }
}

void s21::ModelRender::setEdgesColor(const QColor& color) {
  if (color != settings_.edges_color) {
    settings_.edges_color = color;
    saveSettings();
    update();
  }
}

void s21::ModelRender::setLineType(int type) {
  if (type != settings_.line_type) {
    settings_.line_type = type;
    saveSettings();
    update();
  }
}

void s21::ModelRender::setLineThickness(int value) {
  if (value != settings_.edges_size) {
    settings_.edges_size = value;
    saveSettings();
    update();
  }
}

void s21::ModelRender::setVertexSize(int size) {
  if (settings_.vertex_size != size) {
    settings_.vertex_size = size;
    saveSettings();
    update();
  }
}

void s21::ModelRender::setVertexColor(const QColor& color) {
  if (settings_.vertex_color != color) {
    settings_.vertex_color = color;
    saveSettings();
    update();
  }
}

void s21::ModelRender::setVertexShape(int shape) {
  if (settings_.vertex_shape != shape) {
    settings_.vertex_shape = shape;
    saveSettings();
    update();
  }
}

void s21::ModelRender::setProjectionType(bool isParallel) {
  if (isParallel != settings_.is_parallel_projection) {
    settings_.is_parallel_projection = isParallel;
    saveSettings();
    update();
  }
}

void s21::ModelRender::defaultEdgesSettings() {
  settings_.edges_color = QColor(255, 255, 255);
  settings_.line_type = 1;
  settings_.edges_size = 1;
  saveSettings();
  update();
}

void s21::ModelRender::defaultVerticesSettings() {
  settings_.vertex_color = QColor(255, 255, 255);
  settings_.vertex_size = 5;
  settings_.vertex_shape = 1;
  saveSettings();
  update();
}

void s21::ModelRender::loadSettings() {
  QSettings settings("s21", "3DViewer_v2.0");
  settings_.bg_color =
      settings.value("bg_color", QColor(0, 0, 0)).value<QColor>();
  settings_.edges_color =
      settings.value("edges_color", QColor(255, 255, 255)).value<QColor>();
  settings_.vertex_color =
      settings.value("vertex_color", QColor(255, 255, 255)).value<QColor>();
  settings_.line_type = settings.value("line_type", 1).toInt();
  settings_.edges_size = settings.value("edges_size", 1).toInt();
  settings_.vertex_size = settings.value("vertex_size", 5).toInt();
  settings_.vertex_shape = settings.value("vertex_shape", 1).toInt();
  settings_.is_parallel_projection =
      settings.value("is_parallel_projection", true).toBool();
}

void s21::ModelRender::saveSettings() const {
  QSettings settings("s21", "3DViewer_v2.0");
  settings.setValue("bg_color", settings_.bg_color);
  settings.setValue("edges_color", settings_.edges_color);
  settings.setValue("vertex_color", settings_.vertex_color);
  settings.setValue("line_type", settings_.line_type);
  settings.setValue("edges_size", settings_.edges_size);
  settings.setValue("vertex_shape", settings_.vertex_shape);
  settings.setValue("vertex_size", settings_.vertex_size);
  settings.setValue("is_parallel_projection", settings_.is_parallel_projection);
}

const s21::Settings& ModelRender::getSettings() const { return settings_; }

}  // namespace s21