#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "canoutputwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_button_clicked();
    void connected();
    void disconnected();

private:
    Ui::MainWindow *ui;
    CanOutputWindow *canWindow;
    QTcpSocket* socket;
};
#endif // MAINWINDOW_H
