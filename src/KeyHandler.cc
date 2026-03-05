#include "KeyHandler.h"

#include "imgui.h"

namespace FuncDoodle {
	void KeyHandler::HandleTimelineShortcuts(TimelineKeyContext& context) {
		if (!context.proj || !context.selectedFrame) {
			return;
		}
		if (ImGui::IsAnyItemActive()) {
			return;
		}

		unsigned long& selectedFrame = *context.selectedFrame;

		if (ImGui::IsKeyPressed(ImGuiKey_LeftBracket, true)) {
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
		if (ImGui::IsKeyPressed(ImGuiKey_RightBracket, true)) {
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
		if (ImGui::IsKeyPressed(ImGuiKey_E, true) ||
			ImGui::IsMouseClicked(X1)) {
			SharedPtr<ProjectFile> proj =
				context.player ? context.player->Proj() : context.proj;
			if (proj) {
				proj->Undo();
			}
		}
		if (ImGui::IsKeyPressed(ImGuiKey_R, true) ||
			ImGui::IsMouseClicked(X2)) {
			SharedPtr<ProjectFile> proj =
				context.player ? context.player->Proj() : context.proj;
			if (proj) {
				proj->Redo();
			}
		}
		if (ImGui::IsKeyPressed(ImGuiKey_P, true)) {
			context.proj->AnimFrames()->InsertAfterEmpty(selectedFrame);
			InsertFrameAction action =
				InsertFrameAction(selectedFrame + 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
		if (ImGui::IsKeyPressed(ImGuiKey_O, true)) {
			context.proj->AnimFrames()->InsertBeforeEmpty(selectedFrame);
			selectedFrame++;
			InsertFrameAction action =
				InsertFrameAction(selectedFrame - 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
		if (ImGui::IsKeyPressed(ImGuiKey_I, true)) {
			context.proj->AnimFrames()->MoveForward(selectedFrame);
			selectedFrame++;
			MoveFrameRightAction action =
				MoveFrameRightAction(selectedFrame, context.proj);
			context.proj->PushUndoableMoveFrameRightAction(action);
		}
		if (ImGui::IsKeyPressed(ImGuiKey_U, true)) {
			if (selectedFrame != 0) {
				context.proj->AnimFrames()->MoveBackward(selectedFrame);
				MoveFrameLeftAction action =
					MoveFrameLeftAction(selectedFrame, context.proj);
				selectedFrame--;
				context.proj->PushUndoableMoveFrameLeftAction(action);
			}
		}
		if (ImGui::IsKeyPressed(ImGuiKey_Backslash, true)) {
			if (context.proj->AnimFrameCount() != 1) {
				Frame deletedFrame =
					*context.proj->AnimFrames()->Get(selectedFrame);
				context.proj->AnimFrames()->Remove(selectedFrame);
				DeleteFrameAction action = DeleteFrameAction(
					selectedFrame, &deletedFrame, context.proj);
				context.proj->PushUndoableDeleteFrameAction(action);
			}
		}
		if (ImGui::IsKeyPressed(ImGuiKey_Comma, true)) {
			context.proj->AnimFrames()->Get(selectedFrame)->CopyToClipboard();
		}
		if (ImGui::IsKeyPressed(ImGuiKey_Period, true)) {
			Frame* frame = Frame::PastedFrame();
			context.proj->AnimFrames()->InsertBefore(selectedFrame, frame);
			selectedFrame++;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_Slash, true)) {
			Frame* frame = Frame::PastedFrame();
			context.proj->AnimFrames()->InsertAfter(selectedFrame, frame);
		}
		if (ImGui::IsKeyPressed(ImGuiKey_M, true)) {
			Frame* frame =
				new Frame(*context.proj->AnimFrames()->Get(selectedFrame));
			context.proj->AnimFrames()->InsertAfter(selectedFrame, frame);
			InsertFrameAction action =
				InsertFrameAction(selectedFrame + 1, context.proj);
			context.proj->PushUndoableInsertFrameAction(action);
		}
		if (ImGui::IsKeyPressed(ImGuiKey_N, true)) {
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
