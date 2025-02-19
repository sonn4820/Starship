#pragma  once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/DoubleVec2.hpp"
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/DoubleVec4.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/IntVec3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/DoubleAABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/DoubleOBB3.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/Capsule3.hpp"
#include "Engine/Math/DoubleCapsule3.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/LineSegment3.hpp"
#include "Engine/Math/DoubleLineSegment3.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/DoubleRange.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/DoublePlane3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/DoubleMat44.hpp"
#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/DoubleQuaternion.hpp"
#include "ThirdParty/SquirrelNoise/RawNoise.hpp"
#include "ThirdParty/SquirrelNoise/SmoothNoise.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <math.h>


// Forward Declaration and Const
constexpr float PI = 3.14159265359f;
struct Mat44;
struct Quaternion;

enum class BilboardType
{
	NONE = -1,
	WORLD_UP_CAMERA_FACING,
	WORLD_UP_CAMERA_OPPOSING,
	FULL_CAMERA_FACING,
	FULL_CAMERA_OPPOSING,
	COUNT
};

struct CollisionInfo
{
	bool isColliding = false;
	DoubleVec3 contactPoint;
	DoubleVec3 normal;
};

//Angle Utilities
float ConvertDegreesToRadians(float degrees);
float ConvertRadiansToDegrees(float radians);
double ConvertDegreesToRadiansDouble(double degrees);
double ConvertRadiansToDegreesDouble(double radians);
float CosDegrees(float degrees);
float SinDegrees(float degrees);
float Atan2Degrees(float y, float x);
double CosDegreesDouble(double degrees);
double SinDegreesDouble(double degrees);
double Atan2DegreesDouble(double y, double x);

float GetShortestAngularDispDegrees(float startDegrees, float endDegrees);
float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees);
float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b);
float GetAngleDegreesBetweenVectors3D(Vec3 const& a, Vec3 const& b);
float GetNormalizedAngle(float angle);

//Basic 2D and 3D Utilities
float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB);
float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB);
float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB);
float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB);
double GetDistanceSquared3D_Double(DoubleVec3 const& positionA, DoubleVec3 const& positionB);
float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB);
float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB);
int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB);
float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto);
Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto);
Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto);
DoubleVec3 const GetProjectedOnto3D_Double(DoubleVec3 const& vectorToProject, DoubleVec3 const& vectorToProjectOnto);

//Geometric Query Utilities
bool IsPointInsideZHexagon3D(Vec3 const& point, Vec3 const& center, float radius);
void GetAllPointsZHexagon3D(Vec3* out_Points, Vec3 const& center, float radius);

bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius);
bool IsPointInsidePolygon2D(Vec2 const& point, std::vector<Vec2> polygon);
bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box);
bool IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule);
bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& oriendtedBox);
bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);

bool DoDiscsOverlap2D(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB);
bool DoAABBsOverlap2D(AABB2 const& boxA, AABB2 const& boxB);
bool DoCapsuleAndPlaneOverlap2D(Capsule2 const& capsule, Vec2 planeStart, Vec2 planeEnd);

bool PushCapsuleOutOfPoint2D(Vec2 point, Capsule2& capsule);
bool PushCapsuleOutOfPlane2D(Vec2 planeStart, Vec2 planeEnd, Capsule2& capsule);

bool IsPointInsideAABB3D(Vec3 const& point, AABB3 const& box);
bool IsPointInsideCapsule3D(Vec3 const& point, Capsule3 const& capsule);
bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float radius);
bool IsPointInsideZCylinder3D(Vec3 const& point, Vec2 const& cylCenterXY, FloatRange cylMinMaxZ, float radius);

bool IsPointInsideAABB3D_Double(DoubleVec3 const& point, DoubleAABB3 const& box);
bool IsPointInsideSphere3D_Double(DoubleVec3 const& point, DoubleVec3 const& sphereCenter, double radius);
bool IsPointInsideCapsule3D_Double(DoubleVec3 const& point, DoubleCapsule3 const& capsule);

