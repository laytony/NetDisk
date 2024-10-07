#include "usrfri.h"
#include "ui_usrfri.h"
#include <windows.h>
#include <QInputDialog>//用于输入
#include <QKeyEvent>
#include <QApplication>
#include <QDir>
#include <apiset.h>
#include <QDateTime>
#include <QBitmap>


UsrFri::UsrFri(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrFri)
{
    ui->setupUi(this);
    ///
    //m_onlie = new Onlie;
    //ui->LastOUT->addWidget(m_onlie);
    //m_onlie->hide();
//    pchat = new Privatechat;
//    pchat->hide();
    setAcceptDrops(true);//拖动文件 到发送框
    setLayout(ui->LastOUT);
    ui->ADD_PB->hide();
    ui->ListWidget->hide();
    ui->m_showMSG_single->hide();
    ui->ListWidget->setFixedSize(490, 200);
    ui->lineEdit_INPUTMESG->setFixedSize(490, 120);
    ui->m_showMSG->setFixedWidth(330);
    ui->listWidget_shouFriendList->setFixedWidth(160);
    this->setWindowTitle(TcpClient::getInstance().getName());
    this->setFixedSize(600, 800);
    connect(ui->listWidget_shouFriendList, SIGNAL(itemDoubleClicked(QListWidgetItem *)) //双击好友触发私聊效果
            , this, SLOT(doubleclicked_pchat(QListWidgetItem *)));
    ui->lineEdit_INPUTMESG->installEventFilter(this);//绑定键盘回车发送信息
    input_img_path = "";
    dltimer = new QTimer();
    connect(dltimer, &QTimer::timeout, this, &UsrFri::show_lt_img);
    //按钮透明度
    ///
    QPixmap pixmap(":/OPBG.bmp");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    ui->m_showMSG->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ui->listWidget_shouFriendList->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ui->lineEdit_INPUTMESG->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ui->ListWidget->setStyleSheet("background-color: rgba(255, 255, 255,60);");
//    ui->m_showMSG->setMask(pixmap1.mask());
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    ui->pushButton_wqz->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_sendMSG->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_deletefri->setStyleSheet("background-color: rgb(255, 255, 255,175);");
    ui->pushButton_flushfries->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_searchusrs->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_private_chat->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_showonlineusrs->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_private_chat_close->setStyleSheet("background-color: rgba(255, 255, 255,175);");
}

void UsrFri::show_ALLONLINE_usr(PDU *pdu) //显示所有在线用户
{
    if(pdu == NULL) {
        return ;
    }
    ui->ListWidget->clear();
    uint uiSize = pdu->uiMsgLen / 32; //消息总长度除以消息的大小32无符号
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap(":/all_usr.ico"));
    item->setText("在线用户");
    QFont font("黑体");
    item->setFont(font);
    ui->ListWidget->addItem(item);
    for(uint i{0}; i < uiSize; ++i) { //类型匹配
        char caTmp[32] = "";
        memcpy(caTmp, (char *)pdu->caMsg + 32 * i, 32); //每隔32位取一次输出；
        QListWidgetItem *t = new QListWidgetItem;
        if(QString(caTmp) == TcpClient::getInstance().getName()) {
            t->setText(caTmp);
            t->setIcon(QPixmap(":/me.ico"));
            ui->ListWidget->addItem(t);
        } else {
            t->setText(caTmp);
            t->setIcon(QPixmap(":/all_single_usr.ico"));
            ui->ListWidget->addItem(t);
        }
    }
    ui->ADD_PB->show();
    ui->ListWidget->show();
    delete pdu;
    pdu = NULL;
}

void UsrFri::show_search_usr(PDU *pdu)
{
    ui->listWidget_shouFriendList->addItem(pdu->caData);
    delete pdu;
    pdu = NULL;
}

void UsrFri::updatefriendlist(PDU *pdu) //更新好友列表
{
    ui->listWidget_shouFriendList->clear();
    if(pdu == NULL) {
        return ;
    }
    uint uisize = pdu->uiMsgLen / 32;
    qDebug() << "好友数目！！" << uisize;
    char temp[32] = "";
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap(":/fri_me.ico"));
    item->setText("好友列表");
    QFont font("黑体");
    item->setFont(font);
    ui->listWidget_shouFriendList->addItem(item);
    for(uint i{0}; i < uisize; ++i) {
        memcpy(temp, (char *)(pdu->caMsg) + 32 * i, 32);
        qDebug() << temp;
        QListWidgetItem *item = new QListWidgetItem;
//        item->setIcon(QPixmap(":/fri_me.ico"));
        item->setText(temp);
        ui->listWidget_shouFriendList->addItem(item);
    }
    delete pdu;
    pdu = NULL;
}

