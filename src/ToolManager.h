#pragma once

#include "Gui.h"

#include "Tool.h"

#include "Frame.h"

#include "AssetLoader.h"

namespace FuncDoodle {
	class ToolManager {
		public:
			ToolManager(AssetLoader* assetLoader);
			~ToolManager();
			void RenderTools();
			const int SelectedTool() { return m_SelectedTool; }
			const float* Col() { return m_Col; }
			void SetCol(struct Col col) {
				unsigned char colArr[3] = {col.r, col.g, col.b};
				for (int j = 0; j < 3; j++) {
					m_Col[j] =
						static_cast<unsigned char>(colArr[j] * 255.0f + 0.5f);
				}
			}
			const int Size() const { return m_Size; }
			void SetSize(int size) { m_Size = size; }

		private:
			int m_SelectedTool = 0;
			float m_Col[3] = {0.0f, 0.0f, 0.0f};
			int m_Size = 1;
			AssetLoader* m_AssetLoader;
	};
}  // namespace FuncDoodle
