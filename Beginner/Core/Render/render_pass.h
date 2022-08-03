#pragma once
#include <glad/glad.h>

#include "Mesh/Mesh.h"

namespace Beginner
{
	struct RenderPassParam
	{
		bool enableSetting = true;

		bool enableDepthTest = true;
		bool enableStencilTest = true;
		bool enableBlender = true;
		bool enableCull = false;
	};

	class RenderPass
	{
	private:
		RenderPassParam _currentRenderParam;


	public:
		explicit RenderPass(const RenderPassParam param)
		{
			_currentRenderParam = param;
		}

		void setRenderState() const
		{
			if (!_currentRenderParam.enableSetting) return;

			if (_currentRenderParam.enableDepthTest)
			{
				// 开启深度测试
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
			}
			else
				glDisable(GL_DEPTH_TEST);

			if (_currentRenderParam.enableStencilTest)
			{
				// 开启模板测试
				glEnable(GL_STENCIL_TEST);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			}
			else
				glDisable(GL_STENCIL_TEST);

			if (_currentRenderParam.enableBlender)
			{
				// 开启混合
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else
				glDisable(GL_BLEND);

			if (_currentRenderParam.enableCull)
			{
				// 开启正面剔除
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				glFrontFace(GL_CCW);
			}
			else
				glDisable(GL_CULL_FACE);
		}

		static void setShaderMVPParam(const Shader* shader, const CameraControl& camera, float screenWidth,
		                              float screenHeight)
		{
			if (shader == nullptr) return;

			shader->use();

			glm::mat4 view = camera.viewLookAtMat4();
			glm::mat4 projection = glm::perspective(glm::radians(camera.fov()),
			                                        screenWidth / screenHeight,
			                                        camera.near,
			                                        camera.far);

			shader->setMatrix4("view", glm::value_ptr(view));
			shader->setMatrix4("projection", glm::value_ptr(projection));
			shader->setVec3("worldCameraPos", camera.pos);
		}

		static void setShaderLightParam(const Shader* shader, const DirectionLight* dirLight)
		{
			if (shader		== nullptr) return;
			if (dirLight	== nullptr) return;

			shader->setVec3("dirLight.color",		dirLight->color);
			shader->setVec3("dirLight.position",		dirLight->pos);
			shader->setVec3("dirLight.ambient",		dirLight->ambient);
			shader->setVec3("dirLight.diffuse",		dirLight->diffuse);
			shader->setVec3("dirLight.specular",		dirLight->specular);

			// TODO: 历史遗留问题，用的pos去表示的方向光的方向
			shader->setVec3("dirLight.direction",	dirLight->pos);

			// TODO: 聚光灯、点光源的处理，这里先用默认值
			shader->setFloat("light.cutOff", glm::cos(glm::radians(12.0f)));
			shader->setFloat("light.outerCutOff", glm::cos(glm::radians(24.0f)));
		}
	};
}