void UsrFri:: show_group_msg(PDU *pdu)//显示群聊信息
{
    QString img_p = "./Chat/IMG/", msg_p = "./Chat/MSG/";
    QDir dir;
    if(!dir.exists(img_p)) {
        dir.mkdir(img_p);
    }
    if(!dir.exists(msg_p)) {
        dir.mkdir(msg_p);
    }
    //    QFile file = toString("yyyy-MM-dd hh:mm:ss")
    //获取当前日期
    QDate current_date = QDate::currentDate();
    QString currentDate = current_date.toString("yyyy-MM-dd");//2021-1-18
    QFile msgfile(msg_p + currentDate + ".txt");
    msgfile.open(QIODevice::ReadWrite | QIODevice::Append);
    if(pdu == NULL) {
        return ;
    }
    QString msg = (char *)pdu->caMsg;;
    QString title;
    int n = msg.size();
    int m{0};
    for(auto x : msg) {
        if(x == '\n') {
            break;
        }
        ++m;
        title.push_back(x);
    }
    qDebug() << "title" << title;
    msg = msg.right(n - m);
    QStringList msgd = msg.split('#');
    msg = msgd[0];
    int mindex = msg.lastIndexOf('\\');
//    qDebug() << "mindex" << mindex;
    msg = msg.left(mindex);
    qDebug() << msg;
    // ui->m_showMSG->insertPlainText(msg);
    auto cl = ui->m_showMSG->textColor();
    ui->m_showMSG->setTextColor(Qt::blue);
    ui->m_showMSG->insertPlainText(title);
    ui->m_showMSG->setTextColor(cl);
    ui->m_showMSG->insertPlainText(msg);
    msgfile.write(title.toStdString().c_str());
    msgfile.write(msg.toStdString().c_str());
//    qDebug() << (char *)pdu->caMsg;
    for(int i{1}; i < msgd.size() - 1; ++i) {
        QString f_name = msgd[i].split('$').at(0);
        TcpClient::getInstance().change_file_total_size( msgd[i].split('$').at(1).toInt());
        ltimg = img_p + f_name;
        msgfile.write(title.toStdString().c_str());
        msgfile.write(QString("图片信息:[" + ltimg + "]\n").toStdString().c_str());
        Operatewidget::getInstance().get_usrfiles()->on_pushButton_downloadimage_clicked(f_name, img_p, msgd[i].split('$').at(1).toInt());
    }
//    for(int i{1}; i < msgd.size() - 1; ++i) {
//        QString f_name = msgd[i].split('$').at(0);
//        QImage image(img_p + f_name);
//        qDebug() << "t图像高度为：" << image.height();
//        QTextCursor cursor =  ui->m_showMSG->textCursor();
//        QTextImageFormat *tif = new QTextImageFormat;
//        tif->setName(img_p + f_name);
//        if(image.height() > ui->lineEdit_INPUTMESG->height()) {
//            tif->setWidth(image.width() * ui->lineEdit_INPUTMESG->height() / image.height());
//            tif->setHeight(ui->lineEdit_INPUTMESG->height());
//        } else {
//            tif->setWidth(image.width());
//            tif->setHeight(image.height());
//        }
//        cursor.insertImage(*tif);
//        ui->m_showMSG->setTextCursor(cursor);
//    }
    delete pdu;
    pdu = NULL;
    msgfile.close();
    dltimer->start(200);
}

void UsrFri::show_pchat_msg(PDU *pdu)//私聊操作
{
    if(pdu == NULL) {
        return ;
    }
    char friname[32] = "";
    qDebug() << "测试私聊";
    memcpy(friname, pdu->caData, 32); //对方的名字
    Privatechat *pchat;
    if(!pchat_hash.count(friname)) {
        pchat = new Privatechat;
        pchat_hash[friname] = pchat;
        pchat->upgrade_twopchat(friname);
    } else {
        pchat = pchat_hash[friname];
    }
    qDebug() << "进入私聊界面";
    pchat->receive_msg(pdu);
    delete pdu;
    pdu = NULL;
}

