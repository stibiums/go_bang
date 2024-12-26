cmake_minimum_required(VERSION 3.16)

project(GomokuGUI VERSION 0.1 LANGUAGES CXX)

# 启用Qt的自动处理功能
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt Widgets模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 定义UI和主窗口源文件
set(UI_SOURCES
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# 定义棋盘视图源文件
set(BOARD_WIDGET_SOURCES
    BoardWidget.cpp
    BoardWidget.h
)

# 定义游戏逻辑源文件
set(GAME_LOGIC_SOURCES
    game_logic/boards.cpp
    game_logic/aimove2.cpp
    game_logic/law.cpp
    game_logic/menu.cpp
)

# 合并所有源文件
set(PROJECT_SOURCES
    main.cpp
    ${UI_SOURCES}
    ${BOARD_WIDGET_SOURCES}
    ${GAME_LOGIC_SOURCES}
    # 如果有资源文件，取消注释并添加
    # resources.qrc
)

# 根据Qt版本添加可执行文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GomokuGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    # 如果有资源文件，取消注释并添加
    # qt_add_resources(GomokuGUI "resources"
    #     PREFIX "/"
    #     FILES
    #         resources/icon.png
    # )
else()
    if(ANDROID)
        add_library(GomokuGUI SHARED
            ${PROJECT_SOURCES}
        )
        # 定义Android的属性（如果需要）
        # set_property(TARGET GomokuGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        #              ${CMAKE_CURRENT_SOURCE_DIR}/android)
    else()
        add_executable(GomokuGUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接Qt Widgets库
target_link_libraries(GomokuGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 设置目标属性
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GomokuGUI)
endif()

set_target_properties(GomokuGUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标
include(GNUInstallDirs)
install(TARGETS GomokuGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 对于Qt 6，完成可执行文件的最终配置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GomokuGUI)
endif()
