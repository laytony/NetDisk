#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include <string.h>
#include <QString>

using namespace std;

typedef unsigned int uint;

#define REGISTER_OK "register ok!"
#define REGISTER_FAILED "register failed: this name has been registerd!"
#define LOGIN_OK "login ok!"
#define LOGIN_FAILED "LOGIN failed: this ID has logined or ERROR ID or ERROR psw!"
#define LOGOFF_OK "logoff ok!"
#define LOGOFF_FAILED "LOGOFF failed: this ID has logoffed or ERROR ID or ERROR psw!"
#define DESTORY_OK "destory ok!"
#define DESTORY_FAILED "DESTORY failed: this ID does not exist or ERROR ID or ERROR psw!"
#define ADD_NOTONLIE "Failed to add! Usr not online!"
#define ALREADY_FRIENDS "The person has been your friend!"

#define SYSTEM_BREAKDOWN "Cann't add yourself!"

#define UPLOAD_PORT 10000//定义下载 上传的端口号 最多十个
#define DOWNLOAD_PORT 11000


enum ENUM_MSG_TYPE { //使用枚举定义消息类型
    ENUM_MSG_TYPE_MIN = 0,//使用枚举类型自动增长 1 2 3;
    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,//注册回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,//登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,//登录回复
    ENUM_MSG_TYPE_LOGOFF_REQUEST,//退出登陆请求
    ENUM_MSG_TYPE_LOGOFF_RESPOND,//退出登录回复
    ENUM_MSG_TYPE_DESTORY_REQUEST,//退出登陆请求
    ENUM_MSG_TYPE_DESTORY_RESPOND,//退出登录回复

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,//获取所有在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,//获取所有在线用户回复


    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,//获取搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,//获取搜索用户回复

    ENUM_MSG_TYPE_ADD_USR_REQUEST,//添加好友请求
    ENUM_MSG_TYPE_ADD_USR_RESPOND,//添加好友回复


    ENUM_MSG_TYPE_YES_ADD_USR_REQUEST,//同意添加好友请求
    ENUM_MSG_TYPE_NO_ADD_USR_RESPOND,//拒绝添加好友回复
    ENUM_MSG_TYPE_NO_ADD_USR_REQUEST,//拒绝添加好友请求
    ENUM_MSG_TYPE_YES_ADD_USR_RESPOND,//同意添加好友回复

    ENUM_MSG_TYPE_FLUSH_FRI_REQUSET,//刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRI_RESPOND,//刷新好友回复


    ENUM_MSG_TYPE_DELETE_FRI_REQUSET,//删除好友请求
    ENUM_MSG_TYPE_DELETE_FRI_RESPOND,//删除好友回复


    ENUM_MSG_TYPE_SDELETE_FRI_REQUSET,//确定删除好友请求
    ENUM_MSG_TYPE_SDELETE_FRI_RESPOND,//确定删除好友回复

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,//私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,//私聊回复


    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,//群聊请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,//群聊回复

    ENUM_MSG_TYPE_CREATE_DIR_REQUEST,//用户创建文件夹请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND,//用户创建文件夹回复

    ENUM_MSG_TYPE_CREATE_DIR_FAILED_RESPOND,//用户创建文件夹失败回复
    ENUM_MSG_TYPE_CREATE_DIR_SUCCESS_RESPOND,//用户创建文件夹成功·回复
    ENUM_MSG_TYPE_CREATE_ROOT_DIR_RESPOND,//用户创建根路径回复

    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,//用户刷新文件夹请求
    ENUM_MSG_TYPE_FLUSH_FILE_RESPOND,//用户刷新文件夹回复

    ENUM_MSG_TYPE_DELETE_FILE_REQUEST,//用户删除文件夹请求
    ENUM_MSG_TYPE_SURE_DELETE_FILE_REQUEST,//用户确认删除文件夹请求
    ENUM_MSG_TYPE_DELETE_FILE_RESPOND,//用户删除文件夹回复


