#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QTextCodec *cyrillicCodec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(cyrillicCodec);

    QCoreApplication::setApplicationName("VoPlayer");
    QCoreApplication::setOrganizationName("4th group");

    return a.exec();
}
