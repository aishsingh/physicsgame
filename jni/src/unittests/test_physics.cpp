#include <assert.h>
#include "test_physics.h"


void TestPhysicsEngine::testSplitValueFromAngle() {
    // Test 1
    float hori = 0;
    float vert = 0;
    PhysicsEngine::splitValueFromAngle(10, 0, &hori, &vert);

    assert(hori == 5);
    assert(vert == 5);

    // Test 2
    hori = 0;
    vert = 0;
    PhysicsEngine::splitValueFromAngle(50, 0, &hori, &vert);

    assert(hori == 25);
    assert(vert == 25);
}

/* vim: set ts=4 */
