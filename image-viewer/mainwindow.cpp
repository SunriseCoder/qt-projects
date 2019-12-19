#include <iostream>

#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();

    // TODO Rewrite with saving to ini-file or so
    // If there is no ini-file (first run), then no resize is needed
    // Setting initial TreeView and Image Area width proportions
    QList<int> list;
    list << 200 << 10000;
    ui->areaSplitter->setSizes(list);
}

MainWindow::~MainWindow()
{
    delete ui;
}
