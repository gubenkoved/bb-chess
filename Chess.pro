TEMPLATE = app
TARGET = Chess

CONFIG += qt warn_on debug_and_release cascades

INCLUDEPATH += ../src
SOURCES += ../src/*.cpp
HEADERS += ../src/*.hpp ../src/*.h
LIBS += -lbbsystem

# I've added to allow Scoped::Enums http://supportforums.blackberry.com/t5/Native-Development/C-11-mode-and-qcc/td-p/1717539
QMAKE_CXXFLAGS = -Wc,-std=c++0x

lupdate_inclusion {
    SOURCES += ../assets/*.qml
}

device {
	CONFIG(release, debug|release) {
		DESTDIR = o.le-v7
	}
	CONFIG(debug, debug|release) {
		DESTDIR = o.le-v7-g
	}
}

simulator {
	CONFIG(release, debug|release) {
		DESTDIR = o
	}
	CONFIG(debug, debug|release) {
		DESTDIR = o-g
	}
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

TRANSLATIONS = \
    $${TARGET}_ru.ts \
    $${TARGET}.ts
