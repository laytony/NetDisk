#include "files.h"
#include "ui_files.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFont>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTimer>
#include <windows.h>
#include <QUrl>
#include <QDateTime>
#include <QListWidget>
#include <QProgressDialog>
#include <QBitmap>

Files::Files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Files)
{
    ui->setupUi(this);
    this->setFixedSize(600, 800);
    ui->listWidget_showfiles->setFixedHeight(680);
    ui->listWidget__show_usr_op->setFixedHeight(80);
    ui->listWidget_showfiles->setFixedWidth(450);
    ui->pushButton_return->setFixedWidth(140);
    ui->pushButton_downloadfile->setFixedWidth(140);
    ui->pushButton_overloadfile->setFixedWidth(140);
    ui->progressBar_up->setRange(0, 100);
    ui->progressBar_up->setFixedWidth(450);
    ui->progressBar_up->setValue(0);
    ui->progressBar_down->setRange(0, 100);
//    ui->progressBar_down->setFixedWidth(450);
    ui->progressBar_down->setValue(0);
    item = new QListWidgetItem;
    QFont font("黑体");
    item->setFont(font);
    item->setText("￥用户操作提示￥");
    ui->listWidget__show_usr_op->addItem(item);
    setLayout(ui->Final_Layout);
    connect(ui->listWidget_showfiles, SIGNAL(itemDoubleClicked(QListWidgetItem *))
            , this, SLOT(Enter_dir(QListWidgetItem *))); //双击进入文件夹
    setAttribute(Qt::WA_DeleteOnClose);//在构造函数中设置 一旦关闭就销毁内存
    m_ptimer_up = new QTimer;
    up_t = new QTimer;
    //connect(m_ptimer_up,SIGNAL(timeout()),this,SLOT(uploadfile()));//关联设置槽函数定时时间到 运行此函数
    //connect(up_t, SIGNAL(timeout()), this, SLOT(uploadfile_th())); //关联设置槽函数定时时间到 运行此函数
    ui->pushButton_sure_movefile_folders->setEnabled(false);
    connect(ui->listWidget_showfiles, SIGNAL(itemPressed(QListWidgetItem *))//单击显示文件信息
            , this, SLOT(Show_FD_MSG(QListWidgetItem *)));
    file_total_size = 0;
    file_send_size = 0;
    index = 0;
    upload_port = UPLOAD_PORT;
    srand((int)time(NULL));
    download_port = QString::number(11000 + rand() % 54535);//产生下载文件的随机端口号！
//    download_port = QString::number(DOWNLOAD_PORT);//产生下载文件的随机端口号！
    qDebug() << "下载监听端口：" << download_port;
    //每个用户一个在线时段内只使用同一个下载端口
    //隐藏bar
    ///文件操作多线程：
    ///1、创建任务对象
    //  my_fileop = new FileOp;
    ///操作文件的进程
    /// 创建线程对象
//    myfile_oprate = new QThread;
    //创建任务对象
//    my_fileop = new FileOp;
//    my_fileop->moveToThread(myfile_oprate);
//    connect(this,&Files::upload_filesig,my_fileop,&FileOp::sub_upload_file);//连接处理子线程
//    myfile_oprate->start();//线程启动
//   sendfile->connectToHost(QHostAddress(ip),usr_port);
//   sendfile->write((char*)pdu,pdu->uiPDULen);
//   sendfile->close();
//    ///进度条
//    QFont mfont("songti");
//    mfont.setWeight(16);
//    QProgressDialog upprogress;
//    upprogress.reset();
//    upprogress.reset();
//    upprogress.setValue(0);
//    upprogress.hide();
//    upprogress.setMinimumDuration(0);
//    upprogress.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);		//不显示进度条上的“最小化”“最大化”“关闭”
//    upprogress.setFont(mfont);
//    upprogress.setFixedSize(300, 60);
//    upprogress.setLabelText("please wait...");	//显示的文本
//    //upprogress.setModal(true);//以模态方式弹出对话框
//    upprogress.setRange(0, 100);				//设置进度条的极值，默认为[0,100]
//    upprogress.setCancelButton(NULL);			//不显示取消按钮
//    upprogress.setWindowModality(Qt::ApplicationModal);
    ///
    dowload_server = new QTcpServer(this);
    start_listen(download_port);
    connect(dowload_server, &QTcpServer::newConnection, this, [ = ]() {
        //设置进度条
        ui->progressBar_down->show();
        ui->progressBar_down->setStyleSheet("background-color: rgba(255, 255, 255,160);");
        QTcpSocket *my_socket =  dowload_server->nextPendingConnection();
        qDebug() << "服务器发来下载文件子线程新连接！";
        qint64 cnt{0}, total = TcpClient::getInstance().get_download_file_total_size();
        ui->listWidget__show_usr_op->addItem("开始下载文件" + QString("%1").arg(download_file_name) + "(" +
                                             TcpClient::getInstance().get_file_size(total) + "):"
                                             + "(" + TcpClient::getInstance().getsystemtime() + ")。");
        DownLoadF *dpwnload_th = new DownLoadF(my_socket, cnt, total, TcpClient::getInstance().get_download_path());
        ///进度条;
        QProgressDialog *downida = new QProgressDialog(this);
        downida->reset();
        downida->setValue(0);
        downida->setRange(0, 100);
        downida->setLabelText("Please wait ...");
        downida->setWindowTitle("正在下载文件" + QString("%1").arg(upload_file_name) + "(" +
                                TcpClient::getInstance().get_file_size(file_total_size) + ")");
        downida->setFixedSize(400, 80);
        downida->setFixedSize(0, 0);
        downida->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        // connect(dpwnload_th, &DownLoadF::CurPercent, downida, &QProgressDialog::setValue);
        downida->setCancelButtonText(nullptr);
        connect(dpwnload_th, &DownLoadF::CurPercent, ui->progressBar_down, &QProgressBar::setValue);
        dpwnload_th->start();
        connect(dpwnload_th, &DownLoadF::rev_over, this, [ = ]() {
            qDebug() << "下载文件结束";
            ui->progressBar_down->hide();
            ui->listWidget__show_usr_op->addItem("结束下载文件" + QString("%1").arg(download_file_name) + "(" +
                                                 TcpClient::getInstance().get_file_size(total) + "):"
                                                 + "(" + TcpClient::getInstance().getsystemtime() + ")。");
            dpwnload_th->quit();
            dpwnload_th->wait();
            dpwnload_th->deleteLater();
            my_socket->close();
            my_socket->deleteLater();
            //dowload_server->close();//停止监听
            downida->close();
            downida->deleteLater();
            //QMessageBox::information(this, "Download file", QString("Successfully download file %1!").arg(download_file_name));
        });
    });
    ui->progressBar_up->hide();
    ui->progressBar_down->hide();
    ui->listWidget_showfiles->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ui->listWidget__show_usr_op->setStyleSheet("background-color: rgba(255, 255, 255,60);");
    ///
    QPixmap pixmap(":/OPBG.bmp");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
//    ui->m_showMSG->setMask(pixmap1.mask());
    setAutoFillBackground(true);
    setWindowOpacity(0.9); //设置widget窗体透明度
    ///设置按钮透明
    ui->pushButton_return->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_move_file->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_sharefile->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_downloadfile->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_overloadfile->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_creat_folders->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_flush_folders->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_delete_folders->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_rename_folders->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_show_msg_folders->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    ui->pushButton_sure_movefile_folders->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    connect(this, &Files::create_rtp, this, [&]() {
        if(!TcpClient::getInstance().get_onlienstate()) {
            return;
        }
        qDebug() << "将要创建根目录！";
        QString creat_path = TcpClient::getInstance().get_rootpath();
        PDU *pdu = mkPDU_1(creat_path.size() * 2, ENUM_MSG_TYPE_CREATE_DIR_REQUEST); //caData前32位放自己名字 后32位放新文件名字 caMsg放根路径
        memcpy(pdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32);
        strcpy((char *)pdu->caMsg, creat_path.toStdString().c_str());
        TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
        delete pdu;
        pdu = NULL;
    });
}
//connect(imgList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enlargeImage(QListWidgetItem *)));
Files::~Files()
{
    delete ui;//手动析构任务对象
    //my_fileop->deleteLater();
}

