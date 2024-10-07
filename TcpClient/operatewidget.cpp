#include "operatewidget.h"
#include <windows.h>
#include <QBitmap>

Operatewidget::Operatewidget(QWidget *parent) : QWidget(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    QFont font;
    font.setPointSize(8);//字体大小
    font.setFamily("黑体");
    m_pListW = new QListWidget(this);
//    //添加状态栏处理动作
    st_bar = new QStatusBar;
    m_lable = new QLabel;
    m_lable1 = new QLabel;
    m_lable->setPixmap(QPixmap(":/connection.ico").scaled(15, 15)); //指定图片高度和宽度
    m_lable1->setPixmap(QPixmap(":/disconnected.ico").scaled(15, 15)); //指定图片高度和宽度
    st_bar->addWidget(new QLabel("连接状态:"));
    st_bar->setFont(font);
    st_bar->addWidget(m_lable);
    st_bar->addWidget(m_lable1);
    try_connect_to_host = new QPushButton;
    try_connect_to_host->setText("尝试重新连接服务器");
    try_connect_to_host->setFont(font);
    try_connect_to_host->setFixedSize(130, 18);
    st_bar->addWidget(try_connect_to_host);
    try_connect_to_host->hide();
    st_bar->setFixedSize(750, 20);
    m_lable1->hide();
    font.setPointSize(16);//字体大小
    font.setFamily("黑体");
    QListWidgetItem *item = new QListWidgetItem;
    item->setText("好友");
    item->setIcon(QPixmap(":/fri.ico"));
    item->setFont(font);
    m_pListW->addItem(item);
    item = new QListWidgetItem;
    item->setText("文件");
    item->setIcon(QPixmap(":/wjj_list.ico"));
    m_pListW->addItem(item);
//    item = new QListWidgetItem;
//    item->setText("五子棋游戏");
//    item->setIcon(QPixmap(":/wzq.ico"));
//    m_pListW->addItem(item);
    ///
    QPixmap pixmap(":/OPBG.bmp");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    ///
    m_pListW->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    fri = new UsrFri;
    m_files = new Files;
    m_psw = new QStackedWidget;
    m_psw->addWidget(fri);
    m_psw->addWidget(m_files);
    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_psw);
    m_psw->setFont(font);
    m_pListW->setFont(font);
    this->setFixedSize(750, 860); //750*640
    //setLayout(pMain);
    connect(m_pListW, SIGNAL(currentRowChanged(int)), m_psw, SLOT(setCurrentIndex(int))); //关联设置的按钮和窗
    connect(m_pListW, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Init_filelist(QListWidgetItem *))); //设置单击文件按钮触发 初始化文件 好友界面界面
    //重新连接服务器
    connect(try_connect_to_host, SIGNAL(clicked()), this, SLOT(reconnect()));
    setWindowIcon(QPixmap(":/netdisc.ico"));
    QVBoxLayout *pLast = new QVBoxLayout;
    pLast->addWidget(st_bar);
    pLast->addLayout(pMain);
    setLayout(pLast);
//    this->setWindowTitle(TcpClient::getInstance().getName());
//    qDebug()<<"窗口显示："<<TcpClient::getInstance().getName();
}

Operatewidget &Operatewidget::getInstance() //添加单例函数的定义 每次调用都只是调原操作界面
{
    static Operatewidget instance;
    return instance;
}

UsrFri *Operatewidget::getusrfir()
{
    return this->fri;
}

Files *Operatewidget::get_usrfiles()
{
    return m_files;
}

void Operatewidget::set_unonline()
{
    if(!this->isHidden()) { //关闭窗口后不加以显示 否则程序异常结束
        m_lable->hide();
        m_lable1->show();
        try_connect_to_host->show();
    }
}

Operatewidget::~Operatewidget()
{
}


void Operatewidget::Init_filelist(QListWidgetItem *item)
{
    item->setFont(QFont("黑体"));
    if(item->text() == "文件") {
        m_files->on_pushButton_flush_folders_clicked();
    } else if(item->text() == "好友") {
        fri->on_pushButton_flushfries_clicked();
    }
}

void Operatewidget::delete_all_pchats()
{
    qDebug() << "删除所有私聊窗口";
    fri->auto_close_pchat();
}

void Operatewidget::reconnect()
{
    int n{1000};
    if(n-- && TcpClient::getInstance().reconnect()) { //关闭窗口后不加以显示 否则程序异常结束
        m_lable->show();
        m_lable1->hide();
        try_connect_to_host->hide();
    }
}

void Operatewidget::change_connected()
{
    this->setWindowTitle(TcpClient::getInstance().getName());
    m_lable->show();
    m_lable1->hide();
    try_connect_to_host->hide();
}

void Operatewidget::closeEvent(QCloseEvent *event)//关闭窗口触发关闭所有的私聊窗口
{
    if(this->isVisible()) {
        this->hide();
        fri->auto_close_pchat();
    } else {
        QMessageBox::information(this, "Notice", "You can't close the window");
    }
    // event->ignore();
}
