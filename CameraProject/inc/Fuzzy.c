/*
 * Fuzzy.c
 * Fuzzy Logic functions
 * December 22, 2019
 */
/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/
#include <stdint.h>
#include "Fuzzy.h"


// returns N-M, bounded to -128 to +127
/*
int8_t static Subtract(uint8_t N, uint8_t M){
uint32_t N32,M32;
int32_t Result32;
  N32 = N;              // Promote N,M
  M32 = M;
  Result32 = N32-M32;   // -256<Result32<+256
  if(Result32<-128){
    Result32 = -128; // floor
  }
  if(Result32>127){
    Result32 = 127;  // ceiling
  }
  return(Result32);
}*/
// Fuzzy NOT is complement
fuz_t not(fuz_t u1){
  return 255-u1;
}

// Fuzzy AND is minimum
fuz_t and(fuz_t u1,fuz_t u2){
  if(u1>u2){
    return(u2);
  }
  return(u1);
}

// Fuzzy AND is minimum
fuz_t and3(fuz_t u1,fuz_t u2,fuz_t u3){
  if(u1>u2){
    if(u2>u3){
      return u3;
    }
    return(u2);
  }
  if(u1>u3){
    return u3;
  }
  return(u1);
}

// Fuzzy OR is maximum
fuz_t or(fuz_t u1,fuz_t u2){
  if(u1<u2){
    return(u2);
  }
  return(u1);
}
// Fuzzy OR is maximum
fuz_t or3(fuz_t u1,fuz_t u2,fuz_t u3){
  if(u1<u2){
    if(u2<u3){
      return u3;
    }
    return(u2);
  }
  if(u1<u3){
    return u3;
  }
  return(u1);
}

// debugging  and visualization
// 'a' is false, 'z' is true
char FuzzyChar(fuz_t fuz){
  return 'a' + (25*fuz)/255;
}

uint32_t umin32(uint32_t u1,uint32_t u2){
  if(u1>u2){
    return(u2);
  }
  else{
    return(u1);
  }
}

int32_t min32(int32_t n1,int32_t n2,int32_t n3){
  if(n1>n2){
    if(n2>n3){
      return n3;
    }
    return(n2);
  }
  else{
    if(n1>n3){
      return n3;
    }
    return(n1);
  }
}

fuz_t MinFuzzification(int32_t crisp, const int32_t MIN, const int32_t MAX){
  if(crisp <= MIN){
    return 255;
  }
  if(crisp >= MAX){
    return 0;
  }
  return (255*(MAX-crisp))/(MAX-MIN);
}

fuz_t MaxFuzzification(int32_t crisp, const int32_t MIN, const int32_t MAX){
  if(crisp <= MIN){
    return 0;
  }
  if(crisp >= MAX){
    return 255;
  }
  return (255*(crisp-MIN))/(MAX-MIN);
}

fuz_t CenterFuzzification(int32_t crisp, const int32_t MIN, const int32_t CENTER, const int32_t MAX){
  if(crisp <= MIN){
    return 0;
  }
  if(crisp >= MAX){
    return 0;
  }
  if(crisp <= CENTER){
    return (255*(crisp-MIN))/(CENTER-MIN);
  }
  return (255*(MAX-crisp))/(MAX-CENTER);
}


fuz_t LeftRightFuzzification(int32_t crisp, const int32_t MIN,
    const int32_t LEFT, const int32_t RIGHT, const int32_t MAX){
  if(crisp <= MIN){
    return 0;
  }
  if(crisp >= MAX){
    return 0;
  }
  if(crisp < LEFT){
    return (255*(crisp-MIN))/(LEFT-MIN);
  }
  if(crisp > RIGHT){
    return (255*(MAX-crisp))/(MAX-RIGHT);
  }
  return 255; // between LEFT and RIGHT
}