bool DoAABBsOverlap3D(AABB3 const& boxA, AABB3 const& boxB);
bool DoAABBsOverlap3D_Double(DoubleAABB3 const& boxA, DoubleAABB3 const& boxB);
bool IsAABBInside(DoubleAABB3 const& parent, DoubleAABB3 const& check);
bool DoAABBAndPlaneOverlap3D(AABB3 const& box, Plane3 const& plane);
bool DoSpheresOverlap3D(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);
bool DoSphereAndAABBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, AABB3 const& box);
bool DoSphereAndOBBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, OBB3 const& box);
bool DoSphereAndPlaneOverlap3D(Vec3 const& center, float radius, Plane3 const& plane);
bool DoZCylindersOverlap3D(Vec2 const& cylinderAcenterXY, float cylinderARadius, FloatRange const& cylinderAMinMaxZ, Vec2 const& cylinderBcenterXY, float cylinderBRadius, FloatRange const& cylinderBMinMaxZ);
bool DoZCylinderAndAABBOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange const& cylinderMinMaxZ, AABB3 const& box);
bool DoZCylinderAndSphereOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange const& cylinderMinMaxZ, Vec3 const& sphereCenter, float sphereRadius);
bool DoZCylinderAndPlaneOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange const& cylinderMinMaxZ, Plane3 const& plane);
bool DoOBBsOverlap3D(OBB3 const& boxA, OBB3 const& boxB);
bool DoOBBAndPlaneOverlap3D(OBB3 const& box, Plane3 const& plane);
bool DoOBBAndAABBOverlap3D(OBB3 const& obb, AABB3 const& aabb);
bool DoCapsuleAndAABBOverlap3D(Capsule3 const& capsule, AABB3 const& box);
bool DoCapsuleAndOBBOverlap3D(Capsule3 const& capsule, OBB3 const& obb);
bool DoCapsuleAndPlaneOverlap3D(Capsule3 const& capsule, Plane3 const& plane);
bool DoCapsuleAndSphereOverlap3D(Capsule3 const& capsule, Vec3 const& sphereCenter, float sphereRadius);
bool DoCapsulesOverlap3D(Capsule3 const& capsuleA, Capsule3 const& capsuleB);

bool DoSpheresOverlap3D_Double(DoubleVec3 const& centerA, double radiusA, DoubleVec3 const& centerB, double radiusB);
bool DoSphereAndAABBOverlap3D_Double(DoubleVec3 const& sphereCenter, double sphereRadius, DoubleAABB3 const& box);
bool DoSphereAndPlaneOverlap3D_Double(DoubleVec3 const& center, double radius, DoublePlane3 const& plane);
bool DoCapsuleAndSphereOverlap3D_Double(DoubleCapsule3 const& capsule, DoubleVec3 const& sphereCenter, double sphereRadius);
bool DoCapsulesOverlap3D_Double(DoubleCapsule3 const& capsule, DoubleCapsule3 const& otherCapsule);
bool DoSphereAndOBBOverlap3D_Double(DoubleVec3 const& sphereCenter, double sphereRadius, DoubleOBB3 const& box);
bool DoCapsuleAndPlaneOverlap3D_Double(DoubleCapsule3 const& capsule, DoublePlane3 const& plane);

CollisionInfo DoCapsuleAndAABBOverlap3D_Info(DoubleCapsule3 const& capsule, DoubleAABB3 const& box);
CollisionInfo DoCapsuleAndOBBOverlap3D_Info(DoubleCapsule3 const& capsule, DoubleOBB3 const& box);
CollisionInfo DoCapsuleAndSphereOverlap3D_Info(DoubleCapsule3 const& capsule, DoubleVec3 const& center, double radius);
CollisionInfo DoCapsulesOverlap3D_Info(DoubleCapsule3 const& capsuleA, DoubleCapsule3 const& capsuleB);
CollisionInfo DoSpheresOverlap3D_Info(DoubleVec3 const& centerA, double radiusA, DoubleVec3 const& centerB, double radiusB);

