#ifndef NMLGAS
#define NMLGAS

#include "gas.h"

class PlcClient;

class NmlGas: public Gas {
 public:
  NmlGas(PlcClient &plc_client): plc_client_(plc_client) {}
  virtual int Open(int gas_id, double pressure);
  virtual int Close();
  virtual ~NmlGas() {}

 private:
  PlcClient &plc_client_;
};

#endif // NMLGAS

