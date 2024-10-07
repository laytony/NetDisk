#include "sharefile.h"
#include "ui_sharefile.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QBitmap>

shareFile::shareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::shareFile)
{
    ui->setupUi(this);
    group_button = new QButtonGroup(ui->widget_show_onfri);
    group_button->setExclusive(false);
    vbox = new QVBoxLayout(ui->widget_show_onfri);
    connect(ui->pushButton_nonselect_all, SIGNAL(clicked()), this,
            SLOT(cancle_select()));
    connect(ui->pushButton_select_all, SIGNAL(clicked()), this,
            SLOT(select_all()));
    connect(ui->pushButton_ok, SIGNAL(clicked()), this,
            SLOT(sure_to_select()));
    connect(ui->pushButton_cancle, SIGNAL(clicked()), this,
            SLOT(cancle_to_select()));
    this->setFixedSize(300, 400);
    QPixmap pixmap(":/SHAREF.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    ui->pushButton_ok->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_cancle->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_select_all->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_nonselect_all->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    setLayout(ui->verticalLayout);
//    test();
}

shareFile::~shareFile()
{
    delete ui;
}

void shareFile::test()
{
    QCheckBox *chb = NULL;
    for(int i{0}; i < 15; ++i) {
        chb = new QCheckBox("lily");
        group_button->addButton(chb);
        vbox->addWidget(chb);
    }
    ui->scrollArea->setWidget(ui->widget_show_onfri);
}

void shareFile::upodatefir(QListWidget *listw)//获取好友界面的好友列表用于更新showfile界面的好友列表
{
    if(listw == NULL) {
        return;
    }
    QAbstractButton *temp = NULL;
    QList<QAbstractButton *> buttonlist = group_button->buttons();
    for(int i{0}; i < buttonlist.size(); ++i) {
        temp = buttonlist[i];
        vbox->removeWidget(temp);
        group_button->removeButton(temp);
        buttonlist.removeAll(temp);
        delete temp;
        temp = NULL;
        --i;
    }
    QCheckBox *chb = NULL;
    for(int i{0}; i < listw->count(); ++i) {
        if(listw->item(i)->text() == "好友列表") {
            continue;
        }
        chb = new QCheckBox(listw->item(i)->text());
        group_button->addButton(chb);
        vbox->addWidget(chb);
    }
    ui->scrollArea->setWidget(ui->widget_show_onfri);
}


shareFile &shareFile::getInstance()
{
    static shareFile  instance;
    return instance;
}

void shareFile::cancle_select()//取消选择槽函数
{
    QList<QAbstractButton *> buttonlist = group_button->buttons();
    for(int i{0}; i < buttonlist.count(); ++i) {
        if(buttonlist[i]->isChecked()) {
            buttonlist[i]->setChecked(false);
        }
    }
}


void shareFile::select_all()//全部选择槽函数
{
    QList<QAbstractButton *> buttonlist = group_button->buttons();
    for(int i{0}; i < buttonlist.count(); ++i) {
        if(!buttonlist[i]->isChecked()) {
            buttonlist[i]->setChecked(true);
        }
    }
}

void shareFile::sure_to_select()//确定选择按钮
{
    QString myname = TcpClient::getInstance().getName();
    QString filename = Operatewidget::getInstance().get_usrfiles()->get_sharefile_name();// 待分享文件名
    QString cur_path = TcpClient::getInstance().get_curpath();
    QString file_path = cur_path + filename;
    QList<QAbstractButton *> buttonlist = group_button->buttons();
    QStringList fri_tosharelist;
    int share_cnt{0};
    for(int i{0}; i < buttonlist.count(); ++i) {
        if(buttonlist[i]->isChecked()) {
            QString sharename = buttonlist[i]->text();
            if(sharename.contains("not online", Qt::CaseSensitive)) {
                continue;
            }
            fri_tosharelist.push_back(sharename.left(sharename.size() - 9));
            share_cnt++;
        }
    }//CaMsg存放file_name;
    PDU *pdu = mkPDU_2((share_cnt + 1) * 32 + file_path.size() * 2, ENUM_MSG_TYPE_SHARE_FILE_REQUEST, myname, QString::number(share_cnt), "");
    for(int i{0}; i < share_cnt; ++i) {
        memcpy((char *)pdu->caMsg + 32 * i, fri_tosharelist[i].toStdString().c_str(), 32);
    }
    memcpy((char *)pdu->caMsg + 32 * share_cnt, filename.toStdString().c_str(), 32);
    strcpy((char *)pdu->caMsg + 32 * (share_cnt + 1), file_path.toStdString().c_str());
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void shareFile::cancle_to_select()
{
    this->hide();
}

