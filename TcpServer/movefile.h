#ifndef MOVEFILE_H
#define MOVEFILE_H

#include <QObject>
#include <QThread>
#include <protocol.h>

class MoveFile_ : public QThread
{
    Q_OBJECT
public:
    explicit MoveFile_(QString move_flie, QString from_path, QString dest_path, QObject *parent = nullptr);
    void Copy_dir(QString from_path, QString to_path); //文件夹的拷贝！；

private:
    QString from_path;
    QString dest_path;
    QString move_file;
protected:
    void run() override;

signals:
    void end_move(int ret);
};

#endif // MOVEFILE_H
