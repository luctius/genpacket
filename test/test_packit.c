#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "crc.h"


START_TEST (test_crc8)
{
    // ./pycrc.py --model=crc-8 --check-string=teststring
    uint8_t crc_calc = crc8((uint8_t*)strdup("teststring"), 10);
    ck_assert_int_eq(crc_calc,0x66);
}
END_TEST

START_TEST (test_crc32)
{
    // ./pycrc.py --model=crc-32 --check-string=teststring  
    uint32_t crc_calc = crc32(strdup("teststring"), 10);
    ck_assert_int_eq(crc_calc,0x1F58F83E);
}
END_TEST
    
 	
Suite * test_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Packit");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_crc8);
    tcase_add_test(tc_core, test_crc32);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = test_suite();
    sr = srunner_create(s);
    srunner_set_tap(sr, "-");
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}