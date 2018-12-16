#-------------------------------------------------
#
# Project created by QtCreator 2016-10-15T16:40:54
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cabinet
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    widget/home.cpp \
    misc/zdiylist.cpp \
    dialog/verifydialog.cpp \
    widget/fingermanage.cpp \
    dialog/infodialog.cpp \
    misc/mediaplayer.cpp \
    widget/setup.cpp \
    widget/setuplock.cpp \
    dialog/downdialog.cpp \
    widget/setuprfid.cpp \
    misc/msgwapper.cpp \
    misc/subcabinfo.cpp \
    dialog/setdutydialog.cpp \
    dialog/inputintdialog.cpp \
    widget/setupserial.cpp \
    widget/setuprs485.cpp \
    misc/databaseprivate.cpp \
    misc/database.cpp \
    widget/dutymanage.cpp \
    misc/policeinfo.cpp \
    widget/applymanage.cpp \
    dialog/datetimedialog.cpp \
    misc/zdiylistitem.cpp \
    dialog/addidcarddialog.cpp \
    widget/setupmusic.cpp \
    dialog/taskitemdialog.cpp \
    dialog/dialog.cpp \
    widget/query.cpp \
    widget/taskcabinfo.cpp \
    widget/cabinetinfo.cpp \
    misc/application.cpp \
    dialog/verify_p.cpp \
    network/server.cpp \
    dialog/alcoholdialog.cpp \
    dialog/rfiddialog.cpp \
    misc/config.cpp \
    device/seriallock.cpp \
    device/serialrfid.cpp \
    network/dataloop.cpp

HEADERS  += mainwidget.h \
    widget/home.h \
    misc/zdiylist.h \
    dialog/verifydialog.h \
    widget/fingermanage.h \
    dialog/infodialog.h \
    misc/mediaplayer.h \
    widget/setup.h \
    widget/setuplock.h \
    dialog/downdialog.h \
    widget/setuprfid.h \
    misc/msgwapper.h \
    misc/subcabinfo.h \
    dialog/setdutydialog.h \
    dialog/inputintdialog.h \
    widget/setupserial.h \
    widget/setuprs485.h \
    misc/databaseprivate.h \
    misc/database.h \
    widget/dutymanage.h \
    misc/policeinfo.h \
    widget/applymanage.h \
    dialog/datetimedialog.h \
    misc/zdiylistitem.h \
    dialog/addidcarddialog.h \
    widget/setupmusic.h \
    dialog/taskitemdialog.h \
    dialog/dialog.h \
    widget/query.h \
    widget/taskcabinfo.h \
    widget/cabinetinfo.h \
    misc/application.h \
    dialog/verify_p.h \
    common.h \
    network/dataloop.h \
    network/datamodel.h \
    network/server.h \
    misc/config.h \
    dialog/alcoholdialog.h \
    dialog/rfiddialog.h \
    device/seriallock.h \
    device/serialrfid.h

FORMS    += mainwidget.ui \
    widget/home.ui \
    misc/zdiylist.ui \
    dialog/verifydialog.ui \
    widget/fingermanage.ui \
    dialog/infodialog.ui \
    widget/setup.ui \
    widget/setuplock.ui \
    dialog/downdialog.ui \
    widget/setuprfid.ui \
    misc/subcabinfo.ui \
    dialog/setdutydialog.ui \
    dialog/inputintdialog.ui \
    widget/setupserial.ui \
    widget/setuprs485.ui \
    widget/dutymanage.ui \
    misc/policeinfo.ui \
    widget/applymanage.ui \
    dialog/datetimedialog.ui \
    dialog/addidcarddialog.ui \
    widget/setupmusic.ui \
    dialog/taskitemdialog.ui \
    widget/query.ui \
    widget/taskcabinfo.ui \
    widget/cabinetinfo.ui \
    dialog/alcoholdialog.ui \
    dialog/rfiddialog.ui

RESOURCES += \
    ImgSrc.qrc

#check Qt version
equals(QT_MAJOR_VERSION, 4) {
    HEADERS += misc/input/myinputpanel.h \
        misc/input/myinputpanelcontext.h
    SOURCES += misc/input/myinputpanel.cpp \
        misc/input/myinputpanelcontext.cpp
    FORMS += misc/input/myinputpanelform.ui
}

DESTDIR = $$PWD/bin

MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui
OBJECTS_DIR = obj

#兼容XP
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/widget
DEPENDPATH += $$PWD/widget
INCLUDEPATH += $$PWD/misc
DEPENDPATH += $$PWD/misc
INCLUDEPATH += $$PWD/dialog
DEPENDPATH += $$PWD/dialog
INCLUDEPATH += $$PWD/network
DEPENDPATH += $$PWD/network
INCLUDEPATH += $$PWD/device
DEPENDPATH += $$PWD/device

# 如果是Debug模式，在名称最后加d
# build_pass:CONFIG(debug, debug|release) { TARGET = $$join(TARGET,,,d) }

unix: LIBS += -Wl,-rpath=./

# 包含USB指纹仪动态链接库
unix: LIBS += -L$$DESTDIR -lzazlib
win32: LIBS += -L$$DESTDIR -lZAZAPIt

win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR -lDevice -lNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR -lDeviced -lNetworkd
else:unix:!macx: LIBS += -L$$DESTDIR -lDevice -lNetwork
