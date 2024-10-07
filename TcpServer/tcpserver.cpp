#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"

///进行封装最初的界面 导入ip  port 以支持Qtcpserver

tcpserver::tcpserver(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tcpserver)
{
    ui->setupUi(this);
    loadConfig();
//    Mytcpserver::getInstance().listen(QHostAddress(m_strIP), m_usPort);//实现监听
    Mytcpserver::getInstance().listen(QHostAddress::Any, m_usPort);//实现监听 可以监听任意地址
    this->ui->textEdit_show_request->setFixedSize(300, 500);
    this->ui->listWidget_show_usr_state->setFixedSize(290, 500);
    this->setFixedSize(600, 600);
    setLayout(ui->verticalLayout);
    this->setWindowTitle("Raynetdisc");
    show_usr_state(DBoperate::getInstance().Init());
    ui->textEdit_show_request->append("用户请求:\n");
    qDebug() << "tcpserver thread id:" << GetCurrentThreadId();
}

tcpserver::~tcpserver()
{
    //delete ui;
}

void tcpserver::loadConfig()
{
    QFile configfile("./tcpserver.config");//"D:\\cpp\\pros\\NetDisk\\qt\\TcpServer\\tcpserver.config"
    if (configfile.open(QIODevice::ReadOnly)) {//返回类型bool类型指示是否打开该文件
        QByteArray bytedata = configfile.readAll();//字节数组类型数据
        QString strdata = bytedata.toStdString().c_str();//将字节数组转换成字符串类型->char*类型的字符
        //qDebug() << strdata;//打印配置信息
        //strdata.replace("\r\n", " ");//将字符串中的\r\n替换成" "
        //qDebug() << strdata;//打印配置信息
        QStringList strList = strdata.split("\r\n");
        /*for (auto str : strList) {
            qDebug() << "-->" << str;
        }*/
        ui->lineEdit_id->setText(strList[0]);
        ui->lineEdit_port->setText(strList[1]);
        ui->lineEdit_save_dir->setText(strList[2]);
        m_strIP = strList[0];//.at(0)
        m_usPort = strList[1].toUShort();//转换为短整型
        usr_file_dir = strList[2].toStdString().c_str();//路径
        qDebug() << "IP: " << m_strIP ;
        qDebug() << "PORT: " << m_usPort;
        qDebug() << "DIR: " << usr_file_dir;
        configfile.close();
    } else {
        QMessageBox::critical(this, "open configfaile", "Failed to open configfile");
    }
}

QString tcpserver::get_dir()
{
    return usr_file_dir;
}

tcpserver &tcpserver::getinstance()
{
    static tcpserver instance;
    return instance;
}

void tcpserver::show_usr_request(PDU *pdu)
{
    QString data = pdu->caData;
    data += "(消息类型)：";
    int type = pdu->uiMsgType;
    data += to_string(type).c_str();
    data += "\n";
    ui->textEdit_show_request->insertPlainText(data);
}

void tcpserver::show_usr_files(QByteArray data)
{
    ui->textEdit_show_request->setText(data);
}

void tcpserver::show_usr_state(QStringList state_list)
{
    ui->listWidget_show_usr_state->clear();
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap(":/all_usr.ico"));
    item->setText("用户状态");
    item->setFont(QFont("黑体"));
    ui->listWidget_show_usr_state->addItem(item);
    for(auto x : state_list) {
        ui->listWidget_show_usr_state->addItem(x);
    }
}

QString tcpserver::get_m_ip()
{
    return m_strIP;
}



void tcpserver::is_connected()
{
    QMessageBox::information(this, "try to connect from client", "successfully connected form client!");
}



void tcpserver::on_pushButton_cahnge_ip_port_clicked()//更改ip和端口号
{
    QString ip = ui->lineEdit_id->text();
    QString port = ui->lineEdit_port->text();
    QString save_path = ui->lineEdit_save_dir->text();
    QFile cgFile("./tcpserver.config");
    QFile file("./tcpserver.config");
    if (cgFile.open(QIODevice::ReadOnly)) {//返回类型bool类型指示是否打开该文件
        QByteArray bytedata = cgFile.readAll();//字节数组类型数据
        QString strdata = bytedata.toStdString().c_str();//将字节数组转换成字符串类型->char*类型的字符
        QStringList strList = strdata.split("\r\n");
        strList[0] = ip;
        strList[1] = port;
        strList[2] = save_path;
        cgFile.close();
        if(file.open(QIODevice::WriteOnly)) {
            for(int i{0}; i < strList.size() - 1; ++i) {
                QByteArray data = strList[i].toStdString().c_str();
                data += "\r\n";
                file.write(data);
            }
            file.close();
            QMessageBox::information(this, "Change config file", "Successfully changed config file!");
        } else {
            QMessageBox::critical(this, "Change config file", "Failed to change config file!");
        }
    } else {
        QMessageBox::critical(this, "Change config file", "Failed to change config file!");
    }
}


















