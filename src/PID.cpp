#include "PID.h"
#include <math.h>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    KPID[0] = Kp;
    KPID[1] = Ki;
    KPID[2] = Kd;

    prev_cte = 0;
    int_cte = 0;

}

double PID::Steering(double cte) {
    diff_cte = (cte - prev_cte);
    prev_cte = cte;
    int_cte += cte;
    err += fabs(cte);
    return - KPID[0] * cte - KPID[2] * diff_cte - KPID[1] * int_cte;

}





