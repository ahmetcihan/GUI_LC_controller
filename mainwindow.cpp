#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    set_gradient();

    remote  = new RemoteConnection(1000,this);
    remote->setTarget("192.168.1.177");

    _100_msec_timer = new QTimer(this);
    _100_msec_timer->setInterval(100);
    connect(_100_msec_timer, SIGNAL(timeout()),this,SLOT(periodic_send()));
    connect(_100_msec_timer, SIGNAL(timeout()),this,SLOT(_100_msec_loop()));
    _100_msec_timer->start();

    //QTimer::singleShot(1000,this,SLOT(start_comm()));
    connect(remote,SIGNAL(periodic_response_handler(QByteArray)),this,SLOT(periodic_response_handler(QByteArray)));

    initilize_plot();

    connect(ui->pushButton_reconnect,SIGNAL(clicked()),this,SLOT(eth_reconnect()));

    connect(ui->radioButton_relay1_on,SIGNAL(toggled(bool)),this,SLOT(relay_1_status_handle(bool)));
    connect(ui->radioButton_relay2_on,SIGNAL(toggled(bool)),this,SLOT(relay_2_status_handle(bool)));
    connect(ui->radioButton_relay3_on,SIGNAL(toggled(bool)),this,SLOT(relay_3_status_handle(bool)));
    connect(ui->radioButton_relay4_on,SIGNAL(toggled(bool)),this,SLOT(relay_4_status_handle(bool)));
    connect(ui->radioButton_relay5_on,SIGNAL(toggled(bool)),this,SLOT(relay_5_status_handle(bool)));
    connect(ui->radioButton_relay6_on,SIGNAL(toggled(bool)),this,SLOT(relay_6_status_handle(bool)));

    connect(ui->pushButton_step_run_left,SIGNAL(clicked()), this,SLOT(step_run_left()));
    connect(ui->pushButton_step_run_right,SIGNAL(clicked()),this,SLOT(step_run_right()));
    connect(ui->pushButton_step_stop,SIGNAL(clicked()),     this,SLOT(step_stop()));
    connect(ui->pushButton_step_reset_pos,SIGNAL(clicked()),this,SLOT(step_pos_reset()));
    connect(ui->pushButton_step_go_pos,SIGNAL(clicked()),   this,SLOT(step_go_pos()));

    for(u8 i = 0; i < 6 ; i++){
        relay_status[i] = 0;
    }
    step_motor_command = STEPPER_COMMAND_STOP;
}
void MainWindow::_100_msec_loop(void){

    if(global_downcounter > 0){
        global_downcounter--;
        ui->label_communication->setText("Communication is OK");
    }
    else{
        ui->label_communication->setText("No communication");
    }

}
void MainWindow::eth_reconnect(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        delete remote;
        _100_msec_timer->stop();
        QTimer::singleShot(500,this,SLOT(eth_reconnect()));
        tmp++;
        break;
    case 1:
        remote  = new RemoteConnection(1000,this);
        remote->setTarget("192.168.1.177");
        connect(remote,SIGNAL(periodic_response_handler(QByteArray)),this,SLOT(periodic_response_handler(QByteArray)));
        _100_msec_timer->start();
        tmp = 0;
        break;
    }
    qDebug() << __FUNCTION__;
}
void MainWindow::relay_1_status_handle(bool val){
    relay_status[0] = val;
}
void MainWindow::relay_2_status_handle(bool val){
    relay_status[1] = val;
}
void MainWindow::relay_3_status_handle(bool val){
    relay_status[2] = val;
}
void MainWindow::relay_4_status_handle(bool val){
    relay_status[3] = val;
}
void MainWindow::relay_5_status_handle(bool val){
    relay_status[4] = val;
}
void MainWindow::relay_6_status_handle(bool val){
    relay_status[5] = val;
}
void MainWindow::set_gradient(void){
    QFile file;

    file.setFileName(":/qss/qmc2-black-fast/qmc2-black-0.10.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
    currentStyleSheet = styleSheet;
}
void MainWindow::step_run_left(void){
    step_motor_command = STEPPER_COMMAND_RUN_LEFT;
    step_motor_speed = ui->spinBox_step_motor_speed->value();
}
void MainWindow::step_run_right(void){
    step_motor_command = STEPPER_COMMAND_RUN_RIGHT;
    step_motor_speed = ui->spinBox_step_motor_speed->value();
}
void MainWindow::step_stop(void){
    step_motor_command = STEPPER_COMMAND_STOP;
}
void MainWindow::step_pos_reset(void){
    step_motor_command = STEPPER_COMMAND_POS_RESET;
}
void MainWindow::step_go_pos(void){
    step_motor_command = STEPPER_COMMAND_GO_POS;
}

MainWindow::~MainWindow()
{
    delete ui;
}
