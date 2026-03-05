#pragma once

#include "Action.h"
#include "Frame.h"
#include "Ptr.h"

#include <unordered_map>
#include <vector>

namespace FuncDoodle {
	class ToolManager;
	class AnimationPlayer;

	class EditorController {
		public:
			EditorController();
			bool Paint(Frame* frame, unsigned long frameI,
				ToolManager* toolManager, AnimationPlayer* player, int pixelX,
				int pixelY, bool mouseDown, bool mouseClicked);
			void SetUndoByStroke(bool undoByStroke, AnimationPlayer* player);
			void EndStroke(AnimationPlayer* player);

		private:
			bool PaintPencil(Frame* frame, unsigned long frameI,
				ToolManager* toolManager, AnimationPlayer* player, int pixelX,
				int pixelY, bool mouseDown);
			bool PaintEraser(Frame* frame, unsigned long frameI,
				ToolManager* toolManager, AnimationPlayer* player, int pixelX,
				int pixelY, bool mouseDown);
			bool PaintBucket(Frame* frame, unsigned long frameI,
				ToolManager* toolManager, AnimationPlayer* player, int pixelX,
				int pixelY, bool mouseClicked);
			bool PaintPicker(
				Frame* frame, ToolManager* toolManager, int pixelX, int pixelY);
			void FloodFill(int x, int y, Col targetCol, Col fillCol,
				Frame* targetFrame, std::vector<std::pair<int, int>>& changed);
			void RecordStrokeChange(unsigned long frameI, int x, int y,
				const Col& prev, const Col& next);
			void FinalizeStroke(AnimationPlayer* player);

			bool m_UndoByStroke = false;
			bool m_StrokeActive = false;
			unsigned long m_StrokeFrameI = 0;
			std::vector<StrokeAction::PixelChange> m_StrokeChanges;
			std::unordered_map<unsigned long long, size_t> m_StrokeIndexByKey;
	};
}  // namespace FuncDoodle
