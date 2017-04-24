cmake_minimum_required(VERSION 3.5)

project(split-screen)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Quick)
find_package(Qt5QuickControls2)

set(Qt5_LIBS Qt5::Quick Qt5::QuickControls2)

qt5_add_resources(SRC_RESOURCE "resources.qrc")

find_package(OpenCV REQUIRED)

file(GLOB SRC "*.cpp")
add_executable(${PROJECT_NAME} WIN32 ${SRC} ${SRC_RESOURCE})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Qt5_LIBS})

FILE(GLOB QML_SRC "*.qml")
ADD_CUSTOM_TARGET(qml_files SOURCES ${QML_SRC})
