#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canoutputwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket=new QTcpSocket(this);
    canWindow = new CanOutputWindow(socket, nullptr);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canWindow;
}

void MainWindow::on_connect_button_clicked()
{
    //  get written port and ip strings
    QString ip_address = ui->ip_addr_input->text();
    QString port_number = ui->port_num_input->text();

    //  convert port number to string
    bool ok;
    int port = port_number.toInt(&ok, 10);
    if (ok && port > 0){
        socket->connectToHost(QHostAddress(ip_address),port);
    } else {
        ui->ip_addr_input->setText("");
        ui->port_num_input->setText("");
        //  show error message
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Incorrect ip address and port format!");
        messageBox.setFixedSize(500,200);
    }
//    hide();  //  hide current window
//    canWindow->show();
}

void MainWindow::connected()
{
    qDebug() << "Successfully connected...";
    hide();  //  hide current window
    canWindow->show();  //  show window with receiving messages
}

void MainWindow::disconnected()
{
    qDebug() << "disconnected...";
    //  if disconection happend
    canWindow->hide();  //  close receive window
    show();  //  show connection window
}


