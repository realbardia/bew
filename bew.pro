TARGET = bew
QT += core gui webenginewidgets widgets network

CONFIG += c++17

SOURCES += \
    adddialog.cpp \
    bew.cpp \
    bewappitem.cpp \
    bewsharedialog.cpp \
    bewstore.cpp \
    bewstoreengine.cpp \
    bewstoreitemwidget.cpp \
    bewwebview.cpp \
    commandparser.cpp \
    main.cpp \
    settingsdialog.cpp

HEADERS += \
    adddialog.h \
    bew.h \
    bew_globals.h \
    bewappitem.h \
    bewsharedialog.h \
    bewstore.h \
    bewstoreengine.h \
    bewstoreitemwidget.h \
    bewwebview.h \
    commandparser.h \
    settingsdialog.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    adddialog.ui \
    bewsharedialog.ui \
    bewstore.ui \
    bewstoreitemwidget.ui \
    settingsdialog.ui

RESOURCES += \
    resource.qrc
