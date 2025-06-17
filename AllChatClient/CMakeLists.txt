cmake_minimum_required(VERSION 3.5)
project(AllChatClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 查找 Qt 模块
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql Concurrent)

# 添加头文件路径（如果有额外 include 文件夹，可在此添加）
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 所有源文件
set(SOURCES
    Core/chathistorymanager.cpp
    Core/control.cpp
    Core/datatransfer.cpp
    Delegate/chatdelegate.cpp
    Model/Packet.cpp
    Model/chatmodel.cpp
    Core/currentuser.cpp
    Model/group.cpp
    Model/message.cpp
    Model/strangermodel.cpp
    Model/user.cpp
    View/Components/confirmbox.cpp
    View/Components/dialogtitlebar.cpp
    View/Login/hollow_button.cpp
    View/Login/input_box.cpp
    View/Login/login_button.cpp
    View/Login/login_form.cpp
    View/Login/registration_form.cpp
    View/Login/responsive_form.cpp
    View/Login/scroll_bar.cpp
    View/Login/transparent_transition_interface.cpp
    View/UserDetailView.cpp
    View/Components/avatarlabel.cpp
    View/Components/customtitlebar.cpp
    View/creategroup.cpp
    View/login.cpp
    View/Components/onlinestatelabel.cpp
    View/Components/tipsbox.cpp
    View/updateavatar.cpp
    View/addfriends.cpp
    Delegate/applydelegate.cpp
    Delegate/friendsdelegate.cpp
    Model/friendsmodel.cpp
    View/imageviewer.cpp
    main.cpp
    mainwindow.cpp
    Delegate/messagedelegate.cpp
    View/messagelistview.cpp
    Model/messagemodel.cpp
    Delegate/strangerdelegate.cpp
    View/searchbar.cpp
)

# 所有头文件（可选，用于 IDE 支持）
set(HEADERS
    Core/chathistorymanager.h
    Core/control.h
    Core/datatransfer.h
    Delegate/chatdelegate.h
    Model/Packet.h
    Model/chatmodel.h
    Core/currentuser.h
    Model/group.h
    Model/message.h
    Model/strangermodel.h
    Model/user.h
    Utils/CustomTypes.h
    View/Components/confirmbox.h
    View/Components/dialogtitlebar.h
    View/Login/hollow_button.h
    View/Login/input_box.h
    View/Login/login_button.h
    View/Login/login_form.h
    View/Login/registration_form.h
    View/Login/responsive_form.h
    View/Login/scroll_bar.h
    View/Login/transparent_transition_interface.h
    View/UserDetailView.h
    View/Components/avatarlabel.h
    View/Components/customtitlebar.h
    View/creategroup.h
    View/login.h
    View/Components/onlinestatelabel.h
    View/Components/tipsbox.h
    View/updateavatar.h
    View/addfriends.h
    Delegate/applydelegate.h
    Delegate/friendsdelegate.h
    Model/friendsmodel.h
    View/imageviewer.h
    mainwindow.h
    Delegate/messagedelegate.h
    View/messagelistview.h
    Model/messagemodel.h
    Delegate/strangerdelegate.h    
    View/searchbar.h
)

# UI 文件
set(UIS
    View/UserDetailView.ui
    View/updateavatar.ui
    View/addfriends.ui
    mainwindow.ui
)

# 资源文件
set(RESOURCES
    resource.qrc
)

# 生成可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RESOURCES}
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Concurrent
)

# 可选：静态链接 libgcc/libstdc++
target_compile_options(${PROJECT_NAME} PRIVATE
    -static-libgcc
    -static-libstdc++
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/bin/include)

# target_link_libraries(${PROJECT_NAME} "D:/data/Code/Qt/AllChat/AllChatClient/include/lib/libElaWidgetTools.a")

set(MYLIB_LIB_DIR ${CMAKE_SOURCE_DIR}/bin/lib)
set(MYLIB_NAME ElaWidgetTools.dll)

target_link_libraries(${PROJECT_NAME} PRIVATE ${MYLIB_LIB_DIR}/${MYLIB_NAME})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${MYLIB_LIB_DIR}/ElaWidgetTools.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
