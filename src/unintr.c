#include <unistd.h>

void main() 
{  
    if (!vfork()) 
        sleep(100);  
} 
