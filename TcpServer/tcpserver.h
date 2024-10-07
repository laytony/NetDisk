#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QFile>  //使用头文件Qfile读取文件
#include <QByteArray>  //添加字节数组头文件
#include <QDebug> //打印数据头文件
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>
#include "protocol.h"
#include "dboperate.h"
#include <windows.h>
#include <unordered_set>
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class tcpserver;
}
QT_END_NAMESPACE

class tcpserver : public QWidget
{
    Q_OBJECT

public:
    tcpserver(QWidget *parent = nullptr);
    ~tcpserver();
    void loadConfig();//加载配置 初始化函数
    QString get_dir();//获取存储路径
    static tcpserver &getinstance();
    void show_usr_request(PDU *pdu);//显示用户信息求情
    void show_usr_files(QByteArray data);//显示用户信息求情
    void show_usr_state(QStringList state_list);//显示用户状态
    QString get_m_ip();

public slots:
    void is_connected();


private:
    Ui::tcpserver *ui;
    QString m_strIP;//使用qstring类存放ip
    quint16 m_usPort;//使用无符号的16位存放端口号
    QString usr_file_dir;
public:
    //PDU* mypdu;
    //QTcpSocket server;
private slots:


    void on_pushButton_cahnge_ip_port_clicked();
};
#endif // TCPSERVER_H
