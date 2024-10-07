#ifndef BANGGO_H
#define BANGGO_H

#include <QWidget>

namespace Ui {
class Banggo;
}

class Banggo : public QWidget
{
    Q_OBJECT

public:
    explicit Banggo(QWidget *parent = nullptr);
    ~Banggo();

private:
    Ui::Banggo *ui;
};

#endif // BANGGO_H
