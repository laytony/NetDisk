#include "mytcpsocket.h"
#include "mytcpserver.h"
#include <string>
#include <QDir>
#include <QFileInfoList>
#include <QDateTime>
#include "downloadf.h"




MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(recvMsg())); //存在多个客户需求，分开处理 处理单个客户
    connect(this, SIGNAL(disconnected()), this, SLOT(clientoffline())); //关联客户端连接 一旦断开连接就执行掉线函数内容
    file_name = "";
    uploadS = false;//置为非上传文件状态
    m_ptimer = new QTimer;
    file_total_size = 0;//总的大小
    file_rev_size = 0;//已经接受的大小
    connect(m_ptimer, SIGNAL(timeout()), this, SLOT(handle_download_file())); //关联设置槽函数定时时间到 运行此函数
    // connect(this,SIGNAL(off(MyTcpSocket*)),&Mytcpserver::getInstance(),SLOT(deleteSocket(MyTcpSocket*)));//将此连接socket的断开信号关联至删除函数 一掉线自动删除
//    setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 4096);
    m_file = new QFile;
    index = 0;
    my_uploadserver = new QTcpServer(this);
    my_uploadserver->listen(QHostAddress::Any, UPLOAD_PORT);
    connect(my_uploadserver, &QTcpServer::newConnection, this, [ = ]() {//上传文件
        QTcpSocket *my_socket =  my_uploadserver->nextPendingConnection();
        qDebug() << "客户端发来上传文件子线程新连接！";
        qint64 cnt{0}, total{0};
        FileOp *upload_th = new FileOp(my_socket, cnt, total);
        upload_th->start();
        connect(upload_th, &FileOp::rev_over, this, [ = ](QString usr_name, QString file_name, qint64 size) {
            qDebug() << "上传文件结束！";
            qDebug() << "请求上传的人为：" << usr_name;
            upload_th->quit();
            upload_th->wait();
            upload_th->deleteLater();
            my_socket->close();
            my_socket->deleteLater();
            if(size > 5 * 1024 * 1024) {
                PDU *respdu = mkPDU_1(1, ENUM_MSG_TYPE_upload_FILE_RESPOND);
                memcpy(respdu->caData, file_name.toStdString().c_str(), 64);
                Mytcpserver::getInstance().resend(usr_name, respdu);
            }
            qDebug() << "用户" << usr_name << "成功上传文件：" << file_name;
            //QMessageBox::information(&tcpserver::getinstance(), "send file", "Client send file over!");
        });
    });
}