void UsrFri::auto_close_pchat() //用户下线 关闭所有私聊窗口
{
    for(auto &x : pchat_hash) {
        Privatechat *pchat;
        pchat = x.second;
        pchat->close_pchat();
        delete pchat;
    }
    pchat_hash.clear();
}

UsrFri &UsrFri::getinstance()
{
    static UsrFri instance;
    return instance;
}

UsrFri::~UsrFri()
{
    delete ui;
}

void UsrFri::on_pushButton_showonlineusrs_clicked()
{
    if(ui->ListWidget->isHidden()) {
        PDU *mypdu = mkPDU_1(0, ENUM_MSG_TYPE_ALL_ONLINE_REQUEST); //获取所有在线用户请求
        TcpClient::getInstance().getTcpSocket().write((char *)mypdu, mypdu->uiPDULen); //mypdu本身就是地址符号 不用取地址
        delete mypdu;
        mypdu = NULL;
    } else {
        ui->ADD_PB->hide();
        ui->ListWidget->hide();
    }
    //mypdu需要放在里面
//    if(ui->ONLINE_2->isHidden()){
//            ui->ONLINE_2->show();
//             PDU*mypdu = new PDU(0,ENUM_MSG_TYPE_ALL_ONLINE_REQUEST);//获取所有在线用户请求
//             TcpClient::getInstance().getTcpSocket().write((char*)mypdu,mypdu->uiPDULen);//mypdu本身就是地址符号 不用取地址
//             delete mypdu;
//             mypdu = NULL;
//        }else{
//            ui->ONLINE_2->hide();
//            ui->ListWidget->clear();//清空掉
//        }
}

void UsrFri::on_pushButton_searchusrs_clicked()//搜索用户
{
    QString name = QInputDialog::getText(this, "搜索用户", "请输入用户名", QLineEdit::Normal);
    if(NULL != name) {
        qDebug() << name;
        PDU *mypdu = mkPDU_1(0, ENUM_MSG_TYPE_SEARCH_USR_REQUEST); //获取所有在线用户请求
        //memcpy((char*)(respdu->caMsg)+32*i,retlist[i].toStdString().c_str(),32);
        memcpy(mypdu->caData, name.toStdString().c_str(), 32);
        TcpClient::getInstance().getTcpSocket().write((char *)mypdu, mypdu->uiPDULen);
        delete mypdu;
        mypdu = NULL;
    }
}

void UsrFri::on_ADD_PB_clicked() //添加好友槽函数，封装pdu发送给服务器
{
    PDU *mypdu = mkPDU_1(0, ENUM_MSG_TYPE_ADD_USR_REQUEST); //获取所有在线用户请求
    if(ui->ListWidget->currentItem() == NULL || ui->ListWidget->currentItem()->text() == "在线用户") {
        QMessageBox::critical(this, "warning", "invalid clicked!");
        return;
    }
    QString a = ui->ListWidget->currentItem()->text().toStdString().c_str(), b;
    qDebug() << a;
    for(auto x : a) {
        if(x == ' ') {
            break;
        }
        b.push_back(x);
    }
    if(b == TcpClient::getInstance().getName()) {
        QMessageBox::critical(this, "warning", "Can't add yourself!");
        return;
    }
    memcpy(mypdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32); //前32位保存加好友者 后32位保存要加的好友；
    memcpy(mypdu->caData + 32, b.toStdString().c_str(), 32);
    TcpClient::getInstance().getTcpSocket().write((char *)mypdu, mypdu->uiPDULen);
    delete mypdu;
    mypdu = NULL;
}



void UsrFri::on_pushButton_flushfries_clicked()//刷新好友
{
    QString name = TcpClient::getInstance().getName();
    PDU *mypdu = mkPDU_1(0, ENUM_MSG_TYPE_FLUSH_FRI_REQUSET); //获取所有在线用户请求
    memcpy(mypdu->caData, name.toStdString().c_str(), 32);
    TcpClient::getInstance().getTcpSocket().write((char *)mypdu, mypdu->uiPDULen);
    delete mypdu;
    mypdu = NULL;
}

void UsrFri::doubleclicked_pchat(QListWidgetItem *item)
{
    on_pushButton_private_chat_clicked();
}

