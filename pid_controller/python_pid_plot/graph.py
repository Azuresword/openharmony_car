import socket  #网口
import pyqtgraph as pg #画图
import threading  #线程
import array #数组
import numpy as np #数学基础库
import time #时间
from functools import reduce
def str2float(s):
    def fn(x,y):
            return x*10+y
    n=s.index('.')
    s1=list(map(int,[x for x in s[:n]]))
    s2=list(map(int,[x for x in s[n+1:]]))
    return reduce(fn,s1)+reduce(fn,s2)/10**len(s2)

i = 0
def socket1():
    while(True):
            dat = connection.recv(5)#单次读取一个数据 int.from_bytes(sockfd.,byteorder='little')  # 格式转换
            print(dat)
            dat=dat.decode("ascii")
            print(dat)
#            print(dat.hex())#打印实时数据查看

            # if(dat.hex()=='aa55' or dat.hex()=='bbcc'):#帧头 帧尾识别去掉不显示
            #     print(dat.hex())
            # else:
            #     dat=int.from_bytes(dat,byteorder='big')#格式转换2个字节 一个点
            global i
            if i < historyLength :#数据处理
                data[i]=str2float(dat)
                print(data[i])
                i=i+1
                
def plotData():
    curve.setData(data)#实时数据呈现

if __name__ == "__main__":
    app = pg.mkQApp()  # 建立app
    win = pg.GraphicsLayoutWidget()  # 建立窗口
    win.setWindowTitle(u'pyqtgraph逐点画波形图')#自定义名称
    win.resize(800, 500)  # 小窗口大小--可自定义窗口大小
    data = array.array('i')  # 可动态改变数组的大小,double型数组
    historyLength = 600  # 横坐标长度 
    data=np.zeros(historyLength).__array__('d')#把数组长度定下来
    p = win.addPlot()  # 把图p加入到窗口中
    p.showGrid(x=True, y=True)  # 把X和Y的表格打开
    p.setRange(xRange=[0, historyLength], yRange=[0, 255], padding=0)#坐标设置
    p.setLabel(axis='left', text='y / V')  # 靠左
    p.setLabel(axis='bottom', text='x / point')
    p.setTitle('Wave')  # 表格的名字
    curve = p.plot()  # 绘制一个图形
#   curve.setData(data)
from socket import *

sockfd = socket(AF_INET, SOCK_STREAM)    # 默认值其实就是这个, tcp套接字
sockfd.bind(("localhost", 8080))
sockfd.listen(5)
connection, address = sockfd.accept()
print(connection, address)

th1 = threading.Thread(target=socket1)  #目标函数一定不能带（）
th1.start()
timer = pg.QtCore.QTimer()
timer.timeout.connect(plotData)  # 定时刷新数据显示
#print(data.hex()) #打印数据查看效果
timer.start(1000)  # 多少ms调用一次，刷新显示数据的时间间隔

app.exec_()    #应用运行呈现
