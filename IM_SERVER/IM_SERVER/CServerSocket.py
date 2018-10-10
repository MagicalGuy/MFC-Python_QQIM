#encoding:utf-8
from enum import Enum
from select import select
from socket import *
from CDataBase import *
import struct
import queue
import random
import time


# 枚举类型
class EnumMessageType(Enum):
    REGISTER = 1  # 注册
    LOGIN = 2  # 登录
    SEARCHUSER = 3  # 搜索
    ADDFRIEND = 4  # 添加好友
    ONE2ONE = 5  # 1v1
    ONE2GROUP = 6  # 1vN
    MSGTEXT = 7  # 文字消息,可插入表情
    MSGOFFLINE = 8  # 离线消息
    MSGIMAGE = 9  # 图片
    MSGVOICE = 10  # 语音
    MSGVIDEO = 11  # 视频
    FILETRANS = 12  # 文件传输
    UPDATEOL = 13  # 更新在线状态
    UPDATEFRIEND = 14  # 更新好友列表
    UPDATEGROUP = 15  # 更新群列表
    UPDATEGROUPMEMBER = 16  # 更新群成员列表
    SEARCHGROUP = 17  # 搜索群
    REPLOGIN = 18  # 重复登录
    MSGRECORD = 19 #聊天记录


# 套接字类
class CServerSocket():
    # 构造函数
    def __init__(self, ip, port):
        ADDR = (ip, port)

        # 初始化
        print('正在启动服务器')
        self.socketServer = socket(AF_INET, SOCK_STREAM)
        self.socketServer.bind(ADDR)
        self.socketServer.listen(SOMAXCONN)

        # 设为非阻塞
        self.socketServer.setblocking(False)
        print('服务器启动成功,等待客户端连接...')

        # 监听列表
        self.input_list = [self.socketServer]
        self.output_list = []
        #客户端
        self.client_dict = {}
        #消息
        self.msg_dict = {}

        # 通信列表
        #接收
        self.recv_list = []
        #发送
        self.send_list = []
        #错误
        self.err_list = []

        # 连接数据库
        self.coon = CDataBase()

    # 选择模型
    def ChatSelect(self):
        # 循环接收
        while True:
            # 初始化列表
            self.recv_list, self.send_list, self.err_list = select(self.input_list, self.output_list, self.input_list)
            # 处理客户端
            self.__RecvClient__()
            # 处理消息
            self.__MsgMange__()
            # 处理异常
            self.__Abormal__()

    # 处理接收客户端
    def __RecvClient__(self):

        for sock in self.recv_list:
            # 服务器响应客户端
            if sock is self.socketServer:
                client, addr = sock.accept()
                # 加入客户端列表;
                self.input_list.append(client)
                # 设置客户端异步接收消息
                client.setblocking(False)
                print(client.getpeername(), '已上线')
            # 处理客户端消息
            else:
                try:
                    msg = sock.recv(1024)
                    #检测数据是否有效
                    if len(msg)<1024:
                        continue
                    # 判断消息类型
                    type, = struct.unpack("i", msg[:4])
                    # 注册或登录信息立即处理
                    if type == EnumMessageType.REGISTER.value:
                        self.__ChatForRegister__(sock, msg)
                    elif type == EnumMessageType.LOGIN.value:
                        self.__ChatForLogin__(sock, msg)
                    # 过滤无效信息
                    elif type == 0:
                        pass
                    # 其他消息存入字典待处理
                    else:
                        # 创建消息队列
                        self.msg_dict[sock] = queue.Queue()
                        self.msg_dict[sock].put(msg)
                        # 放入监听发送消息列表
                        if sock not in self.output_list:
                            self.output_list.append(sock)
                # 异常处理
                except ConnectionResetError:
                    # 客户端断开,从监听列表移除
                    self.input_list.remove(sock)
                    print(sock.getpeername(), '已下线')
                    # 将客户端从监听列表中移除
                    if sock in self.output_list:
                        self.output_list.remove(sock)
                    # 移除客户端消息队列
                    if sock in self.msg_dict:
                        del self.msg_dict[sock]

                    # 移除客户端在线状态
                    for key, val in self.client_dict.items():
                        if self.client_dict[key] == sock:
                            del self.client_dict[key]
                            break

    # 转发消息处理
    def __MsgMange__(self):

        for sock in self.send_list:
            try:
                # 队列中有消息获取消息
                if not self.msg_dict[sock].empty():
                    # 提取消息
                    msg = self.msg_dict[sock].get()
                    # 判断消息类型
                    type, = struct.unpack("i", msg[:4])
                    # 分配至各个函数
                    self.dictFun[type](self, sock, msg)
                else:
                    # 移除监听等待下一次
                    self.output_list.remove(sock)
            # 处理异常
            except ConnectionResetError:
                # 删除消息队列，设为下线,
                del self.msg_dict[sock]
                self.output_list.remove(sock)
                print(sock.getpeername(), '已下线')

    # 异常处理
    def __Abormal__(self):
        for sock in self.err_list:
            print(sock.getpeername(), '出现异常,请尽快处理...')
            # 队列中删除
            if sock in self.input_list:
                self.input_list.remove(sock)
            if sock in self.output_list:
                self.output_list.remove(sock)
            # 消息队列中删除
            if sock in self.msg_dict:
                del self.msg_dict[sock]

    # 注册
    def __ChatForRegister__(self, s, msg):
        # 消息处理
        name, = struct.unpack("50s", msg[4:54])
        pwd, = struct.unpack("50s", msg[54:104])
        name = name.decode('gbk').rstrip('\0')
        pwd = pwd.decode('gbk').rstrip('\0')
        uid = random.randint(10000, 99999)

        # 提交数据库
        result = self.coon.DML("INSERT INTO `userInfo` (`userID`, `userName`, `pwd`) VALUES (%s, %s, %s)", (uid, name, pwd))
        # 返回结果

        if result == None:
            message = "".encode('gbk')
        else:
            message = (("%d" % (uid)).encode('gbk'))

        message_type = EnumMessageType.REGISTER
        message_send = struct.pack("l2048s", message_type.value, message)

        # 加入公共群组
        result = self.coon.DML("INSERT INTO `groupList` (`groupID`, `memberID`) VALUES (%s, %s)", (888888, uid))
        if result == None:
            return
        s.sendall(message_send)

    # 登录
    def __ChatForLogin__(self, s, msg):
        # 消息处理
        uid, = struct.unpack("50s", msg[4:54])
        pwd, = struct.unpack("50s", msg[54:104])
        uid = uid.decode('gbk').rstrip('\0')
        pwd = pwd.decode('gbk').rstrip('\0')

        # 查询
        result = self.coon.query("SELECT * from userinfo where userID=%s and pwd=%s", (uid, pwd))

        # 返回登录结果
        if result == None or result[1] == 0:
            message = "登录失败！".encode('gbk')
        else:
            #message = (result[0][0][1]).decode('utf8').encode('gbk')
            message = (result[0][0][1]).encode('utf8').decode('utf8').encode('gbk')
            #print(result[0][0][1])


        message_type = EnumMessageType.LOGIN
        message_send = struct.pack("l2048s", message_type.value, message)

        # 重复登录踢掉前一个
        if uid in self.client_dict:
            message_type = EnumMessageType.REPLOGIN
            message = "已在其他客户端登录！".encode('gbk')
            message_send2 = struct.pack("l50s", message_type.value, message)
            self.client_dict[uid].sendall(message_send2)

        # 加入在线客户端
        self.client_dict[uid] = s
        s.sendall(message_send)

    # 搜索用户
    def __ChatForSearchUser__(self, s, msg):
        # 消息处理
        uid, = struct.unpack("50s", msg[4:54])
        uid = uid.decode('gbk').rstrip('\0')

        # 查询用户信息
        result = self.coon.query("SELECT * from userinfo where userID=%s ", (uid,))

        if result == None or result[1] == 0:
            uid = "用户不存在！".encode('gbk')
            uname = ''.encode('gbk')
        else:
            uid = uid.encode('gbk')
            uname = (result[0][0][1]).encode('utf-8').decode('utf8').encode('gbk')

        message_type = EnumMessageType.SEARCHUSER
        message_send = struct.pack("l50s50s", message_type.value, uid, uname)

        # 发送
        s.sendall(message_send)

    # 搜索群
    def __ChatForSearchGroup__(self, s, msg):
        # 消息处理
        gid, = struct.unpack("50s", msg[4:54])
        gid = gid.decode('gbk').rstrip('\0')

        # 查询用户信息
        result = self.coon.query("SELECT * from groupInfo where groupID=%s ", (gid,))

        if result == None or result[1] == 0:
            gid = "此群不存在！".encode('gbk')
            uname = ''.encode('gbk')
        else:
            gid = gid.encode('gbk')
            uname = (result[0][0][1]).encode('utf-8').decode('utf8').encode('gbk')

        message_type = EnumMessageType.SEARCHGROUP
        message_send = struct.pack("l50s50s", message_type.value, gid, uname)
        # 发送
        s.sendall(message_send)

    # 添加好友
    def __ChatForAddFriend__(self, s, msg):
        # 消息处理
        ret, = struct.unpack("i", msg[4:8])
        uid, = struct.unpack("50s", msg[8:58])
        fid, = struct.unpack("50s", msg[58:108])
        uid = uid.decode('gbk').rstrip('\0')
        fid = fid.decode('gbk').rstrip('\0')

        # 判断添加好友消息类型
        if ret == 1:
            # 查询好友关系是否存在
            result = self.coon.query(
                "SELECT * from `userfriend`  where (userID=%s and friendID=%s)or(friendID=%s and userID=%s)",
                (uid, fid, fid, uid))
            # 不是好友的话就申请添加好友
            if result == None or result[1] == 0:
                message_type = EnumMessageType.ADDFRIEND
                message_send = struct.pack("ll50s50s", message_type.value, 1, uid.encode('gbk'), fid.encode('gbk'))
                # 在线直接发送
                if fid in self.client_dict:
                    self.client_dict[fid].sendall(message_send)
                    return
                # 用户不在线则暂存数据库
                else:
                    result = self.coon.DML("INSERT INTO `sysmsg` (`msgType`, `userA`, `userB`) VALUES (%s, %s,%s)",
                                           (message_type.value, uid, fid))
                    # 返回数据库操作结果
                    # 添加失败发回空
                    if result == None:
                        message = "".encode('gbk')
                        message_send = struct.pack("ll2048s", message_type.value, 0, message)
                        s.sendall(message_send)
                    # 成功什么也不做
                    else:
                        pass
        # 同意添加好友
        if ret == 2:
            # 提交数据库
            result = self.coon.DML("INSERT INTO `userfriend` (`userID`, `friendID`) VALUES (%s,%s)", (uid, fid))
            message_type = EnumMessageType.ADDFRIEND
            message = "".encode('gbk')

            # 数据库操作失败发送消息
            if result == None:
                message_send = struct.pack("ll50s50s", message_type.value, 0, uid.encode('gbk'), fid.encode('gbk'))
                s.sendall(message_send)
            # 操作成功发送消息
            else:
                message_send = struct.pack("ll50s50s", message_type.value, 2, uid.encode('gbk'), fid.encode('gbk'))
                if uid in self.client_dict:
                    self.client_dict[uid].sendall(message_send)
                if fid in self.client_dict:
                    self.client_dict[fid].sendall(message_send)

    # 1v1
    def __ChatForOne2One__(self, s, msg):
        # 消息处理
        uid, = struct.unpack("20s", msg[4:24])
        uname, = struct.unpack("20s", msg[24:44])
        fid, = struct.unpack("20s", msg[44:64])
        servertime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        content, = struct.unpack("930s", msg[94:1118])
        servertime = servertime.encode('gbk')

        message_type = EnumMessageType.ONE2ONE
        message_send = struct.pack("l20s20s20s30s1978s", message_type.value, uid, uname, fid, servertime, content)

        fid = fid.decode('gbk').rstrip('\0')

        # 在线转发
        if fid in self.client_dict:
            # 保存聊天记录到数据库
            uid = uid.decode('gbk').rstrip('\0')
            uname = uname.decode('gbk').rstrip('\0')
            content = content.decode('gbk').rstrip('\0')
            #servertime = servertime.decode('gbk')
            servertime = servertime.decode('gbk').rstrip('\0')
            self.coon.DML(
                "INSERT INTO `chatRecord`(`sendID`, `sendName`, `receiveID`, `sendTime`, `msgContent`) VALUES (%s,%s, %s,%s,%s)",
                (uid, uname, fid, servertime, content))
            self.client_dict[fid].sendall(message_send)
             
        # 不在线存储数据库
        else:
            # 提交数据库
            uid = uid.decode('gbk').rstrip('\0')
            uname = uname.decode('gbk').rstrip('\0')
            content = content.decode('gbk').rstrip('\0')
            #servertime = servertime.decode('gbk')
            servertime = servertime.decode('gbk').rstrip('\0')
            self.coon.DML(
                "INSERT INTO `offlinemsg`(`sendID`, `sendName`, `receiveID`, `sendTime`, `msgContent`) VALUES (%s,%s, %s,%s,%s)",
                (uid, uname, fid, servertime, content))

    # 1vN
    def __ChatForGroup__(self, s, msg):
        # 消息处理
        gid, = struct.unpack("20s", msg[4:24])
        servertime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        uname, = struct.unpack("20s", msg[54:74])
        content, = struct.unpack("950s", msg[74:1098])
        servertime = servertime.encode('gbk')

        message_type = EnumMessageType.ONE2GROUP
        message_send = struct.pack("l20s30s20s1978s", message_type.value, gid, servertime, uname, content)

        # 转发
        for sock in self.msg_dict:
            sock.sendall(message_send)

    # 更新群列表
    def __UpdataGroup__(self, s, msg):
        # 消息处理
        name, = struct.unpack("50s", msg[4:54])
        name = name.decode('gbk').rstrip('\0')

        # 查询
        result = self.coon.query("SELECT * from groupList where memberID=%s", (name,))

        message_type = EnumMessageType.UPDATEGROUP

        # 没有就结束
        if result == None or result[1] == 0:
            gid2 = "~~~end~~~".encode('gbk')
            message_send = struct.pack("l2048s", message_type.value, gid2)
            s.sendall(message_send)
            time.sleep(0.01)
            return


        # 发送群列表
        for i in range(result[1]):
            gid = ((str)(result[0][i][1])).encode('gbk')
            message_send = struct.pack("l2048s", message_type.value, gid)
            s.sendall(message_send)
            time.sleep(0.01)

        # 发送结束标志
        gid = "~~~end~~~".encode('gbk')
        message_send = struct.pack("l2048s", message_type.value, gid)
        s.sendall(message_send)
        time.sleep(0.01)

    # 更新群成员列表
    def __UpdataGroupMember__(self, s, msg):
        # 消息处理
        gid, = struct.unpack("50s", msg[4:54])
        gid = gid.decode('gbk').rstrip('\0')

        # 查询
        result = self.coon.query("SELECT * from groupList where groupID=%s", (gid,))

        gid = gid.encode('gbk')
        message_type = EnumMessageType.UPDATEGROUPMEMBER

        # 没有群成员直接发送结束标志
        if result == None or result[1] == 0:
            uid = "~~~end~~~".encode('gbk')
            message_send = struct.pack("l50s50s", message_type.value, gid, uid)
            s.sendall(message_send)
            time.sleep(0.01)
            return
        # 正常发送群成员列表
        for i in range(result[1]):
            uid = ((str)(result[0][i][2])).encode('gbk')
            message_send = struct.pack("l50s50s", message_type.value, gid, uid)
            s.sendall(message_send)
            # 发送太快会来不及接收,间隔时间发送
            time.sleep(0.01)

        # 发送结束标志
        uid = "~~~end~~~".encode('gbk')
        message_send = struct.pack("l50s50s", message_type.value, gid, uid)
        s.sendall(message_send)
        time.sleep(0.01)

    # 更新好友列表
    def __UpdataFriend__(self, s, msg):
        # 消息处理
        uid, = struct.unpack("50s", msg[4:54])
        uid = uid.decode('gbk').rstrip('\0')

        # 查询
        result = self.coon.query("SELECT * from `userfriend`  where userID=%s or friendID=%s", (uid, uid))

        # 返回查询结果
        message_type = EnumMessageType.UPDATEFRIEND

        # 没有好友直接结束
        if result == None or result[1] == 0:
            # 发送结束标志
            fid = "~~~end~~~".encode('gbk')
            message_send = struct.pack("l2048s", message_type.value, fid)
            print(fid)
            s.sendall(message_send)
            time.sleep(0.01)
            return



        # 发送好友列表
        for i in range(result[1]):
            # 判断uid在userID or friendID
            if uid == ((str)(result[0][i][0])):
                fid = ((str)(result[0][i][1])).encode('gbk')
                message_send = struct.pack("l2048s", message_type.value, fid)
                print(fid)
                s.sendall(message_send)
                time.sleep(0.01)
            else:
                fid = ((str)(result[0][i][0])).encode('gbk')
                message_send = struct.pack("l2048s", message_type.value, fid)
                print(fid)
                s.sendall(message_send)
                time.sleep(0.01)

        # 发送结束标志
        fid = "~~~end~~~".encode('gbk')
        message_send = struct.pack("l2048s", message_type.value, fid)
        print(fid)
        s.sendall(message_send)
        #time.sleep(0.01)

    # 接收离线聊天消息
    def __RecvOffLineMessage__(self, s, msg):
        # 消息处理
        uid, = struct.unpack("50s", msg[4:54])
        uid = uid.decode('gbk').rstrip('\0')
        # 删除时用uid2
        uid2 = uid

        # 获取离线系统消息
        self.__RecvOffLineSystemMessage__(s, uid)

        # 查询
        result = self.coon.query("SELECT * from `offlinemsg`  where receiveID=%s", (uid,))

        # 无消息结束
        if result == None or result[1] == 0:
            return

        # 发送消息
        for i in range(result[1]):
            # 数据库处理
            uid = str(result[0][i][1])
            uname = result[0][i][2]
            fid = str(result[0][i][3])
            servertime = str(result[0][i][6])
            content = result[0][i][5]
            #消息编码
            uid = uid.encode('gbk')
            fid = fid.encode('gbk')
            uname = uname.encode('gbk')
            servertime = servertime.encode('gbk')
            content = content.encode('gbk').decode('gbk').encode('gbk')

            message_type = EnumMessageType.ONE2ONE
            message_send = struct.pack("l20s20s20s30s1978s",message_type.value, uid, uname, fid, servertime, content)


            #str(b, encoding = "utf8")# bytes to str  

            s.sendall(message_send)
            # 发送太快会来不及接收
            time.sleep(0.5)

        # 删除离线消息
        result = self.coon.DML("DELETE FROM `offlinemsg` WHERE  `receiveID` = %s ", (uid2,))

    #接收请求获取聊天记录消息
    def __RecvMessageRecord__(self, s, msg):
        # 消息处理
        uid, = struct.unpack("50s", msg[4:54])
        uid = uid.decode('gbk').rstrip('\0')
        # 删除时用uid2
        uid2 = uid

        # 查询
        result = self.coon.query("SELECT * from `chatRecord`  where receiveID=%s", (uid,))

        # 无消息结束
        if result == None or result[1] == 0:
            return

        # 发送消息
        for i in range(result[1]):
            # 数据库处理
            uid = str(result[0][i][1])
            uname = result[0][i][2]
            fid = str(result[0][i][3])
            servertime = str(result[0][i][6])
            content = result[0][i][5]
            #消息编码
            uid = uid.encode('gbk')
            fid = fid.encode('gbk')
            uname = uname.encode('gbk')
            servertime = servertime.encode('gbk')
            content = content.encode('gbk').decode('gbk').encode('gbk')

            message_type = EnumMessageType.MSGRECORD
            message_send = struct.pack("l20s20s20s30s1978s",message_type.value, uid, uname, fid, servertime, content)


            #str(b, encoding = "utf8")# bytes to str  

            s.sendall(message_send)
            # 发送太快会来不及接收
            time.sleep(0.5)


    # 接收离线系统消息   查看是否有加好友的离线消息
    def __RecvOffLineSystemMessage__(self, s, uid):
        # 查询
        result = self.coon.query("SELECT * FROM sysmsg WHERE  `userB` = %s", (uid,))
        if result == None or result[1] == 0:
            return
        # 发送消息
        for i in range(result[1]):
            fid = str(result[0][i][2])
            message_type = EnumMessageType.ADDFRIEND
            message_send = struct.pack("ll50s50s", message_type.value, 1, fid.encode('gbk'), uid.encode('gbk'))
            s.sendall(message_send)

        # 删除消息
        result = self.coon.DML("DELETE FROM `sysmsg` WHERE  `userB` = %s", (uid,))

    # 文件传输
    def __FileTrans__(self, s, msg):
        # 消息处理
        nRet, = struct.unpack("i", msg[4:8])
        uid,=struct.unpack("50s", msg[12:62])
        fid,=struct.unpack("50s", msg[62:112])
        filename, = struct.unpack("255s", msg[112:367])
        uid=uid.decode('gbk').rstrip('\0')
        fid=fid.decode('gbk').rstrip('\0')
        filename = filename.decode('gbk').rstrip('\0')

        message_type = EnumMessageType.FILETRANS
        nlength=0
        #申请
        if nRet==1:
            if fid in self.client_dict:
                message_send = struct.pack("lll50s50s1024s", message_type.value, 1, nlength,uid.encode('gbk'),fid.encode('gbk'),filename.encode('gbk'))
                self.client_dict[fid].sendall(message_send)
            else:
                filename="用户不在线"
                message_send = struct.pack("lll50s50s1024s", message_type.value, 4, nlength, fid.encode('gbk'),uid.encode('gbk'), filename.encode('gbk'))
                s.sendall(message_send)
        #同意
        elif nRet==2:
            if fid in self.client_dict:
                fidadd = (s.getsockname())[0]
                message_send = struct.pack("lll50s50s1024s", message_type.value, 2, nlength,uid.encode('gbk'),fid.encode('gbk'),fidadd.encode('gbk'))
                self.client_dict[fid].sendall(message_send)
            else:
                filename="用户不在线"
                message_send = struct.pack("lll50s50s1024s", message_type.value, 4, nlength, fid.encode('gbk'),uid.encode('gbk'), filename.encode('gbk'))
                s.sendall(message_send)
        #拒绝
        elif nRet==3:
            if fid in self.client_dict:
                filename = "对方拒绝接收文件"
                message_send = struct.pack("lll50s50s1024s", message_type.value, 3, nlength,uid.encode('gbk'),fid.encode('gbk'),filename.encode('gbk'))
                self.client_dict[fid].sendall(message_send)
            else:
                pass

    # 视频聊天
    def __ChatForVideo__(self, s, msg):
        # 消息处理
        nRet, = struct.unpack("i", msg[4:8])
        uid, = struct.unpack("20s", msg[8:28])
        fid, = struct.unpack("20s", msg[48:68])
        uid = uid.decode('gbk').rstrip('\0')
        fid = fid.decode('gbk').rstrip('\0')

        message_type = EnumMessageType.MSGVIDEO
        content = " "
        #根据返回值区分处理
        #申请
        if nRet == 1:
            if fid in self.client_dict:
                message_send = struct.pack("ll20s20s20s", message_type.value, 1,uid.encode('gbk'),content.encode('gbk'),fid.encode('gbk'))
                self.client_dict[fid].sendall(message_send)
            else:
                content = "用户不在线"
                message_send = struct.pack("ll20s20s20s", message_type.value, 4,fid.encode('gbk'),content.encode('gbk'),uid.encode('gbk'))
                s.sendall(message_send)
        #同意
        elif nRet == 2:
            if fid in self.client_dict:
                fidadd = (s.getsockname())[0]
                message_send = struct.pack("ll20s20s20s", message_type.value, 2,uid.encode('gbk'),fidadd.encode('gbk'),fid.encode('gbk'))
                self.client_dict[fid].sendall(message_send)
            else:
                content = "用户不在线"
                message_send = struct.pack("ll20s20s20s", message_type.value, 4, fid.encode('gbk'),content.encode('gbk'),uid.encode('gbk'))
                s.sendall(message_send)
        #拒绝
        elif nRet == 3:
            if fid in self.client_dict:
                content = "对方不方便视频聊天"
                message_send = struct.pack("ll20s20s20s", message_type.value, 3, uid.encode('gbk'), content.encode('gbk'),fid.encode('gbk'))
                self.client_dict[fid].sendall(message_send)
            else:
                pass

    # 函数字典
    dictFun = {
        EnumMessageType.REGISTER.value: __ChatForRegister__,                #注册
        EnumMessageType.LOGIN.value: __ChatForLogin__,                      #登录
        EnumMessageType.SEARCHUSER.value: __ChatForSearchUser__,            #搜索用户
        EnumMessageType.ADDFRIEND.value: __ChatForAddFriend__,              #添加好友
        EnumMessageType.ONE2ONE.value: __ChatForOne2One__,                  #1v1
        EnumMessageType.ONE2GROUP.value: __ChatForGroup__,                  #1vN
        EnumMessageType.MSGOFFLINE.value: __RecvOffLineMessage__,           #离线消息
        EnumMessageType.FILETRANS.value:__FileTrans__,                      #文件传输
        EnumMessageType.UPDATEFRIEND.value: __UpdataFriend__,               #更新好友列表
        EnumMessageType.UPDATEGROUP.value: __UpdataGroup__,                 #更新群列表
        EnumMessageType.UPDATEGROUPMEMBER.value: __UpdataGroupMember__,     #更新群成员
        EnumMessageType.SEARCHGROUP.value: __ChatForSearchGroup__,          #搜索群
        EnumMessageType.MSGVIDEO.value: __ChatForVideo__,                    #视频聊天
        EnumMessageType.MSGRECORD.value:__RecvMessageRecord__              #聊天记录
    }

    BUFSIZE = 2048 + 4
    dictClient = {}


# 主函数
if __name__ == '__main__':
    HOST = '192.168.3.184'  # 服务器IP
    PORT = 1234
    server = CServerSocket(HOST, PORT)
    # 选择模型
    server.ChatSelect()
