#include "sharefile.h"

#include <QDir>
#include <QFileInfo>

ShareFile::ShareFile(QString from_path, QString to_path, QObject *parent) : QThread(parent)
{
    this->from_path = from_path;
    dest_path = to_path;
}

void ShareFile::Copy_dir(QString from_path, QString to_path)
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

void ShareFile::run()
{
    QFileInfo from_file(from_path);
    QFileInfo to_file(dest_path);
    if(from_file.isFile()) { //常规文件的拷贝
        QFile::copy(from_path, dest_path);
    } else if(from_file.isDir()) {
        Copy_dir(from_path, dest_path);
    }
}
