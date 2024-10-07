#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include "tcpclient.h"
#include <unordered_map>
#include "chessboard.h"

namespace Ui
{
    class Privatechat;
}

class Privatechat : public QWidget
{
    Q_OBJECT

public:
    explicit Privatechat(QWidget *parent = nullptr);
    ~Privatechat();
    void receive_msg(PDU *pdu);
    void close_pchat();//关闭私聊窗口
    void upgrade_twopchat(QString tname);
    QString get_pchatusr();
    bool eventFilter(QObject *object, QEvent *event) override;//触发事件

private slots:
    void on_pushButton_sendmsg_clicked();

    void on_pushButton_wzq_clicked();//开启五子棋游戏

private:
    QString pchatfri;
    Ui::Privatechat *ui;
    Chessboard *m_chessboard;

};

#endif // PRIVATECHAT_H
