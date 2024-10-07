#include "myfieinfo.h"
#include "ui_myfieinfo.h"
#include <QBitmap>

myfieinfo::myfieinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myfieinfo)
{
    ui->setupUi(this);
    this->setFixedSize(400, 200);
    QPixmap pixmap(":/FILEINFO.bmp");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    palette.setColor(QPalette::WindowText, Qt::white);
    ui->label->setPalette(palette);
    ui->label_2->setPalette(palette);
    ui->label_3->setPalette(palette);
    ui->label_4->setPalette(palette);
    ui->label_5->setPalette(palette);
    ui->checkBox_isdir->setPalette(palette);
    ui->checkBox_isexe->setPalette(palette);
    ui->checkBox_isfile->setPalette(palette);
    ui->checkBox_canread->setPalette(palette);
    ui->checkBox_canwrite->setPalette(palette);
    ui->checkBox_isHidden->setPalette(palette);
    setLayout(ui->Last_Lay);
}

myfieinfo::~myfieinfo()
{
    delete ui;
}

void myfieinfo::upgrade_finfo(QString name, QString size, QString createTime, QString lastVisitedtime, QString lastmodify,
                              bool is_File, bool is_folder, bool is_canread, bool is_canwrie, bool is_hidden, bool is_exe)
{
    ui->lineEdit_filename->setText(name);
    ui->lineEdit_filesize->setText(size);
    ui->lineEdit_createname->setText(createTime);
    ui->lineEdit_lastvisited->setText(lastVisitedtime);
    ui->lineEdit_lastnodify->setText(lastmodify);
    ui->checkBox_isdir->setCheckState(is_folder ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_isfile->setCheckState(is_File ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_canread->setCheckState(is_canread ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_canwrite->setCheckState(is_canwrie ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_isHidden->setCheckState(is_hidden ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_isexe->setCheckState(is_exe ? Qt::Checked : Qt::Unchecked);
    this->setWindowTitle(name);
}
