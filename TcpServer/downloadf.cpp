#include "downloadf.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <windows.h>
#include <QThread>

DownLoadF::DownLoadF(QObject *parent) : QObject(parent)
{
}


void DownLoadF::connect_to_server(QString ip, int port)
{
    my_socket = new QTcpSocket;//必须在此处进行实例化
    qDebug() << "连接服务器子线程id" << QThread::currentThread();
    qDebug() << "尝试连接服务器！";
    my_socket->connectToHost(QHostAddress(ip), port);
    connect(my_socket, &QTcpSocket::connected, this, &DownLoadF::connectedok);
    connect(my_socket, &QTcpSocket::disconnected, this, [ = ]() {
        qDebug() << "与客户端上传文件服务器断开连接！！！";
        my_socket->close();
        my_socket->deleteLater();
    });
}

void DownLoadF::download_file(qint64 file_size, QString file_path) //开始_上传文件
{
    qDebug() << "下载文件子线程id: " << QThread::currentThread();
    qDebug() << "下载文件的大小为：" << file_size;
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    qint64 to_send{0};
//    my_socket->write((char *)&file_size);//发送文件大小
    qint64 num{0};
//    while(!file.atEnd()) {
    while(num < file_size) {
//        if(num == 0) {
//            qDebug() << "发送文件大小!" << file_size;
//            my_scket->write((char *)&file_size, 64);   //发送文件大小 64位
//        }
//        QByteArray line = file.readLine();//此处开始写文件num的值改变
//        QByteArray line = file.readAll();//此处开始写文件num的值改变
        QByteArray line = file.read(5);//此处开始写文件num的值改变
//        qDebug() << "单次传输大小：" << line.size();
        num += line.size();
        my_socket->write(line);
        to_send = my_socket->bytesToWrite();
        if(to_send > 5) { //4095参数 655536
            my_socket->flush();
        }
//        if(num + 65535 > file_size) {//flush掉最后一段
//            my_socket->flush();
//        }
    }
    my_socket->flush();
    qDebug() << "传输总大小" << num;
    emit end_thread(file_path);
    file.close();
//    qint64 sned_size = 0;
//    char *pBuffer = new char[4096];//循环读取文件大小的字节数4096
//    qint64 ret{0};//实际读取到数据的大小
//    qint64 bytestowrite = 0;
//    while(!file.atEnd()) {
//        ret = file.read(pBuffer, 4096); //读取数据并返回大小
//        if(ret > 0 && ret <= 4096) {
//            my_socket->write(pBuffer, ret); //发送实际读取到的数据的大小
////            my_socket->flush();
//        } else if(ret == 0) { //读取数据结尾
//            break;
//        }
//        bytestowrite = my_socket->bytesToWrite();
//        if(bytestowrite > 1023) { //设置门限值flush掉
//            my_socket->flush();;
//        }
//        sned_size += ret;
//        memset(pBuffer, ' ', sizeof(&pBuffer));
//    }
//    delete[] pBuffer;
//    emit end_thread(file_path);
//    file.close();
//    qDebug() << "总传输大小" << sned_size;
    qDebug() << "子线程执行结束！";
}

void DownLoadF::my_connect_Host(QString ip, int port, QTcpSocket *m_s)
{
    m_s->connectToHost(QHostAddress(ip), port);
}

QString DownLoadF::get_file_size(qint64 f_size)
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
