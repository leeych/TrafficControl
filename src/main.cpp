#include "mainwindow.h"
#include "systeminstaller.h"
#include "macrostring.h"
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(":/languages/qt_zh_CN.qm");
    QTranslator custom_translator;
    custom_translator.load(":/languages/trafficCtrl_cn.qm");
    a.installTranslator(&translator);
    a.installTranslator(&custom_translator);

    SystemInstaller initer;
    if (initer.initUserDir() == SystemInstaller::UserDirErr)
    {
        QMessageBox::information(NULL, STRING_TIP, STRING_SYSTEM_CONFIG_INIT_ERR);
        return EXIT_FAILURE;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
