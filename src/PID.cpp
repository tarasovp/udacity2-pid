#include "PID.h"
#include <math.h> 

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    /*Kp=Kp_;
    Ki=Ki_;
    Kd=Kd_;*/
    KPID[0]=Kp;
    KPID[1]=Ki;
    KPID[2]=Kd;
    
    prev_cte=0;
    int_cte=0;
    
    //initialize for twiddle
    for (int i=0;i<3;i++)  dKPID[i]=KPID[i]/100;
    err=0;
    best_error=9999999;
    
    
}

double PID::Steering(double cte) {
    diff_cte=(cte-prev_cte);
    prev_cte=cte;
    first=0;
    int_cte+=cte;
    err+=fabs(cte);
    return - KPID[0] * cte - KPID[2] * diff_cte - KPID[1] * int_cte;
    
}

double PID::TotalError() {
    return err;
}

void PID::ResetError(){
    err=0;
}

void PID::twiddle(){
    //if going up
    
    //getting current error
    float err=TotalError();
    
    
    if (twiddle_dirrection==0)
    {
        if (err<best_error) best_error=err;
        twiddle_dirrection=1;
        
        KPID[twiddle_variable]+=dKPID[twiddle_variable];
        
    }
    else
    {
        if (err<best_error)
        {
            dKPID[twiddle_variable]*=1.1;
            twiddle_dirrection=0;
            twiddle_variable=(twiddle_variable+1)%3;
            twiddle();
            
        }
        else
        {
            if (twiddle_dirrection==1)
            {
                twiddle_dirrection=2;
                KPID[twiddle_variable]-=2*dKPID[twiddle_variable];
            }
            else
            {
                KPID[twiddle_variable]+=dKPID[twiddle_variable];
                dKPID[twiddle_variable]*=0.9;
                twiddle_dirrection=0;
                twiddle_variable=(twiddle_variable+1)%3;
                twiddle();
            }
        }
        
    }
    ResetError();
    
}

/*def run(robot, tau_p, tau_d, tau_i, n=100, speed=1.0):
 x_trajectory = []
 y_trajectory = []
 prev_cte = robot.y
 int_cte = 0
 for i in range(n):
 cte = robot.y
 diff_cte = cte - prev_cte
 prev_cte = cte
 int_cte += cte
 steer = -tau_p * cte - tau_d * diff_cte - tau_i * int_cte
 robot.move(steer, speed)
 x_trajectory.append(robot.x)
 y_trajectory.append(robot.y)
 return x_trajectory, y_trajectory*/
