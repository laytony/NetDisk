#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QList>
#include "mytcpsocket.h"
class MyTcpSocket;

class Mytcpserver :public QTcpServer{//派生类继承基类,基类中定义了各种类型的网络编程接收端的函数等；
    Q_OBJECT
public:
    Mytcpserver();

    static Mytcpserver& getInstance();
    void incomingConnection(qintptr socketDescriptor);//qt提供的一种虚函数 对其进行重载，再连接上后自动发送提示

    void resend(QString name,PDU*pdu);//申请的人以及PDU

signals:



public slots:
    void deleteSocket(MyTcpSocket* mysocket);//删除下线用户的socket


private:
    //MyTcpSocket
    //QList<MyTcpSocket*>m_tcpSocketList;//定义一组MyTcpSocket链表用于接受客户的信息
    unordered_set<MyTcpSocket*>m_tcpSocketList;//定义与客户端一一对应的tcpsocket
};
#endif // MYTCPSERVER_H