void UsrFri::show_lt_img()//显示群聊图片
{
    dltimer->stop();
    QImage image(ltimg);
    qDebug() << "群聊显示图片的名称为：" << ltimg << "大小：" << image.size();
    QTextCursor cursor =  ui->m_showMSG->textCursor();
    QTextImageFormat *tif = new QTextImageFormat;
    tif->setName(ltimg);
    if(image.height() > ui->lineEdit_INPUTMESG->height()) {
        tif->setWidth(image.width() * ui->lineEdit_INPUTMESG->height() / image.height());
        tif->setHeight(ui->lineEdit_INPUTMESG->height());
    } else {
        tif->setWidth(image.width());
        tif->setHeight(image.height());
    }
    cursor.insertImage(*tif);
    ui->m_showMSG->setTextCursor(cursor);
    ui->m_showMSG->append("\n");
    ltimg = "";
}



void UsrFri::dragEnterEvent(QDragEnterEvent *event)//拖动文件
{
    if(event->mimeData()->hasUrls()) {                  // 数据中是否包含URL
        event->acceptProposedAction();    // 如果是则接收动作
    } else {
        event->ignore();
    }
}



void UsrFri::dropEvent(QDropEvent *event)//放下文件操作
{
    const QMimeData *mimeData = event->mimeData();      // 获取MIME数据
    if(mimeData->hasUrls()) {                           // 如果数据中包含URL
        QList<QUrl> urlList = mimeData->urls();         // 获取URL列表
        // 将其中第一个URL表示为本地文件路径
        QString fileName = urlList.at(0).toLocalFile();
        input_img_path += fileName + "#";
        if(!((fileName[fileName.size() - 3] == 'j' &&
                fileName[fileName.size() - 2] == 'p' &&
                fileName[fileName.size() - 1] == 'g')
                ||
                (fileName[fileName.size() - 4] == 'j' &&
                 fileName[fileName.size() - 3] == 'p' &&
                 fileName[fileName.size() - 2] == 'e' &&
                 fileName[fileName.size() - 1] == 'g'
                ) ||
                (fileName[fileName.size() - 3] == 'p' &&
                 fileName[fileName.size() - 2] == 'n' &&
                 fileName[fileName.size() - 1] == 'g'
                )
                ||
                (fileName[fileName.size() - 3] == 'b' &&
                 fileName[fileName.size() - 2] == 'm' &&
                 fileName[fileName.size() - 1] == 'p'
                ) ||
                (fileName[fileName.size() - 3] == 'g' &&
                 fileName[fileName.size() - 2] == 'i' &&
                 fileName[fileName.size() - 1] == 'f'
                ) ||
                (fileName[fileName.size() - 3] == 'i' &&
                 fileName[fileName.size() - 2] == 'c' &&
                 fileName[fileName.size() - 1] == 'o'
                ) ||
                (fileName[fileName.size() - 4] == 'w' &&
                 fileName[fileName.size() - 3] == 'e' &&
                 fileName[fileName.size() - 2] == 'b' &&
                 fileName[fileName.size() - 1] == 'p'
                ))) {
            if(!fileName.isEmpty()) {                       // 如果文件路径不为空
                QFile file(fileName);     // 建立QFile对象并且以只读方式打开该文件
                if(!file.open(QIODevice::ReadOnly)) {
                    return;
                }
                QTextStream in(&file);                      // 建立文本流对象
                ui->lineEdit_INPUTMESG->append(in.readAll());  // 将文件中所有内容读入编辑器
            }
            return;
        }
//        if(!fileName.isEmpty()) {                       // 如果文件路径不为空
//            QFile file(fileName);     // 建立QFile对象并且以只读方式打开该文件
//            if(!file.open(QIODevice::ReadOnly)) {
//                return;
//            }
//            QTextStream in(&file);                      // 建立文本流对象
//            ui->lineEdit_INPUTMESG->append(in.readAll());  // 将文件中所有内容读入编辑器
//        }
        QImage image(fileName);
        QTextCursor cursor =  ui->lineEdit_INPUTMESG->textCursor();
        QTextImageFormat *tif = new QTextImageFormat;
        tif->setName(fileName);
        if(image.height() > ui->lineEdit_INPUTMESG->height()) {
            tif->setWidth(image.width() * ui->lineEdit_INPUTMESG->height() / image.height());
            tif->setHeight(ui->lineEdit_INPUTMESG->height());
        } else {
            tif->setWidth(image.width());
            tif->setHeight(image.height());
        }
        cursor.insertImage(*tif);
        ui->lineEdit_INPUTMESG->setTextCursor(cursor);
    }
}
void UsrFri::wheelEvent(QWheelEvent *e)
{
    if(QApplication::keyboardModifiers() == Qt::ControlModifier) { //ctrl键的判断
        if(e->delta() > 0) {
            ui->lineEdit_INPUTMESG->zoomIn();//放大
        } else {
            ui->lineEdit_INPUTMESG->zoomOut(); //缩小
        }
    }
}
void UsrFri::on_pushButton_deletefri_clicked()   //删除好友
{
    if(ui->listWidget_shouFriendList->currentItem() == NULL || ui->listWidget_shouFriendList->currentItem()->text() == "好友列表") {
        QMessageBox::critical(this, "warning", "invalid clicked!");
        return;
    }
    QString a = ui->listWidget_shouFriendList->currentItem()->text().toStdString().c_str(), b;
    qDebug() << a;
    for(auto x : a) {
        if(x == ' ') {
            break;
        }
        b.push_back(x);
    }
    PDU *pdu = mkPDU_1(0, ENUM_MSG_TYPE_DELETE_FRI_REQUSET);
    memcpy(pdu->caData + 32, b.toStdString().c_str(), 32); //后32位存放要删除人的姓名
    memcpy(pdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32); //前32位存放自己的姓名
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}
void UsrFri::on_pushButton_private_chat_clicked()   //私聊请求
{
    if(ui->listWidget_shouFriendList->currentItem() == NULL) {
        QMessageBox::critical(this, "warning", "invalid clicked! please select a friend");
        return;
    }
    if(ui->listWidget_shouFriendList->currentItem()->text() == "好友列表") {
        return;
    }
    QString a = ui->listWidget_shouFriendList->currentItem()->text().toStdString().c_str(), friname;
    qDebug() << a;
    for(auto x : a) {
        if(x == ' ') {
            break;
        }
        friname.push_back(x);
    }
    Privatechat *pchat;
    if(!pchat_hash.count(friname)) {
        pchat = new Privatechat;
        pchat_hash[friname] = pchat;
        pchat->upgrade_twopchat(friname);
    } else {
        pchat = pchat_hash[friname];
    }
    if(pchat->isHidden()) {
        pchat->show();
    }
    friname = TcpClient::getInstance().getName() + "->" + friname  + "(私聊)";
    pchat->setWindowTitle(friname);
}
void UsrFri::on_pushButton_sendMSG_clicked()   //群聊
{
    QString c_pa = "./Chat/";
    QString img_p = "./Chat/IMG/", msg_p = "./Chat/MSG/";
    QDir dir;
    if(!dir.exists(c_pa)) {
        qDebug() << "创建聊天记录目录";
        dir.mkdir(c_pa);
    }
    if(!dir.exists(img_p)) {
        dir.mkdir(img_p);
    }
    if(!dir.exists(msg_p)) {
        dir.mkdir(msg_p);
    }
    //    QFile file = toString("yyyy-MM-dd hh:mm:ss")
    //获取当前日期
    QDate current_date = QDate::currentDate();
    QString currentDate = current_date.toString("yyyy-MM-dd");//2021-1-18
    QFile msgfile(msg_p + currentDate + ".txt");
    msgfile.open(QIODevice::ReadWrite | QIODevice::Append);
    QString msg = ui->lineEdit_INPUTMESG->toPlainText().toStdString().c_str();
    QString name = TcpClient::getInstance().getName();
    if(msg == "") {
        QMessageBox::critical(this, "Group chat", "Group message can't be empty!");
        return;
    }
    QString sendtime = TcpClient::getInstance().getsystemtime();
    ui->lineEdit_INPUTMESG->clear();
    QString title = name + "(" + sendtime + ")" + ": " + "\n";
    auto cl =  ui->m_showMSG->textColor();
    ui->m_showMSG->setTextColor(Qt::red);
    ui->m_showMSG->insertPlainText(title);
    ui->m_showMSG->setTextColor(cl);
    ui->m_showMSG->insertPlainText(msg + "\n\n");
    msgfile.write(title.toStdString().c_str());
    msgfile.write(msg.toStdString().c_str());
    msgfile.write("\n");
    ////
    QString mpath = input_img_path;
    input_img_path = "";
    //  if(mpath != "") {
    QStringList img_list = mpath.split("#");
    for(auto x : img_list) {
        msgfile.write(title.toStdString().c_str());
        msgfile.write("\n");
        msgfile.write(QString("图片信息:[" + x + "]\n").toStdString().c_str());
        msgfile.close();
//        qDebug() << "图像名称" << x << endl;
        if(x == "") {
            break;
        }
        QImage image(x);
        QTextCursor cursor =  ui->m_showMSG->textCursor();
        QTextImageFormat *tif = new QTextImageFormat;
        tif->setName(x);
        if(image.height() > ui->lineEdit_INPUTMESG->height()) {
            tif->setWidth(image.width() * ui->lineEdit_INPUTMESG->height() / image.height());
            tif->setHeight(ui->lineEdit_INPUTMESG->height());
        } else {
            tif->setWidth(image.width());
            tif->setHeight(image.height());
        }
        cursor.insertImage(*tif);
        ui->m_showMSG->setTextCursor(cursor);
        ui->m_showMSG->append("\n");
    }
    for(auto &x : img_list) {
        if(x == "") {
            continue;
        }
        Operatewidget::getInstance().get_usrfiles()->on_pushButton_overloadiamge_clicked(x);
    }
    // }
    ///保存图片信息
    QString img_name_list = "#";
    for(auto x : img_list) {
        if(x == "") {
            continue;
        }
        QFile file(x);
        qint64 f_size = file.size();
        int index = x.lastIndexOf('/');
        QString img_name = x.right(x.size() - index - 1);
        QString p = img_name + "$" + QString::number(f_size) + "#";
        img_name_list.append(p);
    }
    QString data = name + "(" + sendtime + ")" + ": " + "\n" + msg + "\n" + img_name_list;
    ////
    PDU *pdu = mkPDU_1(2 * data.size(), ENUM_MSG_TYPE_GROUP_CHAT_REQUEST); //实际收到数据大小留有冗余因为中文字符更大
    memcpy(pdu->caData + 32, name.toStdString().c_str(), 32);
    memcpy(pdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32);
    memcpy((char *)pdu->caMsg, data.toStdString().c_str(), 2 * data.size());
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    TcpClient::getInstance().getTcpSocket().flush();
}
void UsrFri::on_pushButton_private_chat_close_clicked()   //关闭私聊窗口
{
    for(auto &x : pchat_hash) {
        Privatechat *pchat;
        pchat = x.second;
        pchat->close_pchat();
    }
}
unordered_map<QString, Privatechat *> UsrFri::get_pchat_window()
{
    return pchat_hash;
}
QListWidget *UsrFri::get_fri_list()
{
    return ui->listWidget_shouFriendList;
}
bool UsrFri::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->lineEdit_INPUTMESG && event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast <QKeyEvent *> (event);
        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) { //步骤三
            on_pushButton_sendMSG_clicked();//步骤四
            return true;
        }
    }
    return QWidget::eventFilter(object, event);
}

void UsrFri::Input_setfocus()
{
    ui->lineEdit_INPUTMESG->setFocus();
}
void UsrFri::on_m_showMSG_textChanged()
{
    ui->m_showMSG->moveCursor(QTextCursor::End);
}

void UsrFri::on_pushButton_wqz_clicked()//进行五子棋游戏
{
    int ret = QMessageBox::information(this, "Start Banggo!", "Do you want to start with Black Chess?", QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes) { //黑棋开始
        Chessboard *chessboard = new Chessboard;//棋盘大小 横向边界 众向边界 棋子大小
        chessboard->i_Chessboard(13, 40, 39, 67.5, 0);
        chessboard->show();//用指针的形式反复显示
        connect(chessboard, &Chessboard::destroyed, this, [ = ]() {
            delete chessboard;
        });
    } else if(ret == QMessageBox::No) { //白旗开始
        Chessboard *chessboard = new Chessboard;//棋盘大小 横向边界 众向边界 棋子大小
        chessboard->i_Chessboard(13, 40, 39, 67.5, 1);
        chessboard->show();//用指针的形式反复显示
        connect(chessboard, &Chessboard::destroyed, this, [ = ]() {
            delete chessboard;
        });
    } else {
    }
}
