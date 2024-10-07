#include "fileop.h"

FileOp::FileOp(QTcpSocket *m_s, int  cnt, int total,  QObject *parent) : QThread(parent)
{
    this->cnt = cnt;
    this->total = total;
    this->m_s = m_s;
    file = new QFile;
    flag = 0;
}

void FileOp::end_thread()
{
//    uptier->stop();
//    file->close();
//    file->deleteLater();
//    m_s->close();
//    m_s->deleteLater();
//    qDebug() << "接受文件结束！！！";
}


void FileOp::run()//    //32 人名 64 文件名 64 文件大小 128 用户路径
{
    qDebug() << "接受上传文件的子线程id为：" << QThread::currentThread();
//    QFile *file = new QFile;
    char predata[288] = "";
    char usr_name[32] = "";
    char file_name[64] = "";
    char usr_path[128] = "";
    qint64 cnt = 0;
    qint64 total = 0;
    qint64 f_size{0};
    qDebug() << "初始值" << cnt;
    connect(m_s, &QTcpSocket::readyRead, this, [ & ]() {//32 人名 64 文件名 64 文件大小 128 用户路径
        if(cnt == 0 && !flag) {
            flag = 1;
//            qDebug() << "cnt0" << cnt;
            m_s->read(predata, 288);
//            qDebug() << "cnt1" << cnt;
            memcpy(usr_path, predata + 160, 128);
//            qDebug() << "cnt2" << cnt;
            memcpy((char *)&total, predata + 96, 64);
//            qDebug() << "cnt3" << cnt;
            memcpy(file_name, predata + 32, 64);
//            qDebug() << "cnt4" << cnt;
            memcpy(usr_name, predata, 32);
            file->setFileName(QString(usr_path) + file_name);
            if(file->exists()) {
                file->setFileName(QString(usr_path) + "1" + file_name );
            }
            file->open(QIODevice::WriteOnly);
            qDebug() << "file_name" << file_name;
            qDebug() << "total" << total;
            f_size = total;
            QTimer *revtimer = new QTimer();
            connect(revtimer, &QTimer::timeout, this, [ & ]() {
                file->close();
                file->deleteLater();
                m_s->close();
                m_s->deleteLater();
                qDebug() << "接受文件结束！！！";
                emit rev_over(usr_name, file_name, total);
            });
            revtimer->start(total / 100);//延时毫秒级//6m一分钟的速度 6*1024*1024/(60*1000) = 100bit/ms 用时为 total/1048 ms
            qDebug() << "usr_name" << usr_name;
            qDebug() << "usr_path" << usr_path;
            qDebug() << "cnt5" << cnt;
        }
//        QByteArray all = m_s->readLine();
        QByteArray all = m_s->readAll();
        // qDebug() << "all.size()" << all.size();
//        if(all.size() > 4096) {
//            file->flush();
//        }
        file->write(all);
        cnt += all.size();
//        qDebug() << "cnt" << cnt;
        if(cnt == total) {
            qDebug() << "文件总大小11111：" << total;
            cnt = 0;
            total = 0;
            file->close();
            file->deleteLater();
            m_s->close();
            m_s->deleteLater();
            qDebug() << "接受文件结束！！！";
            emit rev_over(usr_name, file_name, f_size);
        }
    });
    exec();
}
