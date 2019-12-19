#include <iostream>

#include <QImageReader>
#include <QMouseEvent>
#include <QScrollBar>

#include "imageviewer.h"

using namespace std;

ImageViewer::ImageViewer(QWidget *parent) : QScrollArea(parent) {
    imageLabel = new QLabel();
    imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
    imageLabel->setScaledContents(true);
    setWidget(imageLabel);
}

void ImageViewer::setImage(QString filename) {
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    const QImage image = reader.read();

    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->adjustSize();

    // TODO It seems, QT bug - imageLabel doesn't emit QEvent::Resize
    // This method updates its geometry and scrollbars
    setWidgetResizable(false);
}

// Move Image with Mouse
QPointF lastMousePos;
void ImageViewer::mousePressEvent(QMouseEvent *event) {
    if (Qt::LeftButton == event->buttons()) {
        lastMousePos = event->pos();
        //qApp->setOverrideCursor(Qt::ClosedHandCursor);
    }
}
void ImageViewer::mouseMoveEvent(QMouseEvent *event) {
    QPointF mouseDelta = event->pos() - lastMousePos;
    lastMousePos = event->pos();

    if (Qt::LeftButton == event->buttons()) {
        moveImage(mouseDelta);
        emit imageMoved(mouseDelta);
    }
}
void ImageViewer::mouseReleaseEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) == 0) {
        //qApp->restoreOverrideCursor();
    }
}
void ImageViewer::moveImage(QPointF delta) {
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
    verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
}

// Zoom In and Out Image with Mouse Wheel
double scaleFactor = 1;
void ImageViewer::wheelEvent(QWheelEvent *event) {
    double oldScaleFactor = scaleFactor;
    if (event->delta() > 0) {
        scaleFactor *= 1.1;
    } else {
        scaleFactor /= 1.1;
    }
    scaleImage(scaleFactor);
    emit imageScaled(scaleFactor);

    // Adjust ScrollBar positions that the position
    // under the mouse cursor would be on the same place
    int oldOffsetX = horizontalScrollBar()->value() + event->x();
    int newOffsetX = oldOffsetX * scaleFactor / oldScaleFactor;
    int oldOffsetY = verticalScrollBar()->value() + event->y();
    int newOffsetY = oldOffsetY * scaleFactor / oldScaleFactor;
    QPointF *delta = new QPointF(oldOffsetX - newOffsetX, oldOffsetY - newOffsetY);
    moveImage(*delta);
    emit imageMoved(*delta);
}
void ImageViewer::scaleImage(double scaleFactor) {
    QSize imageSize = scaleFactor * imageLabel->pixmap()->size();
    imageLabel->resize(imageSize);

    // TODO It seems, QT bug - imageLabel doesn't emit QEvent::Resize
    // This method updates its geometry and scrollbars
    setWidgetResizable(false);
}

ImageViewer::~ImageViewer() {
    delete imageLabel;
}
