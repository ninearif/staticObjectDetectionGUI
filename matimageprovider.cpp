#include "matimageprovider.h"

#include <stdexcept>
#include <iostream>
#include <stdio.h>

std::map<std::string, QPixmap> images;

MatImageProvider::MatImageProvider()
    :QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

MatImageProvider::~MatImageProvider()
{

}

QPixmap MatImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap p;
    try {
        p = images.at(id.toStdString());
    } catch (std::out_of_range &e) {
        QImage i(":/color_bars.png");
        p.convertFromImage(i);
        images[id.toStdString()] = p;
    }

    return p;
}

void MatImageProvider::setMat(std::string id, cv::Mat mat)
{
    QImage i = QImage(mat.data,
                    mat.cols,
                    mat.rows,
                    QImage::Format_RGB888);
    auto p = QPixmap::fromImage(i);

    images[id] = p;
}

MatImageProvider* MatImageProvider::instance()
{
    static auto instance = new MatImageProvider;
    return instance;
}