Vec2 GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius);
Vec2 GetNearestPointOnAABB2D(Vec2 const& referencePosition, AABB2 const& box);
Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& referencePosition, LineSegment2 const& infiniteLine);
Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& referencePosition, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine);
Vec2 GetNearestPointOnLineSegment2D(Vec2 const& referencePosition, LineSegment2 const& lineSegment);
Vec2 GetNearestPointOnLineSegment2D(Vec2 const& referencePosition, Vec2 const& lineSegStart, Vec2 const& lineSegEnd);
Vec2 GetNearestPointOnCapsule2D(Vec2 const& referencePosition, Capsule2 const& capsule);
Vec2 GetNearestPointOnCapsule2D(Vec2 const& referencePosition, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
Vec2 GetNearestPointOnOBB2D(Vec2 const& referencePosition, OBB2 const& orientedBox);

Vec3 GetNearestPointOnAABB3D(Vec3 const& referencePosition, AABB3 const& box);
Vec3 GetNearestPointOnCapsule3D(Vec3 const& referencePosition, Capsule3 const& capsule);
Vec3 GetNearestPointOnOBB3D(Vec3 const& referencePosition, OBB3 const& box);
Vec3 GetNearestPointOnSphere3D(Vec3 const& referencePosition, Vec3 const& center, float radius);
Vec3 GetNearestPointOnZCynlinder3D(Vec3 const& referencePosition, Vec2 const& centerXY, FloatRange minMaxZ, float radius);
Vec3 GetNearestPointOnLineSegment3D(Vec3 start, Vec3 end, Vec3 point);
Vec3 GetNearestPointOnLineSegment3D(LineSegment3 line, Vec3 point);
std::vector<Vec3> GetNearestPointsBetweenLines3D(LineSegment3 const& lineA, LineSegment3 const& lineB);

DoubleVec3 GetNearestPointOnAABB3D_Double(DoubleVec3 const& referencePosition, DoubleAABB3 const& box);
DoubleVec3 GetNearestPointOnCapsule3D_Double(DoubleVec3 const& referencePosition, DoubleCapsule3 const& capsule);
DoubleVec3 GetNearestPointOnOBB3D_Double(DoubleVec3 const& referencePosition, DoubleOBB3 const& box);
DoubleVec3 GetNearestPointOnSphere3D_Double(DoubleVec3 const& referencePosition, DoubleVec3 const& center, double radius);
DoubleVec3 GetNearestPointOnLineSegment3D_Double(DoubleVec3 start, DoubleVec3 end, DoubleVec3 point);
DoubleVec3 GetNearestPointOnLineSegment3D_Double(DoubleLineSegment3 line, DoubleVec3 point);
std::vector<DoubleVec3> GetNearestPointsBetweenLines3D_Double(DoubleLineSegment3 const& lineA, DoubleLineSegment3 const& lineB);
void Optimized_GetNearestPointsBetweenLines3D_Double(DoubleVec3* out_points, DoubleLineSegment3 const& lineA, DoubleLineSegment3 const& lineB);

bool PushDiscOutOfPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint);
bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius);
bool PushDiscOutOfAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox);

bool PushZCylinderOutOfPoint3D(Vec2& mobileCylXYCenter, FloatRange& cylMinMaxZ, float cylRadius, Vec3 const& fixedPoint);
bool PushZCylinderOutOfSphere3D(Vec2& mobileCylXYCenter, FloatRange& cylMinMaxZ, float cylRadius, Vec3 const& fixedDiscCenter, float fixedDiscRadius);
bool PushZCylindersOutOfZCylinders3D(Vec2& mobileCylCenter, FloatRange& mobileCylMinMaxZ, float mobileCylRadius, Vec2 const& fixedCylCenter, FloatRange const& fixedCylMinMaxZ, float fixedCylRadius);
bool PushZCylindersOutOfEachOther3D(Vec2& cylACenter, FloatRange& cylAMinMaxZ, float cylARadius, Vec2& cylBCenter, FloatRange& cylBMinMaxZ, float cylBRadius);
bool PushZCylinderOutOfAABB3D(Vec2& mobileCylXYCenter, FloatRange& cylMinMaxZ, float cylRadius, AABB3 const& fixedBox);

bool PushSphereOutOfSphere3D(Vec3& posA, float radiusA, Vec3& posB, float radiusB, bool isBStatic);
bool PushSphereOutOfZCylinder3D(Vec3& pos, float radius, Vec2 const& cylCenter, FloatRange const& cylMinMaxZ, float cylRadius);
bool PushSphereOutOfPlane3D(Vec3& center, float radius, Plane3 const& plane);
bool PushSphereOutOfAABB3(Vec3& center, float radius, AABB3 const& box);
bool PushSphereOutOfOBB3(Vec3& center, float radius, OBB3 const& obb);
bool PushSphereOutOfCapsule3(Vec3& center, float radius, Capsule3& capsule, bool isCapsuleStatic);
bool PushSphereOutOfPoint3D(Vec3& center, float radius, Vec3 const& point);

bool PushSphereOutOfPlane3D_Double(DoubleVec3& center, double radius, DoublePlane3 const& plane);
bool PushSphereOutOfAABB3_Double(DoubleVec3& center, double radius, DoubleAABB3 const& box);
bool PushSphereOutOfOBB3_Double(DoubleVec3& center, double radius, DoubleOBB3 const& obb);
bool PushSphereOutOfCapsule3_Double(DoubleVec3& center, double radius, DoubleCapsule3& capsule, bool isCapsuleStatic);
bool PushSphereOutOfPoint3D_Double(DoubleVec3& center, double radius, DoubleVec3 const& point);

