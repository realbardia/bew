TARGET = bew
QT += core gui webenginewidgets widgets network

CONFIG += c++11

SOURCES += \
    adddialog.cpp \
    bew.cpp \
    bewwebview.cpp \
    main.cpp

HEADERS += \
    adddialog.h \
    bew.h \
    bewwebview.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    adddialog.ui

RESOURCES += \
    resource.qrc
