#ifndef FILEOP_H
#define FILEOP_H

#include <QWidget>
#include "tcpclient.h"
#include "protocol.h"
#include <QListWidgetItem>
#include "myfieinfo.h"
#include "operatewidget.h"
#include "sharefile.h"
#include <QProgressDialog>
#include <QThread>
#include "operatewidget.h"




class FileOp : public QObject
{
    Q_OBJECT
public slots:

    void connect_to_server(QString ip, int port); //连接服务器
    void upload_file(QString usr_name, QString file_name, QString file_path, QString usr_cur_path, qint16 upload_param); //上传文件槽函数
    // void download_file();//下载文件槽函数
    void my_connect_Host(QString ip, int port, QTcpSocket *m_s);
public:
    explicit FileOp(QObject *parent = nullptr);

    QString get_file_size(qint64 f_size);


signals:
    void connectedok();

    void CurPercent(int percent);//更新进度条

    void Initial_failed(int id);//没有选择上传 下载的文件发出次信号 主线程弹出对应的警告对话框

    void end_thread(QString file_name);

    void Failed_working(QString file_name);//中途上传 下载失败 上传下载的总字节数 与文件的总字节数不匹配 主线程弹出对应的警告对话框

private:

//    QString struploadfilepath;//保存文件的路径
    qint64 file_total_size;//文件总大小
    qint64 file_temp_size;//当前大小
    int ID;
//    QString file_name;//文件名
    QTcpSocket *my_socket;

};

#endif // FILEOP_H
