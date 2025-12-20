#include "chatclient.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
chatclient::chatclient(QObject *parent): QObject{parent}
{
    m_clientSocket =new QTcpSocket(this);

    connect(m_clientSocket,&QTcpSocket::connected,this,&chatclient::connected);
    connect(m_clientSocket,&QTcpSocket::readyRead,this,&chatclient::onReadyRead);
}

void chatclient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);

    for (;;) {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {
            //emit messageReceived(QString::fromUtf8(jsonData));
            QJsonParseError parseError;
            const QJsonDocument jsonDoc =QJsonDocument::fromJson(jsonData,&parseError);
            if(parseError.error ==QJsonParseError::NoError){
                if(jsonDoc.isObject()){
                    // emit logMessage(QJsonDocument(jsonDoc).toJson(QJsonDocument::Compact));
                    emit jsonReceived(jsonDoc.object());
                }
            }
        } else {

            break;
        }
    }
}

void chatclient::sendMessage(const QString &text, const QString &type)
{
    if (m_clientSocket->state()!= QAbstractSocket::ConnectedState)
        return;

    if (!text.isEmpty()) {
        // create a QDataStream operating on the socket
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_5_12);

        // Create the JSON we want to send
        QJsonObject message;
        message["type"] = type;
        message["text"] = text;

        // send the JSON using QDataStream
        serverStream << QJsonDocument(message).toJson();
    }
}

void chatclient::connectToServer(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(address,port);
}

void chatclient::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}
