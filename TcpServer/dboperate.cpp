#include "dboperate.h"

DBoperate::DBoperate(QObject *parent) : QObject(parent){
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName("localhost");
    //m_db.setDatabaseName("E:\\cpp\\pros\\NetDisk\\qt\\TcpServer\\cloud.db");//使用绝对路径能够正常输出
    m_db.setDatabaseName("./cloud.db");
    if(m_db.open()){
        QSqlQuery query;
        query.exec("update usrInfo set online = 0");//update usrInfo set online = 0 where name = \'%1\' and pwd = \'%2\' "
    }else QMessageBox::critical(NULL,"Try to open DB","Failed to open DB!");
    m_db.close();
}

DBoperate &DBoperate::getInstance(){
    static DBoperate instance;
    return instance;
}

QStringList DBoperate::Init(){
    QStringList qlist;
    m_db.open();
   // if(m_db.open()){//查询数据
        QSqlQuery query;
        query.exec("select * from usrInfo");
        while(query.next()){
            //QString data = QString ("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString().arg(query.value(3).toString()));
             QString data = QString ("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
             int show_online = query.value(3).toInt();
             if(show_online == 1)data.append(" (online)");
             else data.append(" (not online)");
             //qDebug()<<data;
             qlist.push_back(data);
             //tcpserver::getinstance().flush_usr_info(data);
        }
   // }
    return qlist;
}

DBoperate::~DBoperate(){
    m_db.close();
}

bool DBoperate::handle_register(const char *name, const char *pwd){//将用户名和密码拼接成一个语句插入到数据库中
    m_db.open();
    if(!name || !pwd){
        //qDebug()<<"name || pwd is empty";
        return false;
    }
    //注意语句是用来values('','');需要用字符串；\'转义
    QString data = QString ("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
//    qDebug()<<data;
    QSqlQuery query;//产生一个数据库操作对象
    if( query.exec(data)){
        return true;
    }
    return false;
}

bool DBoperate::handle_login(const char *name, const char *pwd){
    m_db.open();
    if(!name || !pwd){
        return false;
    }
    //注意语句是用来values('','');需要用字符串；\'转义
    QString data = QString ("select * from usrInfo where name = \'%1\' and pwd = \'%2\' and online = 0").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
//    qDebug()<<data;
    QSqlQuery query;//产生一个数据库操作对象
    query.exec(data);
    if(query.next()){//查询到此次记录
        QString data = QString ("update usrInfo set online = 1 where name = \'%1\' and pwd = \'%2\' ").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
//        qDebug()<<data;
        QSqlQuery query;//产生一个数据库操作对象
        query.exec(data);
        return true;
    };//获取满足条件的数据库总的数据 用户名唯一 要么获得信息返回1否则没有获得返回0m_db.close()
    return false;

}

bool DBoperate::handle_logoff(const char *name, const char *pwd){
    if(!name || !pwd){
        return false;
    }
    //注意语句是用来values('','');需要用字符串；\'转义 查询到已经处于登录状态
    QString data = QString ("select * from usrInfo where name = \'%1\' and pwd = \'%2\' and online = 1").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
//    qDebug()<<data;
    QSqlQuery query;//产生一个数据库操作对象
    query.exec(data);//未查询到已经登陆的信息
    if(query.next()){//查询到此次记录 改为不在线状态 已经登出
        QString data = QString ("update usrInfo set online = 0 where name = \'%1\' and pwd = \'%2\' ").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
       //qDebug()<<data;
        QSqlQuery query;//产生一个数据库操作对象
        query.exec(data);
        return true;
    };//获取满足条件的数据库总的数据 用户名唯一 要么获得信息返回1否则没有获得返回0
    return false;
}

bool DBoperate::handle_destory(const char *name, const char *pwd)
{
    if(!name || !pwd){
        return false;
    }
    //注意语句是用来values('','');需要用字符串；\'转义 查询到已经处于登录状态
    QString data = QString ("select * from usrInfo where name = \'%1\' and pwd = \'%2\'").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
//    qDebug()<<data;
    QSqlQuery query;//产生一个数据库操作对象
    query.exec(data);//未查询到已经登陆的信息
    if(query.next()){//查询到进行注销
        QString data = QString ("delete from usrInfo where name = \'%1\' and pwd = \'%2\' ").arg(name).arg(pwd);//用名字替换%1的地方 用密码替换%2的地方
        //qDebug()<<data;
        QSqlQuery query;//产生一个数据库操作对象
        query.exec(data);
        return true;
    };//获取满足条件的数据库总的数据 用户名唯一 要么获得信息返回1否则没有获得返回0
    return false;
}

void DBoperate::handle_offline(const char *name){//客户端退出，自动掉线在线状态设置为0
    if(name == NULL){//通过用户名确定从数据库中更改哪一个在线的用户
        return;
    }
    //注意语句是用来values('','');需要用字符串；\'转义 查询到已经处于登录状态
    QString data = QString ("update usrInfo set online = 0 where name = \'%1\'").arg(name);//用名字替换%1的地方 用密码替换%2的地方
//    qDebug()<<data;
    QSqlQuery query;//产生一个数据库操作对象
    query.exec(data);//未查询到已经登陆的信息
}

QStringList DBoperate::handle_all_online(){
    Sleep(10);
    QStringList list;
    QString data = QString("select name from usrInfo where online = 1");//
    //qDebug()<<data;
    QSqlQuery query;//产生一个数据库操作对象
    query.exec(data);//
    while(query.next()){//使用query.value(0)来获取查询到的数据，0 ，1,2对应查询数据的顺序
        list.push_back(query.value(0).toString());
    }
//    list.clear();
    return list;
}

int DBoperate::handle_search_usr(QString name)
{

    qDebug()<<"seachname"<<name;
    if(NULL == name){
        return -1;
    }
    QString data = QString("select online from usrInfo where name = \'%1\'").arg(name);//
    QSqlQuery query;//产生一个数据库操作对象
    query.exec(data);//
    if(query.next()){
        switch (query.value(0).toInt()) {
        case 1:{
            return 1;
        }
        case 0:{
            return 0;
        }
        default:
            break;
    }
    }
    return -1;
}

int DBoperate::handle_add_usr(QString name,QString requestame){
    if(requestame == NULL || name == NULL){
        return -1;
    }
    //查询 在friendInfo中查询好友关系，当在1的好友中能查到2或者2中能查到1即可二者互为好友关系
    QString data = QString("select * from friendInfo where (id = (select id from usrInfo where name = \'%1\') and "
                           "friendId = (select id from usrInfo where name = \'%2\')) "
                           "or (id = (select id from usrInfo where name = \'%2\') and "
                           "friendId = (select id from usrInfo where name = \'%1\'))").arg(name).arg(requestame);
    //qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next()){
        qDebug()<<"好友关系："<<query.value(0).toInt()<<"  "<< query.value(1).toInt();
        return 0;//双方已经是好友
    }
    else{
        return 1;//不是好友 可进行加好友操作
    }
    return -1;
}

void DBoperate::handle_yes_add_fri(QString name_0, QString name_1){
    if(name_0 == NULL || name_1 == NULL){return;}
    QString data = QString("insert into friendInfo(id,friendId) values((select id from usrInfo where name = \'%1\'),(select id from usrInfo where name = \'%2\'))").arg(name_0).arg(name_1);
    //qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
}

QStringList DBoperate::handle_flush_fri(QString name)
{//刷新好友列表显示在线的好友
    Sleep(50);
    QStringList list;
    list.clear();
    if(name == NULL){return list;}
    //提取id 用id 去好友表中查询 再返回好友的id查询好友的名字
    //"select name from usrInfo where id = "
    //"select id from friendInfo where friendId = (select id from usrInfo where name = \'%1\')).arg(name)"//以查询人为朋友的人
    //"select friendId from friendInfo where id = (select id from usrInfo where name = \'%1\')).arg(name)"//以查询人为主的id
    //找到以name为主 的 id
    QString data = QString(" select friendId from friendInfo where id = (select id from usrInfo where name = \'%1\')").arg(name);
   // qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    while(query.next()){
        list.append(query.value(0).toString());//存储id
    }
    data = QString(" select id from friendInfo where friendId = (select id from usrInfo where name = \'%1\')").arg(name);// and online = 1
 //   qDebug()<<data;
    query.exec(data);
    while(query.next()){
        list.append(query.value(0).toString());//存储id
    }
    QStringList res;
    for(int i{0};i<list.size();++i){
        data = QString("select name from usrInfo where id = %1 and online = 1").arg(list[i].toInt());
   //     qDebug()<<data;
        query.exec(data);
        Sleep(10);
        if(query.next())res.append(query.value(0).toString()+" (online)");
    }
    for(int i{0};i<list.size();++i){
        data = QString("select name from usrInfo where id = %1 and online = 0").arg(list[i].toInt());
    //    qDebug()<<data;
        query.exec(data);
        Sleep(10);
        if(query.next())res.append(query.value(0).toString()+" (not online)");
    }
    list.clear();
    return res;
}

void DBoperate::handle_delete_fri(QString name_0,QString name_1)
{
    QString data = QString("delete from friendInfo where id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\');").arg(name_0).arg(name_1);
//    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    data = QString("delete from friendInfo where id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\');").arg(name_1).arg(name_0);
   // qDebug()<<data;
    query.exec(data);
}
















