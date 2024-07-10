QT       += core gui
QT += network sql mqtt
QT += multimedia
include(qzxing/src/QZXing.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FaceSql.cpp \
    admin.cpp \
    approach.cpp \
    autoCheckmqtt.cpp \
    autocheck.cpp \
    http.cpp \
    iamgeprocess.cpp \
    main.cpp \
    mainwindow.cpp \
    my_lineedit.cpp \
    readkeythread.cpp \
    readpe15thread.cpp \
    syszuxpinyin.cpp \
    v4l2api.cpp

HEADERS += \
    approach.h \
    autocheck.h \
    http.h \
    iamgeprocess.h \
    mainwindow.h \
    my_lineedit.h \
    readkeythread.h \
    readpe15thread.h \
    syszuxpinyin.h \
    v4l2api.h

FORMS += \
    autocheck.ui \
    mainwindow.ui \
    syszuxpinyin.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    qtr.qrc
