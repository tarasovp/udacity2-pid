//
//  twiddler.h
//  PID
//
//  Created by Тарасов Павел on 02/09/2017.
//
//

#include "PID.h"
#ifndef twiddler_h


class Twiddler {
public:
    /*
     * Errors
     */
    int params;
    double *t;
    double *d;
    
    double best_error;
    
    
    /*
     * Constructor
     */
    Twiddler(int cnt_params, double *params, double *dparams);
    
    /*
     * Destructor.
     */
    virtual ~Twiddler();
    
    bool Twiddle(error, max_error);

};



#define twiddler_h


#endif /* twiddler_h */
