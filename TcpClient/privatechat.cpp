#include "privatechat.h"
#include "ui_privatechat.h"
#include  <QMessageBox>
#include <windows.h>
#include <unordered_map>
#include <QKeyEvent>
#include <QBitmap>


Privatechat::Privatechat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Privatechat)
{
    ui->setupUi(this);
    this->setFixedSize(600, 400);
    ui->textEdit_private_chat->setFixedSize(500, 50);
    setLayout(this->ui->gridLayout);
    ui->textEdit_private_chat->installEventFilter(this);//绑定键盘回车发送信息
    QPixmap pixmap(":/PCHAT.bmp");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    ui->pushButton_sendmsg->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_wzq->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->textEdit_private_chat->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ui->textBrowser_showprivatemsg->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ui->textEdit_private_chat->setFocus();
//    m_chessboard = new Chessboard;
}

Privatechat::~Privatechat()
{
    delete ui;
}

void Privatechat::receive_msg(PDU *pdu)
{
    if(this->isHidden()) {
        this->show();
    }
    char name[32];
    memcpy(name, pdu->caData, 32);
    QString friname = name;
    friname = TcpClient::getInstance().getName() + "->" + friname  + "(私聊)";
    this->setWindowTitle(friname);
    QString msg = (char *)pdu->caMsg;
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
    qDebug() << "msg" << msg;
    // ui->m_showMSG->insertPlainText(msg);
    auto cl = ui->textBrowser_showprivatemsg->textColor();
    ui->textBrowser_showprivatemsg->setTextColor(Qt::blue);
    ui->textBrowser_showprivatemsg->insertPlainText(title);
    ui->textBrowser_showprivatemsg->setTextColor(cl);
    ui->textBrowser_showprivatemsg->insertPlainText(msg + "\n");
    //ui->textBrowser_showprivatemsg->append();
    qDebug() << (char *)pdu->caMsg;
}

void Privatechat::close_pchat()
{
    ui->textBrowser_showprivatemsg->clear();
    ui->textEdit_private_chat->clear();
    this->hide();
}

void Privatechat::upgrade_twopchat(QString tname)
{
    if(pchatfri == "") {
        pchatfri = tname;
    }
}

QString Privatechat::get_pchatusr()
{
    return pchatfri;
}

bool Privatechat::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->textEdit_private_chat && event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast <QKeyEvent *> (event);
        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) { //步骤三
            on_pushButton_sendmsg_clicked();//步骤四
            return true;
        }
    }
    return QWidget::eventFilter(object, event);
}


void Privatechat::on_pushButton_sendmsg_clicked()//私聊界面发送按钮
{
    QString msg = ui->textEdit_private_chat->toPlainText().toStdString().c_str();
    if(msg == "") {
        QMessageBox::critical(this, "Private chat", "Please input your message!");
        return;
    }
    ui->textEdit_private_chat->clear();
    QString sendtime = TcpClient::getInstance().getsystemtime();
    QString name = TcpClient::getInstance().getName();
    QString data = name + "(" + sendtime + ")" + ": " + "\n" + msg + "\n";
    PDU *pdu = mkPDU_1(2 * data.size(), ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST);
    qDebug() << data;
    QString title = name + "(" + sendtime + ")" + ": ";
    auto cl =  ui->textBrowser_showprivatemsg->textColor();
    ui->textBrowser_showprivatemsg->setTextColor(Qt::red);
    ui->textBrowser_showprivatemsg->insertPlainText(title);
    ui->textBrowser_showprivatemsg->setTextColor(cl);
    ui->textBrowser_showprivatemsg->append(msg + "\n\n");
    QString pchatname = get_pchatusr();
    qDebug() << "私聊好友的名字" << pchatname;
    memcpy(pdu->caData + 32, pchatname.toStdString().c_str(), 32); //拷贝要私聊朋友的名字
    memcpy(pdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32); //拷贝自己的名字
    strcpy((char *)pdu->caMsg, data.toStdString().c_str()); //拷贝消息
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送给服务器私聊请求
    delete pdu;
    pdu = NULL;
}

void Privatechat::on_pushButton_wzq_clicked()
{
    PDU *pdu = mkPDU_2(0, ENUM_MSG_TYPE_WZQ, TcpClient::getInstance().getName(), pchatfri, "");
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
}
