#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>  //使用头文件Qfile读取文件
#include <QByteArray>  //添加字节数组头文件
#include <QDebug> //打印数据头文件
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QLineEdit>
#include "protocol.h"
#include <string.h>
#include <stdio.h>
#include <unordered_set>
#include "myfieinfo.h"
#include <QProgressDialog>
#include <QTcpServer>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class TcpClient;
}
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();//加载配置 初始化函数
    static TcpClient &getInstance();
    QTcpSocket &getTcpSocket();
    QString getName();//获取当前用户名
    QString getsystemtime();//获取系统当前时间戳
    QString get_rootpath();//获取用户根文件 所在跟目录
    QString get_curpath();//获取用户文件 所在当前目录
    void change_curpath(QString name);
    void change_download_path(QString path);//改变文件保存地址
    QString get_download_path();//获取下载文件保存地址
    void set_file(QString file_path);
    void handle_download_file();//下载文件
    void show_file_msg(PDU *mypdu);
    QString get_file_size(qint64 f_size);
    bool get_onlienstate();//返回是否在线状态
    bool reconnect();
    bool eventFilter(QObject *object, QEvent *event) override;//触发事件 绑定回车键实现登录操作
    QString get_m_ip();
    int get_m_port();
    QString get_downloadfile_name();
    qint64 get_download_file_total_size();
    void change_file_total_size(qint64 size);
    //void showConnect();
    QString get_mydownip();//获取本机的ip
    qint16 get_m_upload_param();//获取上传文件参数

public slots://添加槽函数 捕获数据
    void showConnect();//使用槽函数的形式确保在界面打开后再弹出消息提示，否在在界面打开之前提示
    void recvMsg();
    void processRet();

private slots:
//    void on_send_p_clicked();//测试发送

    void on_login_pb_clicked();

    void on_register_pb_clicked();

    void on_log_off_clicked();//按钮退出

    void on_logoff_pb_clicked(); //注销账号

    bool clientoffline();//槽函数一旦不在线发送信息

    void on_change_ip_port_pb_clicked();

    void on_sure_to_change_ip_port_pb_clicked();



private:
    Ui::TcpClient *ui;
    QString m_strIP;//使用qstring类存放ip
    quint16 m_usPort;//使用无符号的16位存放端口号
    quint16 m_upload_param;//存放上传文件的参数
    QString Usr_name;//存放当前用户ID
    QString Usr_psd;
    QTcpSocket m_tcpSocket;//连接服务器，和服务器进行数据交互 每个客户处定义一个socket用于与服务器进行数据交换
    QString m_strCurpath;//记录用户的文件路径
    QString m_strRootpath;//记录用户的根文件路径
    QString m_save_path;//下载文件的路径
    qint64 file_total_size;//总的大小
    qint64 file_rev_size;//已经接受的大小
    QFile m_file;//下载的文件
    QString m_file_name;//下载文件的名字
    bool downloadS;//是否处于下载状态
    bool is_online;

    QTcpServer *dowload_server;
    QString Mydownip;

    //QProgressDialog downprogress;


signals:
    void recvmsg(PDU *pdu);//定义下线函数，客户端连接中断自动下线


};
#endif // TCPCLIENT_H
