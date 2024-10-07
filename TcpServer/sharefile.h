#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QThread>

class ShareFile : public QThread
{
    Q_OBJECT
public:
    explicit ShareFile(QString from_path, QString to_path, QObject *parent = nullptr);
    void Copy_dir(QString from_path, QString to_path); //文件夹的拷贝！；
signals:
    void end_share();
private:
    QString from_path;
    QString dest_path;
protected:
    void run() override;

};

#endif // SHAREFILE_H