void Files::Flush_folders(PDU *pdu)//刷新文件列表（打开文件夹）回复 操作
{
    if(pdu == NULL) {
        return;
    }
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    ui->listWidget_showfiles->clear();
    QString show_usrPath = "", curpath = TcpClient::getInstance().get_curpath(), rootpath = TcpClient::getInstance().get_rootpath();
    qDebug() << "当前路径：" << curpath << "根路径： " << rootpath;
    while(curpath.size() != rootpath.size()) {
        show_usrPath += curpath.back();
        curpath.chop(1);
    }
    qDebug() << curpath << " " << rootpath;
    show_usrPath.append("/:");
    reverse(show_usrPath.begin(), show_usrPath.end());
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(show_usrPath);
    item->setIcon(QPixmap(":/root.ico"));//设置图标
    QFont f("黑体");
    item->setFont(f);
    ui->listWidget_showfiles->addItem(item);
    int filecnt = pdu->uiMsgLen / sizeof(FileInfo); //文件的个数
    FileInfo *fileinfo;
//    for(int i{0};i<filecnt;++i){//QListWidget 可以添加不同的图标等等
//        fileinfo = mkFileInfo_0();
//        memcpy(fileinfo,(char*)pdu->caMsg+i*sizeof(FileInfo),sizeof(FileInfo));
//        QString name = fileinfo->caFiamename;
//        int i_type = fileinfo->caFiletype;
//        QString q_type;
//        if(i_type == 1)q_type = "";
//        else q_type = "(文件夹)";
//        ui->listWidget_showfiles->addItem(name+q_type);
//    }
    if(filecnt == 0) {
        return;//为空的 不显示任何内容
    }
    unordered_set<int>WJJSET;
    for(int i{0}; i < filecnt; ++i) {//先显示文件夹
        fileinfo = (FileInfo *)(pdu->caMsg) + i; //转换一个FileInfo类型的大小
        QListWidgetItem *pItem = new QListWidgetItem;
        if(fileinfo->caFiletype == 0) { //是文件夹
            pItem->setIcon(QIcon(QPixmap(":/wjj.ico")));
            pItem->setText(fileinfo->caFiamename);
            ui->listWidget_showfiles->addItem(pItem);
            WJJSET.insert(i);
        }
    }
    for(int i{0}; i < filecnt && !WJJSET.count(i); ++i) {
        fileinfo = (FileInfo *)(pdu->caMsg) + i; //转换一个FileInfo类型的大小
        QListWidgetItem *pItem = new QListWidgetItem;
        if(fileinfo->caFiletype == 0) { //是文件夹
            pItem->setIcon(QIcon(QPixmap(":/wjj.ico")));
        } else if(fileinfo->caFiletype == 1) {
            if(QString(fileinfo->file_type) == "pdf") {
                pItem->setIcon(QIcon(QPixmap(":/PDFTB.ico")));
            } else if (QString(fileinfo->file_type) == "xlsx" || QString(fileinfo->file_type) == "xls") {
                pItem->setIcon(QIcon(QPixmap(":/EXTB.ico")));
            } else if (QString(fileinfo->file_type) == "doc" || QString(fileinfo->file_type) == "docx") {
                pItem->setIcon(QIcon(QPixmap(":/WDTB.ico")));
            } else if (QString(fileinfo->file_type) == "zip" || QString(fileinfo->file_type) == "rar" ) {
                pItem->setIcon(QIcon(QPixmap(":/YSB.ico")));
            } else if (QString(fileinfo->file_type) == "exe") {
                pItem->setIcon(QIcon(QPixmap(":/EXETB.ico")));
            } else if (QString(fileinfo->file_type) == "jpg" || QString(fileinfo->file_type) == "jpeg"
                       || QString(fileinfo->file_type) == "bmp"
                       || QString(fileinfo->file_type) == "giff"
                       || QString(fileinfo->file_type) == "png"
                       || QString(fileinfo->file_type) == "webp") {
                pItem->setIcon(QIcon(QPixmap(":/TPTB.ico")));
            } else if (QString(fileinfo->file_type) == "mp4" || QString(fileinfo->file_type) == "avi"
                       || QString(fileinfo->file_type) == "mpg"
                       || QString(fileinfo->file_type) == "mpeg"
                       || QString(fileinfo->file_type) == "mov"
                       || QString(fileinfo->file_type) == "rmvb") {
                pItem->setIcon(QIcon(QPixmap(":/SPTB.ico")));
            } else if (QString(fileinfo->file_type) == "mp3" || QString(fileinfo->file_type) == "wav"
                       || QString(fileinfo->file_type) == "wma") {
                pItem->setIcon(QIcon(QPixmap(":/YYTB.ico")));
            } else {
                pItem->setIcon(QIcon(QPixmap(":/regular.ico")));
            }
        }
        pItem->setText(fileinfo->caFiamename);
        ui->listWidget_showfiles->addItem(pItem);
    }
    delete pdu;
    pdu = NULL;
    fileinfo = NULL;
}