bool PushCapsuleOutOfPoint3D(Capsule3& capsule, Vec3 const& point, bool fixLength = true);
bool PushCapsuleOutOfPlane3D(Capsule3& capsule, Plane3 const& plane, bool fixLength = true);
bool PushCapsuleOutOfAABB3D(Capsule3& capsule, AABB3 const& box, bool fixLength = true);
bool PushCapsuleOutOfOBB3D(Capsule3& capsule, OBB3 const& box, bool fixLength = true);
bool PushCapsuleOutOfCapsule3D(Capsule3& capsuleA, Capsule3& capsuleB, bool isBStatic = true);
bool PushCapsuleOutOfSphere3D(Capsule3& capsule, Vec3& sCenter, float sRadius, bool isSphereStatic = true);

bool PushSphereOutOfPoint3D_Double(DoubleVec3& center, double radius, DoubleVec3 const& point);
bool PushCapsuleOutOfPoint3D_Double(DoubleCapsule3& capsule, DoubleVec3 const& point, bool fixLength = true);
bool PushCapsuleOutOfPlane3D_Double(DoubleCapsule3& capsule, DoublePlane3 const& plane, bool fixLength = true);
bool PushCapsuleOutOfAABB3D_Double(DoubleCapsule3& capsule, DoubleAABB3 const& box, bool fixLength = true);
bool PushCapsuleOutOfOBB3D_Double(DoubleCapsule3& capsule, DoubleOBB3 const& box, bool fixLength = true);

bool PushSphereOutOfSphere3D_Double(DoubleVec3& posA, double radiusA, DoubleVec3& posB, double radiusB, bool isBStatic = true);
bool PushCapsuleOutOfCapsule3D_Double(DoubleCapsule3& capsuleA, DoubleCapsule3& capsuleB, bool isBStatic = true);
bool PushCapsuleOutOfSphere3D_Double(DoubleCapsule3& capsule, DoubleVec3& sCenter, double sRadius, bool isSphereStatic = true);

//Transform Utilities
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotaitonDegrees, Vec2 const& translation);
void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float zRotationDegrees, Vec2 const& translation);
void TransformPositionXY3D(Vec3& positionToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);

// Clamp and lerp
float Clamp(float value, float minValue, float maxValue);
Vec3 Clamp(Vec3 value, Vec3 minValue, Vec3 maxValue);
Vec2 Clamp(Vec2 value, Vec2 minValue, Vec2 maxValue);
double Clamp_Double(double value, double minValue, double maxValue);
float ClampZeroToOne(float value);
EulerAngles Clamp(EulerAngles value, EulerAngles minValue, EulerAngles maxValue);
Rgba8 Clamp(Rgba8 value, Rgba8 minValue, Rgba8 maxValue);
float Interpolate(float start, float end, float fraction);
double Interpolate(double start, double end, double fraction);
Vec2 Interpolate(Vec2 start, Vec2 end, float zeroToOne);
Vec3 Interpolate(Vec3 start, Vec3 end, float zeroToOne);
DoubleVec3 Interpolate(DoubleVec3 start, DoubleVec3 end, double zeroToOne);
Rgba8 Interpolate(Rgba8 start, Rgba8 end, float zeroToOne);
float GetFractionWithinRange(float value, float rangeStart, float rangeEnd);
float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd);
float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd);
Vec2 RangeMapClamped(Vec2 inValue, Vec2 inStart, Vec2 inEnd, Vec2 outStart, Vec2 outEnd);
int RoundDownToInt(float value);

//Dot and Cross
float DotProduct2D(Vec2 const& a, Vec2 const& b);
double DotProduct2D_Double(DoubleVec2 const& a, DoubleVec2 const& b);
float DotProduct3D(Vec3 const& a, Vec3 const& b);
double DotProduct3D_Double(DoubleVec3 const& a, DoubleVec3 const& b);
float DotProduct4D(Vec4 const& a, Vec4 const& b);
double DotProduct4D_Double(DoubleVec4 const& a, DoubleVec4 const& b);

float CrossProduct2D(Vec2 A, Vec2 B);
double CrossProduct2D_Double(DoubleVec2 A, DoubleVec2 B);
Vec3 CrossProduct3D(Vec3 A, Vec3 B);
DoubleVec3 CrossProduct3D_Double(DoubleVec3 A, DoubleVec3 B);
DoubleVec3 GetReflected3D_Double(DoubleVec3 orginal, DoubleVec3 normal);

