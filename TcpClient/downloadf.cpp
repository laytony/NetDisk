#include "downloadf.h"
#include "windows.h"

DownLoadF::DownLoadF(QTcpSocket *m_s, qint64 cnt, qint64 total, QString save_path, QObject *parent) : QThread(parent)
{
    file_path = save_path;
    this->cnt = cnt;
    this->total = total;
    this->m_s = m_s;
//    file = new QFile(file_path);
    // m_s->waitForReadyRead(300);
}

void DownLoadF::end_thread()
{
//    file->close();
//    file->deleteLater();
//    m_s->close();
//    m_s->deleteLater();
//    qDebug() << "接受文件结束！！！";
//    emit rev_over();
}


void DownLoadF::run()//
{
    qDebug() << "接受上传文件的子线程id为：" << QThread::currentThread();
    QFile *file = new QFile(file_path);
    file->open(QIODevice::WriteOnly);
//    static qint64 total{0};
//    static qint64 cnt{0};
    char predata[64];
    qDebug() << "下载文件总大小1111：" << total;
    QTimer *revtimer = new QTimer;//timer需要在里面定义
    connect(revtimer, &QTimer::timeout, this, [ = ]() {
        qDebug() << "ds定时到达接受文件结束强制！！！";
        file->close();
        file->deleteLater();
        m_s->close();
        m_s->deleteLater();
        emit rev_over();
    });
    revtimer->start(total / 10);//延时毫秒级total / 1049
    connect(m_s, &QTcpSocket::disconnected, this, [&]() {
        qDebug() << "socket断开";
//        file->close();
//        file->deleteLater();
//        m_s->close();
//        m_s->deleteLater();
    });
    connect(m_s, &QTcpSocket::readyRead, this, [ & ]() {//32 人名 64 文件名 64 文件大小 128 用户路径
//        if(cnt == 0) {
//            m_s->read((char *)&total, 64);
//            qDebug() << "total" << total;
//        }
        QByteArray all = m_s->readAll();
        cnt += all.size();
        file->write(all);
        file->flush();
        int percent = double(cnt) / total * 100;
        emit CurPercent(percent);
        //    qDebug() << "传输中的cnt" << cnt;
        if(cnt == total) {
            qDebug() << "已经下载的大小：" << cnt;
            file->close();
            file->deleteLater();
            m_s->close();
            m_s->deleteLater();
            qDebug() << "接受文件结束！！！";
            emit rev_over();
        }
    });
    exec();
}
