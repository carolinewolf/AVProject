#ifndef VIDEOENGINE_H
#define VIDEOENGINE_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include "videoprocessor.h"
#include "cvmattoqimage.h"
#include "videoformat.h"
#include "videowidget.h"


class VideoEngine : public QThread
{
    Q_OBJECT
public:
    VideoEngine();
    ~VideoEngine();
    void openFile(const QString& file);
    void openCamera(int device = 0);
    void setProcessor(VideoProcessor*);
    void setInputWidget(VideoWidget* inputWidget);
    void setProcessedWidget(VideoWidget* processedWidget);
    const VideoFormat& videoFormat() const;
    int getWidth();
    int getHeight();
    int framePosition();

protected:
    void run();
public slots:
    void stop();
signals:
    void sendInputImage(const QImage&);
    void sendProcessedImage(const QImage&);
private:
    cv::VideoCapture videoCapture;
    VideoFormat _videoFormat;
    bool stopped;
    QMutex mutex;
    VideoProcessor* processor;
    bool usingCamera;
};


#endif // VIDEOENGINE_H
