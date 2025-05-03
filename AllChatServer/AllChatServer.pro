QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/datatransfer.cpp \
    Core/database.cpp \
    Core/redisclient.cpp \
    main.cpp \
    server.cpp

HEADERS += \
    Core/datatransfer.h \
    Core/database.h \
    Core/redisclient.h \
    server.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#使用vcpkg install hiredis
INCLUDEPATH += D:/app/vcpkg/installed/x64-windows/include
LIBS += -LD:/app/vcpkg/installed/x64-windows/lib -lhiredis