QString Files::get_sharefile_name()
{
    return shared_filename;
}

void Files::handle_down_load_file()
{
}

void Files::start_listen(QString port)
{
    qDebug() << "正在监听";
    dowload_server->listen(QHostAddress::Any, port.toInt());
}

void Files::on_pushButton_overloadiamge_clicked(QString path)//聊天信息存储图片
{
    QString cur_filepath = TcpClient::getInstance().get_curpath();
    cur_filepath.chop(1);
    int index = cur_filepath.lastIndexOf('/');
    cur_filepath  = cur_filepath.left(index);
    cur_filepath += "/Chat/";
    struploadfilepath = path;//获取要选择的文件的路径 打开窗口
    qDebug() << "要上传文件的路径：" << struploadfilepath;
    index = struploadfilepath.lastIndexOf('/');
    upload_file_name = struploadfilepath.right(struploadfilepath.size() - index - 1); //上传的文件名
    qDebug() << "要上传文件名字：" << upload_file_name;
    QFile upload_file(struploadfilepath);
    file_total_size = upload_file.size();
    QString filesize = QString::number(file_total_size);//获取文件大小
    qDebug() << "文件大小" << filesize;
    up_usr_name = TcpClient::getInstance().getName();
    QString usr_ip = TcpClient::getInstance().get_m_ip();
    QThread *up_th = new QThread;
    FileOp *up_work = new FileOp;
    up_work->moveToThread(up_th);
    up_th->start();
    ui->progressBar_up->setValue(0);
    connect(this, &Files::start_connected, up_work, &FileOp::connect_to_server);
    connect(this, &Files::up_signal, up_work, &FileOp::upload_file);
    connect(up_work, &FileOp::Failed_working, this, [ = ](QString str) {
        QMessageBox::warning(this, "upload file", QString("Failed upload file %1!").arg(str));
    });
    // connect(up_work, &FileOp::CurPercent, ui->progressBar_up, &QProgressBar::setValue);
    connect(up_work, &FileOp::connectedok, this, [ = ]() {
        qDebug() << "上传文件子线程成功连接服务器！";
        // QMessageBox::information(this, "upload file", "upload thread successfully connect to server!");
    });
    ///进度条;
    QProgressDialog *updia = new QProgressDialog(this);
    updia->reset();
    updia->setValue(0);
    updia->setRange(0, 100);
    updia->setLabelText("Please wait ...");
    updia->setWindowTitle("正在上传文件" + QString("%1").arg(upload_file_name) + "(" +
                          TcpClient::getInstance().get_file_size(file_total_size) + ")");
    updia->hide();
    updia->setFixedSize(0, 0);
    updia->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(up_work, &FileOp::CurPercent, updia, &QProgressDialog::setValue);
    updia->setCancelButtonText(nullptr);
    connect(up_work, &FileOp::end_thread, this, [ = ](QString str) {
        updia->close();
        updia->deleteLater();
        up_th->quit();
        up_th->wait();
        up_th->deleteLater();
        up_work->deleteLater();
    });
    emit start_connected(usr_ip, upload_port);
    Sleep(80);
    emit up_signal(up_usr_name, upload_file_name, struploadfilepath
                   , cur_filepath, TcpClient::getInstance().get_m_upload_param());
}

