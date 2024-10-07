#ifndef USRFRI_H
#define USRFRI_H

#include <QWidget>
#include "protocol.h"
#include "tcpclient.h"
#include "privatechat.h"
#include "unordered_set"
#include "unordered_map"
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QMimeData>
#include <QTimer>
#include "files.h"
#include "chessboard.h"


namespace Ui
{
    class UsrFri;
}

class UsrFri : public QWidget
{
    Q_OBJECT

public:
    explicit UsrFri(QWidget *parent = nullptr);
    void show_ALLONLINE_usr(PDU *pdu);
    void show_search_usr(PDU *pdu);
    void updatefriendlist(PDU *pdu);//刷新好友列表
    void show_group_msg(PDU *pdu);//群聊操作
    void show_pchat_msg(PDU *pdu); //私聊操作
    void auto_close_pchat();//用户掉线关闭所有私聊窗口
    static UsrFri &getinstance();

    unordered_map<QString, Privatechat *> get_pchat_window(); //获取所有私聊窗口

    QListWidget *get_fri_list();//获取好友列表
    bool eventFilter(QObject *object, QEvent *event) override;//触发事件
    void Input_setfocus();//设置光标
    ~UsrFri();

private slots:


    void on_pushButton_searchusrs_clicked();

    void on_ADD_PB_clicked();


    void on_pushButton_showonlineusrs_clicked();//显示在线用户
    void on_pushButton_deletefri_clicked();

    void on_pushButton_private_chat_clicked();

    void on_pushButton_sendMSG_clicked();

    void on_pushButton_private_chat_close_clicked();

    void on_m_showMSG_textChanged();

    void on_pushButton_wqz_clicked();

public slots:
    void on_pushButton_flushfries_clicked();//刷新好友列表按钮
    void doubleclicked_pchat(QListWidgetItem *item);
    void show_lt_img();
signals:
    void recvmsg(PDU *pdu);


private:
    Ui::UsrFri *ui;
//    Privatechat* pchat;
    unordered_map<QString, Privatechat *>pchat_hash; //friend_name - > 窗口

///实现拖动文件放入框内发送小文件 小图片
protected:
    void dragEnterEvent(QDragEnterEvent *event) override; // 拖动进入事件
    void dropEvent(QDropEvent *event) override;           // 放下事件

//    PDU* mypdu;

protected://滚轮缩放文字大小
    void wheelEvent(QWheelEvent *e);//滚轮事件

private:
    QString input_img_path;
    QTimer *dltimer;
    QString ltimg;
};

#endif // USRFRI_H
