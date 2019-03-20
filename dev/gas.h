#ifndef DEV_GAS_H_
#define DEV_GAS_H_

#include "plcmsg/plc_nml.h"

#ifdef toLine
#undef toLine
#endif

class Gas {
 public:
  virtual int Open(int gas_id, double pressure) = 0;
  virtual int Close() = 0;
  virtual ~Gas() {}

 protected:
  PLC_GAS_STAT status_;
};

#endif
