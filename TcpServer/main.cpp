#include "tcpserver.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBoperate::getInstance().Init();//数据库初始化
    tcpserver::getinstance().show();//服务器也写成单例模式
    return a.exec();
}

