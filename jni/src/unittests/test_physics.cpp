#include "test_physics.h"
#include "../log.h"

bool TestPhysicsEngine::runTests() {
    int error_count = testSplitValueFromAngle();
    if (error_count) { 
        LOGI("   (%i) tests failed", error_count);
        return true;
    }
    else {
        LOGI("   [done]");
        return false;
    }
}

int TestPhysicsEngine::testSplitValueFromAngle() {
    int error_count = 0;
    // Test 1
    float hori = 0;
    float vert = 0;
    PhysicsEngine::splitValueFromAngle(50, 0, &hori, &vert);

    if (hori == 25 && vert == 25)
        LOGI("   [PASSED]: testSplitValueFromAngle[1]");
    else {
        error_count++;
        LOGI("   [FAILED]: testSplitValueFromAngle[1]");
    }

    // Test 2
    hori = 0;
    vert = 0;
    PhysicsEngine::splitValueFromAngle(50, 90, &hori, &vert);

    if (hori == 25 && vert == 25)
        LOGI("   [PASSED]: testSplitValueFromAngle[2]");
    else {
        error_count++;
        LOGI("   [FAILED]: testSplitValueFromAngle[2]");
    }

    return error_count;
}

/* vim: set ts=4 */
