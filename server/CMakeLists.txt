cmake_minimum_required(VERSION 3.16)

project(server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Boost REQUIRED COMPONENTS system) 
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(yaml-cpp REQUIRED)

qt_standard_project_setup()

qt_add_executable(appserver
    src/message.h
    src/message.cpp
    src/database.h
    src/database.cpp
    src/sha256.h
    src/sha256.cpp
    src/serverThread.h
    src/serverThread.cpp
    src/main.h
    src/main.cpp
)

set_target_properties(appserver PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_link_libraries(appserver
    PRIVATE 
    Qt6::Quick
)
target_link_libraries(appserver
    PRIVATE
    Boost::system
)
target_link_libraries(appserver
    PRIVATE 
    OpenSSL::SSL
    OpenSSL::Crypto
)
target_link_libraries(appserver
    PRIVATE
    ${SQLite3_LIBRARIES}
)
target_link_libraries(appserver
    PRIVATE 
    yaml-cpp
)


include(GNUInstallDirs)
install(TARGETS appserver
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
