#ifndef REMOTECONNECTION
#define REMOTECONNECTION

#include <QUdpSocket>
#include <QEventLoop>
#include <QStringList>
#include <QElapsedTimer>


class RemoteConnection : public QUdpSocket
{
    Q_OBJECT
public:
    explicit RemoteConnection(int port = 1000, QObject *parent = 0);
    void setTarget(const QString &targetIp);
    QString get(const QString &key);
    int set(const QString &key, const QString &value);

    QEventLoop el;
    QHostAddress sender;
    QHostAddress target;
    quint16 senderPort;
    QStringList resp;
    int dstPort;
signals:
    void periodic_response_handler(QByteArray data);

public slots:
    void readPendingDatagrams();

};

#endif // REMOTECONNECTION

