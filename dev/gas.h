#ifndef DEV_GAS_H_
#define DEV_GAS_H_

class Gas {
 public:
  virtual int Open(int gas_id, double pressure) = 0;
  virtual int Close() = 0;
  virtual ~Gas() {}
};

#endif
