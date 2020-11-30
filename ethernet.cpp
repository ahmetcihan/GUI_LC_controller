#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::periodic_send(void){
    QByteArray arr;

    arr[0] = 'm';
    arr[1] = 'a';
    arr[2] = 's';
    arr[3] = 't';
    arr[4] = 'e';
    arr[5] = 'r';
    for(u8 i = 0; i < 6; i++){
        arr[6 + i] = relay_status[i];
    }
    arr[12] = ui->spinBox_relay_address->value();
    arr[13] = ui->spinBox_input_address->value();
    arr[14] = ui->spinBox_stepper_address->value();
    arr[15] = step_motor_command;

    arr[16] = (ui->spinBox_go_pos->value() / 256) % 256;
    arr[17] = (ui->spinBox_go_pos->value() ) % 256;

    arr[18] = ((ui->spinBox_step_motor_speed->value() / 65536) % 256);
    arr[19] = ((ui->spinBox_step_motor_speed->value() / 256) % 256);
    arr[20] = ((ui->spinBox_step_motor_speed->value()) % 256);

    arr[21] = ui->spinBox_adc_address->value();

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
    _my_resp my_resp;
    int start_stream = 0;
    int tmp = 0;

    QString str(datagram);
    if(str.mid(0,6) == "cpuans"){
        qDebug() << "got the answer";
        global_downcounter = 10;
    }
    //qDebug() << "relay address" << (u8)datagram[6];
    if((u8)datagram[7] == 1){
        ui->label_relay_comm_status->setText("Address is valid");
    }
    else{
        ui->label_relay_comm_status->setText("Address is unvalid");
    }

    ui->label_input_1->setText(QString::number(datagram[8]));
    ui->label_input_2->setText(QString::number(datagram[9]));
    ui->label_input_3->setText(QString::number(datagram[10]));
    ui->label_input_4->setText(QString::number(datagram[11]));
    ui->label_input_5->setText(QString::number(datagram[12]));
    ui->label_input_6->setText(QString::number(datagram[13]));

    //qDebug() << "input address" << (u8)datagram[14];
    if((u8)datagram[15] == 1){
        ui->label_input_comm_status->setText("Address is valid");
    }
    else{
        ui->label_input_comm_status->setText("Address is unvalid");
    }

    //qDebug() << "stepper address" << (u8)datagram[16];
    if((u8)datagram[17] == 1){
        ui->label_stepper_comm_status->setText("Address is valid");
    }
    else{
        ui->label_stepper_comm_status->setText("Address is unvalid");
    }

    u32 step_abs_position = (65536 * (u8)datagram[18]) + (256* (u8)datagram[19]) + (u8)datagram[20];

    ui->label_stepper_current_position->setText(QString::number(step_abs_position));

    //qDebug() << "adc address" << (u8)datagram[21] << "adc_com_status" << (u8)datagram[22];
    if((u8)datagram[22] == 1){
        ui->label_adc_comm_status->setText("Address is valid");
    }
    else{
        ui->label_adc_comm_status->setText("Address is unvalid");
    }

    u32 adc_value = (65536 * (u8)datagram[23]) + (256* (u8)datagram[24]) + (u8)datagram[25];
    ui->label_adc_raw->setText(QString::number(adc_value));


    /*
    start_stream = (unsigned char)datagram[0] + ((unsigned char)datagram[1] << 8) + ((unsigned char)datagram[2] << 16) + ((unsigned char)datagram[3] << 24);
    if(start_stream == 0x11223344){
        ui->label_communication->setText("Communication is established");

        real_time.test_no = (unsigned char)datagram[4] + ((unsigned char)datagram[5] << 8) + ((unsigned char)datagram[6] << 16) + ((unsigned char)datagram[7] << 24);

        tmp = (unsigned char)datagram[8] + ((unsigned char)datagram[9] << 8);
        if(tmp == 0){
            real_time.status = "Test is STOPPED";
        }
        else if(tmp == 1){
            real_time.status = "Test is RUNNING";
        }
        else if(tmp == 2){
            real_time.status = "Test is PAUSED";
        }
        else if(tmp == 3){
            real_time.status = "Test is FINISHED";
        }

        my_resp._integer = ((unsigned char)datagram[10] + ((unsigned char)datagram[11] << 8) + ((unsigned char)datagram[12] << 16) + ((unsigned char)datagram[13] << 24));
        real_time.area = my_resp._float;

        my_resp._integer = ((unsigned char)datagram[15] + ((unsigned char)datagram[16] << 8) + ((unsigned char)datagram[17] << 16) + ((unsigned char)datagram[18] << 24));
        real_time.load = my_resp._float;

        my_resp._integer = ((unsigned char)datagram[35] + ((unsigned char)datagram[36] << 8) + ((unsigned char)datagram[37] << 16) + ((unsigned char)datagram[38] << 24));
        real_time.stress = my_resp._float;

        ui->label_main_load->setText(QString::number(real_time.load,'f',3));
        plot_graph();
        emit test_value_tracker(real_time.status);
    }
    else{
        ui->label_communication->setText("No communication");

        QString str = datagram;
        QString first_section = str.section(":",0,0);
        QString second_section = str.section(":",1,1);
        QString third_section = str.section(":",2,2);

        if(first_section == "get"){
            if(second_section == "param.pace_rate"){
//                ui->doubleSpinBox_pace_rate->setValue(third_section.toDouble());
//                ui->doubleSpinBox_pace_rate_mpa->setValue(1000*(double)third_section.toDouble()/(double)real_time.area);
            }
        }
    }
    */

}
