#include "KeyHandler.h"

#include "Keybinds.h"
#include "imgui.h"

namespace FuncDoodle {
	void KeyHandler::RegisterKeybinds(KeybindsRegistry& keybinds) {
		keybinds.Register(
			"prev_frame", {false, false, false, ImGuiKey_LeftBracket});
		keybinds.Register(
			"next_frame", {false, false, false, ImGuiKey_RightBracket});
		keybinds.Register("undo", {false, false, false, ImGuiKey_E});
		keybinds.Register("redo", {false, false, false, ImGuiKey_R});
		keybinds.Register("insert_after", {false, false, false, ImGuiKey_P});
		keybinds.Register("insert_before", {false, false, false, ImGuiKey_O});
		keybinds.Register("move_forward", {false, false, false, ImGuiKey_I});
		keybinds.Register("move_backward", {false, false, false, ImGuiKey_U});
		keybinds.Register(
			"delete_frame", {false, false, false, ImGuiKey_Backslash});
		keybinds.Register("copy_frame", {false, false, false, ImGuiKey_Comma});
		keybinds.Register(
			"paste_frame_before", {false, false, false, ImGuiKey_Period});
		keybinds.Register(
			"paste_frame_after", {false, false, false, ImGuiKey_Slash});
		keybinds.Register(
			"duplicate_frame_after", {false, false, false, ImGuiKey_M});
		keybinds.Register(
			"duplicate_frame_before", {false, false, false, ImGuiKey_N});
	}

	void KeyHandler::HandleTimelineShortcuts(
		TimelineKeyContext& context, KeybindsRegistry& keybinds) {
		if (!context.proj || !context.selectedFrame) {
			return;
		}
		if (ImGui::IsAnyItemActive()) {
			return;
		}

		unsigned long& selectedFrame = *context.selectedFrame;

		if (keybinds.Get("prev_frame").IsPressed()) {
			if (selectedFrame > 0) {
				Frame* prevFrame =
					context.proj->AnimFrames()->Get(selectedFrame);
				selectedFrame--;

				Frame* frame = context.proj->AnimFrames()->Get(selectedFrame);
				if (context.frameRenderer) {
					context.frameRenderer->SetPreviousFrame(prevFrame);
					context.frameRenderer->SetFrame(frame);
				}
			}
		}
		if (keybinds.Get("next_frame").IsPressed()) {
			if (selectedFrame < context.proj->AnimFrameCount() - 1) {
				Frame* prevFrame =
					context.proj->AnimFrames()->Get(selectedFrame);
				selectedFrame++;

				Frame* frame = context.proj->AnimFrames()->Get(selectedFrame);
				if (context.frameRenderer) {
					context.frameRenderer->SetPreviousFrame(prevFrame);
					context.frameRenderer->SetFrame(frame);
				}
			}
		}

		const int X1 = 3;
		const int X2 = 4;
		if (keybinds.Get("undo").IsPressed() || ImGui::IsMouseClicked(X1)) {
			SharedPtr<ProjectFile> proj =
				context.player ? context.player->Proj() : context.proj;
			if (proj) {
				proj->Undo();
			}
		}
		if (keybinds.Get("redo").IsPressed() || ImGui::IsMouseClicked(X2)) {
			SharedPtr<ProjectFile> proj =
				context.player ? context.player->Proj() : context.proj;
			if (proj) {
				proj->Redo();
			}
		}
		if (keybinds.Get("insert_after").IsPressed()) {
			context.proj->AnimFrames()->InsertAfterEmpty(selectedFrame);
			InsertFrameAction action =
				InsertFrameAction(selectedFrame + 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
		if (keybinds.Get("insert_before").IsPressed()) {
			context.proj->AnimFrames()->InsertBeforeEmpty(selectedFrame);
			selectedFrame++;
			InsertFrameAction action =
				InsertFrameAction(selectedFrame - 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
		if (keybinds.Get("move_forward").IsPressed()) {
			context.proj->AnimFrames()->MoveForward(selectedFrame);
			selectedFrame++;
			MoveFrameRightAction action =
				MoveFrameRightAction(selectedFrame, context.proj);
			context.proj->PushUndoableMoveFrameRightAction(action);
		}
		if (keybinds.Get("move_backward").IsPressed()) {
			if (selectedFrame != 0) {
				context.proj->AnimFrames()->MoveBackward(selectedFrame);
				MoveFrameLeftAction action =
					MoveFrameLeftAction(selectedFrame, context.proj);
				selectedFrame--;
				context.proj->PushUndoableMoveFrameLeftAction(action);
			}
		}
		if (keybinds.Get("delete_frame").IsPressed()) {
			if (context.proj->AnimFrameCount() != 1) {
				Frame deletedFrame =
					*context.proj->AnimFrames()->Get(selectedFrame);
				context.proj->AnimFrames()->Remove(selectedFrame);
				DeleteFrameAction action = DeleteFrameAction(
					selectedFrame, &deletedFrame, context.proj);
				context.proj->PushUndoableDeleteFrameAction(action);
			}
		}
		if (keybinds.Get("copy_frame").IsPressed()) {
			context.proj->AnimFrames()->Get(selectedFrame)->CopyToClipboard();
		}
		if (keybinds.Get("paste_frame_before").IsPressed()) {
			Frame* frame = Frame::PastedFrame();
			context.proj->AnimFrames()->InsertBefore(selectedFrame, frame);
			selectedFrame++;
		}
		if (keybinds.Get("paste_frame_after").IsPressed()) {
			Frame* frame = Frame::PastedFrame();
			context.proj->AnimFrames()->InsertAfter(selectedFrame, frame);
		}
		if (keybinds.Get("duplicate_frame_after").IsPressed()) {
			Frame* frame =
				new Frame(*context.proj->AnimFrames()->Get(selectedFrame));
			context.proj->AnimFrames()->InsertAfter(selectedFrame, frame);
			InsertFrameAction action =
				InsertFrameAction(selectedFrame + 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
		if (keybinds.Get("duplicate_frame_before").IsPressed()) {
			Frame* frame =
				new Frame(*context.proj->AnimFrames()->Get(selectedFrame));
			context.proj->AnimFrames()->InsertBefore(selectedFrame, frame);
			selectedFrame++;
			InsertFrameAction action =
				InsertFrameAction(selectedFrame - 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
	}
}  // namespace FuncDoodle
