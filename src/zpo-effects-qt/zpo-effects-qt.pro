#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T14:11:03
#
#-------------------------------------------------

TARGET = zpo-effect-qt
TEMPLATE = app

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic

equals(QT_MAJOR_VERSION, 4) {
    QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
    CONFIG += c++11
}

INCLUDEPATH += C:/opencv/build/include
LIBS += C:/opencv/release/lib/libopencv_core2410.dll.a \
    C:/opencv/release/lib/libopencv_highgui2410.dll.a \
    C:/opencv/release/lib/libopencv_imgproc2410.dll.a

SOURCES += \
    ImageFilter.cpp \
    ImageSource.cpp \
    ImageViewerOpenGl.cpp \
    LabelChanger.cpp \
    LoadingDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    Pixel.cpp \
    VoronoiPoint.cpp

HEADERS  += \
    Debug.h \
    ImageFilter.h \
    ImageSource.h \
    ImageViewerOpenGl.h \
    LabelChanger.h \
    LoadingDialog.h \
    MainWindow.h \
    Pixel.h \
    QListWidgetItemFilterType.hpp \
    VoronoiPoint.h

FORMS    += \
    LoadingDialog.ui \
    MainWindow.ui
