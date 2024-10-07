#ifndef DBOPERATE_H
#define DBOPERATE_H

#include <QObject>
#include <QSqlDatabase>//定义数据库
#include <QSqlQuery>//查询数据库
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <unordered_set>
#include <windows.h>
//#include "tcpserver.h"



class DBoperate : public QObject
{
    Q_OBJECT
public:
    explicit DBoperate(QObject *parent = nullptr);
    //定义成静态的类型，每次使用时都是单例，调用一次该静态成员，不会多次调用，产生较多的内存消耗
    static DBoperate& getInstance();
    QStringList Init();
    ~DBoperate();//析构函数
    bool handle_register(const char* name,const char* pwd);//处理注册，注册成功返回1 否则返回0；
    bool handle_login(const char* name,const char* pwd);//处理登录，注册成功返回1 否则返回0；
    bool handle_logoff(const char* name,const char* pwd);//处理退出登录 成功返回1 否则返回0；
    bool handle_destory(const char* name,const char* pwd);//处理退出登录 成功返回1 否则返回0；
    void handle_offline(const char* name);//客户掉线 数据库自动设置为0 不在 线
    //处理获取在线用户请求 返回字符串列表
    QStringList handle_all_online();//获取所有在线用户
    int handle_search_usr(QString name);//处理搜索用户请求
    //处理添加好友操作
    int handle_add_usr(QString name,QString requestame);
    //同意添加好友后将二者改为好友关系
    void handle_yes_add_fri(QString name_0,QString name_1);//客户掉线 数据库自动设置为0 不在 线
    QStringList handle_flush_fri(QString name);//刷新好友//返回好友列表
    void handle_delete_fri(QString name_0,QString name_1);

signals:

public slots:

private:
    QSqlDatabase m_db;//定义用于连接数据库

};

#endif // DBOPERATE_H
