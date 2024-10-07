#include "movefile.h"

#include <QDir>
#include <QFileInfo>

MoveFile_::MoveFile_(QString move_file, QString from_path, QString dest_path, QObject *parent) : QThread(parent)
{
    this->from_path = from_path;
    this->dest_path = dest_path;
    this->move_file = move_file;
}

void MoveFile_::Copy_dir(QString from_path, QString to_path)
{
    QDir dir;
    dir.mkdir(to_path);
    dir.setPath(from_path);
    QString from_temp, to_temp;
    QFileInfoList infolist = dir.entryInfoList();
    for(int i{0}; i < infolist.size(); ++i) {
        if(infolist[i].fileName() == "." || infolist[i].fileName() == "..") {
            continue;
        }
        from_temp = from_path + "/" + infolist[i].fileName();
        to_temp = to_path + "/" + infolist[i].fileName();
        if(infolist[i].isFile()) {
            QFile::copy(from_temp, to_temp);
        } else if(infolist[i].isDir()) {
            Copy_dir(from_temp, to_temp);
        }
    }
}

void MoveFile_::run()
{
    QFileInfo to_file(dest_path);
    dest_path += "/" + QString(move_file);
    QFileInfo from_file(from_path);
    to_file.setFile(dest_path);
    if(from_file.isFile()) { //常规文件的拷贝
        QFile::copy(from_path, dest_path);
    } else if(from_file.isDir()) { //拷贝文件夹
        Copy_dir(from_path, dest_path);
    }
    bool ret{false};
    if(from_file.isDir()) { //是否是文件夹
        QDir qdir;//存放根路径
        qdir.setPath(from_path);
        if(qdir.removeRecursively()) { //删除文件
            ret = true;
        }
    } else { //常规文件
        QFile qfile(from_path);
        if(qfile.remove()) {
            ret = true;
        }
    }
    emit end_move(ret);
}
