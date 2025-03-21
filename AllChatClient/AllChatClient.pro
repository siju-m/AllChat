QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17


QT += sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/datatransfer.cpp \
    View\addfriends.cpp \
    Delegate\applydelegate.cpp \
    Delegate\friendsdelegate.cpp \
    Model\friendsmodel.cpp \
    View\imageviewer.cpp \
    View\login.cpp \
    main.cpp \
    mainwindow.cpp \
    Delegate\messagedelegate.cpp \
    View\messagelistview.cpp \
    Model\messagemodel.cpp \
    View\regist.cpp \
    Delegate\strangerdelegate.cpp

HEADERS += \
    CommonEnum.h \
    Core/datatransfer.h \
    View\addfriends.h \
    Delegate\applydelegate.h \
    Delegate\friendsdelegate.h \
    Model\friendsmodel.h \
    View\imageviewer.h \
    View\login.h \
    mainwindow.h \
    Delegate\messagedelegate.h \
    View\messagelistview.h \
    Model\messagemodel.h \
    View\regist.h \
    Delegate\strangerdelegate.h

FORMS += \
    View\addfriends.ui \
    View\login.ui \
    mainwindow.ui \
    View\regist.ui
QMAKE_CXXFLAGS += -static-libgcc
QMAKE_CXXFLAGS += -static-libstdc++

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
