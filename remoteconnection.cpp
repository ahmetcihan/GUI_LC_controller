#include "remoteconnection.h"

#include <QTimer>
#include <QDebug>

RemoteConnection::RemoteConnection(int port, QObject *parent) :
    QUdpSocket(parent)
{
    if (!bind())
        qDebug("error binding remote connection object");
    connect(this, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    target = QHostAddress::LocalHost;
    dstPort = port;
}

static bool wait(QEventLoop *el, int timeout){
    QTimer t;
    QObject::connect(&t, SIGNAL(timeout()), el, SLOT(quit()));
    if (timeout)
        t.start(timeout);
    el->exec();
    if (!timeout || t.isActive())
        return true;
    return false;
}
void RemoteConnection::setTarget(const QString &targetIp){
    if (targetIp == "Localhost" || targetIp.isEmpty())
        target = QHostAddress::LocalHost;
    else
        target = QHostAddress(targetIp);
}
void RemoteConnection::readPendingDatagrams(){
    QByteArray datagram;

    while (hasPendingDatagrams()) {
        datagram.resize(pendingDatagramSize());
        readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    }
//    qDebug() << "Message from: " << sender.toString();
//    qDebug() << "Message port: " << senderPort;
//    qDebug() << "Message: " << datagram.toHex();

    //qDebug() << "s8" << (char) datagram[0] << "u8" << (unsigned char) datagram[0] << QString::number(datagram[0],16);

    emit periodic_response_handler(datagram);

    //el.quit();
}
QString RemoteConnection::get(const QString &key){
    QString str = QString("get %1\n").arg(key);
    writeDatagram(str.toUtf8(), target, dstPort);
    if (!wait(&el, 100))
        return "__timeout__";
    foreach (QString line, resp) {
        QString pre = QString("get:%1:").arg(key);
        if (!line.startsWith(pre))
            continue;
        resp.removeOne(line);
        return line.remove(pre);
    }
    return "";
}
int RemoteConnection::set(const QString &key, const QString &value){
    QString str = QString("set %1 %2\n").arg(key).arg(value);
    writeDatagram(str.toUtf8(), target, dstPort);
    return 0;
}

