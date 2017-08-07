/*
 * main.cpp
 */

#include "libpfc/include/libpfc.h"

#include <string>
#include <chrono>
#include <x86intrin.h>
#include <stdio.h>
#include <stdexcept>


#define CALIBRATION_LOOPS (300 * 1000 * 1000)
#define CPU_W "%4"
#define REF_W "%8"
#define TSC_W "%8"
#define MHZ_W "%8"


int64_t nanos() {
    auto t = std::chrono::high_resolution_clock::now();
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
}

void busy_loop(uint64_t iters) {
    volatile int sink;
    do {
        sink = 0;
    } while (--iters > 0);
    (void)sink;
}


static void run_test() {
    auto err = pfcInit();
    if (err) {
        const char* msg = pfcErrorString(err);
        throw std::runtime_error(std::string("pfcInit() failed (error ") + std::to_string(err) + ": " + msg + ")");
    }

    err = pfcPinThread(0);
    if (err) {
        // let's treat this as non-fatal, it could occur if, for example
        printf("WARNING: Pinning to CPU 0 failed, continuing without pinning\n");
    } else {
        printf("Pinned to CPU 0\n");
    }

    // just use the fixed counters for now
    PFC_CFG  cfg[7] = {2,2,2};

    err = pfcWrCfgs(0, sizeof(cfg)/sizeof(cfg[0]), cfg);
    if (err) {
        const char* msg = pfcErrorString(err);
        throw std::runtime_error(std::string("pfcWrCfgs() failed (error ") + std::to_string(err) + ": " + msg + ")");
    }

    printf(CPU_W "s" REF_W "s" TSC_W "s" MHZ_W "s\n", "CPU#", "REF_TSC", "rdtsc", "Eff Mhz");

    /* calculate CPU frequency using reference cycles */
    for (int i = 0; i < 100; i++) {
        PFC_CNT cnt[7] = {};

        int64_t start = nanos();
        PFCSTART(cnt);
        int64_t tsc =__rdtsc();
        busy_loop(CALIBRATION_LOOPS);
        PFCEND(cnt);
        int64_t tsc_delta   = __rdtsc() - tsc;
        int64_t nanos_delta = nanos() - start;

        printf(CPU_W "d" REF_W ".2f" TSC_W ".2f" MHZ_W ".2f\n",
                sched_getcpu(),
                1000.0 * cnt[PFC_FIXEDCNT_CPU_CLK_REF_TSC] / nanos_delta,
                1000.0 * tsc_delta / nanos_delta,
                1000.0 * CALIBRATION_LOOPS / nanos_delta);
    }
}

int main(int argc, char **argv) {
	run_test();
	return 0;
}


