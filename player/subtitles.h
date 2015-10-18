#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <QObject>
#include <QTextBrowser>
#include "requestmanager.h"

class MainWindow;
class TimeConteiner;
class Subtitles : public QTextBrowser
{
    Q_OBJECT
    public:
        explicit Subtitles(QWidget *parent,MainWindow* w);
        QMap<TimeConteiner, QString> *loadFile(QString &filename);
        ~Subtitles();
    protected:
        virtual void enterEvent(QEvent *);
        virtual void leaveEvent(QEvent *);
        virtual void keyPressEvent(QKeyEvent *);
        virtual void mouseReleaseEvent(QMouseEvent *);
    private:
        RequestManager* trans;
        MainWindow * wind;
    signals:

    public slots:
};

#endif // SUBTITLES_H
