#include "mypacket.h"

MyPacket::MyPacket(qint32 id, QString data1, QString data2)
    : id(id)
    , data1(data1)
    , data2(data2)
{

}

MyPacket::MyPacket(QByteArray data){
    QDataStream stream(&data, QIODevice::ReadWrite);
    stream>>id;
    stream>>data1;
    stream>>data2;
}

MyPacket::operator QByteArray() const{
    QByteArray result;
    QDataStream stream(&result, QIODevice::ReadWrite);
    stream<<id;
    stream<<data1;
    stream<<data2;
    return result;
}