Quaternion FindLookAtRotationQuaternion( Vec3 Start,  Vec3 Target);
DoubleQuaternion FindLookAtRotationQuaternionDouble(DoubleVec3 Start, DoubleVec3 Target);
EulerAngles FindLookAtRotationEulerAngle( Vec3 Start,  Vec3 Target);

//ByteConvertFloat
float NormalizeByte(unsigned char byte);
unsigned char DenormalizeByte(float f);

// Billboard
Mat44 GetBillboardMatrix(BilboardType type, Mat44 const& cameraMatrix, const Vec3& billboardPosition, const Vec2& billboardScale = Vec2(1.f, 1.f));

// Easing
float SmoothStart2(float t);
float SmoothStart3(float t);
float SmoothStart4(float t);
float SmoothStart5(float t);
float SmoothStart6(float t);

float SmoothStop2(float t);
float SmoothStop3(float t);
float SmoothStop4(float t);
float SmoothStop5(float t);
float SmoothStop6(float t);

float SmoothStep3(float t);
float SmoothStep5(float t);

float Hesitate3(float t);
float Hesitate5(float t);

// Compare

bool FloatEqual(float a, float compareTo, float tolerance);
bool DoubleEqual(double a, double compareTo, double tolerance);
float FloatMin(float a, float b);
float FloatMax(float a, float b);
int IntMin(int a, int b);
int IntMax(int a, int b);
double DoubleMin(double a, double b);
double DoubleMax(double a, double b);

// Bezier Curve
float ComputeCubicBezier1D(float t, float p0, float p1, float p2, float p3);
float ComputeQuinticBezier1D(float t, float p0, float p1, float p2, float p3, float p4, float p5);

// Physics
bool BounceDiscOffPoint(Vec2& discPosition, float discRadius, Vec2& discVelocity, float discElasticity, Vec2& pointPosition, float pointElasticity);
bool BounceDiscOffStaticDisc2D(Vec2& mobileDiscPosition, float mobileDiscRadius, Vec2& mobileDiscVelocity, float mobileDiscElasticity, const Vec2& staticDiscPosition, float staticDiscRadius, float staticDiscElasticity);
bool BounceDiscOffEachOther2D(Vec2& discAPosition, float discARadius, Vec2& discAVelocity, float discAElasticity, Vec2& discBPosition, float discBRadius, Vec2& discBVelocity, float discBElasticity);
bool BounceSphereOffPoint(Vec3& spherePosition, float sphereRadius, Vec3& sphereVelocity, float sphereElasticity, Vec3 pointPosition, float pointElasticity, float pointFriction = 0.f);
bool BounceSphereOffStaticSphere3D(Vec3& mobileSpherePosition, float mobileSphereRadius, Vec3& mobileSphereVelocity, float mobileSphereElasticity, const Vec3& staticSpherePosition, float staticSphereRadius, float staticSphereElasticity);
bool BounceSphereOffEachOther3D(Vec3& posA, Vec3& posB, float radiusA, float radiusB, Vec3& velA, Vec3& velB, float elasticiyA, float elasticiyB, float massA, float massB);

bool BounceSphereOffPoint3D_Double(DoubleVec3& spherePosition, double sphereRadius, DoubleVec3& sphereVelocity, double sphereElasticity, DoubleVec3 pointPosition, double pointElasticity, double pointFriction = 0.f);
bool BounceSphereOffStaticSphere3D_Double(DoubleVec3& mobileSpherePosition, double mobileSphereRadius, DoubleVec3& mobileSphereVelocity, double mobileSphereElasticity, const DoubleVec3& staticSpherePosition, double staticSphereRadius, double staticSphereElasticity);

bool BounceCapsuleOffPoint3D_Double(DoubleCapsule3& capsule, DoubleVec3& startPointVel, DoubleVec3& endPointVel, double elasticity, DoubleVec3 point, double pointElasticity, double pointFriction = 0.f, bool fixLength = true);
bool BounceCapsuleOffPlane3D_Double(DoubleCapsule3& capsule, DoubleVec3& startPointVel, DoubleVec3& endPointVel, double elasticity, DoublePlane3 plane, double planeElasticity, double planeFriction = 0.f, bool fixLength = true);

// S.A.T

FloatRange ProjectVertices(std::vector<Vec2>& vertices, Vec2 axis);
std::vector<Vec2> GetAxes(const std::vector<Vec2>& vertices);
bool DoAABBOverlapConvexPoly2D(AABB2 box, ConvexPoly2 convex);
