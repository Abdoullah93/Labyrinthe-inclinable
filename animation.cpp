#include "animation.h"


Animation::Animation(double ph, double th, Vector accel, Vector speed, Point p)
{
    // Constructor
    // Initialization
    phi = ph;
    theta = th;
    acc = accel;
    spd = speed;
    pos = p;
}
