#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonStart, SIGNAL(clicked()), ui->separationDrawer, SLOT(startDrawTimer()));
    connect(ui->buttonStop, SIGNAL(clicked()), ui->separationDrawer, SLOT(stopDrawTimer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
