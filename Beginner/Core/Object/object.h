#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>

namespace Beginner
{
	class Component
	{
	};

	class Object
	{
	private:
		std::string _name;
		std::vector<Component> _components;

	public:
		std::string name()
		{
			return _name;
		}

		Object(const std::string& name, const std::vector<Component>& components);
		void AddComponent(const Component* component);
	};
}
