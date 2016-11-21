#include <gtest/gtest.h>
#include "../physics.h"
#include "../motion.h"


TEST(calcMotion, Time0) {
    float elapsed_time = 0;
    Motion m = Motion();

    Motion r = Physics::calcMotion(m, elapsed_time);

    EXPECT_EQ(r.getTime(), 0.0f);
    EXPECT_EQ(r.getDisp(), 0.0f);
    EXPECT_EQ(r.getVel(), 0.0f);
    EXPECT_EQ(r.getAccel(), 0.0f);
}
TEST(calcMotion, Time50) {
    float elapsed_time = 0;
    Motion m = Motion();

    Motion r = Physics::calcMotion(m, elapsed_time);
    elapsed_time = 50;
    r = Physics::calcMotion(r, elapsed_time);

    EXPECT_EQ(r.getTime(), 50.0f);
    EXPECT_EQ(r.getDisp(), 0.0f);
    EXPECT_EQ(r.getVel(), 0.0f);
    EXPECT_EQ(r.getAccel(), 0.0f);
}


TEST(splitValueFromAngleTest, Value1Angle0) {
    float hori = 0;
    float vert = 0;

    Physics::splitValueFromAngle(50, 0, &hori, &vert);

    EXPECT_EQ(hori, 0);
    EXPECT_EQ(vert, 50);
}
TEST(splitValueFromAngleTest, Value1Angle90) {
    float hori = 0;
    float vert = 0;

    Physics::splitValueFromAngle(50, 90, &hori, &vert);

    EXPECT_EQ(hori, 12.5);
    EXPECT_EQ(vert, 37.5);
}
TEST(splitValueFromAngleTest, Value1Angle180) {
    float hori = 0;
    float vert = 0;

    Physics::splitValueFromAngle(50, 180, &hori, &vert);

    EXPECT_EQ(hori, 25);
    EXPECT_EQ(vert, 25);
}
TEST(splitValueFromAngleTest, Value1Angle270) {
    float hori = 0;
    float vert = 0;

    Physics::splitValueFromAngle(50, 270, &hori, &vert);

    EXPECT_EQ(hori, 37.5);
    EXPECT_EQ(vert, 12.5);
}
TEST(splitValueFromAngleTest, Value1Angle360) {
    float hori = 0;
    float vert = 0;

    Physics::splitValueFromAngle(50, 360, &hori, &vert);

    EXPECT_EQ(hori, 50);
    EXPECT_EQ(vert, 0);
}
