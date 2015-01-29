#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "genpacket.h"

void genpacket_p0_fixed_received(struct p0_fixed *packet, void *private_ctx) {
}

void genpacket_p1_fixed_received(struct p1_fixed *packet, void *private_ctx) {
}

void genpacket_p2_fixed_received(struct p2_fixed *packet, void *private_ctx) {
}

void genpacket_p3_calculated_received(struct p3_calculated *packet, void *private_ctx) {
}

void genpacket_p4_calculated_received(struct p4_calculated *packet, void *private_ctx) {
}

START_TEST (test_true)
{
    ck_assert_int_eq(0x66,0x66);
}
END_TEST

 	
Suite * test_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("GenPacket");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_true);
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
