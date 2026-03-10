#pragma once

#include "FrameRenderer.h"
#include "Keybinds.h"
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
			static void HandleTimelineShortcuts(
				TimelineKeyContext& context, KeybindsRegistry& keybinds);
			static void RegisterKeybinds(KeybindsRegistry& keybinds);
	};
}  // namespace FuncDoodle
