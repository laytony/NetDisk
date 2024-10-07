#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include <QButtonGroup>
#include <QListWidget>
#include <QVBoxLayout>
#include "tcpclient.h"
#include "operatewidget.h"

namespace Ui {
class shareFile;
}

class shareFile : public QWidget
{
    Q_OBJECT

public:
    explicit shareFile(QWidget *parent = nullptr);
    ~shareFile();
    void test();
    void upodatefir(QListWidget* listw);//更新分享列表中好友
    static shareFile &getInstance();
public slots://自定义槽函数、
    //取消选择槽函数
    void cancle_select();
    void select_all();
    void sure_to_select();//确定选择
    void cancle_to_select();//取消选择


private:
    Ui::shareFile *ui;
    QButtonGroup* group_button;
    QVBoxLayout* vbox;
};

#endif // SHAREFILE_H
