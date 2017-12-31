#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //定义 进程 控制块 PCB
    typedef struct PCB{
          int id;           //ID
          QString name;     //进程名
          int arrival_time; //到达时间
          int start_time;   //开始时间
          int need_time;    //服务时间
          int runned_time;  //已经运行的时间
          int end_time;     //完成时间
          int priority;     //优先级
          QString status;   //状态
    }PCB;

    QVector<PCB> process_input, process_ready, process_run, result;     //QVector容器（数组）
    void Pri_Stablesort(QVector<PCB> *vector); //对 QVector<PCB> 按优先级进行 降序排序
    void Arr_Stablesort(QVector<PCB> *vector); //对 QVector<PCB> 按到达时间进行 升序排序
    void Nee_Stablesort(QVector<PCB> *vector); //对 QVector<PCB> 按服务时间进行 升序排序

    void P_setRunningTable(); //process_run 显示 runningTable 数据
    void P_setInputTable();   //process_input 数据显示到 readyTable
    void P_setReadyTable();   //process_ready 数据显示到 readyTable
    void P_getReadyTable();   //获取 readyTable 数据到 process_input
    void P_setResultTable();  //result 数据显示到 resultTable

    void createProcessPage();   //定义创建进程调度页面函数

    QWidget *processPage;   //1

    QGroupBox *P_arithmeticGroupBox;    //调度算法GroupBox
    QPushButton *PSAbutton_S;       //静态优先权算法按钮
    QPushButton *PSAbutton_D;       //动态优先权算法按钮
    QPushButton *RRbutton;          //简单轮转法算法按钮
    QPushButton *P_Initbutton;      //初始化按钮

    QGroupBox *P_runningGroupBox; //正在运行的进程GroupBox
    QTableWidget *P_runningTable; //正在运行的进程表

    QGroupBox *P_readyGroupBox;   //就绪队列GroupBox
    QTableWidget *P_readyTable;   //就绪队列表

    QGroupBox *P_timeGroupBox;  //时间GroupBox
    QLabel *P_timeLabel;        //当前时间
    QLabel *P_TiLabel;          //服务时间标签
    QLabel *P_WiLabel;          //所需资源标签

    QGroupBox *P_resultGroupBox;    //调度结果GroupBox
    QTableWidget *P_resultTable;    //调度结果表

private slots:  //槽
    void PSA_Sclick();
    void PSA_S_one();
    void PSA_Dclick();
    void PSA_D_one();
    void RRclick();
    void RR_one();
    void P_Initclick();    //初始化 响应
};

#endif // MAINWINDOW_H
