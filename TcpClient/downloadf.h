#ifndef DOWNLOADF_H
#define DOWNLOADF_H

#include <QFile>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <qthread.h>

class DownLoadF : public QThread
{
    Q_OBJECT
public:
    explicit DownLoadF(QTcpSocket *m_s, qint64 count, qint64 total, QString save_path, QObject *parent = nullptr);

signals://index,&m_file,file_total_size,file_rev_size,upload_port
    void rev_over();
    void CurPercent(int num);
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
protected:
    void run() override;
//    QTimer *m_ptimer;
//    QThread* myfile_oprate;//定义操作文件的多线程
//    FileOp* my_fileop;//定义操作文件的任务对象
};

#endif // DOWNLOADF_H
