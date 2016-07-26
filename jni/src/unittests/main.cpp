#include <stdio.h>
#include "test_physics.h"

int totalErrors = 0;

int main() {
    printf("======================================");
    printf("Starting TestPhysicsEngine");
    TestPhysicsEngine physics = TestPhysicsEngine();
    physics.runTests();
    printf("======================================");
    printf("(%d) Errors.", totalErrors);

    return 0;
}
/* vim: set ts=4 */
