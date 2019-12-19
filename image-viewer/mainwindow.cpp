#include <iostream>

#include <QFileDialog>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
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

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_openFolderButton_clicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    if (fileNames.count() == 1) {
        workFolder = fileNames[0];
        on_refreshFolderButton_clicked();
    }
}

void MainWindow::on_refreshFolderButton_clicked() {
    if (workFolder.isEmpty()) {
        return;
    }

    ui->listWidget->clear();

    QDir directory(workFolder);
    QStringList images = directory.entryList(QStringList() << "*.bmp" << "*.jpg" << "*.png" << "*.tif", QDir::Files);
    QStringList itemList;
    foreach(QString filename, images) {
        // Creating QFileInfo due to easy string operations
        QFileInfo fileInfo(filename);
        // Getting the filename up to last dot
        QString filenameBase = fileInfo.completeBaseName();
        if (!filenameBase.endsWith("-before")) {
            continue;
        }

        // Creating base filename (like 1.bmp for 1-before.bmp)
        filenameBase = filenameBase.remove("-before");

        // Getting the file extenstion
        QString fileExtension = fileInfo.suffix();
        // Checking pair file (like 1-before.bmp and 1-after.bmp)
        QString pairFilename = filenameBase + "-after." + fileExtension;
        if (images.contains(pairFilename)) {
            // Creating base file item
            QString item = filenameBase + "." + fileExtension;
            ui->listWidget->addItem(item);
        }
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    // Getting Item Text
    QString value = current->text();

    // Creating QFileInfo for string operations
    QFileInfo fileInfo(value);
    QString baseFilename = fileInfo.completeBaseName();
    QString fileExtension = fileInfo.suffix();

    // Creating full-path to the files
    QString filename1 = workFolder + "/" + baseFilename + "-before." + fileExtension;
    QString filename2 = workFolder + "/" + baseFilename + "-after." + fileExtension;

    // Setting filenames into ImageViewers
    ui->imageViewer1->setImage(filename1);
    ui->imageViewer2->setImage(filename2);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Select next element in ListWidget by pressing Enter
    if (event->key() == Qt::Key_Return) {
        int currentRow = ui->listWidget->currentRow();
        if (currentRow < ui->listWidget->count() - 1) {
            ui->listWidget->setCurrentRow(currentRow + 1);
        }
    }
}
