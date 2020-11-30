#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::initilize_plot(void){
    customPlot = new QCustomPlot(this->ui->plot_widget);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->setGeometry(0,0,ui->plot_widget->width(),ui->plot_widget->height());
    // give the axes some labels:
    customPlot->yAxis->setLabel("LOAD (kN)");
    customPlot->xAxis->setLabel("TIME (s)");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();
}
void MainWindow::plot_graph(void){
    static bool first_in = false;
    static unsigned int counter = 0;
    static double max_y = 0;
    static double min_y = 0;
    double x_axis,y_axis;

    if(first_in == false){
        first_in = true;
        graph_time.start();
        counter = 0;
    }
    x_axis = (double)graph_time.elapsed() / 1000;
    customPlot->xAxis->setRange(0,x_axis+1);

    y_axis = real_time.load;
    if(y_axis > max_y){
        max_y = y_axis;
    }
    if(y_axis < min_y){
        min_y = y_axis;
    }
    customPlot->yAxis->setRange(min_y-1,max_y+1);

    customPlot->graph(0)->addData(x_axis,y_axis);
    counter++;

    customPlot->replot();
}

