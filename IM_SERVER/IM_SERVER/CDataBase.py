#encoding:utf-8
import mysql.connector

class CDataBase():
    # 初始化
    def __init__(self):
        # 连接数据库配置
        config = {'host': '192.168.3.184',
                  'user': 'root',
                  'password': '',
                  'port': '3306',
                  'database': 'imchat',
                  'charset': 'utf8'}
        print('正在连接数据库...')
        # 连接
        self.conn = mysql.connector.connect(**config)
        print('数据库连接成功！')

    # 析构
    def __del__(self):
        self.conn.close()

    # 查询
    def query(self, szSql, param=None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = (cursor.fetchall(), cursor.rowcount)
            cursor.close()
            return result
        #捕捉异常
        except:
            cursor.close()
            # 发生错误时回滚
            self.conn.rollback()
            return None

    # 插入删除操作
    def DML(self, szSql, param=None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = cursor.rowcount
            self.conn.commit()
            cursor.close()
            return result
        #捕捉异常
        except:
            # 回滚
            cursor.close()
            self.conn.rollback()
            return None