void Files::on_pushButton_downloadimage_clicked(QString fname, QString topath, int img_size) //下载图片
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    QString cur_filepath = TcpClient::getInstance().get_curpath();
    cur_filepath.chop(1);
    int index = cur_filepath.lastIndexOf('/');
    cur_filepath  = cur_filepath.left(index);
    cur_filepath += "/Chat/";
    QString file_path = cur_filepath + fname; //云盘中要下载的路径
    QString save_path = "./Chat/IMG/" + fname; //文件保存路径
    TcpClient::getInstance().set_file(save_path);//创建要下载的文件
    TcpClient::getInstance().change_download_path(save_path);
    qDebug() << "向服务器发送图像请求大小为：" << QString::number(img_size).toStdString().c_str();
    PDU *pdu = mkPDU_2(file_path.size() * 2, ENUM_MSG_TYPE_download_FILE_REQUEST, download_port, //download_port
                       TcpClient::getInstance().getName(), file_path); //要下载的文件名 文件路径发送给服务器；
    memcpy(pdu->ELSE + 32, QString::number(img_size).toStdString().c_str(), 32);
    memcpy(pdu->ELSE, TcpClient::getInstance().get_mydownip().toStdString().c_str(), 32);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}




void Files::Enter_dir(QListWidgetItem *item)//双击进入文件夹请求
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    QString stddataname = item->text();//要进入文件夹的名字
    qDebug() << "要打开的文件夹" << stddataname;
    if(*stddataname.begin() == ":") { //不能操作目录提示行
        return;
    }
    for(auto x : stddataname) {
        if(x == '.') {
            Show_File_MSG(item);//如果是文件双击查看信息
            return;
        }
    }
    TcpClient::getInstance().change_curpath(stddataname);
    qDebug() << "双击要打开的文件：" << TcpClient::getInstance().get_curpath();
    QString path = TcpClient::getInstance().get_curpath();
    PDU *pdu = mkPDU_2(path.size() * 2, ENUM_MSG_TYPE_FLUSH_FILE_REQUEST, TcpClient::getInstance().getName().toStdString().c_str(), stddataname, path); //caData前32位放自己名字 后32位放基于根路径的查询文件夹的名字 caMsg放根路径
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void Files::on_pushButton_creat_folders_clicked() //创建文件夹请求 发送给服务器
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    QString newdir = QInputDialog::getText(this, "Create new folder", "Please input folder name!");
    if(newdir == "") {
        QMessageBox::warning(this, "try to create new folder", "Folder name can't be empty!");
        return;
    }
    if(newdir.size() > 32) {
        QMessageBox::warning(this, "try to create new folder", "Folder name can't longer than 32! ");
        return;
    }
    QString creat_path = TcpClient::getInstance().get_curpath();
    PDU *pdu = mkPDU_1(creat_path.size() * 2, ENUM_MSG_TYPE_CREATE_DIR_REQUEST); //caData前32位放自己名字 后32位放新文件名字 caMsg放根路径
    memcpy(pdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, newdir.toStdString().c_str(), 32);
    strcpy((char *)pdu->caMsg, creat_path.toStdString().c_str());
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void Files::on_pushButton_flush_folders_clicked()//刷新文件夹显示当前文件夹的内容
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    QString root_path = TcpClient::getInstance().get_curpath();
//    QDir m_dir(root_path);
//    if(!m_dir.exists()) {
//        emit create_rtp();
//    }
    PDU *pdu = mkPDU_1(root_path.size() * 2, ENUM_MSG_TYPE_FLUSH_FILE_REQUEST); //caData前32位放自己名字 后32位放基于根路径的查询文件夹的名字 caMsg放根路径
    memcpy(pdu->caData, TcpClient::getInstance().getName().toStdString().c_str(), 32);
    strcpy((char *)pdu->caMsg, root_path.toStdString().c_str());
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void Files::Show_FD_MSG(QListWidgetItem *item)//显示文件夹信息请求
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    if(item == NULL) {
        return;
    }
    QString file_name = item->text();
    if(*file_name.begin() == ":") {
        return;
    }
    QString cur_path = TcpClient::getInstance().get_curpath();
    QString file_path = cur_path + file_name;
    QFileInfo fileinfo(file_path);
    PDU *pdu = mkPDU_2(file_path.size() * 2, ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE, file_name, "", file_path);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void Files::Show_File_MSG(QListWidgetItem *item)//显示文件信息请求
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    if(item == NULL) {
        return;
    }
    QString file_name = item->text();
    int flag{0};
    for(auto x : file_name) {
        if(x != '.') {
            flag++;
        }
    }
    if(*file_name.begin() == ":") {
        return;
    }
    if(flag == file_name.size()) {
        return;
    }
    QString cur_path = TcpClient::getInstance().get_curpath();
    QString file_path = cur_path + file_name;
    QFileInfo fileinfo(file_path);
    PDU *pdu = mkPDU_2(file_path.size() * 2, ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST, file_name, "", file_path);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}


