#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T23:49:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wendynator
TEMPLATE = app

INCLUDEPATH +=  /usr/include/opencv2\
                /usr/include/opencv2/core\
                /usr/include/opencv2/imgproc\
                /usr/include/opencv2/highgui\
                /usr/include/opencv2/video\


LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_ml \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_objdetect \
        -lopencv_contrib \
        -lopencv_legacy \
        -lopencv_flann


SOURCES += \
    main.cpp \
    Sensors/ColorDetection/ColorDetection.cpp \
    Sensors/ColorSensor/ColorSensor.cpp \
    Sensors/TemplateMatchSensor/TemplateMatchSensor.cpp \
    Sensors/TemplateSensorConfigWindow/TemplateSensorConfigWindows.cpp \
    myMath/myMath.cpp \
    MainWindow/mainwindow.cpp \
    Draw/Draw.cpp \
    cv2qtimage/cv2qtimage.cpp \
    MainWindow/MainLoop.cpp
HEADERS  += \
    Sensors/ColorDetection/ColorDetection.h \
    Sensors/ColorSensor/ColorSensor.h \
    Sensors/TemplateMatchSensor/TemplateMatchSensor.h \
    Sensors/TemplateSensorConfigWindow/TemplateSensorConfigWindows.h \
    Sensors/InterfaceSensor.h \
    myMath/myMath.h \
    MainWindow/mainwindow.h \
    Draw/Draw.h \
    cv2qtimage/cv2qtimage.h
FORMS    += \
    Sensors/TemplateSensorConfigWindow/TemplateSensorConfigWindows.ui \
    MainWindow/mainwindow.ui
