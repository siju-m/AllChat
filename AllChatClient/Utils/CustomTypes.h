#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include <QObject>

namespace CommonEnum {
enum message_type{
    IMAGE,//图片消息
    LOGIN,//登录请求
    REGISTER,//注册请求
    CHAT,//聊天消息
    LOGIN_SUCCESS,LOGIN_FAILED,//登陆结果
    REGISTER_SUCCESS,REGISTER_FAILED,//注册结果
    USER_LIST,//用户列表消息
    ADD_FRIEND,//添加好友消息
    AGREE_FRIEND,//同意好友请求
    FIND_NEW_FRIEND,//查询用户请求
    NEW_FRIEND_REULT,//查询用户结果
    ONLINE_STATE,//在线用户状态
    ONLINE_LIST,
    UPDATE_AVATAR,
    UPDATE_AVATAR_RESULT,
    REFRESH_AVATAR,
    DELETE_FRIEND,
    CREATE_GROUP,
    GROUP_LIST,
    GROUP_STRANGER_LIST,
    GROUP_CHAT,
    PRIVATE_FILE
};
}

namespace StateEnum{
enum onlineState_type{
    NONE,
    ONLINE,
    OFFLINE,
    BUSY
};
}

#endif // CUSTOMTYPES_H