void Files::on_pushButton_delete_folders_clicked()//删除文件夹按钮
{
    qDebug() << "删除文件夹操作";
    if(ui->listWidget_showfiles->currentItem() == NULL) {
        QMessageBox::warning(this, "Delete folder!", "invalid clicked!");
        return;
    }
    QString filename = ui->listWidget_showfiles->currentItem()->text().toStdString().c_str();//要删除的文件名
    QString cur_path = TcpClient::getInstance().get_curpath();
    PDU *pdu = mkPDU_2(cur_path.size() * 2, ENUM_MSG_TYPE_DELETE_FILE_REQUEST, TcpClient::getInstance().getName(), filename, cur_path);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void Files::on_pushButton_return_clicked()//返回操作根据当前进入的文件夹进行返回操作 依靠刷新列表
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    qDebug() << TcpClient::getInstance().get_curpath();
    qDebug() << TcpClient::getInstance().get_rootpath();
    if(TcpClient::getInstance().get_rootpath() == TcpClient::getInstance().get_curpath()) { //当前在根目录下 无法回退
        QMessageBox::warning(this, "Try to return", "You are already at Root Path! can't return!");
        return;
    }
    TcpClient::getInstance().change_curpath("");
    QString file_path = TcpClient::getInstance().get_curpath();//保存当前目录 进行调用刷新函数
    qDebug() << "file path 要回退的路径" << file_path;
    //用户名 "" 要回退的路径名
    PDU *pdu = mkPDU_2(file_path.size() * 2, ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,
                       TcpClient::getInstance().getName(), "1", file_path);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    qDebug() << "name" << TcpClient::getInstance().getName();
    qDebug() << "o_name" << pdu->caData + 32;
    qDebug() << "Msg" << (char *)pdu->caMsg;
    delete pdu;
    pdu = NULL;
}

void Files::on_pushButton_rename_folders_clicked()//重命名文件夹或者文件
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    qDebug() << "重命名文件(夹)操作";
    if(ui->listWidget_showfiles->currentItem() == NULL) {
        QMessageBox::warning(this, "Rename (file)folder!", "invalid clicked!");
        return;
    }
    QString cur_path = TcpClient::getInstance().get_curpath();//当前文件路径
    QString filename = ui->listWidget_showfiles->currentItem()->text().toStdString().c_str();//原文件名
    QString newfilename = QInputDialog::getText(this, "rename (file)folder", "Please input (file)folder name!");
    if(newfilename.size() > 32) {
        QMessageBox::warning(this, QString("try to rename %1").arg(newfilename), "Folder name can't longer than 32! ");
        return;
    }
    PDU *pdu = mkPDU_2(cur_path.size() * 2, ENUM_MSG_TYPE_RENAME_DIR_REQUEST, filename, newfilename, cur_path);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

///单线程函数：
//void Files::on_pushButton_overloadfile_clicked()//上传文件槽函数
//{
//     if(!TcpClient::getInstance().get_onlienstate())return;
//    QString cur_filepath = TcpClient::getInstance().get_curpath();
//    struploadfilepath = QFileDialog::getOpenFileName();//获取要选择的文件的路径 打开窗口
//    qDebug()<<"要上传文件的路径："<<struploadfilepath;
//    if(struploadfilepath == ""){
//        QMessageBox::warning(this,"upload file","please choose your file!");
//        return;
//    }
//    int index = struploadfilepath.lastIndexOf('/');
//    upload_file_name =struploadfilepath.right(struploadfilepath.size()-index-1);//上传的文件名
//    qDebug()<<"要上传文件名字："<<upload_file_name;
//    QFile upload_file(struploadfilepath);
//    file_total_size = upload_file.size();
//    QString filesize = QString::number(file_total_size);//获取文件大小
//    qDebug()<<"文件大小"<<filesize;
//    ///传递的信息： 文件路径、消息类型、文件名字、文件大小  caData:文件大小 文件名 caMSg 当前路径

