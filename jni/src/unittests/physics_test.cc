#include <gtest/gtest.h>
#include "../physics.h"

TEST(SplitValueFromAngleTest,AngleZero) {
    float hori = 0;
    float vert = 0;
    PhysicsEngine::splitValueFromAngle(50, 0, &hori, &vert);
    EXPECT_EQ(0, hori);
    EXPECT_EQ(50, vert);
}
