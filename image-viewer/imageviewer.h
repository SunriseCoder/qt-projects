#include <iostream>

#include <QScrollArea>
#include <QLabel>

using namespace std;

class ImageViewer : public QScrollArea {
public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

private:
    QLabel *imageLabel;
    QImage image;

    void setImage();
    void setImage(const QImage &newImage);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);
};
