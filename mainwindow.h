#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QSettings>

#include "mypacket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSettings* settings;
    void loadSettings();

public slots:
    void tcpSocketReady();

    void udpStartListening();
    void udpStopListening();
    void udpReadPendingDatagrams();
    void HandleStateChange(QAbstractSocket::SocketState socketState);

private slots:
    void on_tcpConnectButton_clicked();
    void on_tcpDisconnectButton_clicked();

    void on_udpStartListeningButton_clicked();
    void on_udpStopListeningButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket* tcpSocket;
    QUdpSocket* udpSocket;
};
#endif // MAINWINDOW_H
