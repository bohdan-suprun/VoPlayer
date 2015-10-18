#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "registration.h"
#include "requestmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_autorisation_click(bool);
    void setName(QString,QString);
    void on_pushButton_clicked();
    void on_request(QMap<QString,QString>,Action);
    void smError(QString);

    void on_pushButton_2_clicked();

private:
    QString ru;
    unsigned int count;
    RequestManager * manager;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
