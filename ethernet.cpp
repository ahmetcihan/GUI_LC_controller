#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::periodic_send(void){
    QByteArray arr;

    arr[0] = 'l';
    arr[1] = 'c';
    arr[2] = 'c';
    arr[3] = 'o';
    arr[4] = 'n';
    arr[5] = 't';
    for(u8 i = 0; i < 4; i++){
        arr[6 + i] = relay_status[i];
    }
    arr[10] = (ui->spinBox_voltage->value() / 256) % 256;
    arr[11] = (ui->spinBox_voltage->value()) % 256;

    remote->writeDatagram(arr, remote->target, remote->dstPort);
    qDebug() << "eth periodic send";

}
void MainWindow::start_comm(void){
    qDebug(__FUNCTION__);

    //remote->set("device_settings.stream.enable", "1");
    //remote->set("Filan","");
    QString str = QString("Filan");
    remote->writeDatagram(str.toUtf8(), remote->target, remote->dstPort);


}
void MainWindow::periodic_response_handler(QByteArray datagram){
    union _my_resp{
        int _integer;
        float _float;
    };

    QString str(datagram);
    if(str.mid(0,6) == "cpuans"){
        qDebug() << "got the answer";
        global_downcounter = 10;
    }
    //qDebug() << "relay address" << (u8)datagram[6];

    ui->label_input_1->setText(QString::number(datagram[8]));
    ui->label_input_2->setText(QString::number(datagram[9]));
    ui->label_input_3->setText(QString::number(datagram[10]));
    ui->label_input_4->setText(QString::number(datagram[11]));


    u32 adc_value = (65536 * (u8)datagram[23]) + (256* (u8)datagram[24]) + (u8)datagram[25];
    ui->label_adc_raw->setText(QString::number(adc_value));

}
