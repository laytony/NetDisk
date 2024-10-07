#ifndef BOOOK_H
#define BOOOK_H

#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QVBoxLayout>


class Boook : public QWidget
{
    Q_OBJECT
public:
    explicit Boook(QWidget *parent = nullptr);

signals:

private:
    QTextEdit * m_pShowMsgTE;
    QListWidget *m_pFriendLW;

};

#endif // BOOOK_H
