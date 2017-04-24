#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <opencv2/core/mat.hpp>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);

    bool running = false;

    std::string url = "0";

    double_t threshold = 50;

    void run();

signals:
    void result(std::string id, cv::Mat mat);

    void stopped();

    void preview();
};

#endif // WORKER_H
