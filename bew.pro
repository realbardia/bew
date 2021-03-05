TARGET = bew
QT += core gui webenginewidgets

CONFIG += c++11

SOURCES += \
    bew.cpp \
    bewwebview.cpp \
    main.cpp

HEADERS += \
    bew.h \
    bewwebview.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
