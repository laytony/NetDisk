#include "fileop.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <windows.h>

void FileOp::connect_to_server(QString ip, int port)
{
    my_socket = new QTcpSocket;//必须在此处进行实例化
    qDebug() << "连接服务器子线程id" << QThread::currentThread();
    qDebug() << "尝试连接服务器！";
    my_socket->connectToHost(QHostAddress(ip), port);
    connect(my_socket, &QTcpSocket::connected, this, &FileOp::connectedok);
    connect(my_socket, &QTcpSocket::disconnected, this, [ = ]() {
        qDebug() << "与服务器断开连接！！！";
        my_socket->close();
        my_socket->deleteLater();
    });
}

void FileOp::upload_file(QString usr_name, QString file_name, QString file_path, QString usr_cur_path, qint16 upload_param) //开始_上传文件
{
    qDebug() << "上传文件子线程id" << QThread::currentThread();
    QFile file(file_path);
    qint64 file_size = file.size();
    qDebug() << "文件名" << file_name;
    qDebug() << "文件大小：" << file_size;
    char predata[288] = "";//32 人名 64 文件名 64 文件大小 128 用户路径
    memcpy(predata, usr_name.toStdString().c_str(), 32);
    memcpy(predata + 32, file_name.toStdString().c_str(), 64);
    memcpy(predata + 96, (char *)&file_size, 64);
    memcpy(predata + 160, usr_cur_path.toStdString().c_str(), 128 );
    file.open(QIODevice::ReadOnly);
    qint64 num = 0, to_send{0};
    //while(!file.atEnd()) {
    while(num < file_size) {
        if(num == 0) { //只执行一次
            my_socket->write(predata, 288);
            //m_socket_snedfile->write((char*)&file_size,64);
            //  m_socket_snedfile->flush();
        }
        to_send = my_socket->bytesToWrite();
        if(to_send > upload_param * 10) { //4095参数 65535
            my_socket->flush();
        }
        QByteArray line = file.read(upload_param);//此处开始写文件num的值改变
//       QByteArray line = file.readAll();//此处开始写文件num的值改变
        num += line.size();
//        if(num + 1024 > file_size) {
//            my_socket->flush();
//        }
        int percent = double(num) / file_size * 100;
        emit CurPercent(percent);
        my_socket->write(line);
    }
    my_socket->flush();
    emit end_thread(file_name);
    file.close();
//    qint64 sned_size = 0;
//    char *pBuffer = new char[4096];//循环读取文件大小的字节数4096
//    qint64 ret{0};//实际读取到数据的大小
//    qint64 bytestowrite = 0;
//    file.open(QIODevice::ReadOnly);
//    while(!file.atEnd()) {
//        if(sned_size == 0) {
//            my_socket->write(predata, 288);
//        }
//        ret = file.read(pBuffer, 4096); //读取数据并返回大小
//        bytestowrite = my_socket->bytesToWrite();
//        if(bytestowrite > 1048575) { //设置门限值flush掉
//            my_socket->flush();;
//        }
//        if(ret > 0 && ret <= 4096) {
//            my_socket->write(pBuffer, ret); //发送实际读取到的数据的大小
//        } else if(ret == 0) { //读取数据结尾
//            sned_size = 0;
//            break;
//        }
//        sned_size += ret;
//        qint64 percent = (double(sned_size) / file_size) * 100;
//        emit CurPercent(percent);
//        memset(pBuffer, ' ', sizeof(&pBuffer));
//    }
//    sned_size = 0;
//    delete[] pBuffer;
//    emit end_thread(file_name);
//    file.close();
    qDebug() << "子线程执行结束！";
}

void FileOp::my_connect_Host(QString ip, int port, QTcpSocket *m_s)
{
    m_s->connectToHost(QHostAddress(ip), port);
}


FileOp::FileOp(QObject *parent) : QObject(parent)
{
}

QString FileOp::get_file_size(qint64 f_size)
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
