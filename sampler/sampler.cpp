#include "sampler.h"
#include "plcclient/plcclient.h"

#include <QWriteLocker>
#include <QDebug>

Sampler::Sampler(PlcClient &plc, QReadWriteLock &lock, QObject *parent):
    QObject(parent), plc_(plc), lock_(lock), sample_timer_(NULL) {

  t.start();
}

void Sampler::SetTimer(int msec) {
  sample_timer_ = new QTimer(this);
  sample_timer_->setInterval(msec);
  connect(sample_timer_, SIGNAL(timeout()), this, SLOT(doWork()));
  sample_timer_->start();
}

void Sampler::doWork() {
  qDebug("Time elapsed in sampler: %d ms", t.elapsed());
  t.restart();
  QWriteLocker locker(&lock_);
  plc_.UpdateStatus();
  plc_.UpdateError();
}
