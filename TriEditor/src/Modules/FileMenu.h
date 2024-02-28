#pragma once

#include "Core/Base/Core.h"

namespace TriEngine {
	class FileMenu {
	public:
		FileMenu() = default;

		void OnImGuiRender();
	private:
		void RenderFile();
		void RenderProject();

		void RenderProjectSettings(bool* show);
	};
}