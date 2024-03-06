
FILE(GLOB SOURCES
        main.cpp
        bew.cpp bew.h
        bewstore.cpp bewstore.h bewstore.ui
        adddialog.cpp adddialog.h adddialog.ui
        bew_globals.h
        bewappitem.cpp bewappitem.h
        bewsharedialog.cpp bewsharedialog.h bewsharedialog.ui
        bewstore.cpp bewstore.h bewstore.ui
        bewstoreengine.cpp bewstoreengine.h
        bewstoreitemwidget.cpp bewstoreitemwidget.h bewstoreitemwidget.ui
        bewwebview.cpp bewwebview.h
        commandparser.cpp commandparser.h
        settingsdialog.cpp settingsdialog.h settingsdialog.ui
        donatedialog.h donatedialog.cpp donatedialog.ui
)

set(PROJECT_SOURCES
    ${SOURCES}
    ${PROJECT_SOURCES}
    PARENT_SCOPE)
