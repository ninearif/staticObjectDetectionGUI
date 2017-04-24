#include "worker.h"
#include <QThread>
#include <opencv2/opencv.hpp>
#include <iostream>

Worker::Worker(QObject *parent) : QObject(parent)
{
}

void Worker::run()
{
    running = true;
    cv::VideoCapture vc;

    std::cout << "opening " << url << std::endl;
    if (url.length() == 1) {
        vc = cv::VideoCapture(std::stoi(url));
    } else {
        vc = cv::VideoCapture(url);
    }

    if (!vc.isOpened()) {
        std::cerr << "cannot open video " << url << std::endl;
        vc.release();
        running = false;
        emit stopped();
        return;
    }

    std::cout << "ready " << url << " in thread id " << QThread::currentThreadId() << std::endl;
    cv::UMat rawMat;
    int noFrameCount = 0;
    int dropCounter = 0;
    while (running && vc.isOpened() && noFrameCount < 5) {
        // drop frame every x frames
        dropCounter += 1;
        if (dropCounter == 20) {
            dropCounter = 0;
            continue;
        }
        QThread::msleep(30);
        auto ret = vc.read(rawMat);
        if (!ret) {
            noFrameCount += 1;
            continue;
        }
        noFrameCount = 0;

        cv::cvtColor(rawMat, rawMat, cv::COLOR_BGR2RGB);
        emit result("raw", rawMat.getMat(cv::ACCESS_READ));

        cv::UMat grayMat;
        cv::cvtColor(rawMat, grayMat, cv::COLOR_RGB2GRAY);
        cv::threshold(grayMat, grayMat, threshold, 255, cv::THRESH_BINARY);
        cv::cvtColor(grayMat, grayMat, cv::COLOR_GRAY2RGB);
        emit result("gray", grayMat.getMat(cv::ACCESS_READ));

        emit preview();
    }
    std::cout << url << " releasing" << std::endl;
    vc.release();
    std::cout << url << " released" << std::endl;
    running = false;
    emit stopped();
}
