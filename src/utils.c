#include <string.h>

int convert_to_int(char str[4])
{
    int result = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        if(str >= '0' && str <= '9' )
        {
            result = result * 10 + (str[i] - '0');
        }
    }

    return result;
}