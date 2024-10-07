#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "operatewidget.h"
#include <QInputDialog>
#include <QKeyEvent>
#include <windows.h>
#include <QBitmap>
#include <QHostInfo>

TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)//调用父对象的有参构造函数
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);//构造函数，构造函数执行之后会显示出界面
    is_online = false;
    // resize(1000,500);
    loadConfig();
    //showConnect();不使用槽函数的形式
    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(showConnect()));//使用槽函数的形式，一连接上就发送信息
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(recvMsg()));//使用槽函数的形式，时刻准备接受数据
    //连接服务器
    connect(&m_tcpSocket, SIGNAL(disconnected()), this, SLOT(clientoffline())); //关联客户端连接 一旦断开连接就执行掉线函数内容
    connect(ui->name_le, SIGNAL(returnPressed()), this, SLOT(processRet()));
    connect(ui->pwd_le, SIGNAL(returnPressed()), this, SLOT(processRet()));
    //设置光标聚焦
    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);//连接服务器
    ui->pwd_le->installEventFilter(this);//绑定键盘回车登录
//    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
//   m_tcpSocket.connectToHost(QHostAddress::Any, m_usPort);
    downloadS = false;
    file_rev_size = 0;
    file_total_size = 0;
    ui->ip_label->hide();
    ui->Ip_latext->hide();
    ui->port_lable->hide();
    ui->port_latext->hide();
    ui->sure_to_change_ip_port_pb->hide();
    ui->upload_param_latex->hide();
    ui->Upload_param_lable->hide();
    setLayout(ui->verticalLayout);
    //this->setFixedSize(450, 220);
    this->setFixedSize(520, 260);
    QPixmap pixmap(":/BG.bmp");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    palette.setColor(QPalette::WindowText, Qt::white);
    ui->pwd_lab->setPalette(palette);
    ui->name_lab->setPalette(palette);
    ui->ip_label->setPalette(palette);
    ui->port_lable->setPalette(palette);
    ui->Upload_param_lable->setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    this->setWindowTitle("Welcome to use Raydisc!");
    setWindowIcon(QPixmap(":/netdisc.ico"));
    ui->name_le->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->pwd_le->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->login_pb->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->log_off->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->logoff_pb->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->register_pb->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->change_ip_port_pb->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->Ip_latext->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->port_latext->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->upload_param_latex->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ui->sure_to_change_ip_port_pb->setStyleSheet("background-color: rgba(255, 255, 255,150);");
    ///多线程处理
    ui->name_le->setFocus();
    ///获取本机的IP地址
    QString hostNme = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostNme);
    auto addList = hostInfo.addresses();
    if (addList.isEmpty()) {
        Q_ASSERT(false);
    }
    Mydownip = addList.back().toString();
    qDebug() << "本机ip：" << Mydownip;
}
TcpClient::~TcpClient()
{
    delete ui;
}
void TcpClient::loadConfig()   //加载配置文件
{
    QFile configfile("./tcpclient.config");
    if (configfile.open(QIODevice::ReadOnly)) {//返回类型bool类型指示是否打开该文件
        QByteArray bytedata = configfile.readAll();//字节数组类型数据 输出字节类型数组
        QString strdata = bytedata.toStdString().c_str();//将字节数组转换成字符串类型->char*类型的字符
        //qDebug() << strdata;//打印配置信息
        //strdata.replace("\r\n", " ");//将字符串中的\r\n替换成" "
        //qDebug() << strdata;//打印配置信息
        QStringList strList = strdata.split("\r\n");//转换成字符串列表
        /*for (auto str : strList) {
            qDebug() << "-->" << str;
        }*/
        m_strIP = strList[0];//.at(0)
        m_usPort = strList[1].toUShort();//转换为短整型
        m_upload_param = strList[2].toUShort();//上传参数
        m_strCurpath = strList[3].toStdString().c_str();//路径
        ui->Ip_latext->setText(m_strIP);
        ui->port_latext->setText(strList[1]);
        ui->upload_param_latex->setText(strList[2]);
        qDebug() << "IP: " << m_strIP ;
        qDebug() << "PORT: " << m_usPort;
        qDebug() << "DIR: " << m_strCurpath;
        configfile.close();
    } else { //打不开报错
        QMessageBox::critical(this, "open configfaile", "Failed to open configfile");
    }
}
TcpClient &TcpClient::getInstance()
{
//    static TcpClient instance;//获取 对象实例
//    return instance;
    static TcpClient *instance = new TcpClient;//获取 对象实例
    return *instance;
}
QTcpSocket &TcpClient::getTcpSocket()   //获取当前通信的socket
{
    return m_tcpSocket;
}
void TcpClient::showConnect()   //信息框标题："Try to connected servive"，内容:"Connecting service successfully!"
{
    QString ip = m_tcpSocket.peerAddress().toString();
    qint64 port = m_tcpSocket.peerPort();
    QString ipDate = QString("[ip=%1 port=%2] 建立好连接了！！").arg(ip).arg(port);
    qDebug() << "进程id:" << GetCurrentThreadId();
    qDebug() << ipDate;
    if(!is_online) {
        Operatewidget::getInstance().change_connected();
        PDU *mypdu = new PDU();//没有发送实际消息 传递的是用户名和密码
        mypdu = mkPDU_1(0, ENUM_MSG_TYPE_RELOGIN_REQUEST);
        strncpy(mypdu->caData, Usr_name.toStdString().c_str(), 32); //拷贝用户名 前32个字节
        strncpy(mypdu->caData + 32, Usr_psd.toStdString().c_str(), 32); //拷贝密码 后32个字节
        m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen); //通过次socket发送数据给服务端
        delete mypdu;
        mypdu = NULL;
    }
    is_online = true;
    QMessageBox::information(this, "Try to connected server", "Connecting server successfully!");
}

