#ifndef SAMPLER_H
#define SAMPLER_H

#include <QObject>
#include <QReadWriteLock>

#include <QTime>
#include <QTimer>


class PlcClient;

class Sampler : public QObject
{
  Q_OBJECT
public:
  Sampler(PlcClient &plc, QReadWriteLock &lock, QObject *parent = 0);

signals:

public slots:
  void doWork();
  void SetTimer(int msec);

  void Stop() {
    sample_timer_->stop();
  }

  void Start() {
    sample_timer_->start();
  }

private:
  PlcClient &plc_;
  QReadWriteLock &lock_;
  QTimer *sample_timer_;

  QTime t;
};

#endif // SAMPLER_H
