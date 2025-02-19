#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Renderer/Renderer.hpp"

class Camera {
public:
	enum Mode
	{
		eMode_Orthographic,
		eMode_Perspective,

		eMode_Count
	};
	Camera();
	~Camera();


	void SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float znear = 0.f, float zfar = 1.f, bool isDefault = true);
	void SetPerspectiveView(float aspect, float fov, float znear, float zfar, bool isDefault = true);

	Vec2 GetOrthographicBottomLeft() const;
	Vec2 GetOrthographicTopRight() const;
	void Translate2D(const Vec2& translation2D);
	void Translate3D(const Vec3& translation3D);
	void Rotate3D(const EulerAngles& angle3D);

	Mat44 GetOrthographicMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMatrix() const;

	void SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
	Mat44 GetRenderMatrix() const;

	void SetTransform(const Vec3& position, const EulerAngles& orientation);
	Mat44 GetViewMatrix() const;
	Mat44 GetModelMatrix() const;

	EulerAngles GetOrientation() const;

	void SetViewportUVs(AABB2 cameraBoxUVs);
	AABB2 GetViewportUVs() const;

public:
	Vec3 m_position;
	EulerAngles m_orientation;

	float m_zNearPlane;
	float m_zFarPlane;

	Vec2 m_orthographicBottomLeft;
	Vec2 m_orthographicTopRight;

	float m_perspectiveAspect;
	float m_perspectiveFOV;

private:
	Mode m_mode = eMode_Orthographic;

	Vec3 m_renderIBasis = Vec3(1.f, 0.f, 0.f);
	Vec3 m_renderJBasis = Vec3(0.f, 1.f, 0.f);
	Vec3 m_renderKBasis = Vec3(0.f, 0.f, 1.f);

	AABB2 m_viewportUVs = AABB2::ZERO_TO_ONE;
};