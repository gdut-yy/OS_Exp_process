/**
* @brief    进程调度实验
* @details  1、编写并调试一个模拟的进程调度程序，采用“最高优先数优先”调度算法
*           对五个进程进行调度。
*           “最高优先数优先”调度算法的基本思想是把CPU分配给就绪队列中优先数
*           最高的进程。
*           静态优先数
*           动态优先数
*           2、编写并调试一个模拟的进程调度程序，采用“轮转法”调度算法对五个进程
*           进行调度。
*           轮转法可以是简单轮转法、可变时间片轮转法，或多队列轮转法。
* @author   3115005124_张逸扬
* @date     2017年12月31日10:37:21
*/

#include "mainwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QTimer>
#include <QHeaderView>
//#include <QtAlgorithms>
#include <algorithm>

int Time;  //时钟 用于记录当前时间
QTimer *qtimer = new QTimer;    //时钟 用于控制显示速度快慢

//主页面
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("3115005124_张逸扬_操作系统实验_进程调度"));   //设置标题
    createProcessPage();
    setCentralWidget(processPage);
}

MainWindow::~MainWindow(){
}

//processPage页面 布局
void MainWindow::createProcessPage(){
    processPage = new QWidget;

    //arithmeticGroupBox初始化及布局
    P_arithmeticGroupBox = new QGroupBox(tr("调度算法"));
    PSAbutton_S= new QPushButton(tr("静态优先权算法"));
        connect(PSAbutton_S, SIGNAL(clicked(bool)), this, SLOT(PSA_Sclick()));
    PSAbutton_D = new QPushButton(tr("动态优先权算法"));
        connect(PSAbutton_D, SIGNAL(clicked(bool)), this, SLOT(PSA_Dclick()));
    RRbutton = new QPushButton(tr("简单轮转法算法"));
        connect(RRbutton, SIGNAL(clicked(bool)), this, SLOT(RRclick()));
    P_Initbutton = new QPushButton(tr("初始化"));
        connect(P_Initbutton, SIGNAL(clicked(bool)), this, SLOT(P_Initclick()));

    //按钮初始化状态
    PSAbutton_S->setEnabled(false);
    PSAbutton_D->setEnabled(false);
    RRbutton->setEnabled(false);

    QGridLayout *arithmeticLayout = new  QGridLayout;
    arithmeticLayout->setColumnStretch(0, 1);
    arithmeticLayout->setColumnStretch(1, 1);
    arithmeticLayout->setColumnStretch(2, 1);
    arithmeticLayout->setColumnStretch(3, 1);
    arithmeticLayout->setColumnStretch(4, 1);
    arithmeticLayout->addWidget(PSAbutton_S, 0, 0);
    arithmeticLayout->addWidget(PSAbutton_D, 0, 1);
    arithmeticLayout->addWidget(RRbutton, 0, 2);
    arithmeticLayout->addWidget(P_Initbutton, 0, 4);
    P_arithmeticGroupBox->setLayout(arithmeticLayout);


    //P_runningGroupBox初始化及布局
    P_runningGroupBox = new QGroupBox(tr("正在运行的进程"));
    P_runningTable = new QTableWidget(1, 8);    //生成 1 行 8 列的表格

    //设置水平标题栏
    QStringList headerLabels;
    headerLabels << "ID" << "进程名" << "开始运行时间" << "已运行时间" << "还需运行时间" << "优先权" << "服务时间" << "状态";
    P_runningTable->setHorizontalHeaderLabels(headerLabels);

    P_runningTable->verticalHeader()->hide();   //隐藏垂直标题栏
    P_runningTable->setMinimumHeight(60);       //设置表格最小行高
    P_runningTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);     //设置自适应列宽
    P_runningTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);       //设置自适应行高
    P_runningTable->setEditTriggers(QAbstractItemView::NoEditTriggers);         //设置表格不可修改

    QGridLayout *runningLayout = new  QGridLayout;
    runningLayout->addWidget(P_runningTable);
    P_runningGroupBox->setLayout(runningLayout);


    //readyGroupBox初始化及布局
    P_readyGroupBox = new QGroupBox(tr("进程 输入/ 就绪 队列"));
    P_readyTable = new QTableWidget(5, 6);
    QStringList readyLabels;
    readyLabels << "ID" << "进程名称" << "提交时间" << "服务时间" << "优先权" << "状态";
    P_readyTable->setHorizontalHeaderLabels(readyLabels);

    P_readyTable->verticalHeader()->hide();   //隐藏垂直标题
    P_readyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    P_readyTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QGridLayout *readyLayout = new  QGridLayout;
    readyLayout->setColumnStretch(0, 2);
    readyLayout->setColumnStretch(1, 1);
    readyLayout->addWidget(P_readyTable, 0, 0);
    P_readyGroupBox->setLayout(readyLayout);


    //timeGroupBox初始化及布局
    P_timeGroupBox = new QGroupBox(tr("时间"));   //时间GroupBox
    P_timeLabel = new QLabel(tr("当前时刻：0"));
    P_TiLabel  = new QLabel(tr("平均周转时间：0"));
    P_WiLabel  = new QLabel(tr("平均带权周转时间：0"));

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addStretch();
    timeLayout->addWidget(P_timeLabel);
    timeLayout->addStretch();
    timeLayout->addWidget(P_TiLabel);
    timeLayout->addStretch();
    timeLayout->addWidget(P_WiLabel);
    timeLayout->addStretch();
    P_timeGroupBox->setLayout(timeLayout);


    //P_resultGroupBox初始化及布局
    P_resultGroupBox = new QGroupBox(tr("调度结果"));
    P_resultTable = new QTableWidget(5, 9);
    QStringList resultLabels;
    resultLabels << "ID" << "进程名称" << "开始时间" << "服务时间" << "完成时间" << "优先权" << "状态" << "周转时间" << "带权周转时间";
    P_resultTable->setHorizontalHeaderLabels(resultLabels);

    P_resultTable->verticalHeader()->hide();
    P_resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    P_resultTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    P_resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QGridLayout *resultLayout = new  QGridLayout;
    resultLayout->addWidget(P_resultTable);
    P_resultGroupBox->setLayout(resultLayout);


    //页面总布局
    QGridLayout *pageLayout = new QGridLayout;
    pageLayout->setMargin(0);
    pageLayout->setRowStretch(0,1);
    pageLayout->setRowStretch(1,1);
    pageLayout->setRowStretch(2,10);
    pageLayout->setRowStretch(3,1);
    pageLayout->setRowStretch(4,10);
    pageLayout->addWidget(P_arithmeticGroupBox, 0, 0);
    pageLayout->addWidget(P_runningGroupBox, 1, 0);
    pageLayout->addWidget(P_readyGroupBox, 2, 0);
    pageLayout->addWidget(P_timeGroupBox, 3, 0);
    pageLayout->addWidget(P_resultGroupBox, 4, 0);
    processPage->setLayout(pageLayout);
}

