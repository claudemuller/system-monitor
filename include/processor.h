#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  double Utilization();

 private:
  long prev_active_{0};
  long prev_total_{0};
  long current_active_{0};
  long current_total_{0};
};

#endif