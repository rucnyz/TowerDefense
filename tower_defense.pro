QT       += core gui multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += resources_big
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
    audioplayer.cpp \
    flyingobject.cpp \
    main.cpp \
    mainwidget.cpp \
    mypushbutton.cpp \
    tmonster.cpp \
    tobject.cpp \
    towerposition.cpp \
    tscene.cpp \
    ttower.cpp \
    waypoint.cpp

HEADERS += \
    audioplayer.h \
    dataset.h \
    flyingobject.h \
    mainwidget.h \
    mypushbutton.h \
    tmonster.h \
    tobject.h \
    towerposition.h \
    tscene.h \
    ttower.h \
    utility.h \
    waypoint.h

FORMS += \
    mainwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    my_res.qrc

DISTFILES += \
    monster/kuai/fastMonster2.gif \
    monster/kuai/image 11026.png \
    monster/kuai/image 11028.png \
    monster/kuai/image 11030.png \
    monster/kuai/image 11032.png \
    monster/kuai/image 11034.png \
    monster/kuai/image 11036.png \
    monster/kuai/image 11038.png \
    monster/kuai/image 11040.png \
    monster/kuai/image 11042.png \
    monster/kuai/image 11044.png \
    monster/kuai_front/fastMonster.gif \
    monster/kuai_front/image 11010.png \
    monster/kuai_front/image 11012.png \
    monster/kuai_front/image 11014.png \
    monster/kuai_front/image 11016.png \
    monster/kuai_front/image 11018.png \
    monster/kuai_front/image 11020.png \
    monster/kuai_front/image 11022.png \
    monster/kuai_front/image 11024.png \
    monster/normalMonster.gif \
    tower/Archer/Archer01.png \
    tower/Artillery/Artillery_2.png
