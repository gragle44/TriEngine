#pragma once

#include "../EditorData.h"
#include "Core/Base/Core.h"

namespace TriEngine {
	class FileMenu {
	public:
		FileMenu(EditorData* data);

		void OnImGuiRender();
	private:
		void RenderProject();

		void RenderProjectSettings(bool* show);

		void RenderGeneralProjectSettings();

		void RenderResourceViewer(bool* show);
		void RenderResourceEditor(bool* show, uint64_t& id);

		void RenderExportMenu(bool* show);

		EditorData* m_Data;
	};
}