//    PDU* pdu = mkPDU_2(cur_filepath.size()*2,ENUM_MSG_TYPE_upload_FILE_REQUEST,
//                       upload_file_name,filesize,cur_filepath);//caData:文件大小 文件名 caMSg 当前路径
//    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
//    delete pdu;
//    pdu=NULL;
//    file_send_size = 0;
//    m_ptimer_up->start(1000);//设置定时 开关1s后启动
//}

///多线程函数：
void Files::on_pushButton_overloadfile_clicked()//上传文件槽函数
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    QString cur_filepath = TcpClient::getInstance().get_curpath();
    struploadfilepath = QFileDialog::getOpenFileName();//获取要选择的文件的路径 打开窗口
    qDebug() << "要上传文件的路径：" << struploadfilepath;
    if(struploadfilepath == "") {
        QMessageBox::warning(this, "upload file", "please choose your file!");
        return;
    }
    int index = struploadfilepath.lastIndexOf('/');
    upload_file_name = struploadfilepath.right(struploadfilepath.size() - index - 1); //上传的文件名
    qDebug() << "要上传文件名字：" << upload_file_name;
    QFile upload_file(struploadfilepath);
    file_total_size = upload_file.size();
    QString filesize = QString::number(file_total_size);//获取文件大小
    qDebug() << "文件大小" << filesize;
    ///传递的信息： 文件路径、消息类型、文件名字、文件大小  caData:文件大小 文件名 caMSg 当前路径
//    MyWork *mywork;
//    QThread *m_thread;
//    mywork = new MyWork;
//    m_thread = new QThread;
//    mywork->moveToThread(m_thread);
//    m_thread->start();
//    connect(this, &Widget::start_connect, mywork, &MyWork::connect_to_server);
//    connect(this, &Widget::start_sendfile, mywork, &MyWork::send_file);
//    connect(mywork, &MyWork::failed_send, this, [&](QString name) {
//        QMessageBox::warning(this, "send file", QString("Failed to open %1!").arg(name));
//    });
//    connect(mywork, &MyWork::CurPercent, ui->progressBar, &QProgressBar::setValue);
//    connect(mywork, &MyWork::connectok, this, [ = ]() {
//        ui->textEdit_showMSG->setText("子线程发送文件连接服务器成功！");
//        qDebug() << "连接服务器成功！";
//    });
//    connect(mywork, &MyWork::end_send, this, [ = ]() {
//        QMessageBox::information(this, "上传文件", "上传文件结束！");
//        m_thread->quit();
//        m_thread->wait();
//        m_thread->deleteLater();
//        mywork->deleteLater();
//    });
//    emit start_connect(ui->lineEdit_ip->text(), ui->lineEdit_port->text());
//    Sleep(1000);
//    ui->progressBar->setValue(0);
//    QString file_path = ui->lineEdit_senMSG->text();
//    int index = file_path.lastIndexOf("/");//从后数第一个非/的下标
//    QString file_name = file_path.right(file_path.size() - index - 1);
//    qDebug() << "上传的文件名" << file_name;
//    emit start_sendfile(file_path, file_name);
//    connect(this,SIGNAL(directConnect()),listWorker[0],SLOT(directConnect()),Qt::DirectConnection);
    up_usr_name = TcpClient::getInstance().getName();
    QString usr_ip = TcpClient::getInstance().get_m_ip();
    QThread *up_th = new QThread;
    FileOp *up_work = new FileOp;
    up_work->moveToThread(up_th);
    up_th->start();
    ui->progressBar_up->setValue(0);
    connect(this, &Files::start_connected, up_work, &FileOp::connect_to_server);
    connect(this, &Files::up_signal, up_work, &FileOp::upload_file);
    connect(up_work, &FileOp::Failed_working, this, [ = ](QString str) {
        QMessageBox::warning(this, "upload file", QString("Failed upload file %1!").arg(str));
    });
    // connect(up_work, &FileOp::CurPercent, ui->progressBar_up, &QProgressBar::setValue);
    connect(up_work, &FileOp::connectedok, this, [ = ]() {
        qDebug() << "上传文件子线程成功连接服务器！";
        // QMessageBox::information(this, "upload file", "upload thread successfully connect to server!");
    });
    ///进度条;
    QProgressDialog *updia = new QProgressDialog(this);
    updia->reset();
    updia->setStyleSheet("background-color: rgba(255, 255, 255,175);");
    updia->setValue(0);
    updia->setRange(0, 100);
    updia->setLabelText("Please wait ...");
    updia->setWindowTitle("正在上传文件" + QString("%1").arg(upload_file_name) + "(" +
                          TcpClient::getInstance().get_file_size(file_total_size) + ")");
    updia->setFixedSize(400, 80);
    updia->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(up_work, &FileOp::CurPercent, updia, &QProgressDialog::setValue);
    updia->setCancelButtonText(nullptr);
    // ui->listWidget__show_usr_op->addItem(item);
    connect(up_work, &FileOp::end_thread, this, [ = ](QString str) {
        ui->listWidget__show_usr_op->addItem("结束上传文件" + QString("%1").arg(upload_file_name) + "(" +
                                             TcpClient::getInstance().get_file_size(file_total_size) + "):"
                                             + "(" + TcpClient::getInstance().getsystemtime() + ")。");
        updia->close();
        updia->deleteLater();
        up_th->quit();
        up_th->wait();
        up_th->deleteLater();
        up_work->deleteLater();
//        QMessageBox::information(this, "upload file", QString("Successfully upload file %1!").arg(str));
        qDebug() << "上传文件成功!";
    });
    emit start_connected(usr_ip, upload_port);
    Sleep(1000);
    emit up_signal(up_usr_name, upload_file_name, struploadfilepath
                   , cur_filepath, TcpClient::getInstance().get_m_upload_param());
    ui->listWidget__show_usr_op->addItem("开始上传文件" + QString("%1").arg(upload_file_name) + "(" +
                                         TcpClient::getInstance().get_file_size(file_total_size) + "):"
                                         + "(" + TcpClient::getInstance().getsystemtime() + ")。");
}