//静态优先权算法 按钮响应
void MainWindow::PSA_Sclick(){
    //运行期间 其他按钮 不可点击
    PSAbutton_S->setEnabled(false);
    PSAbutton_D->setEnabled(false);
    RRbutton->setEnabled(false);
    P_Initbutton->setEnabled(false);

    P_getReadyTable();

    qtimer->start(1000);
    connect(qtimer, SIGNAL(timeout()), this, SLOT(PSA_S_one()));
}

//动态优先权算法 按钮响应
void MainWindow::PSA_Dclick(){
    //运行期间 其他按钮 不可点击
    PSAbutton_S->setEnabled(false);
    PSAbutton_D->setEnabled(false);
    RRbutton->setEnabled(false);
    P_Initbutton->setEnabled(false);

    P_getReadyTable();

    qtimer->start(1000);
    connect(qtimer, SIGNAL(timeout()), this, SLOT(PSA_D_one()));
}

//RR算法按钮 响应
void MainWindow::RRclick(){
    //运行期间 其他按钮 不可点击
    PSAbutton_S->setEnabled(false);
    PSAbutton_D->setEnabled(false);
    RRbutton->setEnabled(false);
    P_Initbutton->setEnabled(false);

    P_getReadyTable();

    qtimer->start(1000);
    connect(qtimer, SIGNAL(timeout()), this, SLOT(RR_one()));
}

