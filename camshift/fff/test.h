#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <opencv2/opencv.hpp>
class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = 0);
    bool busy;
signals:
    void gotFrame(unsigned char *);
public slots:
    void oneFrame(unsigned char *data);
};

#endif // TEST_H
