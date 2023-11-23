#pragma once

#include "Core.h"

namespace TriEngine {

	class TRI_API Application
	{
		public:
			Application();
			virtual ~Application();

			void Run();

	};

	//Create in client
	Application* CreateApplication();

}