///单线程函数：
//void Files::uploadfile()//上传文件
//{

//    QProgressDialog upprogress;
//    QFont mfont("songti");
//    mfont.setWeight(16);
//    upprogress.reset();
//    upprogress.setMinimumDuration(0);
//    upprogress.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);		//不显示进度条上的“最小化”“最大化”“关闭”
//    upprogress.setFont(mfont);
//    upprogress.setFixedSize(300,60);
//    upprogress.setLabelText("please wait...");	//显示的文本
//    //upprogress.setModal(true);//以模态方式弹出对话框
//    upprogress.setRange(0,100);				//设置进度条的极值，默认为[0,100]
//    upprogress.setCancelButton(NULL);			//不显示取消按钮
//    upprogress.hide();
//    upprogress.setWindowModality(Qt::ApplicationModal);
//    upprogress.setWindowTitle("Upload " + upload_file_name+"("+ TcpClient::getInstance().get_file_size(file_total_size) + ")");
//    m_ptimer_up->stop();
//    upprogress.show();
//    QFile file(struploadfilepath);//当前要上传的文件
//    if(!file.open(QIODevice::ReadOnly)){//以只读的形式打开
//        //没打开提示
//        QMessageBox::warning(this,"try to open uploadfile","Failed to open upload file!");
//        return;
//    }
//    char* pBuffer = new char[4096];//循环读取文件大小的字节数4096
//    qint64 ret{0};//实际读取到数据的大小
//    qint64 bytestowrite = 0;
//    while(1){
////        qDebug()<<"文件上传中";
//        ret = file.read(pBuffer,4096);//读取数据并返回大小

////        qDebug()<<"发送的数据："<<pBuffer<<"数据大小:"<<ret;
//        bytestowrite = TcpClient::getInstance().getTcpSocket().bytesToWrite();
////        qDebug()<<"ret"<<ret;
////        qDebug()<<"bytestopwrite"<<bytestowrite;
//        if(bytestowrite > 1048575){//设置门限值flush掉
//            TcpClient::getInstance().getTcpSocket().flush();
////            qDebug()<<"触发flush函数！bytestowrite"<<bytestowrite;
////            file_send_size+=bytestowrite;
////            upprogress.setValue(double(file_send_size)/file_total_size*100);
//        }
//        if(ret > 0 && ret <= 4096){
//            TcpClient::getInstance().getTcpSocket().write(pBuffer,ret);//发送实际读取到的数据的大小
//        }else if(ret == 0){//读取数据结尾
//            file_send_size = 0;
//            break;
//        }else{
//            upprogress.hide();
//            QMessageBox::warning(this,"try to upload file","Failed to upload file!,read file error!");
//            file_send_size = 0;
//            return;
//        }
//        file_send_size+=ret;
//        upprogress.setValue(double(file_send_size)/file_total_size*100);
//        memset(pBuffer,' ',sizeof(&pBuffer));

//    }
//    delete[] pBuffer;
//    file.close();
//}


void Files::on_pushButton_downloadfile_clicked()//下载文件 从自己云盘下载
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    if(ui->listWidget_showfiles->currentItem() == NULL) {
        QMessageBox::warning(this, "download file", "please choose your file to download");
        return;
    }//弹出路径选择框
    QString file_name = ui->listWidget_showfiles->currentItem()->text();//下载的文件名
    for(auto x : file_name) {
        if(x == ':') {
            return;
        }
    }
    download_file_name = file_name;
    QString file_path = TcpClient::getInstance().get_curpath() + file_name; //云盘中要下载的路径
    QString save_path = QFileDialog::getExistingDirectory();
    if(save_path == "") {
        QMessageBox::warning(this, "download file", "please choose your save path!");
        return;
    }
    save_path += +"/" + file_name; //文件保存路径
