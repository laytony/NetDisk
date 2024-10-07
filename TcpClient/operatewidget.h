#ifndef OPERATEWIDGET_H
#define OPERATEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QKeyEvent>
#include "usrfri.h"
#include "files.h"
#include <QStackedWidget>
#include <QHBoxLayout>
#include <unordered_map>
#include <QStatusBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include "chessboard.h"

class UsrFri;
class Files;//这样在FIles中就能包含本头文件
class TcpClient;

class Operatewidget : public QWidget
{
    Q_OBJECT
public:
    explicit Operatewidget(QWidget *parent = nullptr);
    static Operatewidget &getInstance();
    UsrFri *getusrfir();
    Files *get_usrfiles();
    void set_unonline();
    ~Operatewidget();

public slots:
    void Init_filelist(QListWidgetItem *item);
    void delete_all_pchats();
    void reconnect();//尝试重新连接
    void change_connected();//改变连接状态
signals:
    void close_window();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QListWidget *m_pListW;
    UsrFri *fri;
    Files *m_files;
    QStackedWidget *m_psw;
    QStatusBar *st_bar;
    QLabel *m_lable;
    QLabel *m_lable1;
    QPushButton *try_connect_to_host;



};

#endif // OPERATEWIDGET_H
