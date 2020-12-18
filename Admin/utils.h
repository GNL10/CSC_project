#ifndef UTILS_H
#define UTILS_H

#include "admin.h"

class Utils{
    
    public:
    
        static int* parseInt_toBinary(int value){
            static int a[NUM_MAX_BITS];
            for (int i = 0; value>0; i++){
                a[i] = value%2;
                value = value/2;
            }
            return a;
        }


};

#endif