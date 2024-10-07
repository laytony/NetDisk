#include "boook.h"

Boook::Boook(QWidget *parent) : QWidget(parent){
    this->m_pShowMsgTE = new QTextEdit;
    this->m_pFriendLW = new QListWidget;
    QVBoxLayout* pRightPBVBL = new QVBoxLayout;
    pRightPBVBL->addWidget(m_pShowMsgTE);
    pRightPBVBL->addWidget(m_pFriendLW);
    setLayout(pRightPBVBL);
}