void TcpClient::recvMsg()
{
    if(!downloadS) {
        qDebug() << m_tcpSocket.bytesAvailable();
        uint uiPDULen{0};
        //uiPDULen是一个uint类型 先读取sizeof(uint)的大小
        m_tcpSocket.read((char *)&uiPDULen, sizeof(uint)); //PDULen的具体长度也是一个16位无符号！先从PUD中占用一个uint接受PDULEN作为具体的长度
        uint uiMsgLen = uiPDULen - sizeof(PDU);//消息的大小
        //    mypdu = mypdu->mkPDU(uiMsgLen);//创建用于接受消息的单元
        PDU *mypdu = mkPDU(uiMsgLen);
        //uiPDUlen已经赋值
        m_tcpSocket.read((char *)mypdu + sizeof(uint), mypdu->uiPDULen - sizeof(uint)); //read参数后面跟数据 和数据长度之前已经接收了一个uint的数据 后面开始便宜uint的大小即可
        //qDebug()<<mypdu->uiMsgType<<(char*)mypdu->caMsg;//
        //收到数据判断消息类型
        switch(mypdu->uiMsgType) {
            case ENUM_MSG_TYPE_REGIST_RESPOND: { //注册
                    if(0 == strcmp(mypdu->caData, REGISTER_OK)) {
                        QMessageBox::information(this, "Regist", REGISTER_OK);
                    } else if(0 == strcmp(mypdu->caData, REGISTER_FAILED)) {
                        QMessageBox::warning(this, "Regist", REGISTER_FAILED);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_LOGIN_RESPOND: { //登陆成功
                    is_online = true;
                    if(0 == strcmp(mypdu->caData, LOGIN_OK)) {
                        m_strCurpath += getName() + "/"; //用户文件夹 登陆的时候获取 以后得子文件夹均在此文件夹中操作
                        m_strRootpath = m_strCurpath;
                        qDebug() << "root_path" << m_strRootpath;
                        qDebug() << "名称字符串长度" << sizeof(getName());
                        QMessageBox::information(this, "Login", LOGIN_OK);
                        this->hide();//隐藏登录界面
                        Operatewidget::getInstance().change_connected();
                        Operatewidget::getInstance().show();//显示操作界面 单例形式
                        Operatewidget::getInstance().getusrfir()->on_pushButton_flushfries_clicked();//自动刷新好友列表 和文件
                        Operatewidget::getInstance().getusrfir()->Input_setfocus();//设置光标位置
                    } else if(0 == strcmp(mypdu->caData, LOGIN_FAILED)) {
                        QMessageBox::warning(this, "Login", LOGIN_FAILED);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_LOGOFF_RESPOND: { //登出
                    if(0 == strcmp(mypdu->caData, LOGOFF_OK)) {
                        QMessageBox::information(this, "Logoff", LOGOFF_OK);
                    } else if(0 == strcmp(mypdu->caData, LOGOFF_FAILED)) {
                        QMessageBox::warning(this, "Logoff", LOGOFF_FAILED);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_DESTORY_RESPOND: { //注销
                    if(0 == strcmp(mypdu->caData, DESTORY_OK)) {
                        QMessageBox::information(this, "Destory", DESTORY_OK);
                    } else if(0 == strcmp(mypdu->caData, DESTORY_FAILED)) {
                        QMessageBox::warning(this, "Destory", DESTORY_FAILED);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND: { //处理获取显示所有在线用户的
                    Operatewidget::getInstance().getusrfir()->show_ALLONLINE_usr(mypdu);
                    //        QMessageBox::information(this,"show result","All online person have been showed!");
                    break;
                }
            case ENUM_MSG_TYPE_SEARCH_USR_RESPOND: { //搜索好友回复
                    QMessageBox::information(this, "search result", mypdu->caData);
                    //Operatewidget::getInstance().getusrfir()->show_search_usr(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_ADD_USR_RESPOND: { //添加好友回复
                    qDebug() << "RESULT:" << mypdu->caData;
                    QMessageBox::information(this, "add result", mypdu->caData);
                    break;
                }
            case ENUM_MSG_TYPE_ADD_USR_REQUEST: { //由另一个用户发来的添加好友请求
                    char name_0[32] = "";//存放申请者的名字
                    memcpy(name_0, mypdu->caData, 32);
                    //messgebox可用于返回值
                    int ret = QMessageBox::information(this, "Aadd friend request", QString("%1 want to add you as friend!").arg(name_0), QMessageBox::Yes, QMessageBox::No);
                    qDebug() << "同意或者拒绝的结果：" << ret;
                    if(ret == QMessageBox::Yes) { //产生pdu发送给服务器 服务器视情况修改数据库
                        mypdu->uiMsgType = ENUM_MSG_TYPE_YES_ADD_USR_REQUEST;//后面32位放了自己的名字
                        m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen);
//                Sleep(100);
//                Operatewidget::getInstance().getusrfir()->updatefriendlist(mypdu);//同意后刷新自己的好友列表
                    } else if(ret == QMessageBox::No) {
                        mypdu->uiMsgType = ENUM_MSG_TYPE_NO_ADD_USR_REQUEST;//后面32位放了自己的名字
                        m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_NO_ADD_USR_RESPOND: { //你申请加对方好友对方拒绝 并给予回复
                    char name_0[32] = "";//存放申请者的名字
                    memcpy(name_0, mypdu->caData + 32, 32); //想要添加者的名字
                    qDebug() << "添加好友被拒绝！";
                    QMessageBox::information(this, "Aadd friend request", QString("%1 has refused the request to add as friends!").arg(name_0));
                    break;
                }
            case ENUM_MSG_TYPE_YES_ADD_USR_RESPOND: { //你申请加对方好友对方同意 并给予回复
                    char name_0[32] = "";//存放申请者的名字
                    memcpy(name_0, mypdu->caData + 32, 32); //想要添加者的名字
                    qDebug() << "对方同意添加好友！";
                    Operatewidget::getInstance().getusrfir()->updatefriendlist(mypdu);
                    QMessageBox::information(this, "Aadd friend request", QString("%1 has receive the request to add as friends,you are friends!").arg(name_0));
                    break;
                }
            case ENUM_MSG_TYPE_FLUSH_FRI_RESPOND: { //刷新好友回复
                    Operatewidget::getInstance().getusrfir()->updatefriendlist(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SDELETE_FRI_REQUSET: {
                    QString data = QString("Sure to delete %1?").arg(mypdu->caData + 32); //提示是否确定删除该好友
                    int res = QMessageBox::information(this, "delete friend", data, QMessageBox::Yes, QMessageBox::No);
                    if(res == QMessageBox::Yes) {
                        TcpClient::getInstance().getTcpSocket().write((char *)mypdu, mypdu->uiPDULen);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_SDELETE_FRI_RESPOND: { //确认删除操作
                    QString data = QString("Successfully delete friend %1!").arg(mypdu->caData + 32); //提示是否确定删除该好友
                    // Operatewidget::getInstance().getusrfir()->on_pushButton_flushfries_clicked();
                    Operatewidget::getInstance().getusrfir()->updatefriendlist(mypdu);//刷新自己的列表
                    QMessageBox::information(this, "delete friend", data);
                    break;
                }
            case ENUM_MSG_TYPE_GROUP_CHAT_RESPOND: { //群聊操作
                    qDebug() << "收到消息！";
                    Operatewidget::getInstance().getusrfir()->show_group_msg(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST: { //私聊操作
                    qDebug() << "私聊请求";
                    Operatewidget::getInstance().getusrfir()->show_pchat_msg(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_CREATE_DIR_FAILED_RESPOND: { //服务器创建wjj失败！
                    QMessageBox::information(this, "creare folder", "Failed to create folder,exist same name folder!");
                    break;
                }
            case ENUM_MSG_TYPE_CREATE_DIR_SUCCESS_RESPOND: { //服务器创建wjj成功
                    QMessageBox::information(this, "creare folder", QString("Successfully create folder %1!").arg(mypdu->caData + 32));
                    Operatewidget::getInstance().get_usrfiles()->on_pushButton_flush_folders_clicked();
                    break;
                }
            case ENUM_MSG_TYPE_CREATE_ROOT_DIR_RESPOND: { //成功创建根路径
                    QMessageBox::information(this, "creare folder", "Root path don't exist,successfully create root path,please create your own path!");
                    Operatewidget::getInstance().get_usrfiles()->on_pushButton_flush_folders_clicked();
                    break;
                }
            case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND: { //刷新文件目录回复
                    Operatewidget::getInstance().get_usrfiles()->Flush_folders(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST: { //服务器发来确认删除请求
                    QString data = QString("Sure to delete file or folder %1 ?").arg(mypdu->caData + 32); //提示是否确定删除该好友
                    int res = QMessageBox::information(this, "delete file of folder", data, QMessageBox::Yes, QMessageBox::No);
                    if(res == QMessageBox::Yes) {
                        TcpClient::getInstance().getTcpSocket().write((char *)mypdu, mypdu->uiPDULen);
                    }
                    break;
                }
            case ENUM_MSG_TYPE_DELETE_FILE_RESPOND: { //删除文件夹 或者 文件
                    if(mypdu->uiMsgLen) { //删除成功
                        QMessageBox::information(this, "delete file or folder", QString("Successfully delete %1").arg(mypdu->caData + 32));
                        Operatewidget::getInstance().get_usrfiles()->on_pushButton_flush_folders_clicked();
                    } else { //删除失败
                        QMessageBox::information(this, "delete file or folder", QString("Failed to delete %1, may need administrators privileges!").arg(mypdu->caData + 32));
                    }
                    break;
                }
            case ENUM_MSG_TYPE_RENAME_DIR_RESPOND: { //重命名文件夹或者文件
                    char filename[32];
                    memcpy(filename, mypdu->caData, 32);
                    if(mypdu->uiMsgLen) { //重命名成功
                        QMessageBox::information(this, "rename file or folder", QString("Successfully rename %1 to %2!").arg(filename).arg(mypdu->caData + 32));
                        Operatewidget::getInstance().get_usrfiles()->on_pushButton_flush_folders_clicked();
                    } else { //删除失败
                        QMessageBox::information(this, "rename file or folder", QString("Failed to rename %1 to %2! may need administrators privileges!").arg(filename).arg(mypdu->caData + 32));
                    }
                    break;
                }
            case ENUM_MSG_TYPE_upload_FILE_RESPOND: {
                    if(mypdu->uiMsgLen) { // 上传文件成功
                        QMessageBox::information(this, "upload file", QString("Successfully upload %1!").arg(mypdu->caData));
                        Operatewidget::getInstance().get_usrfiles()->on_pushButton_flush_folders_clicked();
                    } else { //上传文件
                        QMessageBox::information(this, "upload file", QString("Failed to upload %1!").arg(mypdu->caData));
                    }
                    break;
                }
            case ENUM_MSG_TYPE_download_FILE_REQUEST: { //下载文件操作
                    char file_size[32];
                    memcpy(file_size, mypdu->caData, 32);
                    m_file_name = mypdu->caData + 32;
                    file_total_size = QString(file_size).toLongLong();
                    qDebug() << "下载的文件为：" << m_file_name;
//                    qDebug() << "下载文件的大小为：" << file_total_size;
                    qDebug() << "要保存的路径为：" << m_save_path;
//                    downprogress.setWindowTitle("Download " + m_file_name + "(" + get_file_size(file_total_size) + ")");	//窗口标题
//                    downprogress.show();
                    //  downloadS = true;
                    file_rev_size = 0;//下载开始前file_rev_size置0;
                    //  Operatewidget::getInstance().get_usrfiles()->start_listen();//服务端开始监听
                    qDebug() << "客户端下载服务器开始监听！";
                    //服务端开始接收数据
                    //Operatewidget::getInstance().get_usrfiles()->handle_down_load_file();
                    break;
                }
            case ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE: {
                    file_total_size = QString(mypdu->caData).toLongLong();
                    qDebug() << "当前文件大小" << file_total_size;
                    break;
                }
            case ENUM_MSG_TYPE_SHOW_FD_MSG_RESPOND: { //显示文件信息
                    show_file_msg(mypdu);
                    break;
                }
            case ENUM_MSG_TYPE_SHARE_FILE_REQUEST: { //你是否接受别人分享的文件
                    char name_0[32] = "";//对方的名字
                    memcpy(name_0, mypdu->caData, 32);
                    char filename[32];
                    char nums[32];
                    memcpy(nums, mypdu->caData + 32, 32);
                    int cnts = QString(nums).toInt();
                    memcpy(filename, (char *)mypdu->caMsg + cnts * 32, 32);
                    QString from_file_path = (char *)mypdu->caMsg + (cnts + 1) * 32;
                    QString to_file_path = m_strCurpath + filename;
                    qDebug() << "起始路径：" << from_file_path;
                    qDebug() << "结束路径：" << to_file_path;
                    //messgebox可用于返回值
                    int ret = QMessageBox::information(this, "shared file request", QString("%1 want to share you %2").arg(name_0).arg(filename), QMessageBox::Yes, QMessageBox::No);
                    if(ret == QMessageBox::Yes) { //产生pdu发送给服务器 服务器视情况修改数据库
                        PDU *respdu = mkPDU_1(400, ENUM_MSG_TYPE_SHARE_FILE_RESPOND);
                        memcpy(respdu->caData, Usr_name.toStdString().c_str(), 32);
                        memcpy(respdu->caData + 32, name_0, 32);
                        memcpy((char *)respdu->caMsg, to_file_path.toStdString().c_str(), 100); //前100是curpath 后200是filepath
                        memcpy((char *)respdu->caMsg + 100, from_file_path.toStdString().c_str(), 200); //最后100是刷新的curpath
                        memcpy((char *)respdu->caMsg + 300, m_strCurpath.toStdString().c_str(), 100);
                        m_tcpSocket.write((char *)respdu, respdu->uiPDULen);
                        delete respdu;
                        respdu = NULL;
                    } else {
                        PDU *respdu = mkPDU_1(0, ENUM_MSG_TYPE_SHARE_FILE_RE_RESPOND);
                        memcpy(respdu->caData, Usr_name.toStdString().c_str(), 32);
                        memcpy(respdu->caData + 32, name_0, 32);
                        m_tcpSocket.write((char *)respdu, respdu->uiPDULen);
                        delete respdu;
                        respdu = NULL;
                    }
                    break;
                }
            case ENUM_MSG_TYPE_SHARE_FILE_RESPOND: {
                    QMessageBox::information(this, "shared file", QString("%1 reject your share file!").arg(mypdu->caData));
                    break;
                }
            case ENUM_MSG_TYPE_SHARE_FILE_RE_RESPOND: {
                    QMessageBox::information(this, "shared file", QString("%1 accept your share file!").arg(mypdu->caData));
                    break;
                }
            case ENUM_MSG_TYPE_MOVE_FILE_REQUEST: { //无法将文件（夹）移动到文件中去
                    QMessageBox::warning(this, "move file", "can't move to a file!");
                    break;
                }
            case ENUM_MSG_TYPE_MOVE_FILE_RESPOND: { //移动文件夹回复
                    if(!mypdu->uiMsgLen) {
                        QMessageBox::warning(this, "move file", QString("Failed to move file!").arg(mypdu->caData));
                    } else {
                        QMessageBox::information(this, "move file", QString("Successfully move file!").arg(mypdu->caData));
                        Operatewidget::getInstance().get_usrfiles()->on_pushButton_flush_folders_clicked();
                    }
                    break;
                }
            case ENUM_MSG_TYPE_REDOWNLOAD: {//下载文件端口号被占用
                    QMessageBox::information(this, "download file", "Your download port has been occupied,"
                                             "please try to download again!");
                    break;
                }
            case ENUM_MSG_TYPE_WZQ: {
                    char name_0[32];
                    memcpy(name_0, mypdu->caData, 32);
                    QString data = QString("%1 want to play banggo with you, do you agree ?").arg(name_0); //提示是否接受五子棋游戏
                    int res = QMessageBox::information(this, "Banggo", data, QMessageBox::Yes, QMessageBox::No);
                    if(res == QMessageBox::Yes) {
                        PDU *respdu = mkPDU_2(1, ENUM_MSG_TYPE_WZQ_A, mypdu->caData + 32, name_0, ""); //同意长度为1
                        TcpClient::getInstance().getTcpSocket().write((char *)respdu, respdu->uiPDULen);
                        delete respdu;
                    } else if(res == QMessageBox::No) {
                        PDU *respdu = mkPDU_2(0, ENUM_MSG_TYPE_WZQ_A, mypdu->caData + 32, name_0, ""); //不同意长度为0
                        TcpClient::getInstance().getTcpSocket().write((char *)respdu, respdu->uiPDULen);
                        delete respdu;
                    }
                    break;
                }
            case ENUM_MSG_TYPE_WZQ_A: {
                    char name_0[32];
                    memcpy(name_0, mypdu->caData, 32);
                    if(mypdu->uiMsgLen == 0) {
                        QString data = QString("%1 refuse to play banggo with you!").arg(name_0); //提示是否接受五子棋游戏
                        QMessageBox::information(this, "Banggo", data);
                    } else if(mypdu->uiMsgLen == 1) {
                    }
                }
                delete mypdu;
                mypdu = NULL;
                break;
            default:
                break;
        }
    } else { //处于下载状态
        // handle_download_file();
    }
}


//void TcpClient::on_send_p_clicked()
//{
//    QString strMsg = ui->lineEdit->text();
//    if(!strMsg.isEmpty()){
//        mypdu = mypdu->mkPDU(strMsg.size() + 1);
//        //mypdu = PDU(strMsg.size() + 1);
//        mypdu->uiMsgType = 8802;//消息类型
//        memcpy(mypdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
//        //qDebug()<<(char*)(mypdu->caMsg)<<mypdu->uiPDULen;
//        m_tcpSocket.write((char*)mypdu,mypdu->uiPDULen);
//        mypdu = NULL;
//    }else{
//        QMessageBox::information(this,"Try to send message","Failed to send message!");
//    }
//}
void TcpClient::on_login_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    Usr_name = strName;
    Usr_psd = strPwd;
    if(!is_online) {
        m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
    } else {
        if(!strName.isEmpty() && !strPwd.isEmpty()) { //都为非空正常注册
            PDU *mypdu = new PDU();//没有发送实际消息 传递的是用户名和密码
            mypdu = mkPDU_1(0, ENUM_MSG_TYPE_LOGIN_REQUEST);
            strncpy(mypdu->caData, strName.toStdString().c_str(), 32); //拷贝用户名 前32个字节
            strncpy(mypdu->caData + 32, strPwd.toStdString().c_str(), 32); //拷贝密码 后32个字节
            m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen); //通过次socket发送数据给服务端
            delete mypdu;
            mypdu = NULL;
        } else {
            QMessageBox::critical(this, "Try to login", "Failed to login:username or password is empty");
        }
    }
}
void TcpClient::on_register_pb_clicked()   //注册按钮
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty() && !strPwd.isEmpty()) { //都为非空正常注册
        //mypdu = new PDU(0);//使用构造函数
        //mypdu = mypdu->mkPDU(0);//没有发送实际消息 传递的是用户名和密码
        //mypdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;//注册请求
        PDU *mypdu = new PDU();
        mypdu = mkPDU_1(0, ENUM_MSG_TYPE_REGIST_REQUEST);
        strncpy(mypdu->caData, strName.toStdString().c_str(), 32); //拷贝用户名 前32个字节
        strncpy(mypdu->caData + 32, strPwd.toStdString().c_str(), 32); //拷贝密码 后32个字节
        m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen);
        delete mypdu;
        mypdu = NULL;
    } else {
        QMessageBox::critical(this, "Try to register", "Failed to register:username or password is empty");
    }
}
void TcpClient::on_log_off_clicked()   //注销
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty() && !strPwd.isEmpty()) { //都为非空正常注册
        PDU *mypdu = new PDU();
        mypdu = mkPDU_1(0, ENUM_MSG_TYPE_DESTORY_REQUEST);
        strncpy(mypdu->caData, strName.toStdString().c_str(), 32); //拷贝用户名 前32个字节
        strncpy(mypdu->caData + 32, strPwd.toStdString().c_str(), 32); //拷贝密码 后32个字节
        m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen); //发送请求到服务端
        delete mypdu;
        mypdu = NULL;
    } else {
        QMessageBox::critical(this, "Try to destory this id", "Failed to destory:username or password is empty");
    }
}
void TcpClient::on_logoff_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty() && !strPwd.isEmpty()) { //都为非空正常注册
        PDU *mypdu = new PDU();//没有发送实际消息 传递的是用户名和密码
        mypdu = mkPDU_1(0, ENUM_MSG_TYPE_LOGOFF_REQUEST);
        strncpy(mypdu->caData, strName.toStdString().c_str(), 32); //拷贝用户名 前32个字节
        strncpy(mypdu->caData + 32, strPwd.toStdString().c_str(), 32); //拷贝密码 后32个字节
        m_tcpSocket.write((char *)mypdu, mypdu->uiPDULen); //发送请求到服务端
        delete mypdu;
        mypdu = NULL;
    } else {
        QMessageBox::critical(this, "Try to logoff", "Failed to logoff:username or password is empty");
    }
}
bool TcpClient::clientoffline()
{
    qDebug() << "wellcome your next login!";
    is_online = false;
    Operatewidget::getInstance().set_unonline();
    return is_online;
}
QString TcpClient::getName()
{
    return Usr_name;
}
QString TcpClient::getsystemtime()
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    QString data = QString("%0/%1/%2 - %3:%4:%5").arg(time.wYear).arg(time.wMonth).arg(time.wDay).arg(time.wHour).arg(time.wMinute).arg(time.wSecond);
    printf("%04d/%02d/%02d %02d:%02d:%02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    return data;
}
QString TcpClient::get_rootpath()
{
    return m_strRootpath;
}
QString TcpClient::get_curpath()
{
    return m_strCurpath;
}
void TcpClient::change_curpath(QString name)
{
    if(name == "") { //pop
        QString file_path = TcpClient::getInstance().get_curpath();
        qDebug() << "cfile path" << file_path;
        file_path.chop(1);//先pop掉最后一个/
        int i = file_path.size() - 1;
        while(i--) {
            file_path.chop(1);//连续从尾部删除直至出现 '/';
            if(file_path[i] == '/') {
                break;
            }
        }
        m_strCurpath = file_path;
    } else {
        m_strCurpath += name + "/";
    }
}
void TcpClient::change_download_path(QString path)
{
    m_save_path = path;
}
QString TcpClient::get_download_path()
{
    return m_save_path;
}
void TcpClient::set_file(QString file_path)   //设置下载的文件初始化之
{
    m_file.setFileName(file_path);
//    m_file.open(QIODevice::WriteOnly);
}
///原下载文件单线程处理函数
//void TcpClient::handle_download_file()
//{
//    QByteArray buff = TcpClient::getInstance().getTcpSocket().readAll();
//    file_rev_size += buff.size();
////    qDebug()<<"单次传输大小："<<buff.size();
//    m_file.write(buff);
////     qDebug()<<"文件大小："<<file_total_size;
////     qDebug()<<"已经接收的大小"<<file_rev_size;
//    downprogress.setValue(double(file_rev_size) / file_total_size * 100);
//    if(file_total_size == file_rev_size) { //文件下载完毕
//        downprogress.hide();
//        m_file.close();
//        downloadS = false;
//        QMessageBox::information(this, "download file", QString("Successfully download file %1").arg(m_file_name));
//        file_rev_size = 0;//下载完后file_rev_size置0；
//    } else if(file_total_size < file_rev_size) { //下载失败 总的数据已经小于当前的数据了！
//        downprogress.hide();
//        m_file.close();
//        downloadS = false;
//        QMessageBox::information(this, "download file", QString("Failed to download file %1").arg(m_file_name));
//    }
//}
void TcpClient::handle_download_file()
{
    QByteArray buff = TcpClient::getInstance().getTcpSocket().readAll();
    file_rev_size += buff.size();
//    qDebug()<<"单次传输大小："<<buff.size();
    m_file.write(buff);
//     qDebug()<<"文件大小："<<file_total_size;
//     qDebug()<<"已经接收的大小"<<file_rev_size;
    if(file_total_size == file_rev_size) { //文件下载完毕
        m_file.close();
        downloadS = false;
        QMessageBox::information(this, "download file", QString("Successfully download file %1").arg(m_file_name));
        file_rev_size = 0;//下载完后file_rev_size置0；
    } else if(file_total_size < file_rev_size) { //下载失败 总的数据已经小于当前的数据了！
        m_file.close();
        downloadS = false;
        QMessageBox::information(this, "download file", QString("Failed to download file %1").arg(m_file_name));
    }
}
void TcpClient::show_file_msg(PDU *mypdu)
{
    FileInfo *fileinfo = (FileInfo *)mypdu->caMsg;
    QString file_name = fileinfo->caFiamename;
    QString file_size = fileinfo->caSize;
    QString create_time = fileinfo->createTime;
    QString last_visited = fileinfo->lastVisitedtime;
    QString last_modify = fileinfo->lastmodify;
    qDebug() << file_name << file_size << create_time << last_visited << last_modify;
    bool is_file = fileinfo->is_File;
    bool is_dir = fileinfo->is_folder;
    bool isReadable = fileinfo->is_canread;	//判断QFileInfo对象的读属性
    bool isWritable = fileinfo->is_canwrie;	//判断QFileInfo对象的写属性
    bool isHidden = fileinfo->is_hidden;		//判断QFileInfo对象的隐藏属性
    bool isExecutable = fileinfo->is_exe;//判断QFileInfo对象的可执行属性
//    FileInfo* rfiinfo = mkFileInfo_1(file_name,is_file,create_time,last_visited,last_modify,is_file,is_dir,
//                                     isReadable,isWritable,isHidden,isExecutable);
    myfieinfo *myfinfo = new myfieinfo;
    myfinfo->upgrade_finfo(file_name, file_size, create_time, last_visited, last_modify,
                           is_file, is_dir, isReadable, isWritable, isHidden, isExecutable);
    myfinfo->show();
}
QString TcpClient::get_file_size(qint64 f_size)
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
bool TcpClient::reconnect()
{
    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
    return is_online;
}


void TcpClient::processRet()
{
    QLineEdit *returnPressedSender = (QLineEdit *)sender();
    if(ui->pwd_le->text() != "" && ui->name_le->text() != "") {
        on_login_pb_clicked();
        return;
    }
    if (returnPressedSender == ui->name_le) {
        ui->pwd_le->setFocus();
    }
    if (returnPressedSender == ui->pwd_le) {
        ui->name_le->setFocus();
    }
}


bool TcpClient::eventFilter(QObject *object, QEvent *event)
{
//    if (object == ui->pwd_le && event->type() == QEvent::KeyPress) {
//        QKeyEvent *e = dynamic_cast <QKeyEvent *> (event);
//        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) { //步骤三
//            on_login_pb_clicked();//步骤四 绑定槽函数
//            return true;
//        }
//    }
//    if (object == ui->name_le && event->type() == QEvent::KeyPress) {
//        QKeyEvent *e = dynamic_cast <QKeyEvent *> (event);
//        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) { //步骤三
//            ui->pwd_le->setFocus();
//            return true;
//        }
//    }
    return QWidget::eventFilter(object, event);
}
void TcpClient::on_change_ip_port_pb_clicked()
{
    if(ui->ip_label->isHidden() && ui->Ip_latext->isHidden() && ui->port_lable->isHidden() &&
            ui->port_latext->isHidden() && ui->sure_to_change_ip_port_pb->isHidden() && ui->upload_param_latex->isHidden()
            && ui->Upload_param_lable->isHidden()) {
        ui->ip_label->show();
        ui->Ip_latext->show();
        ui->port_lable->show();
        ui->port_latext->show();
        ui->sure_to_change_ip_port_pb->show();
        ui->upload_param_latex->show();
        ui->Upload_param_lable->show();
    } else {
        ui->ip_label->hide();
        ui->Ip_latext->hide();
        ui->port_lable->hide();
        ui->port_latext->hide();
        ui->sure_to_change_ip_port_pb->hide();
        ui->upload_param_latex->hide();
        ui->Upload_param_lable->hide();
    }
}
void TcpClient::on_sure_to_change_ip_port_pb_clicked()   //确认更改ip port并存入config配置文件
{
    QString ip = ui->Ip_latext->text(), port = ui->port_latext->text(), upload_param = ui->upload_param_latex->text();
    QFile configfile("./tcpclient.config");
    QFile file("./tcpclient.config");
    if(configfile.open(QIODevice::ReadOnly)) {
        QString cgdata = configfile.readAll();
        QStringList cglist = cgdata.split("\r\n");
        cglist[0] = ip;
        cglist[1] = port;
        cglist[2] = upload_param;
        for(auto x : cglist) {
            qDebug() << x;
        }
        configfile.close();
        if(file.open(QIODevice::WriteOnly)) {
            for(int i{0}; i < cglist.size() - 1; ++i) {
                QByteArray data = cglist[i].toStdString().c_str();
                data += "\r\n";
                qDebug() << "写入的内容：" << data;
                file.write(data);
            }
            file.close();
            //更改完后重新连接
            m_strIP = cglist[0];
            m_usPort = cglist[1].toUShort();
            m_upload_param = cglist[2].toUShort();
            QMessageBox::information(this, "try to change ip and port", "Successfully changed ip and port!");
            Sleep(100);
            m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);//重新尝试连接
        } else {
            QMessageBox::warning(this, "try to change ip and port", "Failed to change ip and port!");
        }
    } else {
        QMessageBox::warning(this, "try to change ip and port", "Failed to change ip and port!");
    }
}
QString TcpClient::get_m_ip()
{
    return m_strIP;
}
int TcpClient::get_m_port()
{
    return m_usPort;
}
QString TcpClient::get_downloadfile_name()
{
    return m_file_name;
}
qint64 TcpClient::get_download_file_total_size()
{
    return file_total_size;
}

void TcpClient::change_file_total_size(qint64 size)
{
    file_total_size = size;
}

QString TcpClient::get_mydownip()
{
    return Mydownip;
}

qint16 TcpClient::get_m_upload_param()
{
    return m_upload_param;
}
bool TcpClient::get_onlienstate()
{
    return is_online;
}
