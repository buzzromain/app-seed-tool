/*
 * Regression test for the missing bounds in sskr_combine_shards_internal().
 *
 * The function sorts incoming shards into
 *
 *     sskr_group_t groups[SSKR_MAX_GROUP_COUNT];
 *
 * and appends members with
 *
 *     groups[j].member_index[groups[j].count++] = ...
 *
 * Upstream bc-sskr sizes that array for its own maximum of 16 groups. The
 * Ledger port reduced SSKR_MAX_GROUP_COUNT to 1 to fit in RAM but did not
 * propagate the constraint into the validation, so a shard set describing more
 * groups than the array holds writes past its end.
 *
 * The shards used here are legitimate: bc-sskr happily produces multi-group
 * sets, and each shard below is individually well-formed. Only this build's
 * reduced capacity makes them unsupported, which is exactly what the function
 * must say instead of overflowing.
 *
 * The assertion is on the exact error code rather than on a negative return.
 * Without the fix the write past groups[0] still happens and the call still
 * comes back negative -- SSKR_ERROR_NOT_ENOUGH_MEMBER_SHARDS (-11) for these
 * shards -- raised further downstream, once the corrupted state reaches
 * recovery. A test that only checked `result < 0` would pass both before and
 * after the fix, and so would protect nothing.
 *
 * The bip85 branch already carries both bounds; this is the develop side.
 */

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "sskr.h"
#include "sss-constants.h"
#include "testutils.h"

#define SHARD_VALUE_LEN (16)
#define SHARD_LEN       (SSKR_METADATA_LENGTH_BYTES + SHARD_VALUE_LEN)

/*
 * Builds one serialized shard. group_spec is the packed
 * (group_threshold - 1, group_count - 1) byte; group_index selects which group
 * the shard joins and shares its byte with member_threshold - 1.
 */
static void build_shard(uint8_t out[SHARD_LEN],
                        uint8_t group_spec,
                        uint8_t group_index,
                        uint8_t member_threshold,
                        uint8_t member_index) {
    out[0] = 0xAB;  // identifier, shared across the whole backup
    out[1] = 0xCD;
    out[2] = group_spec;
    out[3] = (uint8_t) ((group_index << 4) | ((member_threshold - 1) & 0x0F));
    out[4] = member_index;  // reserved nibble stays zero
    memset(&out[SSKR_METADATA_LENGTH_BYTES], 0x42 + member_index, SHARD_VALUE_LEN);
}

/*
 * Two shards of a two-group set, while this build has room for
 * SSKR_MAX_GROUP_COUNT of them. The second must be refused before
 * groups[next_group] is written.
 */
static void test_combine_rejects_more_groups_than_supported(void **state) {
    (void) state;

    uint8_t raw[2][SHARD_LEN];
    const uint8_t *shards[2];
    uint8_t output[32];

    for (uint8_t i = 0; i < 2; i++) {
        build_shard(raw[i], 0x11, i, 1, 0);  // 2-of-2 groups, one member each
        shards[i] = raw[i];
    }

    int16_t result = sskr_combine_shards(shards, SHARD_LEN, 2, output, sizeof(output));

    assert_int_equal(result, SSKR_ERROR_INVALID_SHARD_SET);
}

/*
 * Guards the bound against being too tight: a single group is what this build
 * supports, so it must not be turned away as an invalid shard set. These are
 * synthetic values rather than a recoverable secret, so the call still fails
 * further down -- assert only that it is not rejected here.
 */
static void test_combine_accepts_a_single_group(void **state) {
    (void) state;

    uint8_t raw[2][SHARD_LEN];
    const uint8_t *shards[2];
    uint8_t output[32];

    for (uint8_t i = 0; i < 2; i++) {
        build_shard(raw[i], 0x00, 0, 2, i);  // one 1-of-1 group, 2 members
        shards[i] = raw[i];
    }

    int16_t result = sskr_combine_shards(shards, SHARD_LEN, 2, output, sizeof(output));

    assert_int_not_equal(result, SSKR_ERROR_INVALID_SHARD_SET);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_combine_rejects_more_groups_than_supported),
        cmocka_unit_test(test_combine_accepts_a_single_group),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