//    QString save_path = QFileDialog::getSaveFileName();
    TcpClient::getInstance().set_file(save_path);//创建要下载的文件
    TcpClient::getInstance().change_download_path(save_path);
//    qDebug() << "下载的文件：" << file_name << "保存路径" << save_path;
//    qDebug() << "申请下载的随机端口号！qq" << download_port;
    PDU *pdu = mkPDU_2(file_path.size() * 2, ENUM_MSG_TYPE_download_FILE_REQUEST, download_port,
                       TcpClient::getInstance().getName(), file_path); //要下载的文件名 文件路径发送给服务器；
    //本机ip也要发给服务器
    qDebug() << "客户端的IP是" << TcpClient::getInstance().get_mydownip();
    memcpy(pdu->ELSE, TcpClient::getInstance().get_mydownip().toStdString().c_str(), 64);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
//   start_listen(download_port);
}

void Files::on_pushButton_show_msg_folders_clicked()
{
    QListWidgetItem *item = ui->listWidget_showfiles->currentItem();
    if(item == NULL) {
        return;
    }
    QString file_name = item->text();
    QString cur_path = TcpClient::getInstance().get_curpath();
    QString file_path = cur_path + file_name;
    QFileInfo fileinfo(file_path);
    PDU *pdu = mkPDU_2(file_path.size() * 2, ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST, file_name, "", file_path);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
}

void Files::on_pushButton_sharefile_clicked()//分享文件
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    if(NULL == ui->listWidget_showfiles->currentItem()) {
        QMessageBox::warning(this, "Share file", "Please choose your file to share!");
        return;
    }
    shared_filename = ui->listWidget_showfiles->currentItem()->text();
    QListWidget *pfrilist =  Operatewidget::getInstance().getusrfir()->get_fri_list();
    for(int i{0}; i < pfrilist->count(); ++i) {
        qDebug() << pfrilist->item(i)->text();
    }
    shareFile::getInstance().upodatefir(pfrilist);
    shareFile::getInstance().setWindowTitle("Share file");
    if(shareFile::getInstance().isHidden()) {
        shareFile::getInstance().show();
    } else {
        shareFile::getInstance().close();
    }
}

void Files::on_pushButton_sure_movefile_folders_clicked()
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    if(ui->listWidget_showfiles->currentItem() == NULL) {
        QMessageBox::warning(this, "moeve file", "Please select your destination!");
        return;
    }
    QString dest_path = TcpClient::getInstance().get_curpath() + ui->listWidget_showfiles->currentItem()->text();
    PDU *pdu = mkPDU_1(400, ENUM_MSG_TYPE_MOVE_FILE_REQUEST);
    memcpy(pdu->caData, move_file.toStdString().c_str(), 32);
    //memcpy(pdu->caData+32,QString::number(move_file_path.size()).toStdString().c_str(),32);
    memcpy((char *)pdu->caMsg, move_file_path.toStdString().c_str(), 200);
    memcpy((char *)pdu->caMsg + 200, dest_path.toStdString().c_str(), 200);
    TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
    delete pdu;
    pdu = NULL;
    ui->pushButton_sure_movefile_folders->setEnabled(false);
}

void Files::on_pushButton_move_file_clicked()//移动文件（夹）
{
    if(!TcpClient::getInstance().get_onlienstate()) {
        return;
    }
    if(ui->listWidget_showfiles->currentItem() == NULL) {
        QMessageBox::warning(this, "moeve file", "Please select your file to move!");
        return;
    }
    move_file = ui->listWidget_showfiles->currentItem()->text();
    move_file_path = TcpClient::getInstance().get_curpath() + move_file;
    ui->pushButton_sure_movefile_folders->setEnabled(true);
    QMessageBox::information(this, "moeve file", "Please select your destination");
}

void Files::handle_initial_failed(int id)
{
    QThread *thread = my_thread[id];//找到次线程
    QString sThreadId = QStringLiteral("%1").arg(quintptr(thread->currentThreadId()));
    QMessageBox::warning(this, QString("Thread %1 failed to inital").arg(sThreadId), "Failed to create file or open file!");
}

void Files::handle_failed_working(int id)
{
    QThread *thread = my_thread[id];//找到次线程
    QString sThreadId = QStringLiteral("%1").arg(quintptr(thread->currentThreadId()));
    QMessageBox::warning(this, QString("Thread %1 failed to working").arg(sThreadId), "Failed to working!");
}

void Files::handle_end_thread(int id, FileOp *fileop)
{
    QThread *thread = my_thread[id];//找到次线程
    my_thread.erase(id);//删除线程
    thread->quit();
    thread->deleteLater();
    fileop->deleteLater();
}

void Files::uploadfile_th()
{
    up_t->stop();
//    emit Thread_file_signal(TcpClient::getInstance().getName(), upload_file_name, struploadfilepath,
//                            QString::number(file_total_size),
//                            TcpClient::getInstance().get_m_ip(), upload_port);
    index++;
    upload_port++;
}
