#pragma once

#include "FrameRenderer.h"
#include "Player.h"
#include "Project.h"
#include "Ptr.h"

namespace FuncDoodle {
	struct TimelineKeyContext {
			SharedPtr<ProjectFile> proj;
			AnimationPlayer* player = nullptr;
			FrameRenderer* frameRenderer = nullptr;
			unsigned long* selectedFrame = nullptr;
	};

	class KeyHandler {
		public:
			static void HandleTimelineShortcuts(TimelineKeyContext& context);
	};
}  // namespace FuncDoodle
