#ifndef SAMPLECONTROLLER_H
#define SAMPLECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QReadWriteLock>

class PLC_STAT;
class PlcClient;

class SampleController : public QObject
{
  Q_OBJECT
public:
  SampleController(PlcClient &plc, QObject *parent = 0);
  ~SampleController() {
    worker_thread_.quit();
    worker_thread_.wait();
  }

  QThread worker_thread_;

  void Initialize();
  void ReadPlcStatus(PLC_STAT &plc_status);
  void Start() {
    emit start();
  }
  void Stop() {
    emit stop();
  }

signals:
  void setTimer(int msec);
  void start();
  void stop();

public slots:

private:
  PlcClient &plc_;
  QReadWriteLock lock_;

};

#endif // SAMPLECONTROLLER_H
