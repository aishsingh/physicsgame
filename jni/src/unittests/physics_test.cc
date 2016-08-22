#include <gtest/gtest.h>
#include "../physics.h"
#include "../motion.h"


TEST(calcMotion, TimeZero) {
    float elapsed_time = 0;
    Motion m = Motion();

    Motion r = Physics::calcMotion(m, elapsed_time);

    EXPECT_EQ(r.getTime(), 0.0f);
    EXPECT_EQ(r.getDisp(), 0.0f);
    EXPECT_EQ(r.getVel(), 0.0f);
    EXPECT_EQ(r.getAccel(), 0.0f);
}

TEST(calcMotion, Time1) {
    float elapsed_time = 50;
    Motion m = Motion();
    m.setTime(50.0f);
    m.setDisp(0.0f);
    m.setVel(0.0f);
    m.setAccel(0.0f);

    Motion r = Physics::calcMotion(m, elapsed_time);

    EXPECT_EQ(r.getTime(), 50.0f);
    EXPECT_EQ(r.getDisp(), 0.0f);
    EXPECT_EQ(r.getVel(), 0.0f);
    EXPECT_EQ(r.getAccel(), 0.0f);
}

TEST(splitValueFromAngleTest, AngleZero) {
    float hori = 0;
    float vert = 0;

    Physics::splitValueFromAngle(50, 0, &hori, &vert);

    EXPECT_EQ(hori, 0);
    EXPECT_EQ(vert, 50);
}
