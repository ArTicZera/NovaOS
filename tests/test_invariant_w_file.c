#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "DOOM/w_file.h"

START_TEST(test_w_read_bounds_check_invariant)
{
    // Invariant: W_Read must never read beyond wad->length regardless of offset/buffer_len values
    typedef struct {
        unsigned int offset;
        size_t buffer_len;
        size_t wad_length;
        const char *description;
    } test_case;
    
    test_case cases[] = {
        // Exact exploit: integer overflow bypass
        {UINT32_MAX, 2, 100, "integer_overflow_bypass"},
        // Boundary: offset equals wad->length (should read zero bytes)
        {100, 50, 100, "offset_at_length"},
        // Valid normal case
        {10, 20, 100, "valid_normal_case"},
        // Underflow case: offset > wad->length
        {150, 50, 100, "offset_exceeds_length"},
        // Large buffer_len that would overflow with offset
        {UINT32_MAX - 100, 200, 1000, "near_overflow_boundary"}
    };
    
    int num_cases = sizeof(cases) / sizeof(cases[0]);
    
    for (int i = 0; i < num_cases; i++) {
        // Create a minimal wad_file_t with known data
        wad_file_t wad;
        char *mapped_data = malloc(cases[i].wad_length + 256); // Extra padding for safety
        ck_assert_ptr_nonnull(mapped_data);
        
        // Fill with pattern
        memset(mapped_data, 0xAA, cases[i].wad_length + 256);
        memset(mapped_data + cases[i].wad_length, 0xCC, 256); // Guard bytes
        
        wad.mapped = mapped_data;
        wad.length = cases[i].wad_length;
        
        // Prepare buffer
        char *buffer = malloc(cases[i].buffer_len);
        ck_assert_ptr_nonnull(buffer);
        memset(buffer, 0xBB, cases[i].buffer_len);
        
        // Call the actual function
        size_t result = W_Read(&wad, cases[i].offset, buffer, cases[i].buffer_len);
        
        // Security property: result must be ≤ wad.length - offset (without underflow)
        // and no bytes beyond wad.length should be read
        if (cases[i].offset < wad.length) {
            size_t max_allowed = wad.length - cases[i].offset;
            ck_assert_msg(result <= max_allowed, 
                         "Case %s: Read %zu bytes, maximum allowed is %zu",
                         cases[i].description, result, max_allowed);
        } else {
            // If offset >= wad.length, must read 0 bytes
            ck_assert_msg(result == 0, 
                         "Case %s: Should read 0 bytes when offset >= length, got %zu",
                         cases[i].description, result);
        }
        
        // Verify guard bytes weren't touched (no buffer overflow)
        for (int j = 0; j < 256; j++) {
            ck_assert_msg(mapped_data[wad.length + j] == 0xCC,
                         "Case %s: Guard byte at offset %d corrupted (buffer overflow)",
                         cases[i].description, j);
        }
        
        free(buffer);
        free(mapped_data);
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_w_read_bounds_check_invariant);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}