#include "worker.h"
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
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

    // Background Subtraction variables
    cv::UMat bgModelFrame; //Background model currFrame
    cv::UMat diffMat, grayMat, threshMat;

    // Static object detection variables
    cv::UMat curForeground, prevForeground, staticMask, coloredStaticMask;
    bool isModelSet = false;

    float alpha = 0.99;
    float beta = 1 - alpha;
    int updateInterval = 2;
    int counter = 0;
    while (running && vc.isOpened() && noFrameCount < 5) {
        counter += 1;
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
        if (!isModelSet){
            bgModelFrame = rawMat.clone();
            cv::absdiff(rawMat, rawMat, prevForeground);
            cv::cvtColor(prevForeground, prevForeground, CV_BGR2GRAY);
            isModelSet = true;
        }
        /*****************************************
        * Background Subtraction
        *****************************************/
        cv::absdiff(bgModelFrame, rawMat, diffMat); // Absolute differences between the 2 images
        cv::cvtColor(diffMat, grayMat, CV_BGR2GRAY);
        cv::threshold(grayMat,
                threshMat,
                threshold,
                255,
                CV_THRESH_BINARY); // set threshold to ignore small differences you can also use inrange function

        /*****************************************
        * Reduce Noise & Improve mask
        *****************************************/
        cv::medianBlur(threshMat, threshMat, 5);

        /*****************************************
        * Static Object Detection
        *****************************************/
        if (counter >= updateInterval) {
            cv::addWeighted(prevForeground, alpha, threshMat, beta, 0.0, prevForeground);
            cv::threshold(prevForeground, staticMask, 50, 255, CV_THRESH_TOZERO);
            cv::applyColorMap(staticMask, coloredStaticMask, cv::COLORMAP_JET);

            cv::cvtColor(coloredStaticMask, coloredStaticMask, cv::COLOR_BGR2RGB);
            emit result("gray", coloredStaticMask.getMat(cv::ACCESS_READ));
            counter = 0;
        }

        cv::cvtColor(rawMat, rawMat, cv::COLOR_BGR2RGB);
        emit result("raw", rawMat.getMat(cv::ACCESS_READ));

        emit preview();
    }
    std::cout << url << " releasing" << std::endl;
    vc.release();
    std::cout << url << " released" << std::endl;
    running = false;
    emit stopped();
}
