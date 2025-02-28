QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Wno-deprecated-declarations
CONFIG += c++17
TARGET = 3DViewer

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cc \
    openGLfuncs.cc \
    view.cc \
    ../model/model.cc \
    ../model/parser/parser.cc \
    ../model/affine_transform/affinetransform.cc \
    ../libs/s21_matrix_oop.cc \
    ../model/affine_transform/factory.cc \
    ../controller/controller.cc

HEADERS += \
    ../controller/axis.h\
    view.h \
    ../model/model.h \
    ../model/parser/parser.h \
    ../model/affine_transform/affinetransform.h \
    ../libs/s21_matrix_oop.h \
    ../model/affine_transform/factory.h \
    ../controller/controller.h

FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
