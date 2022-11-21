#pragma once
#include <GLFW/glfw3.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class CameraControl
{
private:
	float _cameraMoveSpeed;
	float _cameraSensitive;


	// OpenGL是右手坐标系，所以摄像机要看向画面就得要看向Z轴负方向

	glm::vec3 _front;
	glm::vec3 _right;
	glm::vec3 _up;

	const glm::vec3 _worldUp;

	// 角度
	float _fov;

	glm::mat4 customLookAtMat4(glm::vec3 eye, glm::vec3 center, glm::vec3 up) const
	{
		// 所有物体都移动到相机相反的向量（相机认为永远是0，此时定义的相机位置，就是所有物体应该位移的向量）
		// 也可以理解成把相机摆放到(0,0,0)看向-z的位置上，所经过的变换就是所有场景物体都要进行的变换（相对不变，则看到的内容都是一样的）
		glm::mat4 translate(1.0f);
		translate[3][0] = -eye.x;
		translate[3][1] = -eye.y;
		translate[3][2] = -eye.z;

		// 将所有物体旋转到相机的正视角（相机永远看向z轴负方向）
		// 等于将此时相机的 x、y、z(z看向负，所以实际坐标应该是-z) 轴都转到世界的 x(1,0,0)、y((0,1,0))、z(0,0,1) 轴
		// 等于把世界转换到当前相机的逆操作，剩余的只有背公式（验证其实就可以用这个矩阵去乘(1,0,0)这种轴，看结果和相机的对应轴向是否一致）
		glm::mat4 rotate(1.0f);

		// 叉乘是右手系
		glm::vec3 camZ = glm::normalize(eye - center); // 相机的Z轴方向应该和相机看向的方向相反，即从center看向eye
		glm::vec3 camX = glm::normalize(glm::cross(up, camZ));
		glm::vec3 camY = glm::normalize(glm::cross(camZ, camX));

		// glm里面矩阵是 mat4[列][行]
		// 转置前，第一列是X轴，转置后就是第一行是X轴
		rotate[0][0] = camX.x;
		rotate[1][0] = camX.y;
		rotate[2][0] = camX.z;

		// 转置前，第二列是Y轴，转置后就是第二行是Y轴
		rotate[0][1] = camY.x;
		rotate[1][1] = camY.y;
		rotate[2][1] = camY.z;

		// 转置前，第三列是Z轴，转置后就是第三行是Z轴
		rotate[0][2] = camZ.x;
		rotate[1][2] = camZ.y;
		rotate[2][2] = camZ.z;

		return rotate * translate;
	}


public:
	glm::vec3 pos;

	float pitch; // 俯仰角
	float yaw; // 偏航角

	float near = 0.1f; // 近平面
	float far = 100.0f; // 远平面

	CameraControl(glm::vec3 mPos, glm::vec3 up = glm::vec3(0, 1, 0), float _pitch = 10.0f,
	              float _yaw = 10.0f) : _worldUp(up), pos(mPos)
	{
		_cameraMoveSpeed = 8.0f;
		_cameraSensitive = 0.01f;

		pitch = _pitch;
		yaw = _yaw;

		_fov = 55.0f;

		updateCameraVectors();
	}

	void handleWindowKeyBoard(Camera_Movement movement, float deltaTime)
	{
		if (movement == FORWARD)
		{
			pos += _front * _cameraMoveSpeed * deltaTime;
		}
		if (movement == BACKWARD)
		{
			pos -= _front * _cameraMoveSpeed * deltaTime;
		}
		if (movement == LEFT)
		{
			pos -= _right * _cameraMoveSpeed * deltaTime;
		}
		if (movement == RIGHT)
		{
			pos += _right * _cameraMoveSpeed * deltaTime;
		}
		updateCameraVectors();
	}

	void processMouseMovement(float xOffset, float yOffset)
	{
		yaw += xOffset * _cameraSensitive;
		pitch += yOffset * _cameraSensitive;

		const float maxPitch = 89;
		const float minPitch = -maxPitch;

		if (pitch > maxPitch)
		{
			pitch = maxPitch;
		}

		if (pitch < minPitch)
		{
			pitch = minPitch;
		}

		updateCameraVectors();
	}

	void processMouseScroll(float zoom)
	{
		_fov -= zoom;
		const float maxFov = 60;
		const float minFov = 1;

		if (_fov <= minFov) _fov = minFov;
		if (_fov >= maxFov) _fov = maxFov;
		// 不用更新Vector，因为是Projection相关的内容，与Camera本身的坐标没有关系
	}

	glm::mat4 viewLookAtMat4() const
	{
		const glm::vec3 eye = pos;
		const glm::vec3 center = pos + _front;
		const glm::vec3 up = _up;

		return customLookAtMat4(eye, center, up);
	}

	float fov() const
	{
		return _fov;
	}

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(pos, pos + _front, _up);
	}

	void updateCameraVectors();
};

inline auto CameraControl::updateCameraVectors() -> void
{
	const float radiansXy = glm::radians(pitch);
	const float radiansXz = glm::radians(yaw);
	const float xzRadius = glm::cos(radiansXy);

	const float directionX = glm::sin(radiansXz) * xzRadius;
	const float directionY = glm::sin(radiansXy);
	const float directionZ = -glm::cos(radiansXz) * xzRadius;

	_front = glm::normalize(glm::vec3(directionX, directionY, directionZ));
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}