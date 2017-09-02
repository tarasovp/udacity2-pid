#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  /*double p_error;
  double i_error;
  double d_error;*/

  /*
  * Coefficients
  */ 
  /*double Kp;
  double Ki;
  double Kd;*/
    
    double KPID[3];
    double dKPID[3];
    
    double prev_cte;
    double int_cte;
    int first;
    double diff_cte;
    double current_error;
    
    
    
  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp_, double Ki_, double Kd_);

  /*
  * Update the PID error variables given cross track error.
  */
  double Steering(double cte);
    

  /*
  * Calculate the total PID error.
  */
  double TotalError();
    
    int twiddle_dirrection=0;
    int twiddle_variable=0;
    double err;
    double now_error;
    double best_error;
    void twiddle();
    
 void ResetError();
};

#endif /* PID_H */
