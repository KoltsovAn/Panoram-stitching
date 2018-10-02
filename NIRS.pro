#-------------------------------------------------
#
# Project created by QtCreator 2017-09-24T12:12:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NIRS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mygraphicsscene.cpp \
    iteminfo.cpp \
    open_form.cpp \
    save_form.cpp \
    minimalstitching.cpp \
    mapupdating.cpp \
    analysis_of_a_manual_stitching.cpp \
    gradient_method_stitching.cpp \
    point.cpp \
    gradient_stitching.cpp \
    minimalstitching_3d.cpp \
    focus.cpp \
    gradient_stitching_3d.cpp \
    recovery_form.cpp

HEADERS += \
        mainwindow.h \
    mygraphicsscene.h \
    iteminfo.h \
    open_form.h \
    save_form.h \
    minimalstitching.h \
    stitching_frames.h \
    mapupdating.h \
    analysis_of_a_manual_stitching.h \
    gradient_method_stitching.h \
    point.h \
    gradient_stitching.h \
    stitching_frames_3d.h \
    minimalstitching_3d.h \
    focus.h \
    gradient_stitching_3d.h \
    recovery_form.h

FORMS += \
        mainwindow.ui \
    open_form.ui \
    save_form.ui \
    recovery_form.ui
