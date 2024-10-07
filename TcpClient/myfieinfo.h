#ifndef MYFIEINFO_H
#define MYFIEINFO_H

#include <QWidget>


namespace Ui {
class myfieinfo;
}

class myfieinfo : public QWidget
{
    Q_OBJECT

public:
    explicit myfieinfo(QWidget *parent = nullptr);
    ~myfieinfo();
    void upgrade_finfo(QString name, QString size,QString createTime, QString lastVisitedtime, QString lastmodify, bool is_File, bool is_folder,
                       bool is_canread, bool is_canwrie, bool is_hidden, bool is_exe);
    //    FileInfo* rfiinfo = mkFileInfo_1(file_name,is_file,create_time,last_visited,last_modify,is_file,is_dir,
    //                                     isReadable,isWritable,isHidden,isExecutable);

private:
    Ui::myfieinfo *ui;
};

#endif // MYFIEINFO_H
