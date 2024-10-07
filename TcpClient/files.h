#ifndef FILES_H
#define FILES_H

#include <QWidget>
#include "tcpclient.h"
#include "protocol.h"
#include <QListWidgetItem>
#include "myfieinfo.h"
#include "operatewidget.h"
#include "sharefile.h"
#include <QProgressDialog>
#include <QThread>
#include "fileop.h"
#include <unordered_map>
#include "downloadf.h"

class FileOp;

class TcpClient;

namespace Ui
{
    class Files;
}

class Files : public QWidget
{
    Q_OBJECT

public:
    explicit Files(QWidget *parent = nullptr);
    ~Files();
    void Flush_folders(PDU *pdu);//刷新文件列表操作
    QString get_sharefile_name();
    void handle_down_load_file();
    void start_listen(QString port);//下载服务器开始监听
    void on_pushButton_overloadiamge_clicked(QString path);//上传图片
    void on_pushButton_downloadimage_clicked(QString path, QString topath, int img_size); //下载图片

signals:
    void create_rtp();

public slots:
    void Enter_dir(QListWidgetItem *item);//进入文件夹请求
    void on_pushButton_flush_folders_clicked();//刷新文件列表请求
    void Show_FD_MSG(QListWidgetItem *item);//显示文件（夹）信息请求
    void Show_File_MSG(QListWidgetItem *item);//显示文件信息
    void on_pushButton_overloadfile_clicked();//上传文件

    //void uploadfile();//上传文件

    void on_pushButton_downloadfile_clicked();//下载文件
private slots:
    void on_pushButton_creat_folders_clicked();//创建文件夹请求 自动调用刷新文件


    void on_pushButton_delete_folders_clicked();//删除文件请求 自动调用刷新文件

    void on_pushButton_return_clicked();//返回文件目录请求 自动调用刷新文件

    void on_pushButton_rename_folders_clicked();//重命名文件（夹）自动调用刷新文件



    void on_pushButton_show_msg_folders_clicked();//显示文件信息

    void on_pushButton_sharefile_clicked();//分享文件夹


    void on_pushButton_sure_movefile_folders_clicked();

    void on_pushButton_move_file_clicked();
    void handle_initial_failed(int id);//处理子线程初始化失败
    void handle_failed_working(int id);//处理子线程工作中故障
    void handle_end_thread(int id, FileOp *fileop); //处理结束子线程

    void uploadfile_th();
public:
    Ui::Files *ui;
private:
    QString struploadfilepath;//保存上传文件的路径
    QTimer *m_ptimer_up;//定时 发送文件用时
    QString shared_filename;//要分享的温江
    QString move_file_path;//要移动的文件路径
    QString move_file;//要移动的文件名
    qint64 file_total_size;//上传文件总大小
    qint64 file_send_size;//以上传的大小
    QString upload_file_name;
    QString download_file_name;
    QString file_size1;

    ///多线程实现：
    //FileOp* my_fileop;//定义操作文件的任务对象
    unordered_map<int, QThread *>my_thread; //用于存放对应的多线程指针 key_>value对应的线程编号-》线程
    //unordered_map<int,FileOp*>my_works;
    int index;//线程编号
    int upload_port;//上传下载的端口号
    QString download_port;
    QThread *dowload_file_thread;
    FileOp *my_fileop;
    QTimer *up_t;
    QString up_usr_name;
    QString up_usr_path;
    QListWidgetItem *item;
    QTcpServer *dowload_server;

signals:
    void up_signal(QString usr_name, QString file_name, QString file_path, QString cur_file_path, qint16 upload_param); //文件路径、消息类型、文件名字、文件大小

    void start_connected(QString ip, int port);
};

#endif // FILES_H
