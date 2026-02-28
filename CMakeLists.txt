cmake_minimum_required(VERSION 4.1)
project(Final_Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:\\Qt\\6.10.2\\mingw_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
        main.cpp
        Book/Book.cpp
        Book/EBook.cpp
        Book/PrintedBook.cpp
        Transaction/Patron.cpp
        Transaction/Transaction.cpp
        Transaction/Date.cpp
        Library.cpp
)

set(HEADERS
        Book/Book.hpp
        Book/EBook.hpp
        Book/PrintedBook.hpp
        Transaction/Patron.hpp
        Transaction/Transaction.hpp
        Transaction/Date.hpp
        Library.hpp
        MainWindow.cpp
        MainWindow.hpp
)

add_executable(Final_Project
        ${SOURCES}
        ${HEADERS}
        resources.qrc
)

target_link_libraries(Final_Project Qt6::Widgets)

if(WIN32)
    set_target_properties(Final_Project PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

# Copy Qt DLLs
add_custom_command(TARGET Final_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/Qt/6.10.2/mingw_64/bin"
        $<TARGET_FILE_DIR:Final_Project>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/Qt/6.10.2/mingw_64/plugins/platforms"
        $<TARGET_FILE_DIR:Final_Project>/platforms
        COMMENT "Copying Qt DLLs and plugins to output directory"
)