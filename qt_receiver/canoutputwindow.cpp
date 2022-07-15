#include "canoutputwindow.h"
#include "ui_canoutputwindow.h"
#include <QDataStream>
#include <vector>

CanOutputWindow::CanOutputWindow(QTcpSocket* socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CanOutputWindow), socket(socket)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("!!Connected!!");
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    setupTable();
}

void CanOutputWindow::setupTable(){
    ui->tableWidget->setRowCount(20);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels( QStringList() <<  "ID" << "Msg Type" << "Data length" << "Data" );
//    for(int row = 0; row < 20; row++){
//        for(int col = 0; col < 4; col++){
//            QTableWidgetItem *item = new QTableWidgetItem("");
//            item->setTextAlignment(Qt::AlignHCenter);
//            ui->tableWidget->setItem(row, col, item);
//        }
//    }

}

CanOutputWindow::~CanOutputWindow()
{
    delete ui;
}

void CanOutputWindow::on_pushButton_clicked()
{
    qDebug() << "Button clicked";
}

void CanOutputWindow::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void CanOutputWindow::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    QString data = QString(socket->readAll());
    parseAndUpdate(data);
}

void CanOutputWindow::parseAndUpdate(QString &data){
    QStringList splitted = data.split("\"");
    for (QString item : splitted)
        qDebug() << item;
    QString msgType = splitted.at(3);
    if ( msgType.compare("can_msg") == 0){
        //  received message is can frame
        parseCanMessage(splitted);
    }
}

void CanOutputWindow::parseCanMessage(QStringList &tokenList){
    QString id_string =  tokenList.at(11);
    QString data_size =  tokenList.at(15);
    QString data      =  tokenList.at(19);
    unsigned long id = stoul(id_string.toStdString(), nullptr, 16);

    //  check if message with the same id was received
    int item_idx = -1;
    for (int idx = 0; idx < id_list.size(); idx++){
        if (id_list[idx] == id){
            item_idx = idx;
            break;
        }
    }
    //  if this ID received first
    if (item_idx == -1){
        id_list.push_back(id);
        item_idx = id_list.size() - 1;
    }




    //  set id
    QTableWidgetItem *item = new QTableWidgetItem(id_string);
    ui->tableWidget->setItem(item_idx, 0, item);
    //  set data length
    item = new QTableWidgetItem(data_size);
    ui->tableWidget->setItem(item_idx, 2, item);
    //  set data length
    item = new QTableWidgetItem(data);
    ui->tableWidget->setItem(item_idx, 3, item);
}
