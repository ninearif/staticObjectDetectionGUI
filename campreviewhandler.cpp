#include "campreviewhandler.h"
#include <iostream>
#include <QThreadPool>
#include "matimageprovider.h"

CamPreviewHandler::CamPreviewHandler(QObject *parent) : QObject(parent)
{
    worker.moveToThread(&thread);

    connect(&thread, &QThread::started, &worker, &Worker::run);

    connect(&worker, &Worker::result, [&](std::string id, cv::Mat mat) {
        MatImageProvider::instance()->setMat(m_baseImageId.toStdString() + id, mat);
    });

    connect(&worker, &Worker::preview, this, &CamPreviewHandler::preview);

    connect(&worker, &Worker::stopped, [&] {
        thread.quit();
        m_status = Stop;
        emit statusChanged();
    });
}

qreal CamPreviewHandler::threshold()
{
    return m_threshold;
}

void CamPreviewHandler::setThreshold(qreal value)
{
    m_threshold = value;
    worker.threshold = m_threshold;
}

CamPreviewHandler::Status CamPreviewHandler::status()
{
    return m_status;
}

void CamPreviewHandler::start(QString url)
{
    if (worker.running == true) {
        throw "Invalid state worker already running";
        return;
    }

    m_status = Running;
    emit statusChanged();
    std::cout << "starting in thread id " << QThread::currentThreadId() << std::endl;

    worker.url = url.toStdString();

    thread.start();
}

void CamPreviewHandler::stop()
{
    worker.running = false;
}