    ENUM_MSG_TYPE_RENAME_DIR_REQUEST,//重命名文件(夹)请求
    ENUM_MSG_TYPE_RENAME_DIR_RESPOND,//重命名文件(夹)回
    ENUM_MSG_TYPE_upload_FILE_REQUEST,//上传文件请求
    ENUM_MSG_TYPE_upload_FILE_RESPOND,//上传文件回复

    ENUM_MSG_TYPE_download_FILE_REQUEST,//下载文件请求
    ENUM_MSG_TYPE_download_FILE_RESPOND,//下载文件回复


    ENUM_MSG_TYPE_SHOW_FD_MSG_REQUEST,//双击显示信息文件请求
    ENUM_MSG_TYPE_SHOW_FD_MSG_RESPOND,//双击显示文件信息回复

    ENUM_MSG_TYPE_SHARE_FILE_REQUEST,//分享文件请求
    ENUM_MSG_TYPE_SHARE_FILE_RESPOND,//分享文件回复

    ENUM_MSG_TYPE_SHARE_FILE_RE_RESPOND,//拒绝接受文件分享

    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,//移动文件夹请求
    ENUM_MSG_TYPE_MOVE_FILE_RESPOND,//移动文件夹回复

    ENUM_MSG_TYPE_RELOGIN_REQUEST,
    ENUM_MSG_TYPE_UPDATE_DOWNLOADFILE_SIZE,

    ENUM_MSG_TYPE_REDOWNLOAD,//更换下载文件的端口号

    ENUM_MSG_TYPE_IMG,//图片专用
    ENUM_MSG_TYPE_WZQ,//五子棋游戏
    ENUM_MSG_TYPE_WZQ_A,//五子棋游戏回复






//    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
//    ENUM_MSG_TYPE_REGIST_RESOND,//注册回复
    //    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    //    ENUM_MSG_TYPE_REGIST_RESOND,//注册回复
    //    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    //    ENUM_MSG_TYPE_REGIST_RESOND,//注册回复
    //    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    //    ENUM_MSG_TYPE_REGIST_RESOND,//注册回复
    //    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    //    ENUM_MSG_TYPE_REGIST_RESOND,//注册回复
    ENUM_MSG_TYPE_MAX = 0x00ffffff//定义成32位符合无符号整型的类型Uint
};

struct FileInfo {
    char caFiamename[32];//文件（夹名字）
    int caFiletype;//文件（夹）类型
    char caSize[32];//文件大小
    char createTime[64];//创建时间
    char lastVisitedtime[64];//最后访问时间
    char lastmodify[64];//最后修改时间
    char file_type[16];//存放文件类型
    bool is_File;
    bool is_folder;
    bool is_canread;
    bool is_canwrie;
    bool is_hidden;
    bool is_exe;

};

FileInfo *mkFileInfo_0();
FileInfo *mkFileInfo(QString name, int type);
FileInfo *mkFileInfo_1(QString name, QString caSize, QString createTime, QString lastVisitedtime, QString lastmodify,
                       bool is_File, bool is_folder, bool is_canread, bool is_canwrie, bool is_hidden, bool is_exe);





class PDU
{
public:

    uint uiPDULen;//总协议单元的大小
    uint uiMsgType;//消息的类型 登陆 加好友 上传 下载等等
    char caData[64];//发送的  存放文件名
    char ELSE[64];
    uint uiMsgLen;//实际消息的长度 除了最后一个的长度
    int caMsg[];//实际消息 总长度为128+12+caMsg的大小
    PDU() {}
};

PDU *mkPDU(uint uiMsgLen);
PDU *mkPDU_1(uint uiMsgLen, uint uiMsgType);
PDU *mkPDU_2(uint uiMsgLen, uint uiMsgType, QString usr_name, QString other_name, QString msg);


#endif // PROTOCOL_H
