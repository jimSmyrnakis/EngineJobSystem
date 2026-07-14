#include <gtest/gtest.h>
#include <JobSystem/fiber/fiber.h>

struct YieldState {
    int count = 0;
    int resumed = 0;
};

static void simple_proc(fiber* fib, void* arg) {
    (void)fib;
    int* p = static_cast<int*>(arg);
    (*p)++;
    // try reset should not be allowed from running fiber
    EXPECT_EQ(__FIBER_ERR_INVLD, fiber_reset(fib, simple_proc, arg));
}

static void yield_proc(fiber* fib, void* arg) {
    (void)fib;
    YieldState* state = static_cast<YieldState*>(arg);
    state->count++;
    fiber_yield();
    state->count++;
    state->resumed = 1;
}

static void reset_proc(fiber* fib, void* arg) {
    (void)fib;
    int* p = static_cast<int*>(arg);
    (*p) += 10;
}

static void fini_proc(fiber* fib, void* arg) {
    int* out = static_cast<int*>(arg);
    *out = fiber_fini(fib);
}

TEST(FiberApiIntegration, InvalidArguments) {
    fiber f;
    int counter = 0;

    EXPECT_EQ(__FIBER_ERR_BARG, fiber_init(nullptr, 4096, simple_proc, &counter));
    EXPECT_EQ(__FIBER_ERR_BARG, fiber_init(&f, 4096, nullptr, &counter));
    EXPECT_EQ(__FIBER_ERR_BARG, fiber_fini(nullptr));
}

TEST(FiberApiIntegration, BasicLifecycle) {
    fiber f;
    int counter = 0;

    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 8192, simple_proc, &counter));
    EXPECT_EQ(FIBER_READY, fiber_get_state(&f));
    EXPECT_NE(0u, fiber_get_id(&f));
    EXPECT_NE(fiber_current(), &f);

    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(1, counter);
    EXPECT_EQ(FIBER_FINISHED, fiber_get_state(&f));
    EXPECT_NE(nullptr, fiber_current());

    EXPECT_EQ(__FIBER_ERR_OK, fiber_fini(&f));
}

TEST(FiberApiIntegration, ResetAndReuse) {
    fiber f;
    int counter = 0;

    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 8192, simple_proc, &counter));
    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(1, counter);

    EXPECT_EQ(__FIBER_ERR_OK, fiber_reset(&f, reset_proc, &counter));
    EXPECT_EQ(FIBER_READY, fiber_get_state(&f));

    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(11, counter);

    EXPECT_EQ(__FIBER_ERR_OK, fiber_fini(&f));
}

TEST(FiberApiIntegration, YieldAndResume) {
    fiber f;
    YieldState state;

    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 8192, yield_proc, &state));

    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(1, state.count);
    EXPECT_EQ(0, state.resumed);

    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(2, state.count);
    EXPECT_EQ(1, state.resumed);
    EXPECT_EQ(FIBER_FINISHED, fiber_get_state(&f));

    EXPECT_EQ(__FIBER_ERR_OK, fiber_fini(&f));
}

TEST(FiberApiIntegration, FinalizeFromRunningFiber) {
    fiber f;
    int finish_result = __FIBER_ERR_OK;

    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 8192, fini_proc, &finish_result));
    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(__FIBER_ERR_INVLD, finish_result);
    EXPECT_EQ(FIBER_FINISHED, fiber_get_state(&f));

    EXPECT_EQ(__FIBER_ERR_OK, fiber_fini(&f));
}

TEST(FiberApiIntegration, FinalizeFromUnitilializedFiber) {
    fiber f;
    int finish_result = __FIBER_ERR_OK;

    EXPECT_EQ(__FIBER_ERR_FNIN, fiber_fini(&f));

}

TEST(FiberApiIntegration, BadFiberInit) {
    fiber f;
    int finish_result = __FIBER_ERR_OK;

    EXPECT_EQ(__FIBER_ERR_TSSS, fiber_init(&f, 16, simple_proc, &finish_result));
    // check now for the minimum of 1024 size stack
    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 1024, simple_proc, &finish_result));
    // check now for giving null routine
    EXPECT_EQ(__FIBER_ERR_BARG, fiber_init(&f, 1024, nullptr, &finish_result));
    // check now for giving null fiber
    EXPECT_EQ(__FIBER_ERR_BARG, fiber_init(nullptr, 1024, simple_proc, &finish_result));
}

TEST(FiberApiIntegration, BadFiberReset) {
    fiber f;
    int finish_result = __FIBER_ERR_OK;
    
    // init the fiber first
    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 1024, simple_proc, &finish_result));
    // check now for giving null routine
    EXPECT_EQ(__FIBER_ERR_BARG, fiber_reset(&f, nullptr, &finish_result));
    // check now for giving null fiber
    EXPECT_EQ(__FIBER_ERR_BARG, fiber_reset(nullptr, simple_proc, &finish_result));
    // check now for giving finilized fiber
    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(&f));
    EXPECT_EQ(fiber_state::FIBER_FINISHED , fiber_get_state(&f));
    EXPECT_EQ(__FIBER_ERR_OK, fiber_reset(&f, simple_proc, &finish_result));
    // check now for giving finished fiber
    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(nullptr));
    EXPECT_EQ(__FIBER_ERR_OK, fiber_reset(&f, simple_proc, &finish_result));
    // check now for giving ready fiber

    
}


TEST(FiberApiIntegration, CurrentAndIdChecks) {
    fiber f;
    int counter = 0;

    EXPECT_EQ(0u, fiber_get_id(nullptr));
    EXPECT_EQ(0, fiber_get_state(nullptr));
    EXPECT_NE(nullptr, fiber_current());

    EXPECT_EQ(__FIBER_ERR_OK, fiber_init(&f, 8192, simple_proc, &counter));
    EXPECT_NE(&f, fiber_current());

    EXPECT_EQ(__FIBER_ERR_OK, fiber_switch(nullptr));
}


