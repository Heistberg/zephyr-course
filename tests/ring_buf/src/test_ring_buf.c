/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	int value = 2;

	zassume_ok(rb_push(value), "New buffer must accept new values");
	zassume_false(rb_is_empty(), "Used buffer must not be empty");
	zassert_ok(rb_init(value), "Buffer must initialize as many times as needed");
	zassert_true(rb_is_empty(), "New buffer must be empty");
	zassert_equal(rb_count(), 0, "New buffer must be emtpy");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int value_push = 42;
	int value_pop = 0;

	zassert_ok(rb_push(value_push), "New buffer must accept new values");
	zassert_ok(rb_pop(&value_pop), "Buffer with values present must pop values");
	zassert_equal(value_push, value_pop, "Buffer must preserve values");
	zassert_true(rb_is_empty(), "After popping all values buffer must be empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int val1 = 15;
	int val2 = 37;
	int val3 = 88;
	int value_pop = 0;

	zassert_ok(rb_push(val1), "Non full buffer must accept new values");
	zassert_ok(rb_push(val2), "Non full buffer must accept new values");
	zassert_ok(rb_push(val3), "Non full buffer must accept new values");
	zassert_ok(rb_pop(&value_pop), "Non empty buffer must pop values");
	zassert_equal(val1, value_pop, "Buffer must pop values in FIFO order");
	zassert_ok(rb_pop(&value_pop), "Non empty buffer must pop values");
	zassert_equal(val2, value_pop, "Buffer must pop values in FIFO order");
	zassert_ok(rb_pop(&value_pop), "Non empty buffer must pop values");
	zassert_equal(val3, value_pop, "Buffer must pop values in FIFO order");
	zassert_true(rb_is_empty(), "After popping all values the buffer must be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	zassume_ok(rb_push(1), "Non full buffer must accept new values");	
	zassume_ok(rb_push(2), "Non full buffer must accept new values");	
	zassume_ok(rb_push(3), "Non full buffer must accept new values");	
	zassume_ok(rb_push(4), "Non full buffer must accept new values");
	zassume_true(rb_is_full(), "Full buffer must return is_full to TRUE");
	zassert_equal(rb_push(5), -ENOSPC, "Full buffer must return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	int val_push = 7;
	int val_peek = 0;

	zassume_ok(rb_push(val_push), "Non full buffer must accept new values");
	zassert_ok(rb_peek(&val_peek), "Non empty buffer must peek values");
	zassert_equal(val_push, val_peek, "Peek must return the oldest value");
	val_peek = 0;
	zassert_ok(rb_peek(&val_peek), "Non empty buffer must peek values");
	zassert_equal(val_push, val_peek, "Peek must return the oldest value");
	zassert_equal(rb_count(), 1, "Peek must not consume");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	zassert_equal(rb_pop(NULL), -EINVAL, "Passing a null pointer must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	zassume_ok(rb_push(1), "Non full buffer must accept new values");	
	zassume_ok(rb_push(2), "Non full buffer must accept new values");	
	zassume_ok(rb_push(3), "Non full buffer must accept new values");	
	zassume_ok(rb_push(4), "Non full buffer must accept new values");
	zassert_true(rb_is_full(), "Full buffer must return TRUE");
	zassert_equal(rb_count(), 4, "Full buffer must return count 4");
}
