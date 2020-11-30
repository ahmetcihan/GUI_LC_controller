#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QElapsedTimer>
#include "remoteconnection.h"

typedef char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

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
    QElapsedTimer graph_time;
    QString currentStyleSheet;
    QTimer *_100_msec_timer;
    u8 global_downcounter;

    bool relay_status[4];

    struct _real_time{
        int test_no;
        QString status;
        double area;
        double load;
        double stress;
    };
    struct _real_time real_time;

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
