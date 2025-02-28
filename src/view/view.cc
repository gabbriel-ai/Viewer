/**
 * @file view.cc
 * @brief Реализация методов для управления интерфейсом модели в классе View.
 *
 * Этот файл содержит реализацию методов для отображения и обработки различных
 * элементов пользовательского интерфейса в классе `View`. Методы обеспечивают
 * функциональность для работы с файлами моделей, отображение информации о
 * модели, а также для управления трансформациями, такими как движение, вращение
 * и масштабирование.
 *
 * Примечания:
 * - Для отображения и обработки графики используется объект `ModelRender`,
 * предоставляющий доступ к текущей модели.
 * - Сигналы и слоты используются для обработки событий, таких как выбор файла,
 * изменение параметров модели и т. д.
 */

#include "view.h"

#include "../libs/gif.h"
#include "ui_view.h"

namespace s21 {

s21::View::View(QWidget* parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  setWindowTitle("3DViewer v2.0");
  setFixedSize(1280, 720);
  SetupMenuBar();
  RenderControlPanels();
  LoadModel();
  syncInterfaceWithSettings();
}

s21::View::~View() {
  delete modelViewWidget;
  delete ui;
}

void s21::View::LoadModel() {
  modelViewWidget = new s21::ModelRender(this);
  modelViewWidget->setMinimumSize(900, 680);
  modelViewWidget->setGeometry(5, 20, 900, 680);
  modelViewWidget->show();
  infoLabel_ = new QLabel(this);
  infoLabel_->setGeometry(5, 695, 900, 30);
  infoLabel_->show();
}

void s21::View::RenderControlPanels() {
  QVBoxLayout* controlPanelLayout = new QVBoxLayout();
  controlPanelLayout->addWidget(MovePanel());
  controlPanelLayout->addWidget(ScalePanel());
  controlPanelLayout->addWidget(RotatePanel());
  controlPanelLayout->addWidget(BackPanel());
  controlPanelLayout->addWidget(ProjectionPanel());
  controlPanelLayout->addWidget(VerticesPanel());
  controlPanelLayout->addWidget(EdgesPanel());
  QWidget* controlPanelWidget = new QWidget(this);
  controlPanelWidget->setLayout(controlPanelLayout);
  controlPanelWidget->setGeometry(910, 20, 350, 680);
  controlPanelWidget->show();
}

void s21::View::SetupMenuBar() {
  QMenuBar* menuBar = new QMenuBar(this);
  QMenu* fileMenu = new QMenu("File", menuBar);
  QAction* openAction = new QAction("Open", fileMenu);
  connect(openAction, &QAction::triggered, this, &View::OnOpenFile);
  QAction* imageAction = new QAction("Save as Image", fileMenu);
  connect(imageAction, &QAction::triggered, this, &View::OnSaveImage);
  QAction* gifAction = new QAction("Save as GIF", fileMenu);
  connect(gifAction, &QAction::triggered, this, &View::OnSaveGIF);
  QAction* exitAction = new QAction("Exit", fileMenu);
  connect(exitAction, &QAction::triggered, this, &QWidget::close);

  fileMenu->addAction(openAction);
  fileMenu->addAction(imageAction);
  fileMenu->addAction(gifAction);
  fileMenu->addAction(exitAction);
  menuBar->addMenu(fileMenu);
  setMenuBar(menuBar);
}

// Вспомогательное

s21::ModelRender* s21::View::getModelRenderWidget() { return modelViewWidget; }

QSlider* s21::View::SliderDesign() {
  QSlider* slider = new QSlider(Qt::Horizontal);
  slider->setStyleSheet(
      "QSlider::handle:horizontal {"
      "    background: #808080;"  // Цвет
      "    width: 7px;"
      "    height: 10px;"
      "    border-radius: 3px;"         // Прямые углы
      "    margin: -2px 0px -2px 0px;"  // Отступы от краев слайдера
      "}"
      "QSlider::groove:horizontal {"
      "    background: #000000;"  // Цвет дорожки
      "    height: 8px;"          // Высота дорожки
      "    border-radius: 4px;"   // Округлые углы
      "}");
  return slider;
}

QWidget* s21::View::createSliderWithInput(const QString& label, int min,
                                          int max, int defaultValue) {
  QWidget* widget = new QWidget(this);
  QHBoxLayout* layout = new QHBoxLayout(widget);
  QSlider* slider = SliderDesign();
  slider->setRange(min, max);
  slider->setValue(defaultValue);
  QLineEdit* input = new QLineEdit();
  input->setText(QString::number(defaultValue));

  input->setFixedWidth(25);
  QFont font = input->font();
  font.setPointSize(10);
  input->setFont(font);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  QLabel* labelWidget = new QLabel(label);
  layout->addWidget(labelWidget);
  layout->addSpacing(10);
  layout->addWidget(slider);
  layout->addSpacing(10);
  layout->addWidget(input);
  QObject::connect(slider, &QSlider::valueChanged, this, [input](int value) {
    input->setText(QString::number(value));
  });
  QObject::connect(input, &QLineEdit::textChanged, this,
                   [slider](const QString& text) {
                     bool ok;
                     int value = text.toInt(&ok);
                     if (ok) {
                       slider->setValue(value);
                     }
                   });
  widget->setProperty("slider",
                      QVariant::fromValue(static_cast<void*>(slider)));
  widget->setProperty("defaultValue", defaultValue);
  return widget;
}

QGroupBox* s21::View::MovePanel() {
  QGroupBox* moveBox = new QGroupBox("MOVE");
  QVBoxLayout* moveLayout = new QVBoxLayout();
  moveLayout->setContentsMargins(10, 3, 10, 3);
  moveLayout->setSpacing(0);

  auto xWidget = createSliderWithInput("X", -100, 100, 0);
  auto yWidget = createSliderWithInput("Y", -100, 100, 0);
  auto zWidget = createSliderWithInput("Z", -100, 100, 0);

  moveLayout->addWidget(xWidget);
  moveLayout->addWidget(yWidget);
  moveLayout->addWidget(zWidget);

  connectSliderToSignal(xWidget, Axis::X);
  connectSliderToSignal(yWidget, Axis::Y);
  connectSliderToSignal(zWidget, Axis::Z);

  moveBox->setLayout(moveLayout);
  moveBox->setFixedSize(325, 110);
  return moveBox;
}

QGroupBox* s21::View::ScalePanel() {
  QGroupBox* scaleBox = new QGroupBox("SCALE");
  QVBoxLayout* scaleLayout = new QVBoxLayout();
  scaleLayout->setContentsMargins(10, 3, 10, 3);
  scaleLayout->setSpacing(0);
  auto scaleWidget = createSliderWithInput("+", 1, 20, 1);
  scaleLayout->addWidget(scaleWidget);
  connectSliderToSignal(scaleWidget, Axis::Scale);
  scaleBox->setLayout(scaleLayout);
  scaleBox->setFixedSize(325, 55);
  return scaleBox;
}

QGroupBox* s21::View::RotatePanel() {
  QGroupBox* rotateBox = new QGroupBox("ROTATE");
  QVBoxLayout* rotateLayout = new QVBoxLayout();
  rotateLayout->setContentsMargins(10, 3, 10, 3);
  rotateLayout->setSpacing(0);

  auto xWidget = createSliderWithInput("X", 0, 360, 180);
  auto yWidget = createSliderWithInput("Y", 0, 360, 180);
  auto zWidget = createSliderWithInput("Z", 0, 360, 180);

  rotateLayout->addWidget(xWidget);
  rotateLayout->addWidget(yWidget);
  rotateLayout->addWidget(zWidget);

  connectSliderToSignal(xWidget, Axis::X_Rotate);
  connectSliderToSignal(yWidget, Axis::Y_Rotate);
  connectSliderToSignal(zWidget, Axis::Z_Rotate);

  rotateBox->setLayout(rotateLayout);
  rotateBox->setFixedSize(325, 110);
  return rotateBox;
}

const QMap<QString, QColor> View::colorMap = {{"White", QColor(255, 255, 255)},
                                              {"Red", QColor(205, 92, 92)},
                                              {"Green", QColor(152, 251, 152)},
                                              {"Blue", QColor(70, 130, 180)},
                                              {"Black", QColor(0, 0, 0)}};

const QMap<QString, int> View::lineTypeMap = {{"Solid", 1}, {"Dashed", 2}};

const QMap<QString, int> View::vertexShapeMap = {
    {"None", 0}, {"Circle", 1}, {"Square", 2}};

const QMap<QString, bool> View::projectionTypeMap = {{"Parallel", true},
                                                     {"Central", false}};

QVBoxLayout* s21::View::createEdgesRightLayout() {
  QVBoxLayout* rightLayout = new QVBoxLayout();
  rightLayout->setContentsMargins(3, 3, 3, 3);
  rightLayout->setSpacing(0);

  QComboBox* lineTypeBox = new QComboBox();
  lineTypeBox->addItems({"Solid", "Dashed"});
  lineTypeBox->setObjectName("LineTypeBox");
  connect(lineTypeBox, &QComboBox::currentTextChanged, this,
          &View::OnLineTypeChanged);
  rightLayout->addWidget(lineTypeBox);

  QPushButton* defaultButton = new QPushButton("DEFAULT");
  connect(defaultButton, &QPushButton::clicked, this,
          &View::resetEdgesSettings);
  rightLayout->addWidget(defaultButton);
  return rightLayout;
}

QVBoxLayout* s21::View::createEdgesLeftLayout() {
  QVBoxLayout* leftLayout = new QVBoxLayout();
  leftLayout->setContentsMargins(3, 3, 3, 3);
  leftLayout->setSpacing(0);
  QWidget* thicknessWidget = createSliderWithInput("Thickness", 1, 10, 1);
  thicknessWidget->setProperty("persistent", true);
  QSlider* slider =
      static_cast<QSlider*>(thicknessWidget->property("slider").value<void*>());
  slider->setObjectName("ThicknessSlider");
  connect(slider, &QSlider::valueChanged, this, &View::OnLineThickness);
  leftLayout->addWidget(thicknessWidget);
  QComboBox* ColorBox = new QComboBox();
  ColorBox->setObjectName("EdgesColorBox");
  ColorBox->addItems({"White", "Red", "Green", "Blue", "Black"});
  connect(ColorBox, &QComboBox::currentTextChanged, this, &View::OnEdgesColor);
  leftLayout->addWidget(ColorBox);
  return leftLayout;
}

QGroupBox* s21::View::EdgesPanel() {
  QGroupBox* edgesBox = new QGroupBox("EDGES");
  QHBoxLayout* edgesLayout = new QHBoxLayout();
  edgesLayout->setContentsMargins(10, 3, 10, 3);
  edgesLayout->setSpacing(0);

  edgesLayout->addLayout(createEdgesLeftLayout());
  edgesLayout->addLayout(createEdgesRightLayout());

  edgesBox->setLayout(edgesLayout);
  edgesBox->setFixedSize(325, 100);
  return edgesBox;
}

void s21::View::resetEdgesSettings() {
  QSlider* thicknessSlider = findChild<QSlider*>("ThicknessSlider");
  if (thicknessSlider) thicknessSlider->setValue(1);
  QComboBox* colorBox = findChild<QComboBox*>("EdgesColorBox");
  if (colorBox) colorBox->setCurrentText("White");
  QComboBox* lineTypeBox = findChild<QComboBox*>("LineTypeBox");
  if (lineTypeBox) lineTypeBox->setCurrentText("Solid");
  if (modelViewWidget) {
    modelViewWidget->defaultEdgesSettings();
  }
}

void s21::View::OnEdgesColor(const QString& color) {
  if (modelViewWidget && colorMap.contains(color)) {
    modelViewWidget->setEdgesColor(colorMap[color]);
    QComboBox* edgesColorBox = findChild<QComboBox*>("EdgesColorBox");
    if (edgesColorBox) {
      edgesColorBox->setCurrentText(color);
    }
  }
}

void s21::View::OnLineTypeChanged(const QString& type) {
  if (modelViewWidget && lineTypeMap.contains(type)) {
    int lineType = lineTypeMap.value(type);
    modelViewWidget->setLineType(lineType);
  }
}

void s21::View::OnLineThickness(int value) {
  if (modelViewWidget) {
    modelViewWidget->setLineThickness(value);
    QSlider* thicknessSlider = findChild<QSlider*>("ThicknessSlider");
    if (thicknessSlider) {
      thicknessSlider->setValue(value);
    }
  }
}

QVBoxLayout* s21::View::createVertexLeftLayout() {
  QVBoxLayout* leftLayout = new QVBoxLayout();
  leftLayout->setContentsMargins(3, 3, 3, 3);
  leftLayout->setSpacing(0);

  QWidget* sizeWidget = createSliderWithInput("Size", 1, 10, 5);
  sizeWidget->setProperty("persistent", true);
  QSlider* sizeSlider =
      static_cast<QSlider*>(sizeWidget->property("slider").value<void*>());
  sizeSlider->setObjectName("VertexSizeSlider");
  connect(sizeSlider, &QSlider::valueChanged, this, &View::OnVertexSizeChanged);
  leftLayout->addWidget(sizeWidget);

  QComboBox* vertexColorBox = new QComboBox();
  vertexColorBox->setObjectName("VertexColorBox");
  vertexColorBox->addItems(colorMap.keys());
  connect(vertexColorBox, &QComboBox::currentTextChanged, this,
          &View::OnVertexColorChanged);
  leftLayout->addWidget(vertexColorBox);
  return leftLayout;
}

QVBoxLayout* s21::View::createVertexRightLayout() {
  QVBoxLayout* rightLayout = new QVBoxLayout();
  rightLayout->setContentsMargins(3, 3, 3, 3);
  rightLayout->setSpacing(0);

  QComboBox* vertexShapeBox = new QComboBox();
  vertexShapeBox->setObjectName("VertexShapeBox");
  vertexShapeBox->addItems(vertexShapeMap.keys());
  connect(vertexShapeBox, &QComboBox::currentTextChanged, this,
          &View::OnVertexShapeChanged);
  rightLayout->addWidget(vertexShapeBox);

  QPushButton* defaultButton = new QPushButton("DEFAULT");
  connect(defaultButton, &QPushButton::clicked, this,
          &View::resetVerticesSettings);
  rightLayout->addWidget(defaultButton);

  return rightLayout;
}

QGroupBox* s21::View::VerticesPanel() {
  QGroupBox* verticesBox = new QGroupBox("VERTICES");
  QHBoxLayout* verticesLayout = new QHBoxLayout();
  verticesLayout->setContentsMargins(10, 3, 10, 3);
  verticesLayout->setSpacing(0);

  verticesLayout->addLayout(createVertexLeftLayout());
  verticesLayout->addLayout(createVertexRightLayout());

  verticesBox->setLayout(verticesLayout);
  verticesBox->setFixedSize(325, 100);
  return verticesBox;
}

void s21::View::OnVertexSizeChanged(int size) {
  if (modelViewWidget) {
    modelViewWidget->setVertexSize(size);
  }
}

void s21::View::OnVertexColorChanged(const QString& color) {
  if (modelViewWidget && colorMap.contains(color)) {
    modelViewWidget->setVertexColor(colorMap[color]);
    QComboBox* vertexColorBox = findChild<QComboBox*>("VertexColorBox");
    if (vertexColorBox) {
      vertexColorBox->setCurrentText(color);
    }
  }
}

void s21::View::OnVertexShapeChanged(const QString& shape) {
  if (modelViewWidget && vertexShapeMap.contains(shape)) {
    int vertexShape = vertexShapeMap.value(shape);
    modelViewWidget->setVertexShape(vertexShape);
  }
}

void s21::View::resetVerticesSettings() {
  QSlider* sizeSlider = findChild<QSlider*>("VertexSizeSlider");
  if (sizeSlider) sizeSlider->setValue(5);
  QComboBox* colorBox = findChild<QComboBox*>("VertexColorBox");
  if (colorBox) colorBox->setCurrentText("White");
  QComboBox* shapeBox = findChild<QComboBox*>("VertexShapeBox");
  if (shapeBox) shapeBox->setCurrentText("Circle");
  if (modelViewWidget) {
    modelViewWidget->defaultVerticesSettings();
  }
}

QGroupBox* s21::View::BackPanel() {
  QGroupBox* backBox = new QGroupBox("BACKGROUND");
  QVBoxLayout* backLayout = new QVBoxLayout();
  QComboBox* backgroundColorBox = new QComboBox();
  backgroundColorBox->setObjectName("BackgroundColorBox");
  backgroundColorBox->addItems({"Black", "Green", "Blue", "Red", "White"});
  connect(backgroundColorBox, &QComboBox::currentTextChanged, this,
          &View::SetBackgroundColor);
  backLayout->addWidget(backgroundColorBox);
  backBox->setLayout(backLayout);
  backBox->setFixedSize(325, 70);

  return backBox;
}

void s21::View::SetBackgroundColor(const QString& color) {
  if (modelViewWidget && colorMap.contains(color)) {
    modelViewWidget->setBackgroundColor(colorMap[color]);
    QComboBox* backgroundColorBox = findChild<QComboBox*>("BackgroundColorBox");
    if (backgroundColorBox) {
      backgroundColorBox->setCurrentText(color);
    }
  }
}

QWidget* s21::View::ProjectionPanel() {
  QGroupBox* projectionGroupBox = new QGroupBox("PROJECTION TYPE", this);
  QHBoxLayout* projectionLayout = new QHBoxLayout(projectionGroupBox);
  QRadioButton* parallelProjectionRadioButton = new QRadioButton("Parallel");
  QRadioButton* centralProjectionRadioButton = new QRadioButton("Central");
  parallelProjectionRadioButton->setObjectName("ParallelRadioButton");
  centralProjectionRadioButton->setObjectName("CentralRadioButton");
  parallelProjectionRadioButton->setChecked(true);
  connect(parallelProjectionRadioButton, &QRadioButton::toggled, this,
          &View::OnProjectionTypeChanged);
  projectionLayout->addWidget(parallelProjectionRadioButton);
  projectionLayout->addWidget(centralProjectionRadioButton);
  projectionGroupBox->setLayout(projectionLayout);
  projectionLayout->setContentsMargins(3, 3, 3, 3);
  projectionLayout->setSpacing(0);
  projectionGroupBox->setFixedSize(325, 70);
  return projectionGroupBox;
}

void s21::View::OnProjectionTypeChanged() {
  QRadioButton* parallelRadioButton =
      findChild<QRadioButton*>("ParallelRadioButton");
  if (!parallelRadioButton) return;
  bool isParallel = parallelRadioButton->isChecked();
  if (modelViewWidget) {
    modelViewWidget->setProjectionType(isParallel);
  }
}

void s21::View::syncInterfaceWithSettings() {
  if (modelViewWidget) {
    const auto& settings = modelViewWidget->getSettings();
    QComboBox* edgesColorBox = findChild<QComboBox*>("EdgesColorBox");
    if (edgesColorBox) {
      edgesColorBox->setCurrentText(
          colorMap.key(settings.edges_color, "White"));
    }
    QComboBox* lineTypeBox = findChild<QComboBox*>("LineTypeBox");
    if (lineTypeBox) {
      if (settings.line_type == 1) {
        lineTypeBox->setCurrentText("Solid");
      } else if (settings.line_type == 2) {
        lineTypeBox->setCurrentText("Dashed");
      }
    }
    QSlider* thicknessSlider = findChild<QSlider*>("ThicknessSlider");
    if (thicknessSlider) {
      thicknessSlider->setValue(settings.edges_size);
    }
    QComboBox* vertexColorBox = findChild<QComboBox*>("VertexColorBox");
    if (vertexColorBox) {
      vertexColorBox->setCurrentText(
          colorMap.key(settings.vertex_color, "White"));
    }
    QSlider* vertexSizeSlider = findChild<QSlider*>("VertexSizeSlider");
    if (vertexSizeSlider) {
      vertexSizeSlider->setValue(settings.vertex_size);
    }
    QComboBox* vertexShapeBox = findChild<QComboBox*>("VertexShapeBox");
    if (vertexShapeBox) {
      vertexShapeBox->setCurrentText(
          vertexShapeMap.key(settings.vertex_shape, "None"));
    }
    QComboBox* backgroundColorBox = findChild<QComboBox*>("BackgroundColorBox");
    if (backgroundColorBox) {
      backgroundColorBox->setCurrentText(
          colorMap.key(settings.bg_color, "Black"));
    }
    QRadioButton* parallelRadioButton =
        findChild<QRadioButton*>("ParallelRadioButton");
    QRadioButton* centralRadioButton =
        findChild<QRadioButton*>("CentralRadioButton");
    if (parallelRadioButton && centralRadioButton) {
      parallelRadioButton->setChecked(settings.is_parallel_projection);
      centralRadioButton->setChecked(!settings.is_parallel_projection);
    }
  }
}

void s21::View::OnOpenFile() {
  QString filePath = QFileDialog::getOpenFileName(this, "Open Model", "",
                                                  "Model Files (*.obj)");
  if (!filePath.isEmpty()) {
    qDebug() << "File selected:" << filePath;
    emit filePathSelected(filePath);
    previous_slider_state_.move_x = 0.0f;
    previous_slider_state_.move_y = 0.0f;
    previous_slider_state_.move_z = 0.0f;
    previous_slider_state_.rotate_x = 180.0f;
    previous_slider_state_.rotate_y = 180.0f;
    previous_slider_state_.rotate_z = 180.0f;
    previous_slider_state_.scale = 1.0f;
    auto verticesCount = modelViewWidget->GetVertices().size() / 3;
    auto facesCount = modelViewWidget->GetFaces().size() / 2;
    infoLabel_->setText(QString("\tVertices: %1\t\t\tEdges: %2\t\t\tFile: %3")
                            .arg(verticesCount)
                            .arg(facesCount)
                            .arg(filePath));
  } else {
    qDebug() << "No file selected.";
  }
}

void View::OnSaveImage() {
  QString fileName = QFileDialog::getSaveFileName(
      this, "Save Image", "", "BMP Files (*.bmp);;JPEG Files (*.jpeg *.jpg)");
  if (fileName.isEmpty()) return;
  if (!fileName.endsWith(".bmp") && !fileName.endsWith(".jpeg") &&
      !fileName.endsWith(".jpg")) {
    fileName.append(".bmp");
  }
  QImage image = modelViewWidget->grabFramebuffer();
  if (!image.save(fileName)) {
    QMessageBox::warning(this, "Save Error", "Failed to save the image.");
  }
}

void View::OnSaveGIF() {
  QString fileName = QFileDialog::getSaveFileName(
      this, "Save GIF", "", "GIF Files (*.gif);;All Files (*)");
  if (fileName.isEmpty()) return;
  GifWriter gif;
  if (!GifBegin(&gif, fileName.toLatin1().data(), 640, 480, 10)) {
    QMessageBox::warning(this, "Error", "Failed to start GIF creation.");
    return;
  }
  QSize gifSize(640, 480);
  QTime timer;
  for (int i = 0; i < 50; ++i) {
    QImage frame = modelViewWidget->grabFramebuffer();
    QImage scaledFrame =
        frame.scaled(gifSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage rgbaFrame = scaledFrame.convertToFormat(QImage::Format_RGBA8888);
    GifWriteFrame(&gif, rgbaFrame.bits(), rgbaFrame.width(), rgbaFrame.height(),
                  10);
    timer = QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < timer) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }
  }
  GifEnd(&gif);
  QMessageBox::information(this, "GIF Created", "GIF saved successfully.");
}

