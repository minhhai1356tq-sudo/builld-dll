#pragma once
#include <string>
#include "Internal/Cheat ff/Math/Entity.h"
#ifndef BOOL3_H
#define BOOL3_H
enum class XPose { Standing = 0, Crouching = 1, Dashing = 2, Creeping = 3, Jumping = 11, Knocked = 8 };
class Player
{
public:
    using Bool3 = ::Bool3;
    bool IsBot;
    bool IsKnown;
    bool IsDead;
    bool IsKnocked;
    bool IsVisible;
    uint64_t ID;
    int Level;
    int CurrentShield;
    float Distance;
    short Health;
    short WeaponID;
    XPose Pose;
    Bool3 IsTeam;
    uint32_t Address;
    std::string Name;
    Vector3 Spine;
    Vector3 LeftCalf;
    Vector3 RightCalf;
    Vector3 LeftWristJoint;
    Vector3 RightWristJoint;
    Vector3 LeftHand;
    Vector3 Head;
    Vector3 Neck;
    Vector3 RightShoulder;
    Vector3 LeftShoulder;
    Vector3 RightElbow;
    Vector3 LeftElbow;
    Vector3 RightWrist;
    Vector3 LeftWrist;
    Vector3 Hip;
    Vector3 RightAnkle;
    Vector3 LeftAnkle;
    Vector3 RightHand;
    Vector3 LeftFoot;
    Vector3 RightFoot;
    Vector3 Root;
    Vector3 Velocity;
    Vector3 LastRootPosition;
};

#endif
