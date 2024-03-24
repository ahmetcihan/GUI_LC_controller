#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

void MainWindow::periodic_send(void){
    QByteArray arr;

    arr[0] = 'l';
    arr[1] = 'c';
    arr[2] = 'c';
    arr[3] = 'o';
    arr[4] = 'n';
    arr[5] = 't';
    for(u8 i = 0; i < 2; i++){
        arr[6 + i] = relay_status[i];
    }
    arr[8] = (ui->spinBox_voltage->value() / 256) % 256;
    arr[9] = (ui->spinBox_voltage->value()) % 256;

    remote->writeDatagram(arr, remote->target, remote->dstPort);
    qDebug() << "eth periodic send";

}
void MainWindow::start_comm(void){
    qDebug(__FUNCTION__);

    remote->set("device_settings.stream.enable", "1");
//    QString str = QString("Filan");
//    remote->writeDatagram(str.toUtf8(), remote->target, remote->dstPort);


}
void MainWindow::periodic_response_handler(QByteArray datagram){
    union _my_resp{
        int _integer;
        float _float;
        unsigned char char_val[4];
    };
    union _my_resp my_resp;

    QString str(datagram);
    if(str.mid(0,6) == "cpuans"){
        qDebug() << "got the answer";
        global_downcounter = 10;

        my_resp.char_val[0] = (u8)datagram[6];
        my_resp.char_val[1] = (u8)datagram[7];
        my_resp.char_val[2] = (u8)datagram[8];
        my_resp.char_val[3] = (u8)datagram[9];

        ui->label_input_1->setText(QString::number(datagram[11]));

        u8 dec_point = (u8)datagram[10];
        qDebug() << "dp" << dec_point;
        float value = my_resp._integer / pow(10,dec_point);
        ui->label_adc_data->setText(QString::number(value));
    }
}
