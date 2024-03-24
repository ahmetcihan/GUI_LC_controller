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

    QTimer::singleShot(1000,this,SLOT(start_comm()));
    connect(remote,SIGNAL(periodic_response_handler(QByteArray)),this,SLOT(periodic_response_handler(QByteArray)));
    connect(ui->pushButton_reconnect,SIGNAL(clicked()),this,SLOT(eth_reconnect()));
    connect(ui->radioButton_relay1_on,SIGNAL(toggled(bool)),this,SLOT(relay_1_status_handle(bool)));
    connect(ui->radioButton_relay2_on,SIGNAL(toggled(bool)),this,SLOT(relay_2_status_handle(bool)));
    connect(ui->horizontalSlider_voltage,SIGNAL(valueChanged(int)),ui->spinBox_voltage,SLOT(setValue(int)));

    for(u8 i = 0; i < 4 ; i++){
        relay_status[i] = 0;
    }
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
void MainWindow::set_gradient(void){
    QFile file;

    file.setFileName(":/qss/qmc2-black-fast/qmc2-black-0.10.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
    currentStyleSheet = styleSheet;
}

MainWindow::~MainWindow()
{
    delete ui;
}
