QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bininjector

QMAKE_CXXFLAGS += -fno-strict-aliasing  -Wno-implicit-fallthrough -std=c++1z

QMAKE_RPATHDIR += /usr/lib/${TARGET}_rl

SOURCES += \
    Dialogs/dialog-about.cpp \
    Dialogs/dialog-error.cpp \
    Dialogs/dialog-manual.cpp \
    Dialogs/dialog-settings.cpp \
    main-hub.cpp \
    main-window.cpp \
    main.cpp \
    tester.cpp

HEADERS += \
    Dialogs/dialog-about.h \
    Dialogs/dialog-error.h \
    Dialogs/dialog-manual.h \
    Dialogs/dialog-settings.h \
    bin-injector-defs.h \
    logger.h \
    main-hub-defs.h \
    main-hub.h \
    main-window.h \
    tester.h

FORMS += \
	Dialogs/dialog-about.ui \
	Dialogs/dialog-error.ui \
	Dialogs/dialog-manual.ui \
	Dialogs/dialog-settings.ui \
	main-window.ui

TRANSLATIONS += \
	bininjector_en_EN.ts

DISTFILES += \
	bininjector \
	bininjector.desktop \
	bininjector_cleaner \
	bininjector_rl/libQt6Core.so.6 \
	bininjector_rl/libQt6DBus.so.6 \
	bininjector_rl/libQt6Gui.so.6 \
	bininjector_rl/libQt6OpenGL.so.6 \
	bininjector_rl/libQt6Widgets.so.6 \
	bininjector_rl/libQt6XcbQpa.so.6 \
	bininjector_rl/libicudata.so.56 \
	bininjector_rl/libicui18n.so.56 \
	bininjector_rl/libicuuc.so.56 \
	bininjector_rl/platforms/libqxcb.so \
	cleaning \
	deployment \
	project_settings.txt \
	site.procyonproject.bininjector.policy

RESOURCES += \
	resources.qrc
