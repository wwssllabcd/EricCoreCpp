#pragma once

//#define USE_BOOST

// put include MyException.h in here, if you wanna use Utility, you msut include MyException.h, too


//#include "Observer.h" // for less include
#include "DefineFiles/DefineFile.h"
#include <string>
#include <vector>

// for gcc
//#include "stdarg.h"

using namespace std;

//#define GetRandom(min,max) rand()*((float)((max)-(min)))/(float)(RAND_MAX)+(min)

namespace EricCore {
    class Utility
    {
    public:
        Utility(void);
        ~Utility(void);

        //estring getDiffStringInTwoBuf(eu32 lbaAddr, int length, eu8* writeBuf, eu8* readBuf);

        /*eu32 getRandom(eu32 min, eu32 max) {
            eu32 res = rand() * ((float)((max)-(min))) / (float)(RAND_MAX)+(min);
            return res;
        }*/
   
    };
}