#include <iostream>

#include <QImageReader>
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

    ui->scrollArea1->horizontalScrollBar()->installEventFilter(this);
    ui->scrollArea1->verticalScrollBar()->installEventFilter(this);
    ui->scrollArea2->horizontalScrollBar()->installEventFilter(this);
    ui->scrollArea2->verticalScrollBar()->installEventFilter(this);

    ui->scrollArea1->setWidgetResizable(false);
    ui->scrollArea1->setVisible(true);
    ui->scrollArea2->setWidgetResizable(false);
    ui->scrollArea2->setVisible(true);

    setImage();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // Disabling Wheel Events on ScrollAreas' Scrollbars
    // due to Zoom In/Out Images with Mouse Wheel
    if (event->type() == QEvent::Wheel
            && (obj == ui->scrollArea1->horizontalScrollBar()
                || obj == ui->scrollArea1->verticalScrollBar()
                || obj == ui->scrollArea2->horizontalScrollBar()
                || obj == ui->scrollArea2->verticalScrollBar())) {
       return true;
    }
    return false;
}

void MainWindow::setImage() {
    QString fileName("C:\\tmp\\1.bmp");
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage image = reader.read();

    ui->imageLabel1->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel1->adjustSize();
}

// Move Image with Mouse
QPointF lastMousePos;
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (Qt::RightButton == event->buttons()) {
        lastMousePos = event->pos();
        qApp->setOverrideCursor(Qt::ClosedHandCursor);
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPointF mouseDelta = event->pos() - lastMousePos;
    lastMousePos = event->pos();

    if (Qt::RightButton == event->buttons()) {
        QScrollBar *horizontalScrollBar = ui->scrollArea1->horizontalScrollBar();
        horizontalScrollBar->setValue(horizontalScrollBar->value() - mouseDelta.x());

        QScrollBar *verticalScrollBar = ui->scrollArea1->verticalScrollBar();
        verticalScrollBar->setValue(verticalScrollBar->value() - mouseDelta.y());
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::RightButton) == 0) {
        qApp->restoreOverrideCursor();
    }
}

// Zoom In and Out Image with Mouse Wheel
double scaleFactor = 1;
void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0) {
        scaleFactor *= 1.1;
    } else {
        scaleFactor /= 1.1;
    }
    ui->imageLabel1->resize(scaleFactor * ui->imageLabel1->pixmap()->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
