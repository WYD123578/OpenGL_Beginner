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


	// OpenGL����������ϵ�����������Ҫ������͵�Ҫ����Z�Ḻ����
	glm::vec3 _pos;

	glm::vec3 _front;
	glm::vec3 _right;
	glm::vec3 _up;

	const glm::vec3 _worldUp;

	// �Ƕ�
	float _pitch; // ������
	float _yaw; // ƫ����
	float _fov;

	glm::mat4 customLookAtMat4(glm::vec3 eye, glm::vec3 center, glm:: vec3 up) const
	{
		// �������嶼�ƶ�������෴�������������Ϊ��Զ��0����ʱ��������λ�ã�������������Ӧ��λ�Ƶ�������
		// Ҳ�������ɰ�����ڷŵ�(0,0,0)����-z��λ���ϣ��������ı任�������г������嶼Ҫ���еı任����Բ��䣬�򿴵������ݶ���һ���ģ�
		glm::mat4 translate(1.0f);
		translate[3][0] = -eye.x;
		translate[3][1] = -eye.y;
		translate[3][2] = -eye.z;

		// ������������ת����������ӽǣ������Զ����z�Ḻ����
		// ���ڽ���ʱ����� x��y��z(z���򸺣�����ʵ������Ӧ����-z) �ᶼת������� x(1,0,0)��y((0,1,0))��z(0,0,1) ��
		// ���ڰ�����ת������ǰ������������ʣ���ֻ�б���ʽ����֤��ʵ�Ϳ������������ȥ��(1,0,0)�����ᣬ�����������Ķ�Ӧ�����Ƿ�һ�£�
		glm::mat4 rotate(1.0f);

		// ���������ϵ
		glm::vec3 camZ = glm::normalize(eye - center); // �����Z�᷽��Ӧ�ú��������ķ����෴������center����eye
		glm::vec3 camX = glm::normalize(glm::cross(up, camZ));
		glm::vec3 camY = glm::normalize(glm::cross(camZ, camX));

		// glm��������� mat4[��][��]
		// ת��ǰ����һ����X�ᣬת�ú���ǵ�һ����X��
		rotate[0][0] = camX.x;
		rotate[1][0] = camX.y;
		rotate[2][0] = camX.z;

		// ת��ǰ���ڶ�����Y�ᣬת�ú���ǵڶ�����Y��
		rotate[0][1] = camY.x;
		rotate[1][1] = camY.y;
		rotate[2][1] = camY.z;

		// ת��ǰ����������Z�ᣬת�ú���ǵ�������Z��
		rotate[0][2] = camZ.x;
		rotate[1][2] = camZ.y;
		rotate[2][2] = camZ.z;

		return rotate * translate;
	}

	void updateCameraVectors()
	{
		const float radiansXy = glm::radians(_pitch);
		const float radiansXz = glm::radians(_yaw);
		const float xzRadius = glm::cos(radiansXy);

		const float directionX = glm::sin(radiansXz) * xzRadius;
		const float directionY = glm::sin(radiansXy);
		const float directionZ = -glm::cos(radiansXz) * xzRadius;

		_front = glm::normalize(glm::vec3(directionX, directionY, directionZ));
		_right = glm::normalize(glm::cross(_front, _worldUp));
		_up = glm::normalize(glm::cross(_right, _front));
	}

public:
	CameraControl(glm::vec3 pos, glm::vec3 up = glm::vec3(0, 1, 0), float pitch = 10.0f, float yaw = 10.0f) : _pos(pos), _worldUp(up)

	{
		_cameraMoveSpeed = 1.0f;
		_cameraSensitive = 0.01f;

		_pitch = pitch;
		_yaw = yaw;
		_fov = 55.0f;

		updateCameraVectors();
	}

	~CameraControl()
	{
	}

	void handleWindowKeyBoard(Camera_Movement movement, float deltaTime)
	{
		if (movement == FORWARD)
		{
			_pos += _front * _cameraMoveSpeed * deltaTime;
		}
		if (movement == BACKWARD)
		{
			_pos -= _front * _cameraMoveSpeed * deltaTime;
		}
		if (movement == LEFT)
		{
			_pos -= _right * _cameraMoveSpeed * deltaTime;
		}
		if (movement == RIGHT)
		{
			_pos += _right * _cameraMoveSpeed * deltaTime;
		}
		updateCameraVectors();
	}

	void processMouseMovement(float xOffset, float yOffset)
	{
		_yaw += xOffset * _cameraSensitive;
		_pitch += yOffset * _cameraSensitive;

		const float maxPitch = 89;
		const float minPitch = -maxPitch;

		if (_pitch > maxPitch)
		{
			_pitch = maxPitch;
		}

		if (_pitch < minPitch)
		{
			_pitch = minPitch;
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
		// ���ø���Vector����Ϊ��Projection��ص����ݣ���Camera���������û�й�ϵ
	}

	glm::mat4 viewLookAtMat4() const
	{
		const glm::vec3 eye = _pos;
		const glm::vec3 center = _pos + _front;
		const glm::vec3 up = _up;

		return customLookAtMat4(eye, center, up);
	}

	float fov() const
	{
		return _fov;
	}
};
