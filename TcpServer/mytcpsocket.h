#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include "tcpserver.h"
#include <queue>
#include <QFile>
#include <QTimer>
#include <QThread>
#include "fileop.h"
#include "movefile.h"
#include "sharefile.h"

class FileOp;

class MyTcpSocket : public QTcpSocket//定义客户端的socket组
{
    Q_OBJECT
public:
    MyTcpSocket();
signals:
    void off(MyTcpSocket *mysocket);//定义下线函数，客户端连接中断自动下线
    //void offline();//定义下线函数，客户端连接中断自动下线
    void down_signal(qint64 file_size, QString file_path); //文件路径、消息类型、文件名字、文件大小
    void start_connected(QString ip, int port);

public slots:
    void recvMsg();//提供接收消息的槽函数 接收处理用户发送的信息
    void clientoffline();//用于处理客户端下线的信号；
    QString getname();
    void handle_download_file();

private:
    //PDU* mypdu;//
    QString m_strName;//用户名 与客户端通信的socket的名字
//    unordered_map<QString,queue<PDU*>>MSG_tofri;//存放按时间戳排序的要发给该用户的消息
    QFile *m_file;
    QString file_name;//接受文件的文件名
    QString file_path;//要发送的文件路径
    qint64 file_total_size;//总的大小
    qint64 file_rev_size;//已经接受的大小
    bool uploadS;//判断是否处于上传文件状态
    QTimer *m_ptimer;
    QThread *myfile_oprate;//定义操作文件的多线程
    FileOp *my_fileop;//定义操作文件的任务对象


public://recv处理子函数
    void handle_rev_file();
    void handle_ENUM_MSG_TYPE_REGIST_REQUEST(PDU *mypdu);//注册请求
    void handle_ENUM_MSG_TYPE_LOGIN_REQUEST(PDU *mypdu);//登陆请求
    void handle_ENUM_MSG_TYPE_LOGOFF_REQUEST(PDU *mypdu);//退出登录
    void handle_ENUM_MSG_TYPE_DESTORY_REQUEST(PDU *mypdu);//注销请求
    void handle_ENUM_MSG_TYPE_ALL_ONLINE_REQUEST(PDU *mypdu);//查询所有在线用户请求
    void handle_ENUM_MSG_TYPE_SEARCH_USR_REQUEST(PDU *mypdu);//搜索用户
    void handle_ENUM_MSG_TYPE_ADD_USR_REQUEST(PDU *mypdu);//添加好友
    void handle_ENUM_MSG_TYPE_NO_ADD_USR_REQUEST(PDU *mypdu);//拒绝添加好友
    void handle_ENUM_MSG_TYPE_YES_ADD_USR_REQUEST(PDU *mypdu);//同意添加好友
    void handle_ENUM_MSG_TYPE_FLUSH_FRI_REQUSET(PDU *mypdu);//刷新好友列表
    void handle_ENUM_MSG_TYPE_DELETE_FRI_REQUSET(PDU *mypdu);//删除好友请求
    void handle_ENUM_MSG_TYPE_SDELETE_FRI_REQUSET(PDU *mypdu);//确认删除好友
    void handle_ENUM_MSG_TYPE_GROUP_CHAT_REQUESTT(PDU *mypdu);//群聊请求
    void handle_ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST(PDU *mypdu);//私聊请求
    void handle_ENUM_MSG_TYPE_CREATE_DIR_REQUEST(PDU *mypdu);//创建文件夹
    void handle_ENUM_MSG_TYPE_FLUSH_FILE_REQUEST(PDU *mypdu);//刷新文件夹
    void handle_ENUM_MSG_TYPE_DELETE_FILE_REQUEST(PDU *mypdu);//删除文件夹
    void handle_ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST(PDU *mypdu);//确定删除请求
    void handle_ENUM_MSG_TYPE_RENAME_DIR_REQUEST(PDU *mypdu);//重命名文件
    void handle_ENUM_MSG_TYPE_upload_FILE_REQUEST(PDU *mypdu);//上传文件
    void handle_ENUM_MSG_TYPE_download_FILE_REQUEST(PDU *mypdu);//下载文件
    void handle_ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST(PDU *myppdu);//获取文件信息
    void handle_ENUM_MSG_TYPE_SHARE_FILE_REQUEST(PDU *mypdu);//分享请求
    void handle_ENUM_MSG_TYPE_SHARE_FILE_RESPOND(PDU *mypdu);//确认分享 进行拷贝
    void handle_ENUM_MSG_TYPE_MOVE_FILE_REQUEST(PDU *mypdu);//移动文件夹请求
    void handle_ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE(PDU *mypdu);

    quint64 dirFileSize(const QString &path);//递归获取文件夹内所有文件大小
    void Copy_dir(QString from_path, QString to_path); //文件夹的拷贝！；
    QString get_file_size(qint64 f_size);


private:
    QTcpServer *my_uploadserver;
    //QTcpSocket* my_socket;
    int index;
    unordered_map<QString, int> m_download_ports; //存放下载的随机端口号
public:
    void remove_download_port(QString name);




};

#endif // MYTCPSOCKET_H
