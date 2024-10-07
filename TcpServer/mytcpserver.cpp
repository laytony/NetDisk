#include "mytcpserver.h"

Mytcpserver::Mytcpserver()
{
}

Mytcpserver &Mytcpserver::getInstance()
{
    // TODO: 在此处插入 return 语句
    static Mytcpserver instance;
    return instance;
}

void Mytcpserver::incomingConnection(qintptr socketDescriptor) //用于封装自己的通信socket 一连接上新的socket就执行qintptr绑定
{
    //后面操作的就是与发送端一一对应的socket并且在此处会进行实时改变！
    // if(this->listen(QHostAddress::Any,8888)){
    qDebug() << "new client connected!";
    qDebug() << "mytcpserver thread id:" << GetCurrentThreadId();
    MyTcpSocket *pTcpSocket = new MyTcpSocket();//创建与客户端socket通信的socket
    pTcpSocket->setSocketDescriptor(socketDescriptor);//自动进行连接关系
    QString ip = pTcpSocket->peerAddress().toString();
    qint64 port = pTcpSocket->peerPort();
    QString ipDate = QString("[ip=%1 port=%2] 建立好连接了！！").arg(ip).arg(port);
    qDebug() << ipDate;
    //m_tcpSocketList.push_back(pTcpSocket);
    m_tcpSocketList.insert(pTcpSocket);
    //当前登录 服务端添加其socket通信 若下线 则删除其socket
//        connect(pTcpSocket,SIGNAL(off(MyTcpSocket*)),this,SLOT(deleteSocket(MyTcpSocket*)));//将此连接socket的断开信号关联至删除函数 一掉线自动删除
    //}//创建时进行信号关联 关联此tcpsocket和 下线操作 信号和处理函数的参数必须一致  因此重载一个Offline //自定义信号操作 一发出信号就删除（this mytecpserver进行删除操作）
}





void Mytcpserver::resend(QString prename, PDU *pdu)
{
    if(prename == NULL || pdu == NULL) {
        return;
    }
    for(auto x : m_tcpSocketList) { //x是与prename对应的服务器这边的socket
        if(x->getname() == prename) { //找到被加好友者
            x->write((char *)pdu, pdu->uiPDULen); //由此名字的人转发给客户端list中的tcpsocket与客户端一一对应
            break;
        }
    }
//   delete pdu;//不能删除否则会报错
//   pdu = NULL;
}

void Mytcpserver::deleteSocket(MyTcpSocket *mysocket) //传递要删除的socket
{
    mysocket->remove_download_port(mysocket->getname());//回收下线用户的下载文件端口号
    qDebug() << "try to delete socket:" << mysocket->getname();
    Sleep(10);
    m_tcpSocketList.erase(mysocket);
    Sleep(10);
    mysocket->deleteLater();
    qDebug() << "Online users:";
    for(auto x : m_tcpSocketList) {
        qDebug() << x->getname();
    }
    tcpserver::getinstance().show_usr_state(DBoperate::getInstance().Init());//下线刷新用户状态列表
    //Sleep(1000);
//    auto iter = m_tcpSocketList.begin();
//    while(iter!=m_tcpSocketList.end()){
//        if(*iter == mysocket){
//            (*iter)->deleteLater();//延迟删除
//            //delete(*iter);
//            *iter = NULL;//出错
//            m_tcpSocketList.erase(iter);
//            iter++;
//        }
//        ++iter;
//    }
//    for(int i{0};i!=m_tcpSocketList.size();++i){//打印服务端下线后仍然在线的客户；
//        qDebug()<<m_tcpSocketList[i]->getname();//.at(i);
//    }
}