void MyTcpSocket::recvMsg() //服务端接收消息
{
    //    uint uiPDULen;//总协议单元的大小
//    uint uiMsgType;//消息的类型 登陆 加好友 上传 下载等等
//    char caData[64];//发送的  存放文件名char caData[64] 用户名，密码；
//    uint uiMsgLen;//实际消息的长度 除了最后一个的长度
//    int caMsg[];//实际消息 总长度为64+12+caMsg的大小
    if(!uploadS) { //非上传文件状态 按照pdu格式接受
        qDebug() << this->bytesAvailable();
        uint uiPDULen{0};
        this->read((char *)&uiPDULen, sizeof(uint)); //PDULen的具体长度也是一个16位无符号！先从PUD中占用一个uint接受PDULEN作为具体的长度
        uint uiMsgLen = uiPDULen - sizeof(PDU);
        PDU *mypdu = mkPDU(uiMsgLen);//创建用于接受消息的单元
        this->read((char *)mypdu + sizeof(uint), mypdu->uiPDULen - sizeof(uint)); //read参数后面跟数据 和数据长度之前已经接收了一个uint的数据 后面开始便宜uint的大小即可
        //qDebug()<<mypdu->uiMsgType<<(char*)mypdu->caMsg;//
        //收到数据判断消息类型
        //每接受一次消息都更新现实在线状态信息 和用户的请求；
        tcpserver::getinstance().show_usr_state(DBoperate::getInstance().Init());
        tcpserver::getinstance().show_usr_request(mypdu);
        switch(mypdu->uiMsgType) {
            case ENUM_MSG_TYPE_REGIST_REQUEST: { //注册
                    handle_ENUM_MSG_TYPE_REGIST_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_LOGIN_REQUEST: { //登陆请求
                    handle_ENUM_MSG_TYPE_LOGIN_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_LOGOFF_REQUEST: { //退出登录请求
                    handle_ENUM_MSG_TYPE_LOGOFF_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_DESTORY_REQUEST: { //注销请求
                    handle_ENUM_MSG_TYPE_DESTORY_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST: { //查询所有在线用户请求
                    handle_ENUM_MSG_TYPE_ALL_ONLINE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SEARCH_USR_REQUEST: { //搜索用户请求
                    handle_ENUM_MSG_TYPE_SEARCH_USR_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_ADD_USR_REQUEST: { //添加好友请求
                    handle_ENUM_MSG_TYPE_ADD_USR_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_NO_ADD_USR_REQUEST: { //拒绝添加好友请求 前32位是申请者 后32位是回复者
                    handle_ENUM_MSG_TYPE_NO_ADD_USR_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_YES_ADD_USR_REQUEST: { //同意添加好友请求 前32位是申请者 后32位是回复者
                    handle_ENUM_MSG_TYPE_YES_ADD_USR_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_FLUSH_FRI_REQUSET: { //刷新好友请求
                    handle_ENUM_MSG_TYPE_FLUSH_FRI_REQUSET(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_DELETE_FRI_REQUSET: { //删除好友请求 请求删除者确认
                    handle_ENUM_MSG_TYPE_DELETE_FRI_REQUSET(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SDELETE_FRI_REQUSET: { //确认删除 并给予回复
                    handle_ENUM_MSG_TYPE_SDELETE_FRI_REQUSET(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST: { //群聊操作
                    handle_ENUM_MSG_TYPE_GROUP_CHAT_REQUESTT(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST: { //私聊请求
                    handle_ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_CREATE_DIR_REQUEST: { //创建文件夹请求
                    handle_ENUM_MSG_TYPE_CREATE_DIR_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST: { //刷新当前目录下的目录文件的请求
                    handle_ENUM_MSG_TYPE_FLUSH_FILE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_DELETE_FILE_REQUEST: { //删除文件（夹）请求
                    handle_ENUM_MSG_TYPE_DELETE_FILE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST: { //用户确定删除文件夹请求 caData :name filename; caMsg rootfile
                    handle_ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_RENAME_DIR_REQUEST: { //处理重命名文件（夹）请求
                    handle_ENUM_MSG_TYPE_RENAME_DIR_REQUEST(mypdu);
                    break;//重命名文件请求
                }
            case ENUM_MSG_TYPE_upload_FILE_REQUEST: { //上传文件请
                    handle_ENUM_MSG_TYPE_upload_FILE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_download_FILE_REQUEST: { //下载文件请
                    handle_ENUM_MSG_TYPE_download_FILE_REQUEST(mypdu);//上传文件(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST: { //查看文件信息请求
                    handle_ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SHARE_FILE_REQUEST: { //分享文件请求
                    handle_ENUM_MSG_TYPE_SHARE_FILE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SHARE_FILE_RESPOND: { //确认分享进行拷贝
                    handle_ENUM_MSG_TYPE_SHARE_FILE_RESPOND(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SHARE_FILE_RE_RESPOND: { //拒绝分享回复
                    char share_person[32];
                    memcpy(share_person, mypdu->caData + 32, 32); //
                    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_SHARE_FILE_RE_RESPOND);
                    memcpy(respdu->caData, mypdu->caData, 32);
                    Mytcpserver::getInstance().resend(share_person, respdu);
                    break;
                }
            case ENUM_MSG_TYPE_MOVE_FILE_REQUEST: { //移动文件请求
                    handle_ENUM_MSG_TYPE_MOVE_FILE_REQUEST(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_RELOGIN_REQUEST: { //重新连接
                    char caName[32] = {'\0'};
                    char caPwd[32] = {'\0'};
                    strncpy(caName, mypdu->caData, 32); //存储mytcpsocket的名字 密码
                    strncpy(caPwd, mypdu->caData + 32, 32);
                    this->m_strName = caName;//登陆时候在服务端次 socket记录此次登录的用户名
                    DBoperate::getInstance().handle_login(caName, caPwd);
                    tcpserver::getinstance().show_usr_state(DBoperate::getInstance().Init());
                    break;
                }
            case ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE: {
                    handle_ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_WZQ: {//向对方转发五子棋申请
                    Mytcpserver::getInstance().resend(mypdu->caData + 32, mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_WZQ_A: {//是否同意开始五子棋 直接转发
                    Mytcpserver::getInstance().resend(mypdu->caData + 32, mypdu);
                    break;
                }
            default:
                break;
        }
        delete mypdu;
        mypdu = NULL;
    } else { //按照上传文件内容的方式进行 接受上传文件
        handle_rev_file();
    }
}

void MyTcpSocket::clientoffline()
{
    DBoperate::getInstance().handle_offline(m_strName.toStdString().c_str());//转换为char*类型
    emit off(this);//删除此次掉线信号 使用完毕 emit signalName(parameters) 发出信号进行;关联后面的tcpserver进行删除操作
}

QString MyTcpSocket::getname()
{
    return this->m_strName;
}


///接收上传的文件
void MyTcpSocket::handle_rev_file()
{
//    QByteArray buff = readAll();
//    file_rev_size+=buff.size();
////    qint64 bytestowrite = this->bytesToWrite();
////    if(bytestowrite > 30000){
////        m_file.flush();
////    }
////    qDebug()<<"单次传输大小："<<buff.size();
//    m_file.write(buff);
//     PDU*respdu = mkPDU_1(1,ENUM_MSG_TYPE_upload_FILE_RESPOND);
//     memcpy(respdu->caData,file_name.toStdString().c_str(),32);
////     qDebug()<<"文件大小："<<file_total_size;
////     qDebug()<<"已经接收的大小"<<file_rev_size;
//    if(file_total_size==file_rev_size){//文件上传完毕 回复
//        m_file.close();
//        uploadS = false;
//        write((char*)respdu,respdu->uiPDULen);
//    }else if(file_total_size < file_rev_size){//接受失败 总的数据已经小于当前的数据了！
//        m_file.close();
//        uploadS = false;
//        respdu->uiMsgLen = 0;
//        write((char*)respdu,respdu->uiPDULen);
//    }
//    delete respdu;
//    respdu = NULL;
}

///处理接受子函数
void MyTcpSocket::handle_ENUM_MSG_TYPE_REGIST_REQUEST(PDU *mypdu)//注册
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    strncpy(caName, mypdu->caData, 32);
    strncpy(caPwd, mypdu->caData + 32, 32);
    //qDebug()<<caName<<caPwd<<mypdu->uiMsgType;
    bool ret = DBoperate::getInstance().handle_register(caName, caPwd);
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_REGIST_RESPOND);
    if(ret) {
        strcpy(respdu->caData, REGISTER_OK);
        //注册成功创建用户文件夹
        QDir dir;
        bool mkdir_ret = dir.mkdir(tcpserver::getinstance().get_dir() + QString("%1").arg(caName));
        if(!mkdir_ret) {
            qDebug() << "try to creat file folder but file name exist!";
        } else {
            qDebug() << "Successfully create file folder!";
        }
    } else {
        strcpy(respdu->caData, REGISTER_FAILED);
    }
    write((char *)respdu, respdu->uiPDULen); //向客户端发送
    delete respdu;
    respdu = NULL;
}

//void MyTcpSocket::handle_ENUM_MSG_TYPE_REGIST_REQUEST(PDU *mypdu)
//{
//    char caName[32] = {'\0'};
//    char caPwd[32] = {'\0'};
//    strncpy(caName,mypdu->caData,32);//存储mytcpsocket的名字 密码
//    strncpy(caPwd,mypdu->caData + 32,32);
//    //qDebug()<<caName<<caPwd<<mypdu->uiMsgType;
//     PDU* respdu = mkPDU_1(0,ENUM_MSG_TYPE_REGIST_RESPOND);
//     bool ret = DBoperate::getInstance().handle_register(caName,caPwd);
//     if(ret){
//         strcpy(respdu->caData,REGISTER_OK);
//         this->m_strName = caName;//登陆时候在服务端次 socket记录此次登录的用户名；
//     }
//     else{
//         strcpy(respdu->caData,REGISTER_FAILED);
//     }
//     write((char*)respdu,respdu->uiPDULen);
//    delete respdu;
//    respdu = NULL;
//}

void MyTcpSocket::handle_ENUM_MSG_TYPE_LOGIN_REQUEST(PDU *mypdu)
{
    //获取当前myqtcsocket的线程id
    qDebug() << "登录:" << GetCurrentThreadId() << endl;
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    strncpy(caName, mypdu->caData, 32); //存储mytcpsocket的名字 密码
    strncpy(caPwd, mypdu->caData + 32, 32);
    //qDebug()<<caName<<caPwd<<mypdu->uiMsgType;
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_LOGIN_RESPOND);
    if(m_strName != "") {
        strcpy(respdu->caData, LOGIN_OK);
        write((char *)respdu, respdu->uiPDULen);
    } else {
        bool ret = DBoperate::getInstance().handle_login(caName, caPwd);
        if(ret) {
            strcpy(respdu->caData, LOGIN_OK);
            this->m_strName = caName;//登陆时候在服务端次 socket记录此次登录的用户名；
            QDir dir;//在此尝试创建用户文件夹
            bool mkdir_ret = dir.mkdir(tcpserver::getinstance().get_dir() + QString("%1").arg(caName));
//            if(!mkdir_ret) {
//                qDebug() << "try to creat file folder but file name exist!";
//            } else {
//                qDebug() << "Successfully create file folder!";
//            }
        } else {
            strcpy(respdu->caData, LOGIN_FAILED);
        }
        write((char *)respdu, respdu->uiPDULen);
    }
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_LOGOFF_REQUEST(PDU *mypdu)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    strncpy(caName, mypdu->caData, 32);
    strncpy(caPwd, mypdu->caData + 32, 32);
    //qDebug()<<caName<<caPwd<<mypdu->uiMsgType;
    bool ret = DBoperate::getInstance().handle_logoff(caName, caPwd);
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_LOGOFF_RESPOND); //产生回复通信协议单元
    if(ret) {
        strcpy(respdu->caData, LOGOFF_OK);
    } else {
        strcpy(respdu->caData, LOGOFF_FAILED);
    }
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_DESTORY_REQUEST(PDU *mypdu)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    strncpy(caName, mypdu->caData, 32);
    strncpy(caPwd, mypdu->caData + 32, 32);
    //qDebug()<<caName<<caPwd<<mypdu->uiMsgType;
    bool ret = DBoperate::getInstance().handle_destory(caName, caPwd);
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_DESTORY_RESPOND); //产生回复通信协议单元
    if(ret) {
        strcpy(respdu->caData, DESTORY_OK);
    } else {
        strcpy(respdu->caData, DESTORY_FAILED);
    }
    write((char *)respdu, respdu->uiPDULen);
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_ALL_ONLINE_REQUEST(PDU *mypdu)
{
    qDebug() << "查询所有在线用户线程id:" << GetCurrentThreadId() << endl;
    QStringList retlist = DBoperate::getInstance().handle_all_online();//retlist中获取的每个名字占用32个字节
    uint uiMsglen = (uint)retlist.size() * 32;
    PDU *respdu = mkPDU_1(uiMsglen, ENUM_MSG_TYPE_ALL_ONLINE_RESPOND); //产生回复通信协议单元
    //将获取到的在线用户的用户名循环拷贝到PDU中
    for(int i{0}; i < retlist.size(); ++i) { // caMsg是int类型 先转换为char*类型计算大小 （首地址、内容、大小）
        memcpy((char *)respdu->caMsg + 32 * i, retlist.at(i).toStdString().c_str(), 32); //按32位填写内容每隔32位填写一次
    }
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_SEARCH_USR_REQUEST(PDU *mypdu)
{
    int ret = DBoperate::getInstance().handle_search_usr(mypdu->caData);//retlist中获取的每个名字占用32个字节
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_SEARCH_USR_RESPOND);
    qDebug() << "ret" << ret;
    if(ret == -1) {
        memcpy(respdu->caData, "No such person!", 32);
    } else if(ret == 1) {
        QString t = mypdu->caData;
        t += " is online.";
        memcpy(respdu->caData, t.toStdString().c_str(), 32);
    } else if(ret == 0) {
        QString t = mypdu->caData;
        t += " is not online.";
        memcpy(respdu->caData, t.toStdString().c_str(), 32);
    }
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_ADD_USR_REQUEST(PDU *mypdu)
{
    char name_0[32], name_1[32];
    memcpy(name_0, mypdu->caData, 32);
    memcpy(name_1, mypdu->caData + 32, 32);
    qDebug() << "usr:" << name_0 << "usr_re" << name_1;
    int ret = DBoperate::getInstance().handle_search_usr(name_1);//先进行搜索！
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_ADD_USR_RESPOND);
    if(ret == -1) {
        memcpy(respdu->caData, SYSTEM_BREAKDOWN, 32);
        write((char *)respdu, respdu->uiPDULen);;
    } else if(ret == 0) { //当前搜索用户不在线
        memcpy(respdu->caData, ADD_NOTONLIE, 32);
        write((char *)respdu, respdu->uiPDULen);
    } else if(ret == 1) { //当前用户存在且在线
        ret = DBoperate::getInstance().handle_add_usr(name_0, name_1);
        if(ret == -1) { //系统故障
        } else if(ret == 0) { //对方已经是好友！
            memcpy(respdu->caData, ALREADY_FRIENDS, 32);
            write((char *)respdu, respdu->uiPDULen);
        } else if(ret == 1) { //进行添加好友操作
            Mytcpserver::getInstance().resend(name_1, mypdu); //直接转发mypdu消息类型为加好友请求
        }
    };
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_NO_ADD_USR_REQUEST(PDU *mypdu)
{
    char name_0[32], name_1[32];
    memcpy(name_0, mypdu->caData, 32);
    memcpy(name_1, mypdu->caData + 32, 32);
    mypdu->uiMsgType = ENUM_MSG_TYPE_NO_ADD_USR_RESPOND;
    Mytcpserver::getInstance().resend(name_0, mypdu); //直接转发mypdu 给申请者 消息类型为拒绝加好友请求
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_YES_ADD_USR_REQUEST(PDU *mypdu)
{
    char name_0[32], name_1[32];
    memcpy(name_0, mypdu->caData, 32); //请求者
    memcpy(name_1, mypdu->caData + 32, 32); //同意者
    //修改数据库中内容并进行回复
    DBoperate::getInstance().handle_yes_add_fri(name_0, name_1);
    QStringList list = DBoperate::getInstance().handle_flush_fri(name_0);//对name_0进行刷新好友操作
    uint uisize = list.size();
    PDU *respdu = mkPDU_1(uisize * 32, ENUM_MSG_TYPE_YES_ADD_USR_RESPOND);
    for(uint i{0}; i < uisize; ++i) {
        qDebug() << list[i];
        memcpy((char *)respdu->caMsg + 32 * i, list.at(i).toStdString().c_str(), 32);
    }
    Mytcpserver::getInstance().resend(name_0, respdu);
    respdu = mkPDU_1(uisize * 32, ENUM_MSG_TYPE_FLUSH_FRI_RESPOND);
    list = DBoperate::getInstance().handle_flush_fri(name_1);//对name_1进行刷新好友操作
    uisize = list.size();
    for(uint i{0}; i < uisize; ++i) {
        qDebug() << list[i];
        memcpy((char *)respdu->caMsg + 32 * i, list.at(i).toStdString().c_str(), 32);
    }
    Mytcpserver::getInstance().resend(name_1, respdu);
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_FLUSH_FRI_REQUSET(PDU *mypdu)
{
    char name_0[32];
    memcpy(name_0, mypdu->caData, 32); //请求者的名字
    QStringList list = DBoperate::getInstance().handle_flush_fri(name_0);
    uint uisize = list.size();
    PDU *respdu  = new PDU();
    respdu = mkPDU_1(uisize * 32, ENUM_MSG_TYPE_FLUSH_FRI_RESPOND);
    for(uint i{0}; i < uisize; ++i) {
        qDebug() << list[i];
        memcpy((char *)respdu->caMsg + 32 * i, list.at(i).toStdString().c_str(), 32);
    }
    this->write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_DELETE_FRI_REQUSET(PDU *mypdu)
{
    char name_0[32];
    memcpy(name_0, mypdu->caData, 32); //请求者的名字
    mypdu->uiMsgType = ENUM_MSG_TYPE_SDELETE_FRI_REQUSET;
    write((char *)mypdu, mypdu->uiPDULen);
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_SDELETE_FRI_REQUSET(PDU *mypdu)
{
    char name_0[32], name_1[32];
    memcpy(name_0, mypdu->caData, 32);
    memcpy(name_1, mypdu->caData + 32, 32);
    DBoperate::getInstance().handle_delete_fri(name_0, name_1);
    QStringList list = DBoperate::getInstance().handle_flush_fri(name_0);//对name_0进行刷新好友操作
    uint uisize = list.size();
    PDU *respdu = mkPDU_1(uisize * 32, ENUM_MSG_TYPE_SDELETE_FRI_RESPOND);
    for(uint i{0}; i < uisize; ++i) {
        qDebug() << list[i];
        memcpy((char *)respdu->caMsg + 32 * i, list.at(i).toStdString().c_str(), 32);
    }
    qDebug() << "更新请求者好友列表！！！";
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_GROUP_CHAT_REQUESTT(PDU *mypdu)
{
    char name_0[32];
    memcpy(name_0, mypdu->caData + 32, 32);
    QStringList retlist = DBoperate::getInstance().handle_all_online();//retlist中获取的每个名字占用32个字节 获取在线用户
    mypdu->uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_RESPOND;
    for(auto name : retlist) {
        if(name == name_0) {
            continue;
        }
        Mytcpserver::getInstance().resend(name, mypdu);
        qDebug() << "转发消息！";
    }
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST(PDU *mypdu)
{
    char name_0[32];//代转发人的名字
    memcpy(name_0, mypdu->caData + 32, 32);
//          MSG_tofri[name_0].push(mypdu);
//          QStringList retlist = DBoperate::getInstance().handle_all_online();//retlist中获取的每个名字占用32个字节 获取在线用户
//          for(auto name:retlist){
//             if(name == name_0){//该用户在线了！
//                 while(!MSG_tofri[name].empty()){//将积累的发给该用户的消息全部发出去
//                     PDU*respdu = MSG_tofri[name].front();
//                     MSG_tofri[name].pop();
//                     Mytcpserver::getInstance().resend(name,respdu);
//                     Sleep(10);
//                     qDebug()<<"积累的消息"<<(char*)respdu->caMsg;
//                 }
//                 qDebug()<<name<<"转发消息！";
//                 break;
//             }
//          }
    QStringList retlist = DBoperate::getInstance().handle_all_online();//retlist中获取的每个名字占用32个字节 获取在线用户
    for(auto name : retlist) {
        if(name == name_0) {
            Mytcpserver::getInstance().resend(name, mypdu); //A发消息(发送给B)给到服务器（服务器与A通信的A'收到了）
            //服务器找到B'（与B进行通信）并把通过B'把消息发送给B;
            qDebug() << name << "转发消息！";
            break;
        }
    }
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_CREATE_DIR_REQUEST(PDU *mypdu)
{
    char name[32];//请求者的名字
    char dirname[32];//新路径名字
    memcpy(name, mypdu->caData, 32);
    memcpy(dirname, mypdu->caData + 32, 32);
    QString new_root = QString((char *)mypdu->caMsg) + QString(dirname) + "/"; //新路径名字
    qDebug() << "待创建路径的名字：" << new_root;
    QDir dir;
    if(dir.exists(QString((char * )mypdu->caMsg))) { //判断旧的路径是否存在 存在
        if(dir.exists(new_root)) { //存在同名文件夹 无法创建
            mypdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_FAILED_RESPOND;
            Mytcpserver::getInstance().resend(name, mypdu); //返回创建失败
            qDebug() << "try to creat file folder but file name exist!";
        } else { //进行创建新路径
            bool mkdir_ret = dir.mkdir(new_root);
            if(!mkdir_ret) {
                qDebug() << "try to creat file folder but file name exist!";
            } else {
                qDebug() << "Successfully create file folder!";
                mypdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_SUCCESS_RESPOND;
                Mytcpserver::getInstance().resend(name, mypdu); //返回创建成功
            }
        }
    } else { //跟路径不存在 创建根路径
        bool mkdir_ret = dir.mkdir(QString((char *)mypdu->caMsg)); //创建根路径
        if(!mkdir_ret) {
            qDebug() << "try to creat root folder but file name exist!";
        } else {
            qDebug() << "Successfully create root folder!";
        }
        mypdu->uiMsgType = ENUM_MSG_TYPE_CREATE_ROOT_DIR_RESPOND;
        Mytcpserver::getInstance().resend(name, mypdu); //返回创建成功
    }
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_FLUSH_FILE_REQUEST(PDU *mypdu)//刷新文件目录
{
    char name[32];//请求者的名字
    memcpy(name, mypdu->caData, 32);
//    qDebug()<<"name"<<name;
//    qDebug()<<"o_name"<<mypdu->caData+32;
//    qDebug()<<"Msg"<<(char*)mypdu->caMsg;
    QString view_root = (char *)mypdu->caMsg;
    qDebug() << "待刷新的路径：" << view_root;
    QDir dir(view_root);//存放待操作的路径
    QFileInfoList fileinfolist = dir.entryInfoList();
    int file_cnts = fileinfolist.size() - 2; //去除前面的当前点和上级目录点
    PDU *respdu = mkPDU_1(file_cnts * sizeof(FileInfo), ENUM_MSG_TYPE_FLUSH_FILE_RESPOND); //存放文件
    QFileInfo qfinfo(view_root);
    if(qfinfo.isDir()) { //是文件夹打开
        if(file_cnts != 0) { //该目录下不为空
            FileInfo *fileinfo;
            for(int i{0}; i < file_cnts; ++i) { //i为偏移量
//                qDebug() << fileinfolist[i + 2].fileName() << fileinfolist[i + 2].size()
//                         << "常规文件：" << fileinfolist[i + 2].isFile()
//                         << "文件夹:" << fileinfolist[i + 2].isDir();
                fileinfo = mkFileInfo(QString(fileinfolist[i + 2].fileName()), fileinfolist[i + 2].isFile()); //前32位存放文件名
                //qDebug() << "文件后缀：" << fileinfolist[i + 2].suffix();
                memcpy(fileinfo->file_type, fileinfolist[i + 2].suffix().toStdString().c_str(), 16); //获取后缀名
                memcpy((char *)respdu->caMsg + sizeof(FileInfo)*i, fileinfo, sizeof(FileInfo));
            }
            write((char *)respdu, respdu->uiPDULen);
            delete fileinfo;
            fileinfo = NULL;
        } else { //该目录下为空
            write((char *)respdu, respdu->uiPDULen); //为空直接发送
        }
    }
    delete respdu;
    respdu = NULL;
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_DELETE_FILE_REQUEST(PDU *mypdu)
{
    mypdu->uiMsgType = ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST;
    char name[32];//请求者的名字
    memcpy(name, mypdu->caData, 32);
    qDebug() << name << "想删除文件（夹）";
    Mytcpserver::getInstance().resend(name, mypdu);
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST(PDU *mypdu)
{
    QString del_path =  (char *)mypdu->caMsg;
    del_path += mypdu->caData + 32;
    qDebug() << "要删除的文件的根目录" << del_path;
    QFileInfo qfinfo(del_path);
    bool ret{false};
    if(qfinfo.isDir()) { //是否是文件夹
        QDir qdir;//存放根路径
        qdir.setPath(del_path);
        if(qdir.removeRecursively()) { //删除文件
            ret = true;
        }
    } else { //常规文件
        QFile qfile(del_path);
        if(qfile.remove()) {
            ret = true;
        }
    }
    if(ret) { //成功删除文件
        qDebug() << "成功删除该文件夹";
        PDU *pdu = mkPDU_2(1, ENUM_MSG_TYPE_DELETE_FILE_RESPOND, "", mypdu->caData + 32, "");
        write((char *)pdu, pdu->uiPDULen);
        delete pdu;
        pdu = NULL;
    } else { //删除文件失败
        qDebug() << "需要管理员权限";
        PDU *pdu = mkPDU_2(0, ENUM_MSG_TYPE_DELETE_FILE_RESPOND, "", mypdu->caData + 32, "");
        write((char *)pdu, pdu->uiPDULen);
        delete pdu;
        pdu = NULL;
    }
}

void MyTcpSocket::handle_ENUM_MSG_TYPE_RENAME_DIR_REQUEST(PDU *mypdu)
{
    QString cur_path = (char *)mypdu->caMsg;
    QString newfilename = mypdu->caData + 32;
    char filename[32];
    memcpy(filename, mypdu->caData, 32);
    QString old_path = cur_path + filename;
    QString new_path = cur_path + newfilename;
    QDir dir;
    QFileInfo finfo(old_path);
    bool ret{false};
    if(dir.rename(old_path, new_path)) {
        ret = true;
    }
    if(ret) { //
        qDebug() << "成功重命名该文件夹";
        PDU *pdu = mkPDU_2(1, ENUM_MSG_TYPE_RENAME_DIR_RESPOND, filename, newfilename, "");
        write((char *)pdu, pdu->uiPDULen);
        delete pdu;
        pdu = NULL;
    } else { //重命名文件失败
        qDebug() << "需要管理员权限";
        PDU *pdu = mkPDU_2(0, ENUM_MSG_TYPE_RENAME_DIR_RESPOND, filename, newfilename, "");
        write((char *)pdu, pdu->uiPDULen);
        delete pdu;
        pdu = NULL;
    }
}

//原单线程处理函数
//void MyTcpSocket::handle_ENUM_MSG_TYPE_upload_FILE_REQUEST(PDU *mypdu)
//{
//    char filename[32];
//    memcpy(filename,mypdu->caData,32);
//    file_name = filename;
//    QString size = mypdu->caData+32;
//    qint64 filesize = size.toULongLong();//转换为长整型
//    QString cur_file_path = (char*)mypdu->caMsg;
//    qDebug()<<"接收文件的信息："<<filename;
//    qDebug()<<filesize;
//    qDebug()<<cur_file_path;
//    m_file.setFileName(cur_file_path+filename);//要创建的路径
//    if(m_file.open(QIODevice::WriteOnly)){//置为只写状态如果不存在文件将会创建
//        uploadS = true;
//        file_total_size = filesize;
//        file_rev_size = 0;
//    }

//}


///多线程处理函数
void MyTcpSocket::handle_ENUM_MSG_TYPE_upload_FILE_REQUEST(PDU *mypdu) //移动至类初始化处 进行server初始化 发送数据
{
//    qDebug() << 11111111111;
//    char filename[32];
//    memcpy(filename, mypdu->caData, 32);
//    file_name = filename;
//    QString size = mypdu->caData + 32;
//    qint64 filesize = size.toULongLong();//转换为长整型
//    QString cur_file_path = (char *)mypdu->caMsg;
//    qDebug() << "接收文件的信息：" << filename;
//    qDebug() << filesize;
//    qDebug() << cur_file_path;
//    m_file->setFileName(cur_file_path + filename); //要创建的路径
//    if(m_file->open(QIODevice::WriteOnly)) { //置为只写状态如果不存在文件将会创建
//        file_total_size = filesize;
//        file_rev_size = 0;
//        qDebug() << "文件已经打开！";
//    }
    //QTcpSocket* m_s,QFile file, QString file_size,
}


///原下载文件单线程函数
//void MyTcpSocket::handle_ENUM_MSG_TYPE_download_FILE_REQUEST(PDU *mypdu)
//{
//    //将文件大小发送给客户端
//    file_path = (char *)mypdu->caMsg;
//    QFile file(file_path);
//    QString file_size = QString::number(file.size());//文件大小
//    PDU *respdu = mkPDU_2(0, ENUM_MSG_TYPE_download_FILE_REQUEST, file_size, mypdu->caData, "");
//    write((char *)respdu, respdu->uiPDULen);
//    delete respdu;
//    respdu = NULL;
//    qDebug() << "待发送给客户的文件信息" << file_path;
//    qDebug() << "文件大小：" << file_size;
//    //m_ptimer->start(1000);
////    Sleep(1000);/**/
//    //    handle_download_file();
//}

void MyTcpSocket::handle_ENUM_MSG_TYPE_download_FILE_REQUEST(PDU *mypdu)
{
    //将文件大小发送给客户端
    char re_name[32];
    char down_port[32];
    memcpy(down_port, mypdu->caData, 32);
    memcpy(re_name, mypdu->caData + 32, 32);
    qDebug() << "down_port:" << down_port;
    for(auto x : m_download_ports) {
        if(x.first != re_name && x.second == QString(down_port).toInt()) {
            PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_REDOWNLOAD);
            write((char *)respdu, respdu->uiPDULen);
            delete respdu;
            respdu = NULL;
            return;
        }
    }
    m_download_ports[re_name] = (QString(down_port).toInt());//保存已经使用的端口号
    file_path = (char *)mypdu->caMsg;
    QFile file(file_path);
    QString file_size = QString::number(file.size());//文件大小
    int findex = file_path.lastIndexOf('/');
    QString f_name = file_path.right(file_path.size() - findex - 1);
    PDU *respdu = mkPDU_2(0, ENUM_MSG_TYPE_download_FILE_REQUEST, file_size, f_name, "");
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
    qDebug() << "待发送给客户的文件路径" << file_path;
    qDebug() << "文件大小：" << file_size;
    ///开始多线程
    DownLoadF *downloadf = new DownLoadF;
    QThread *down_th = new QThread;
    downloadf->moveToThread(down_th);
    down_th->start();
    connect(this, &MyTcpSocket::start_connected, downloadf, &DownLoadF::connect_to_server);
    connect(this, &MyTcpSocket::down_signal, downloadf, &DownLoadF::download_file);
    connect(downloadf, &DownLoadF::connectedok, this, [ = ]() {
        qDebug() << "下载socket成功连接客户端";
    });
    connect(downloadf, &DownLoadF::end_thread, this, [ = ](QString str) {
        down_th->quit();
        down_th->wait();
        down_th->deleteLater();
        downloadf->deleteLater();
        //  m_download_ports.erase( re_name);//释放回收端口号
        qDebug() << "客户端成功下载文件：" << str;
    }) ;
    char cli_ip[32];
    char f_size[32];
    memcpy(cli_ip, mypdu->ELSE, 32);
    memcpy(f_size, mypdu->ELSE + 32, 32);
    emit start_connected(cli_ip, QString(down_port).toInt());
    qDebug() << "客户端的ip是：" << cli_ip;
    qDebug() << "图像大小为：" << QString(f_size).toInt();
    Sleep(100);
//    emit down_signal(file.size(), file_path);
    if(QString( mypdu->ELSE).toInt() > 100) {//发送的是图片
        emit down_signal(QString(f_size).toInt(), file_path);
    } else {
        emit down_signal(file.size(), file_path);
    }
//    m_ptimer->startW(1000);
}
///原显示文件信息按钮
void MyTcpSocket::handle_ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST(PDU *mypdu)    //显示文件信息请求
{
    QFileInfo fileinfo((char *)mypdu->caMsg);
    QString fpath = (char *)mypdu->caMsg;
    qint64 f_size = fileinfo.size();
    if(fileinfo.isFile()) { //是文件求文件大小
        f_size = fileinfo.size();
    } else { //是文件夹求取文件夹内文件大小
        f_size = dirFileSize(fpath);
    }
    QString file_size = get_file_size(f_size);
    QString create_time = fileinfo.birthTime().toString("yyyy-MM-dd hh:mm:ss");//转换成日期格式
    QString last_visited = fileinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss");
    QString last_modify = fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    bool is_file = fileinfo.isFile();
    bool is_dir = fileinfo.isDir();
    bool isReadable = fileinfo.isReadable();	//判断QFileInfo对象的读属性
    bool isWritable = fileinfo.isWritable();	//判断QFileInfo对象的写属性
    bool isHidden = fileinfo.isHidden();		//判断QFileInfo对象的隐藏属性
    bool isExecutable = fileinfo.isExecutable();//判断QFileInfo对象的可执行属性
    qDebug() << file_size << create_time << last_visited << last_modify << is_file
             << is_dir << isReadable << isWritable << isHidden << isExecutable;
//    FileInfo* rfileinfo = mkFileInfo_1(rfile_name,is_file,create_time,last_visited,is_file,is_dir,
//                             isReadable,isWritable,isHidden,isExecutable);
    FileInfo *rfiinfo = mkFileInfo_1(mypdu->caData, file_size, create_time, last_visited, last_modify, is_file, is_dir,
                                     isReadable, isWritable, isHidden, isExecutable);
    // qDebug() << "发送的信息" << rfiinfo->caSize << rfiinfo->createTime << rfiinfo->lastmodify << rfiinfo->lastVisitedtime;
    PDU *respdu = mkPDU_1(sizeof(FileInfo), ENUM_MSG_TYPE_SHOW_FD_MSG_RESPOND);
    memcpy((char *)respdu->caMsg, rfiinfo, sizeof(FileInfo));
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}
void MyTcpSocket::handle_ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE(PDU *mypdu)    //显示文件信息请求
{
    QFileInfo fileinfo((char *)mypdu->caMsg);
    QString fpath = (char *)mypdu->caMsg;
    qint64 f_size = fileinfo.size();
    PDU *respdu = mkPDU_2(0, ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE, QString::number(f_size), "", "");
    write((char *)respdu, respdu->uiPDULen);
    delete respdu;
    respdu = NULL;
}
void MyTcpSocket::handle_ENUM_MSG_TYPE_SHARE_FILE_REQUEST(PDU *mypdu)
{
//    char name_0[32];
    char nums[32];
//    memcpy(name_0,mypdu->caData,32);
    memcpy(nums, mypdu->caData + 32, 32);
    int cnts = QString(nums).toInt();
    char filename[32];
    QString file_path = (char *)mypdu->caMsg + (cnts + 1) * 32;
    qDebug() << "要分享的文件路径" << file_path;
    memcpy(filename, (char *)mypdu->caMsg + cnts * 32, 32);
    qDebug() << "要分享的文件名" << filename;
    char share_fri[32];
    for(int i{0}; i < cnts; ++i) {
        memcpy(share_fri, (char *)mypdu->caMsg + 32 * i, 32);
        qDebug() << "要分享的人名字" << share_fri;
        Mytcpserver::getInstance().resend(share_fri, mypdu); //转发给待分享的好友；
    }
}
void MyTcpSocket::handle_ENUM_MSG_TYPE_SHARE_FILE_RESPOND(PDU *mypdu)
{
    char from_file_path[300];
    char to_file_path[100];
    char agree_per_curp[100];
    char share_person[32];
    memcpy(share_person, mypdu->caData + 32, 32);
    PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_SHARE_FILE_RE_RESPOND);
    memcpy(respdu->caData, mypdu->caData, 32);
    Mytcpserver::getInstance().resend(share_person, respdu);
//    delete respdu;//回复分享者
//    respdu = NULL;
    Sleep(1000);
    memcpy(from_file_path, (char *)mypdu->caMsg + 100, 200); //前100是curpath 后200是filepath
    memcpy(to_file_path, (char *)mypdu->caMsg, 100);
    memcpy(agree_per_curp, (char *)mypdu->caMsg + 300, 100);
    qDebug() << "111分享起始文件夹：" << from_file_path;
    qDebug() << "111分享结束文件夹：" << to_file_path;
    ///多线程分享文件
    ShareFile *my_share = new ShareFile(from_file_path, to_file_path);
    connect(my_share, &ShareFile::end_share, this, [ & ]() {
        my_share->quit();
        my_share->wait();
        my_share->deleteLater();
        respdu = mkPDU_1(100, ENUM_MSG_TYPE_FLUSH_FRI_REQUSET);
        memcpy(respdu->caData, mypdu->caData, 32);
        memcpy((char *)respdu->caMsg, agree_per_curp, 100);
        handle_ENUM_MSG_TYPE_FLUSH_FILE_REQUEST(respdu);
        delete respdu;//回复分享者
        respdu = NULL;
    });
    my_share->start();
    ///原单线程函数
//    QFileInfo from_file(from_file_path);
//    QFileInfo to_file(to_file_path);
//    if(from_file.isFile()) { //常规文件的拷贝
//        QFile::copy(from_file_path, to_file_path);
//    } else if(from_file.isDir()) {
//        Copy_dir(from_file_path, to_file_path);
//    }
//    respdu = mkPDU_1(100, ENUM_MSG_TYPE_FLUSH_FRI_REQUSET);
//    memcpy(respdu->caData, mypdu->caData, 32);
//    memcpy((char *)respdu->caMsg, agree_per_curp, 100);
//    handle_ENUM_MSG_TYPE_FLUSH_FILE_REQUEST(respdu);
//    delete respdu;//回复分享者
//    respdu = NULL;
}
void MyTcpSocket::handle_ENUM_MSG_TYPE_MOVE_FILE_REQUEST(PDU *mypdu)    //移动文件夹请求
{
    char move_file[32];
//   char move_file_path_size[32];
    memcpy(move_file, mypdu->caData, 32);
//   memcpy(move_file_path_size,mypdu->caData+32,32);
//   int move_file_path_s = QString(move_file_path_size).toInt();
    char move_file_path[200];
    memcpy(move_file_path, (char *)mypdu->caMsg, 200);
    QString dest_file_path = (char *)mypdu->caMsg + 200;
    qDebug() << move_file_path << "  " << dest_file_path;
    QFileInfo to_file(dest_file_path);
    if(to_file.isFile()) {
        write((char *)mypdu, mypdu->uiPDULen);
        return;
    }
    MoveFile_ *my_move = new MoveFile_(move_file, move_file_path, dest_file_path);
    connect(my_move, &MoveFile_::end_move, this, [ = ](int ret) {
        my_move->quit();
        my_move->wait();
        my_move->deleteLater();
        PDU *respdu = mkPDU_2(ret, ENUM_MSG_TYPE_MOVE_FILE_RESPOND, move_file, "", "");
        write((char *)respdu, respdu->uiPDULen);
        delete respdu;
        respdu = NULL;
    });
    my_move->start();
//    QFileInfo to_file(dest_file_path);
//    if(to_file.isFile()) {
//        write((char *)mypdu, mypdu->uiPDULen);
//        return;
//    }
//    dest_file_path += "/" + QString(move_file);
//    QFileInfo from_file(move_file_path);
//    to_file.setFile(dest_file_path);
//    if(from_file.isFile()) { //常规文件的拷贝
//        QFile::copy(move_file_path, dest_file_path);
//    } else if(from_file.isDir()) { //拷贝文件夹
//        Copy_dir(move_file_path, dest_file_path);
//    }
//    bool ret{false};
//    if(from_file.isDir()) { //是否是文件夹
//        QDir qdir;//存放根路径
//        qdir.setPath(move_file_path);
//        if(qdir.removeRecursively()) { //删除文件
//            ret = true;
//        }
//    } else { //常规文件
//        QFile qfile(move_file_path);
//        if(qfile.remove()) {
//            ret = true;
//        }
//    }
//    PDU *respdu = mkPDU_2(ret, ENUM_MSG_TYPE_MOVE_FILE_RESPOND, move_file, "", "");
//    write((char *)respdu, respdu->uiPDULen);
//    delete respdu;
//    respdu = NULL;
}
quint64 MyTcpSocket::dirFileSize(const QString &path)    //获取文件夹内所有文件大小
{
    QDir dir(path);
    quint64 size = 0;
    //dir.entryInfoList(QDir::Files)返回文件信息
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files)) {
        //计算文件大小
        size += fileInfo.size();
    }
    //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        //若存在子目录，则递归调用dirFileSize()函数
        size += dirFileSize(path + QDir::separator() + subDir);
    }
    return size;
}
void MyTcpSocket::Copy_dir(QString from_path, QString to_path)
{
    QDir dir;
    dir.mkdir(to_path);
    dir.setPath(from_path);
    QString from_temp, to_temp;
    QFileInfoList infolist = dir.entryInfoList();
    for(int i{0}; i < infolist.size(); ++i) {
        if(infolist[i].fileName() == "." || infolist[i].fileName() == "..") {
            continue;
        }
        qDebug() << "file_name" << infolist[i].fileName();
        from_temp = from_path + "/" + infolist[i].fileName();
        to_temp = to_path + "/" + infolist[i].fileName();
        if(infolist[i].isFile()) {
            QFile::copy(from_temp, to_temp);
        } else if(infolist[i].isDir()) {//递归拷贝
            Copy_dir(from_temp, to_temp);
        }
    }
}
QString MyTcpSocket::get_file_size(qint64 f_size)
{
    QString file_size;
    if(0 <= f_size && f_size < 1024) {
        file_size = QString::number(f_size) + "B";
    } else if(1024 <= f_size && f_size < 1024 * 1024) { //QString xx = QString::number(mm,"f",k);  //保留k位小数
        file_size = QString::number((double)f_size / 1024, 'f', 2) + "KB";
    } else if(1024 * 1024 <= f_size && f_size < qint64(1024 * 1024 * 1024)) {
        file_size = QString::number((double)f_size / (1024 * 1024), 'f', 2) + "MB";
    } else {
        file_size = QString::number((double)f_size / qint64(1024 * 1024 * 1024), 'f', 2) + "GB";
    }
    return file_size;
}

void MyTcpSocket::remove_download_port(QString name)
{
    m_download_ports.erase(name);
}
void MyTcpSocket::handle_download_file()
{
    m_ptimer->stop();
    emit start_connected(tcpserver::getinstance().get_m_ip(), DOWNLOAD_PORT);
    Sleep(500);
    emit down_signal(0, file_path);
//    qDebug() << "开始下载！！";
//    QFile file(file_path);
//    if(!file.open(QIODevice::ReadOnly)) { //以只读的形式打开
//        //没打开提示        qDebug()<<"Failed to open upload file!";
//        return;
//    }
//    while(1) {
//        char *pBuffer = new char[4096];//循环读取文件大小的字节数
//        qint64 ret{0};//实际读取到数据的大小
////        qDebug()<<"文件下载中";
//        ret = file.read(pBuffer, 4096); //读取数据并返回大小
////        qDebug()<<"发送的数据："<<pBuffer<<"数据大小:"<<ret;
//        qint64 bytestowrite = this->bytesToWrite();
//        if(bytestowrite > 65535) { //设置门限值flush掉
//            this->flush();
////            qDebug()<<"触发flush函数！bytestowrite"<<bytestowrite;
//        }
//        if(ret > 0 && ret <= 4096) {
//            this->write(pBuffer, ret); //发送实际读取到的数据的大小
//        } else if(ret == 0) { //读取数据结尾
//            break;
//        } else {
////            qDebug()<<"Failed to upload file!,read file error!";
//            return;
//        }
//        delete[] pBuffer;
//    }
//    file.close();
}
