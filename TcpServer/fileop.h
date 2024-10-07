#ifndef FILEOP_H
#define FILEOP_H

#include <QObject>
#include "mytcpserver.h"
#include <QTcpServer>
#include <QFile>
#include <QThread>

class FileOp : public QThread
{
    Q_OBJECT
public:
    explicit FileOp(QTcpSocket *m_s, int  count, int total,  QObject *parent = nullptr);

signals://index,&m_file,file_total_size,file_rev_size,upload_port
    void rev_over(QString usr_name, QString file_name, qint64 size);
public slots:
    // void download_file();//下载文件槽函数
    void end_thread();//结束进程 防止tcp传输出问题

private:
    // QString m_strName;//用户名 与客户端通信的socket的名字
//    unordered_map<QString,queue<PDU*>>MSG_tofri;//存放按时间戳排序的要发给该用户的消息
    int ID;
    QFile *file;
    QString file_name;//接受文件的文件名
    QString file_path;//要发送的文件路径
    qint64 cnt;
    qint64 total;
    int m_port;
    QTcpServer *myserver;
    QTcpSocket *m_s;
    int flag;
protected:
    void run() override;

//    QTimer *m_ptimer;
//    QThread* myfile_oprate;//定义操作文件的多线程
//    FileOp* my_fileop;//定义操作文件的任务对象

};

#endif // FILEOP_H
