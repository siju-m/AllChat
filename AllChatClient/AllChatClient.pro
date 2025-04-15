QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network quick

CONFIG += c++17


QT += sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/chathistorymanager.cpp \
    Core/control.cpp \
    Core/datatransfer.cpp \
    Delegate/chatdelegate.cpp \
    Model/Packet.cpp \
    Model/chatmodel.cpp \
    Core/currentuser.cpp \
    Model/message.cpp \
    Model/strangermodel.cpp \
    Model/user.cpp \
    View/Components/confirmbox.cpp \
    View/Components/dialogtitlebar.cpp \
    View/Login/hollow_button.cpp \
    View/Login/input_box.cpp \
    View/Login/login_button.cpp \
    View/Login/login_form.cpp \
    View/Login/registration_form.cpp \
    View/Login/responsive_form.cpp \
    View/Login/scroll_bar.cpp \
    View/Login/transparent_transition_interface.cpp \
    View/UserDetailView.cpp \
    View/Components/avatarlabel.cpp \
    View/Components/customtitlebar.cpp \
    View/login.cpp \
    View/Components/onlinestatelabel.cpp \
    View/Components/tipsbox.cpp \
    View/updateavatar.cpp \
    View/addfriends.cpp \
    Delegate/applydelegate.cpp \
    Delegate/friendsdelegate.cpp \
    Model/friendsmodel.cpp \
    View/imageviewer.cpp \
    main.cpp \
    mainwindow.cpp \
    Delegate\messagedelegate.cpp \
    View/messagelistview.cpp \
    Model/messagemodel.cpp \
    Delegate/strangerdelegate.cpp

HEADERS += \
    Core/chathistorymanager.h \
    Core/control.h \
    Core/datatransfer.h \
    Delegate/chatdelegate.h \
    Model/Packet.h \
    Model/chatmodel.h \
    Core/currentuser.h \
    Model/message.h \
    Model/strangermodel.h \
    Model/user.h \
    Utils/CustomTypes.h \
    View/Components/confirmbox.h \
    View/Components/dialogtitlebar.h \
    View/Login/hollow_button.h \
    View/Login/input_box.h \
    View/Login/login_button.h \
    View/Login/login_form.h \
    View/Login/registration_form.h \
    View/Login/responsive_form.h \
    View/Login/scroll_bar.h \
    View/Login/transparent_transition_interface.h \
    View/UserDetailView.h \
    View/Components/avatarlabel.h \
    View/Components/customtitlebar.h \
    View/login.h \
    View/Components/onlinestatelabel.h \
    View/Components/tipsbox.h \
    View/updateavatar.h \
    View/addfriends.h \
    Delegate/applydelegate.h \
    Delegate/friendsdelegate.h \
    Model/friendsmodel.h \
    View/imageviewer.h \
    mainwindow.h \
    Delegate/messagedelegate.h \
    View/messagelistview.h \
    Model/messagemodel.h \
    Delegate/strangerdelegate.h

FORMS += \
    View/UserDetailView.ui \
    View/updateavatar.ui \
    View/addfriends.ui \
    mainwindow.ui
QMAKE_CXXFLAGS += -static-libgcc
QMAKE_CXXFLAGS += -static-libstdc++

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

# INCLUDEPATH += $$PWD/include
# LIBS += -L$$PWD/libs -lElaWidgetTools

