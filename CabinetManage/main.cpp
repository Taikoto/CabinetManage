#include "mainwidget.h"

#include <QFile>
#include <QStyleFactory>
#include <QTranslator>

#include "misc/application.h"
#include <QDebug>

#if (QT_VERSION == QT_VERSION_CHECK(4, 8, 6))
#include "misc/input/myinputpanelcontext.h"
#endif

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    //QApplication::setStyle(QStyleFactory::create("Plastique"));

    QTranslator tran;
    tran.load("zh_CN", QApplication::applicationDirPath());
    a.installTranslator(&tran);

#if (QT_VERSION == QT_VERSION_CHECK(4, 8, 6))
    //Linux code UTF-8
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    MyInputPanelContext *ic = new MyInputPanelContext;
    a.setInputContext(ic);
#endif

    MainWidget w;

    QFile file(QApplication::applicationDirPath() + "/widget.css");
    if(file.open(QFile::ReadOnly)){
        w.setStyleSheet(file.readAll());
        file.close();
    }

    w.show();

    return a.exec();
}
