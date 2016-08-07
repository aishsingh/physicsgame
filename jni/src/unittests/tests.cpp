#include "tests.h"
#include "../log.h"
#include "test_physics.h"


void runTests() {
    bool error = false;
    LOGI("======================================");
    LOGI("Running TestPhysicsEngine");
    TestPhysicsEngine physics = TestPhysicsEngine();
    error = physics.runTests();

    // Next test here
    
    if (error)
        LOGI("--> Tests failed.");
    else
        LOGI("--> Tests complete.");
    LOGI("======================================");
}
