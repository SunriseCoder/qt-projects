#include <QLabel>
#include <QObject>
#include <QScrollArea>

class ImageViewer : public QScrollArea {
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

signals:
    void imageMoved(QPointF delta);
    void imageScaled(double scaleFactor);

public slots:
    void moveImage(QPointF delta);
    void scaleImage(double scaleFactor);

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
