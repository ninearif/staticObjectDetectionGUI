#ifndef MATIMAGEPROVIDER_H
#define MATIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <map>
#include <opencv2/core/mat.hpp>

class MatImageProvider : public QQuickImageProvider
{
public:
    MatImageProvider();

    ~MatImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    void setMat(std::string id, cv::Mat mat);

    static MatImageProvider* instance();
};

#endif // MATIMAGEPROVIDER_H
