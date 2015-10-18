#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *cyrillicCodec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(cyrillicCodec);
    QCoreApplication::setApplicationName("VoPlayer");
    QCoreApplication::setOrganizationName("4th group");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
