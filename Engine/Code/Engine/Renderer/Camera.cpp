#include "Engine/Renderer/Camera.hpp"

Camera::Camera() {

}
Camera::~Camera() {

}
void Camera::SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float znear, float zfar, bool isDefault)
{
	m_mode = eMode_Orthographic;
	m_orthographicBottomLeft = bottomLeft;
	m_orthographicTopRight = topRight;
	m_zNearPlane = znear;
	m_zFarPlane = zfar;

	if (isDefault)
	{
		SetViewportUVs(AABB2::ZERO_TO_ONE);
	}

}
void Camera::SetPerspectiveView(float aspect, float fov, float znear, float zfar, bool isDefault)
{ 
	m_mode = eMode_Perspective;
	m_perspectiveAspect = aspect;
	m_perspectiveFOV = fov;
	m_zNearPlane = znear;
	m_zFarPlane = zfar;

	if (isDefault)
	{
		SetViewportUVs(AABB2::ZERO_TO_ONE);
	}
}
Vec2 Camera::GetOrthographicBottomLeft() const {
	return m_orthographicBottomLeft;
}

Vec2 Camera::GetOrthographicTopRight() const {
	return m_orthographicTopRight;
}

void Camera::Translate2D(const Vec2& translation2D)
{
	m_orthographicBottomLeft += translation2D;
	m_orthographicTopRight += translation2D;
}

void Camera::Translate3D(const Vec3& translation3D)
{
	m_position += translation3D;
}

void Camera::Rotate3D(const EulerAngles& angle3D)
{
	m_orientation += angle3D;
}

Mat44 Camera::GetOrthographicMatrix() const
{
	return Mat44::CreateOrthoProjection(m_orthographicBottomLeft.x, m_orthographicTopRight.x,m_orthographicBottomLeft.y, m_orthographicTopRight.y, m_zNearPlane, m_zFarPlane);
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::CreatePerspectiveProjection(m_perspectiveFOV, m_perspectiveAspect, m_zNearPlane, m_zFarPlane);
}

Mat44 Camera::GetProjectionMatrix() const
{
 	if (m_mode == eMode_Perspective)
	{
		Mat44 projectionMat = GetPerspectiveMatrix();
		projectionMat.Append(GetRenderMatrix());
		return projectionMat;
	}
	else
	{
		Mat44 projectionMat = GetOrthographicMatrix();
		projectionMat.Append(GetRenderMatrix());
		return projectionMat;
	}
}

void Camera::SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_renderIBasis = iBasis;
	m_renderJBasis = jBasis;
	m_renderKBasis = kBasis;
}

Mat44 Camera::GetRenderMatrix() const
{
	Mat44 result = Mat44();
	result.SetIJK3D(m_renderIBasis, m_renderJBasis, m_renderKBasis);
	return result;
}

void Camera::SetTransform(const Vec3& position, const EulerAngles& orientation)
{
	m_position = position;
	m_orientation = orientation;
}

Mat44 Camera::GetViewMatrix() const
{
	Mat44 viewMatrix = Mat44();

 	viewMatrix = m_orientation.GetAsMatrix_IFwd_JLeft_KUp();

	viewMatrix.SetTranslation3D(m_position);

	return viewMatrix.GetOrthonormalInverse();
}

Mat44 Camera::GetModelMatrix() const
{
	Mat44 modelMat = Mat44();
	modelMat = m_orientation.GetAsMatrix_IFwd_JLeft_KUp();
	modelMat.SetTranslation3D(m_position);
	return modelMat;
}

EulerAngles Camera::GetOrientation() const
{
	return m_orientation;
}

void Camera::SetViewportUVs(AABB2 cameraBoxUVs)
{
	m_viewportUVs.m_mins.x = cameraBoxUVs.m_mins.x;
	m_viewportUVs.m_mins.y = 1.f - cameraBoxUVs.m_maxs.y;
	m_viewportUVs.m_maxs.x = cameraBoxUVs.m_maxs.x;
	m_viewportUVs.m_maxs.y = 1.f - cameraBoxUVs.m_mins.y;
}

AABB2 Camera::GetViewportUVs() const
{
	return m_viewportUVs;
}
