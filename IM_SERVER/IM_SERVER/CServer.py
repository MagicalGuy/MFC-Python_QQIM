#encoding:utf-8
from CServerSocket import *
import threading
from CDataBase import *
HOST='192.168.3.184'    #服务器IP
PORT=1234


def threadProc(*args, **kwargs):
    # 数据库手动查询
    sql = CDataBase()
    while True:
        print('1.查询\n2.添加\n3.操作数据库')
        index = int(input())
        if type(index) != int:
            print('输入有误，请重新输入...')
            pass
        if index == 1:
            result = sql.query("SELECT * from userInfo")
            if result != None:
                print(result[0])
        elif index == 2:
            userinfo=input('请输入ID,用户名，密码')
            userinfo=userinfo.split(" ")
            uid=userinfo[0]
            name=userinfo[1]
            pwd=userinfo[2]
            result = sql.DML("INSERT INTO `userinfo` (`userID`, `userName`, `pwd`) VALUES (%s, %s, %s)", (uid, name, pwd))
            if result != None:
                print('操作成功')
        elif index == 3:
            result = sql.DML(input('请输入操作语句'))
            if result != None:
                print('操作成功')
        else:
            print('输入有误，请重新输入...')
            pass

def main():

    tc=threading.Thread(target=threadProc)
    tc.start()

    #启动服务器
    server=CServerSocket(HOST,PORT)

    #选择
    server.ChatSelect()



if __name__=='__main__':
    main()