void s21::View::resetSliders() {
  for (auto* widget : findChildren<QWidget*>()) {
    if (widget->property("slider").isValid() &&
        widget->property("defaultValue").isValid()) {
      if (!widget->property("persistent").toBool()) {
        QSlider* slider =
            static_cast<QSlider*>(widget->property("slider").value<void*>());
        int defaultValue = widget->property("defaultValue").toInt();
        slider->setValue(defaultValue);
      }
    }
  }
}

void s21::View::connectSliderToSignal(QWidget* widget, Axis axis) {
  QSlider* slider =
      static_cast<QSlider*>(widget->property("slider").value<void*>());
  connect(slider, &QSlider::valueChanged, this, [this, axis](int new_value) {
    float delta = 0.0f;
    switch (axis) {
      case Axis::X:
        delta = static_cast<float>(new_value) / 100.0f -
                previous_slider_state_.move_x;
        previous_slider_state_.move_x = static_cast<float>(new_value) / 100.0f;
        emit moveChanged(delta, axis);
        break;

      case Axis::Y:
        delta = static_cast<float>(new_value) / 100.0f -
                previous_slider_state_.move_y;
        previous_slider_state_.move_y = static_cast<float>(new_value) / 100.0f;
        emit moveChanged(delta, axis);
        break;

      case Axis::Z:
        delta = static_cast<float>(new_value) / 100.0f -
                previous_slider_state_.move_z;
        previous_slider_state_.move_z = static_cast<float>(new_value) / 100.0f;
        emit moveChanged(delta, axis);
        break;

      case Axis::X_Rotate:
        delta = static_cast<float>(new_value) - previous_slider_state_.rotate_x;
        previous_slider_state_.rotate_x = static_cast<float>(new_value);
        emit rotateChanged(delta, axis);
        break;

      case Axis::Y_Rotate:
        delta = static_cast<float>(new_value) - previous_slider_state_.rotate_y;
        previous_slider_state_.rotate_y = static_cast<float>(new_value);
        emit rotateChanged(delta, axis);
        break;

      case Axis::Z_Rotate:
        delta = static_cast<float>(new_value) - previous_slider_state_.rotate_z;
        previous_slider_state_.rotate_z = static_cast<float>(new_value);
        emit rotateChanged(delta, axis);
        break;

      case Axis::Scale:
        delta = static_cast<float>(new_value) / previous_slider_state_.scale;
        previous_slider_state_.scale = static_cast<float>(new_value);
        emit scaleChanged(delta);
        break;

      default:
        break;
    }
  });
}

void s21::View::ShowError(const std::string& error_message) {
  QMessageBox::critical(this, "Error", QString::fromStdString(error_message));
}

}  // namespace s21
