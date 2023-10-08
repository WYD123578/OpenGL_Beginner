#pragma once

namespace Beginner{
	namespace Core {

		class CoreSystem {

		public:  
			virtual void InitSystem();
			virtual void TickPerFrame();
		};
	}
}