//静态优先权算法 执行一次
void MainWindow::PSA_S_one(){
    //把 提交时间 <= 当前时间的进程 添加到 process_ready 容器中
    if(!process_input.isEmpty()){
        for(int i=0; i<process_input.size(); ){
            if(process_input[i].arrival_time<=Time){
                process_ready.append(process_input.takeAt(i));
                i = 0;
            } else {
                i++;
            }
        }
    }

    Pri_Stablesort(&process_ready);     //process_ready 按优先权排序
    //当 process_run 为空时，把 process_ready 首元素添加到 job_run 中
    if(process_run.isEmpty()){
        process_run.append(process_ready.takeFirst());
    }
    process_run[0].status = "R";    //状态为Running
    process_run[0].runned_time++;
    //设开始时间
    if(process_run[0].start_time==-1 && process_run[0].runned_time==1){
        process_run[0].start_time=Time;
    }
    P_setRunningTable();
    //进程完成时
    if(process_run[0].runned_time == process_run[0].need_time){
        process_run[0].end_time=Time+1;
        process_run[0].status = "F";
        result.append(process_run.takeFirst());
        P_setResultTable();
    }
    P_setReadyTable();
    //显示当前时间
    Time++;
    P_timeLabel->setText("当前时间：" + QString::number(Time));
    //当进程调度完成时
    if(result.size()==5){
        P_runningTable->clearContents();
        //计算 平均周转时间 Ti 和 平均带权周转时间 Wi
        double Ti_sum=0, Wi_sum=0;
        double Ti, Wi;
        for(int i=0; i<result.size(); i++){
            Ti_sum = Ti_sum + (double)(result[i].end_time - result[i].arrival_time);
            Wi_sum = Wi_sum + (double)((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time);
        }
        Ti = Ti_sum / result.size();
        Wi = Wi_sum / result.size();
        P_TiLabel->setText("平均周转时间：" + QString::number(Ti));
        P_WiLabel->setText("平均带权周转时间：" + QString::number(Wi));
        qtimer->stop();
        qtimer->disconnect();
        QMessageBox::information(NULL, "完成", "进程调度（静态优先权算法）完成");
        P_Initbutton->setEnabled(true);
    }
}

//动态优先权算法 执行一次
void MainWindow::PSA_D_one(){
    //把 提交时间 <= 当前时间的进程 添加到 process_ready 容器中
    if(!process_input.isEmpty()){
        for(int i=0; i<process_input.size(); ){
            if(process_input[i].arrival_time<=Time){
                process_ready.append(process_input.takeAt(i));
                i = 0;
            } else {
                i++;
            }
        }
    }
    //计算 job_ready 里的优先权，R = （等待时间 + 要求服务时间） / 要求服务时间
    for(int i=0; i<process_ready.size(); i++){
        process_ready[i].priority = (Time - process_ready[i].arrival_time + process_ready[i].need_time) / process_ready[i].need_time;
    }
    Pri_Stablesort(&process_ready);     //job_ready 按优先级排序
    //当 job_run 为空时，把 job_ready 首元素添加到 job_run 中
    if(process_run.isEmpty()){
        process_run.append(process_ready.first());
        process_ready.removeFirst();
    }
    process_run[0].status = "R";    //状态为Running
    process_run[0].runned_time++;
    //设开始时间
    if(process_run[0].start_time==-1 && process_run[0].runned_time==1){
        process_run[0].start_time=Time;
    }
    P_setRunningTable();
    //进程完成时
    if(process_run[0].runned_time == process_run[0].need_time){
        process_run[0].end_time=Time+1;
        process_run[0].status = "F";
        result.append(process_run.takeFirst());
        P_setResultTable();
    }
    P_setReadyTable();
    //显示当前时间
    Time++;
    P_timeLabel->setText("当前时间：" + QString::number(Time));
    //当进程调度完成时
    if(result.size()==5){
        P_runningTable->clearContents();
        //计算 平均周转时间 Ti 和 平均带权周转时间 Wi
        double Ti_sum=0, Wi_sum=0;
        double Ti, Wi;
        for(int i=0; i<result.size(); i++){
            Ti_sum = Ti_sum + (double)(result[i].end_time - result[i].arrival_time);
            Wi_sum = Wi_sum + (double)((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time);
        }
        Ti = Ti_sum / result.size();
        Wi = Wi_sum / result.size();
        P_TiLabel->setText("平均周转时间：" + QString::number(Ti));
        P_WiLabel->setText("平均带权周转时间：" + QString::number(Wi));
        qtimer->stop();
        qtimer->disconnect();
        QMessageBox::information(NULL, "完成", "进程调度（动态优先权算法）完成");
        P_Initbutton->setEnabled(true);
    }
}

//RR算法 执行一次
void MainWindow::RR_one(){
    //把 提交时间 <= 当前时间的进程 添加到 process_ready 容器中
    if(!process_input.isEmpty()){
        for(int i=0; i<process_input.size(); ){
            if(process_input[i].arrival_time<=Time){
                process_ready.append(process_input.takeAt(i));
                i = 0;
            } else {
                i++;
            }
        }
    }
    process_run.append(process_ready.takeFirst());
    process_run[0].status = "R";    //状态为Running
    process_run[0].runned_time++;
    //设开始时间
    if(process_run[0].start_time==-1 && process_run[0].runned_time==1){
        process_run[0].start_time=Time;
    }
    P_setRunningTable();
    //进程完成时
    if(process_run[0].runned_time == process_run[0].need_time){
        process_run[0].end_time=Time+1;
        process_run[0].status = "F";
        result.append(process_run.takeFirst());
        P_setResultTable();
    }
    P_setReadyTable();
    //若 process_run 非空，那么把进程放回 process_ready 队列
    if(!process_run.isEmpty()){
        process_ready.append(process_run.takeFirst());
        process_ready.last().status="W";
    }


    //显示当前时间
    Time++;
    P_timeLabel->setText("当前时间：" + QString::number(Time));
    //当进程调度完成时
    if(result.size()==5){
        P_runningTable->clearContents();
        //计算 平均周转时间 Ti 和 平均带权周转时间 Wi
        double Ti_sum=0, Wi_sum=0;
        double Ti, Wi;
        for(int i=0; i<result.size(); i++){
            Ti_sum = Ti_sum + (double)(result[i].end_time - result[i].arrival_time);
            Wi_sum = Wi_sum + (double)((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time);
        }
        Ti = Ti_sum / result.size();
        Wi = Wi_sum / result.size();
        P_TiLabel->setText("平均周转时间：" + QString::number(Ti));
        P_WiLabel->setText("平均带权周转时间：" + QString::number(Wi));
        qtimer->stop();
        qtimer->disconnect();
        QMessageBox::information(NULL, "完成", "进程调度（RR算法）完成");
        P_Initbutton->setEnabled(true);
    }
}

//初始化按钮 响应
void MainWindow::P_Initclick(){
    Time=0;
    P_timeLabel->setText("当前时间：0");
    P_TiLabel->setText("平均周转时间：0");
    P_WiLabel->setText("平均带权周转时间：0");
    process_input.clear();
    process_input.squeeze();
    result.clear();
    result.squeeze();
    P_resultTable->clearContents();

    //预输入数据
//    process_input.append(PCB{1, "P1", 0, -1, 3, 0, -1, 3, "W"});
//    process_input.append(PCB{2, "P2", 2, -1, 6, 0, -1, 5, "W"});
//    process_input.append(PCB{3, "P3", 4, -1, 4, 0, -1, 1, "W"});
//    process_input.append(PCB{4, "P4", 6, -1, 5, 0, -1, 2, "W"});
//    process_input.append(PCB{5, "P5", 8, -1, 2, 0, -1, 4, "W"});

    process_input.append(PCB{1, "P1", 0, -1, 10, 0, -1, 3, "W"});
    process_input.append(PCB{2, "P2", 0, -1, 1, 0, -1, 1, "W"});
    process_input.append(PCB{3, "P3", 0, -1, 2, 0, -1, 5, "W"});
    process_input.append(PCB{4, "P4", 0, -1, 1, 0, -1, 4, "W"});
    process_input.append(PCB{5, "P5", 0, -1, 5, 0, -1, 2, "W"});
    P_setInputTable();

    PSAbutton_S->setEnabled(true);
    PSAbutton_D->setEnabled(true);
    RRbutton->setEnabled(true);
    P_Initbutton->setEnabled(true);
}

//process_run 显示 runningTable 数据
void MainWindow::P_setRunningTable(){
    P_runningTable->clearContents();
    P_runningTable->setItem(0, 0, new QTableWidgetItem(QString::number(process_run[0].id)));  //ID
    P_runningTable->setItem(0, 1, new QTableWidgetItem(process_run[0].name));     //进程名
    P_runningTable->setItem(0, 2, new QTableWidgetItem(QString::number(process_run[0].start_time)));  //开始运行时间
    P_runningTable->setItem(0, 3, new QTableWidgetItem(QString::number(process_run[0].runned_time))); //已运行时间
    P_runningTable->setItem(0, 4, new QTableWidgetItem(QString::number(process_run[0].need_time - process_run[0].runned_time)));  //还需运行时间
    P_runningTable->setItem(0, 5, new QTableWidgetItem(QString::number(process_run[0].priority)));    //优先权
    P_runningTable->setItem(0, 6, new QTableWidgetItem(QString::number(process_run[0].need_time)));   //服务时间
    P_runningTable->setItem(0, 7, new QTableWidgetItem(process_run[0].status));   //状态
}

//process_input 数据显示到 readyTable
void MainWindow::P_setInputTable(){
    P_readyTable->clearContents();
    for(int i=0; i<process_input.size(); i++){
        P_readyTable->setItem(i, 0, new QTableWidgetItem(QString::number(process_input[i].id)));
        P_readyTable->setItem(i, 1, new QTableWidgetItem(process_input[i].name));
        P_readyTable->setItem(i, 2, new QTableWidgetItem(QString::number(process_input[i].arrival_time)));
        P_readyTable->setItem(i, 3, new QTableWidgetItem(QString::number(process_input[i].need_time)));
        P_readyTable->setItem(i, 4, new QTableWidgetItem(QString::number(process_input[i].priority)));
        P_readyTable->setItem(i, 5, new QTableWidgetItem(process_input[i].status));
    }
}

//process_ready 数据显示到 readyTable
void MainWindow::P_setReadyTable(){
    P_readyTable->clearContents();
    for(int i=0; i<process_ready.size(); i++){
        P_readyTable->setItem(i, 0, new QTableWidgetItem(QString::number(process_ready[i].id)));
        P_readyTable->setItem(i, 1, new QTableWidgetItem(process_ready[i].name));
        P_readyTable->setItem(i, 2, new QTableWidgetItem(QString::number(process_ready[i].arrival_time)));
        P_readyTable->setItem(i, 3, new QTableWidgetItem(QString::number(process_ready[i].need_time)));
        P_readyTable->setItem(i, 4, new QTableWidgetItem(QString::number(process_ready[i].priority)));
        P_readyTable->setItem(i, 5, new QTableWidgetItem(process_ready[i].status));
    }
}

//获取 readyTable 数据到 process_input
void MainWindow::P_getReadyTable(){
    for(int i=0; i<process_input.size(); i++){
        process_input[i].id = P_readyTable->item(i, 0)->text().toInt();         //ID
        process_input[i].name = P_readyTable->item(i, 1)->text();               //进程名
        process_input[i].arrival_time = P_readyTable->item(i, 2)->text().toInt();   //到达时间
        process_input[i].need_time = P_readyTable->item(i, 3)->text().toInt();  //服务时间
        process_input[i].priority = P_readyTable->item(i, 4)->text().toInt();   //优先级
        process_input[i].status = P_readyTable->item(i, 5)->text();             //状态
    }
}

//result 数据显示到 resultTable
void MainWindow::P_setResultTable(){
    P_resultTable->clearContents();
    for(int i=0; i<result.size(); i++){
        P_resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(result[i].id)));            //ID
        P_resultTable->setItem(i, 1, new QTableWidgetItem(result[i].name));                           //进程名称
        P_resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(result[i].start_time)));    //开始运行时间
        P_resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(result[i].need_time)));     //服务时间
        P_resultTable->setItem(i, 4, new QTableWidgetItem(QString::number(result[i].end_time)));      //完成时间
        P_resultTable->setItem(i, 5, new QTableWidgetItem(QString::number(result[i].priority)));      //优先级
        P_resultTable->setItem(i, 6, new QTableWidgetItem(result[i].status));                         //状态
        P_resultTable->setItem(i, 7, new QTableWidgetItem(QString::number(result[i].end_time - result[i].arrival_time)));      //周转时间
        P_resultTable->setItem(i, 8, new QTableWidgetItem(QString::number((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time)));    //带权周转时间
    }
}

//对 QVector<PCB> 优先级 降序排序 使用Lambda表达式 std::stable_sort
void MainWindow::Pri_Stablesort(QVector<PCB> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const PCB &A,const PCB &B){
        return A.priority > B.priority;});
}

//对 QVector<PCB> 到达时间 升序排序 使用Lambda表达式 std::stable_sort
void MainWindow::Arr_Stablesort(QVector<PCB> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const PCB &A,const PCB &B){
        return A.arrival_time < B.arrival_time;});
}

//对 QVector<PCB> 服务时间 升序排序 使用Lambda表达式 std::stable_sort
void MainWindow::Nee_Stablesort(QVector<PCB> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const PCB &A,const PCB &B){
        return A.need_time < B.need_time;});
}
