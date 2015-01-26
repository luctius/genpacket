#include <check.h>
#include <stdio.h>
#include "src/decoder.h"
 	
START_TEST (test_packet_p0_fixed)
{
    FILE * p0_fixed_data = fopen("test_packet_p0_fixed.dat","r");
    
    decode_using_packet(0,p0_fixed_data);
    ck_assert_int_eq(5,5);
}
END_TEST
    
 	
int main(void)
{
    return 0;
}