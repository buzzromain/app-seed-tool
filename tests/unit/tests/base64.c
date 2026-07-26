#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>

extern uint8_t base64_encode_64bytes(const uint8_t* src, char* dst);

const uint8_t base64_input[] = {
    0x74, 0xa2, 0xe8, 0x7a, 0x9b, 0xa0, 0xcd, 0xd5, 
    0x49, 0xbd, 0xd2, 0xf9, 0xea, 0x88, 0x0d, 0x55, 
    0x4c, 0x6c, 0x35, 0x5b, 0x08, 0xed, 0x25, 0x08, 
    0x8c, 0xfa, 0x88, 0xf3, 0xf1, 0xc4, 0xf7, 0x46, 
    0x32, 0xb6, 0x52, 0xfd, 0x4a, 0x8f, 0x5f, 0xda, 
    0x43, 0x07, 0x4c, 0x6f, 0x69, 0x64, 0xa3, 0x75, 
    0x3b, 0x08, 0xbb, 0x52, 0x10, 0xc8, 0xf5, 0xe7, 
    0x5c, 0x07, 0xa4, 0xc2, 0xa2, 0x0b, 0xf6, 0xe9
};

const char* encoded_string =
    "dKLoepugzdVJvdL56ogNVUxsNVsI7SUIjPqI8/HE90YytlL9So9f2kMHTG9pZKN1Owi7UhDI9edcB6TCogv26Q==";

static void test_base64(void **state) {
    (void) state;

    char result[88] = {0};
    uint8_t return_num;

    // base64_encode_64bytes() must read exactly the 64 bytes it is given.
    // Copy the input into a larger buffer whose trailing bytes are non-zero, so
    // that any read past the 64-byte boundary changes the output rather than
    // matching the expected string by luck.
    uint8_t guarded_input[sizeof(base64_input) + 2];
    memcpy(guarded_input, base64_input, sizeof(base64_input));
    guarded_input[sizeof(base64_input)] = 0xA5;
    guarded_input[sizeof(base64_input) + 1] = 0x5A;

    return_num = base64_encode_64bytes(guarded_input, result);

    assert_int_equal(return_num, 88);
    // encoded_string is a pointer: sizeof() would compare only 8 bytes.
    assert_memory_equal(result, encoded_string, strlen(encoded_string));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_base64)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
