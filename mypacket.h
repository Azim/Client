#ifndef MYPACKET_H
#define MYPACKET_H

#include <QObject>
#include <QIODevice>

class MyPacket:QObject
{
public:
    MyPacket(int id, QString data1="", QString data2="");
    MyPacket(QByteArray data);
    qint32 id;
    QString data1;
    QString data2;
    operator QByteArray() const;
};

#endif // MYPACKET_H
