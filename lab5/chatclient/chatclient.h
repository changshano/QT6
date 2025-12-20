#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class chatclient : public QObject
{
    Q_OBJECT
public:
    explicit chatclient(QObject *parent = nullptr);

signals:
    void connected();
    void messageReceived(const QString &text);
    void jsonReceived(const QJsonObject &docObj);
private:
    QTcpSocket *m_clientSocket;

public:
    void onReadyRead();
    void sendMessage(const QString &text, const QString &type="message");
    void connectToServer(const QHostAddress &address,quint16 port);
    void disconnectFromHost();
};

#endif // CHATCLIENT_H
