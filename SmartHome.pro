#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T13:56:47
#
#-------------------------------------------------

QT       += core gui network

TARGET = TemplateMatch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    handdetector.cpp \
    svmhanddetector.cpp \
    simplehanddetector.cpp

HEADERS  += mainwindow.h \
    handdetector.h \
    svmhanddetector.h \
    simplehanddetector.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv\
		/usr/local/include
LIBS += /usr/local/lib/libopencv_core.so.2.3.1\
        /usr/local/lib/libopencv_imgproc.so.2.3.1\
        /usr/local/lib/libopencv_highgui.so.2.3.1\
        /usr/local/lib/libopencv_ml.so.2.3.1\
        /usr/local/lib/libopencv_video.so.2.3.1\
        /usr/local/lib/libopencv_features2d.so.2.3.1\
        /usr/local/lib/libopencv_calib3d.so.2.3.1\
        /usr/local/lib/libopencv_objdetect.so.2.3.1\
        /usr/local/lib/libopencv_contrib.so.2.3.1\
        /usr/local/lib/libopencv_legacy.so.2.3.1\
	/usr/local/lib/libopencv_flann.so.2.3.1

RESOURCES += \
    tm.qrc
