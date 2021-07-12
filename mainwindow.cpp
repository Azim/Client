#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpSocketReady()));
    connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(HandleStateChange(QAbstractSocket::SocketState)));
}

MainWindow::~MainWindow()
{
    settings->sync();
    delete ui;
    tcpSocket->deleteLater();
}

void MainWindow::loadSettings(){
    ui->tcpIpLineEdit->setText(settings->value("tcp/ip", "").toString());
    ui->tcpPortLineEdit->setText(settings->value("tcp/port", "").toString());
    ui->udpPortLineEdit->setText(settings->value("udp/port", "").toString());
}

void MainWindow::tcpSocketReady()
{
    MyPacket packet = tcpSocket->readAll();

    ui->tcpTableWidget->insertRow(ui->tcpTableWidget->rowCount());
    ui->tcpTableWidget->setItem(ui->tcpTableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(packet.id)));
    ui->tcpTableWidget->setItem(ui->tcpTableWidget->rowCount()-1, 1, new QTableWidgetItem(packet.data1));
    ui->tcpTableWidget->setItem(ui->tcpTableWidget->rowCount()-1, 2, new QTableWidgetItem(packet.data2));

    //ui->tcpResultTextEdit->appendPlainText(packet.data1+"|"+packet.data2);

    qDebug()<<"Recieved tcp data"<<packet.data1<<"|"<<packet.data2;
}

void MainWindow::on_tcpConnectButton_clicked()
{
    settings->setValue("tcp/ip", ui->tcpIpLineEdit->text());
    settings->setValue("tcp/port", ui->tcpPortLineEdit->text());
    qDebug()<<"Connecting to tcp server";
    tcpSocket->connectToHost(ui->tcpIpLineEdit->text(), ui->tcpPortLineEdit->text().toUInt());
}
void MainWindow::HandleStateChange(QAbstractSocket::SocketState socketState){
    qDebug()<<socketState;
    if(socketState==QAbstractSocket::ConnectedState){
        qDebug()<<"Connected to tcp server";
    }else if(socketState==QAbstractSocket::UnconnectedState){
        qDebug()<<"Disconnected from tcp server/unable to connect to tcp server";
    }
}

void MainWindow::on_tcpDisconnectButton_clicked()
{
    tcpSocket->disconnectFromHost();
    qDebug()<<"Disconnected from tcp server";
}

void MainWindow::udpStartListening()
{
    if(udpSocket==nullptr||udpSocket->state()!=udpSocket->BoundState){
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress::Any, ui->udpPortLineEdit->text().toInt());
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadPendingDatagrams()));
        qDebug()<<"Started listening on udp";
    }else{
        qDebug()<<"Already listening";
    }
}
void MainWindow::udpStopListening()
{
    if(udpSocket!=nullptr&&udpSocket->state()==udpSocket->BoundState){
        disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadPendingDatagrams()));
        udpSocket->close();
        udpSocket->deleteLater();
        qDebug()<<"Stopped listening on udp";
    }else{
        qDebug()<<"Wasnt listening";
    }
}


void MainWindow::udpReadPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        MyPacket packet = datagram.data();
        ui->udpTableWidget->insertRow(ui->udpTableWidget->rowCount());
        ui->udpTableWidget->setItem(ui->udpTableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(packet.id)));
        ui->udpTableWidget->setItem(ui->udpTableWidget->rowCount()-1, 1, new QTableWidgetItem(packet.data1));
        ui->udpTableWidget->setItem(ui->udpTableWidget->rowCount()-1, 2, new QTableWidgetItem(packet.data2));
        qDebug()<<"Recieved udp data"<<packet.data1<<"|"<<packet.data2;
    }
}

void MainWindow::on_udpStartListeningButton_clicked()
{
    settings->setValue("udp/port", ui->udpPortLineEdit->text());
    udpStartListening();
}


void MainWindow::on_udpStopListeningButton_clicked()
{
    udpStopListening();
}

