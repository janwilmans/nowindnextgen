# CMake >= 2.4.0 is needed for Qt4
cmake_minimum_required(VERSION 2.4.0) 

# find and setup Qt4 for this project
find_package(Qt4 REQUIRED)

# add definitions needed for Qt4
add_definitions(${QT_DEFINITIONS})

# include Qt4 cmake file
include(${QT_USE_FILE})

# add binary dir to include path so we can find the Qt4 generated headers
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# CMake fails to add all libraries needed for linking with staticially built Qt, so we do it ourselves
IF(MINGW AND NOT QT_SHARED) 
	SET(QT_LIBRARIES "${QT_LIBRARIES}" Imm32 Winmm gdi32 comdlg32 oleaut32 winspool Ws2_32 ole32 uuid)
ENDIF(MINGW AND NOT QT_SHARED)

# Hide console window for release builds
IF(MINGW)
	SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mwindows")
ENDIF(MINGW)


# List .ui files which need processing by Qt4
SET(UI_FILES
	MainWindow.ui
)

set(QT_USE_OPENGL TRUE)

# Generate headers from .ui files
QT4_WRAP_UI(UI_H_FILES ${UI_FILES})

# List sources needed by our executable, including generated Qt4 headers
ADD_EXECUTABLE(qt-simple-test WIN32
	Main.cpp
	MainWindow.cpp
	MainWindow.h
	GLWidget.cpp
	GLWidget.h
	${UI_FILES}
)

# Link our executable against needed Qt4 libraries
TARGET_LINK_LIBRARIES(qt-simple-test QtOpenGL4 OpenGL32.Lib GlU32.Lib
	${QT_LIBRARIES}
)
