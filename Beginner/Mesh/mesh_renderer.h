#pragma once

#include "Mesh.h"
#include "Beginner/Mesh/material.h"

namespace Beginner
{
	class MeshRenderer
	{
	public:
		MeshRenderer(Mesh& mesh, Material& mat)
		{
			m_mesh = &mesh;
			m_mainMat = &mat;
		}

		void render() const
		{
			m_mainMat->draw();
			m_mesh->draw(*m_mainMat->shader);
		}

	private:
		Material* m_mainMat;
		Mesh* m_mesh;
	};
}
