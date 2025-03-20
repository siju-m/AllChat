QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17


QT += sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Utils/datatransfer.cpp \
    addfriends.cpp \
    applydelegate.cpp \
    friendsdelegate.cpp \
    friendsmodel.cpp \
    imageviewer.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    messagedelegate.cpp \
    messagelistview.cpp \
    messagemodel.cpp \
    regist.cpp \
    strangerdelegate.cpp

HEADERS += \
    CommonEnum.h \
    Utils/datatransfer.h \
    addfriends.h \
    applydelegate.h \
    friendsdelegate.h \
    friendsmodel.h \
    imageviewer.h \
    login.h \
    mainwindow.h \
    messagedelegate.h \
    messagelistview.h \
    messagemodel.h \
    regist.h \
    strangerdelegate.h

FORMS += \
    addfriends.ui \
    login.ui \
    mainwindow.ui \
    regist.ui
QMAKE_CXXFLAGS += -static-libgcc
QMAKE_CXXFLAGS += -static-libstdc++

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
