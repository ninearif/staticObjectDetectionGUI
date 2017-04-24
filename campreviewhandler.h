#ifndef CAMPREVIEWHANDLER_H
#define CAMPREVIEWHANDLER_H

#include <QObject>
#include <QThread>
#include "worker.h"

class CamPreviewHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString baseImageId MEMBER m_baseImageId)

    Q_PROPERTY(qreal threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)

    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    explicit CamPreviewHandler(QObject *parent = 0);

    enum Status { Running, Stop };
    Q_ENUM(Status)

    qreal threshold();

    void setThreshold(qreal value);

    Status status();
signals:
    void preview();

    void thresholdChanged();

    void statusChanged();

public slots:
    void start(QString url);
    void stop();

private:
    Status m_status = Stop;
    qreal m_threshold;
    QString m_baseImageId;
    QThread thread;
    Worker worker;
};

#endif // CAMPREVIEWHANDLER_H
