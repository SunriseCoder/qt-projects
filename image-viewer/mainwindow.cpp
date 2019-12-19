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

    // Connecting Cross-Image Move Signals and Slots
    connect(ui->imageViewer1, &ImageViewer::imageMoved, ui->imageViewer2, &ImageViewer::moveImage);
    connect(ui->imageViewer2, &ImageViewer::imageMoved, ui->imageViewer1, &ImageViewer::moveImage);

    // Connecting Cross-Image Scale Signals and Slots
    connect(ui->imageViewer1, &ImageViewer::imageScaled, ui->imageViewer2, &ImageViewer::scaleImage);
    connect(ui->imageViewer2, &ImageViewer::imageScaled, ui->imageViewer1, &ImageViewer::scaleImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}
