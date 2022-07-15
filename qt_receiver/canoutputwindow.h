#ifndef CANOUTPUTWINDOW_H
#define CANOUTPUTWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTime>
#include <QTimer>
#include <vector>


namespace Ui {
class CanOutputWindow;
}

class CanOutputWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CanOutputWindow(QTcpSocket* s, QWidget *parent = nullptr);
    void showWindow(void);
    ~CanOutputWindow();

private slots:
    void on_pushButton_clicked();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    Ui::CanOutputWindow *ui;
    QTcpSocket* socket;
    std::vector<unsigned long> id_list;

    void setupTable(void);
    void parseAndUpdate(QString &data);
    void parseCanMessage(QStringList &tokenList);
};

#endif // CANOUTPUTWINDOW_H
