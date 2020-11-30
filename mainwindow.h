#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <qcustomplot.h>
#include "remoteconnection.h"

typedef char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

#define STEPPER_COMMAND_RUN_LEFT    1
#define STEPPER_COMMAND_RUN_RIGHT   2
#define STEPPER_COMMAND_STOP        3
#define STEPPER_COMMAND_POS_RESET   4
#define STEPPER_COMMAND_GO_POS      5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initilize_plot(void);
    void plot_graph(void);

    RemoteConnection *remote;
    QCustomPlot *customPlot;
    QElapsedTimer graph_time;
    QString currentStyleSheet;
    QTimer *_100_msec_timer;
    u8 global_downcounter;
    u8 step_motor_command;
    u32 step_motor_speed;

    bool relay_status[6];

    struct _real_time{
        int test_no;
        QString status;
        double area;
        double load;
        double stress;
    };
    struct _real_time real_time;
signals:
    void test_value_tracker(QString val);

public slots:
    void eth_reconnect(void);
    void start_comm(void);
    void periodic_send(void);
    void _100_msec_loop(void);
    void periodic_response_handler(QByteArray datagram);
    void set_gradient(void);
    void relay_1_status_handle(bool val);
    void relay_2_status_handle(bool val);
    void relay_3_status_handle(bool val);
    void relay_4_status_handle(bool val);
    void relay_5_status_handle(bool val);
    void relay_6_status_handle(bool val);
    void step_run_left(void);
    void step_run_right(void);
    void step_stop(void);
    void step_pos_reset(void);
    void step_go_pos(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
