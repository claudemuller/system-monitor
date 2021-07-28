#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor() {
    prev_idle_ = 0;
    prev_io_wait_ = 0;
    prev_non_idle_ = 0;
    prev_user_ = 0;
    prev_nice_ = 0;
    prev_system_ = 0;
    prev_irq_ = 0;
    prev_soft_irq_ = 0;
    prev_steal_ = 0;
    prev_total_ = 0;
  }

  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prev_idle_;
  float prev_io_wait_;
  float prev_non_idle_;
  float prev_user_;
  float prev_nice_;
  float prev_system_;
  float prev_irq_;
  float prev_soft_irq_;
  float prev_steal_;
  float prev_total_;
};

#endif