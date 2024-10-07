#include "protocol.h"

PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen; //整个PDU单元的大小由 前面存在的大小+后面存放数据的实际长度
    PDU *pdu = (PDU *)malloc(uiPDULen); //生成符合实际大小的结构体 字节大小
    if(!pdu) {
        exit(EXIT_FAILURE);
    }
    memset(pdu, 0, uiPDULen); //初始化将 pdu大小为uiPDULen全部初始化为0
    pdu->uiMsgLen = uiMsgLen;
    pdu->uiPDULen = uiPDULen;
    return pdu;
}


PDU *mkPDU_1(uint uiMsgLen, uint uiMsgType)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen; //整个PDU单元的大小由 前面存在的大小+后面存放数据的实际长度
    PDU *pdu = (PDU *)malloc(uiPDULen); //生成符合实际大小的结构体
    if(!pdu) {
        exit(EXIT_FAILURE);
    }
    memset(pdu, 0, uiPDULen); //初始化将 pdu大小为uiPDULen全部初始化为0
    pdu->uiMsgLen = uiMsgLen;
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgType = uiMsgType;
    return pdu;
}

PDU *mkPDU_2(uint uiMsgLen, uint uiMsgType, QString usr_name, QString other_name, QString msg)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen; //整个PDU单元的大小由 前面存在的大小+后面存放数据的实际长度
    PDU *pdu = (PDU *)malloc(uiPDULen); //生成符合实际大小的结构体
    if(!pdu) {
        exit(EXIT_FAILURE);
    }
    memset(pdu, 0, uiPDULen); //初始化将 pdu大小为uiPDULen全部初始化为0
    pdu->uiMsgLen = uiMsgLen;
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgType = uiMsgType;
    if(usr_name != "") {
        memcpy(pdu->caData, usr_name.toStdString().c_str(), 32);
    }
    if(other_name != "") {
        memcpy(pdu->caData + 32, other_name.toStdString().c_str(), 32);
    }
    if(msg != "") {
        memcpy((char *)pdu->caMsg, msg.toStdString().c_str(), uiMsgLen);    //整个拷贝
    }
    return pdu;
}



FileInfo *mkFileInfo(QString name, int type)
{
    FileInfo *fileinfo = (FileInfo *)malloc(sizeof(FileInfo));
    strcpy(fileinfo->caFiamename, name.toStdString().c_str());
    fileinfo->caFiletype = type;
    return fileinfo;
}


FileInfo *mkFileInfo_0()
{
    return (FileInfo *)malloc(sizeof(FileInfo));
}

FileInfo *mkFileInfo_1(QString name, QString caSize, QString createTime, QString lastVisitedtime, QString lastmodify, bool is_File, bool is_folder, bool is_canread, bool is_canwrie, bool is_hidden, bool is_exe)
{
    FileInfo *fileinfo = (FileInfo *)malloc(sizeof(FileInfo));
    strcpy(fileinfo->caFiamename, name.toStdString().c_str());
    strcpy(fileinfo->caSize, caSize.toStdString().c_str());
    strcpy(fileinfo->createTime, createTime.toStdString().c_str());
    strcpy(fileinfo->lastVisitedtime, lastVisitedtime.toStdString().c_str());
    strcpy(fileinfo->lastmodify, lastmodify.toStdString().c_str());
    fileinfo->is_File = is_File;
    fileinfo->is_folder = is_folder;
    fileinfo->is_canread = is_canread;
    fileinfo->is_canwrie = is_canwrie;
    fileinfo->is_exe = is_exe;
    fileinfo->is_hidden = is_hidden;
    return fileinfo;
}
