#include "EditorController.h"

#include "Player.h"
#include "Project.h"
#include "ToolManager.h"

#include <stack>

namespace FuncDoodle {
	EditorController::EditorController() {}

	bool EditorController::Paint(Frame* frame, unsigned long frameI,
		ToolManager* toolManager, AnimationPlayer* player, int pixelX,
		int pixelY, bool mouseDown, bool mouseClicked) {
		if (!frame || !frame->Pixels() || !toolManager || !player ||
			!player->Proj()) {
			return false;
		}

		switch (toolManager->SelectedTool()) {
			case 0:
				return PaintPencil(frame, frameI, toolManager, player, pixelX,
					pixelY, mouseDown);
			case 1:
				return PaintEraser(frame, frameI, toolManager, player, pixelX,
					pixelY, mouseDown);
			case 2:
				return PaintBucket(frame, frameI, toolManager, player, pixelX,
					pixelY, mouseClicked);
			case 3:
				return PaintPicker(frame, toolManager, pixelX, pixelY);
			default:
				return false;
		}
	}

	void EditorController::SetUndoByStroke(
		bool undoByStroke, AnimationPlayer* player) {
		if (m_UndoByStroke == undoByStroke) {
			return;
		}
		if (!undoByStroke && m_StrokeActive) {
			FinalizeStroke(player);
		}
		m_UndoByStroke = undoByStroke;
	}

	void EditorController::EndStroke(AnimationPlayer* player) {
		FinalizeStroke(player);
	}

	bool EditorController::PaintPencil(Frame* frame, unsigned long frameI,
		ToolManager* toolManager, AnimationPlayer* player, int pixelX,
		int pixelY, bool mouseDown) {
		if (!mouseDown) {
			return false;
		}
		const float* colOld = toolManager->Col();
		Col newColor = {static_cast<unsigned char>(colOld[0] * 255.0f + 0.5f),
			static_cast<unsigned char>(colOld[1] * 255.0f + 0.5f),
			static_cast<unsigned char>(colOld[2] * 255.0f + 0.5f)};

		int size = toolManager->Size();
		bool actionPerformed = false;

		for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
			for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
				int newX = pixelX + offsetX;
				int newY = pixelY + offsetY;

				if (newX < 0 || newX >= frame->Pixels()->Width() || newY < 0 ||
					newY >= frame->Pixels()->Height()) {
					continue;
				}

				Col prevColor = frame->Pixels()->Get(newX, newY);
				frame->SetPixel(newX, newY, newColor);

				if (prevColor == newColor) {
					continue;
				}

				if (m_UndoByStroke) {
					RecordStrokeChange(frameI, newX, newY, prevColor, newColor);
				} else {
					DrawAction action(newX, newY, prevColor, newColor, frameI,
						player->Proj());
					player->Proj()->PushUndoableDrawAction(std::move(action));
				}
				actionPerformed = true;
			}
		}
		return actionPerformed;
	}

	bool EditorController::PaintEraser(Frame* frame, unsigned long frameI,
		ToolManager* toolManager, AnimationPlayer* player, int pixelX,
		int pixelY, bool mouseDown) {
		if (!mouseDown) {
			return false;
		}
		int size = toolManager->Size();
		bool actionPerformed = false;
		Col bgColor = player->Proj()->BgCol();

		for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
			for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
				int newX = pixelX + offsetX;
				int newY = pixelY + offsetY;

				if (newX < 0 || newX >= frame->Pixels()->Width() || newY < 0 ||
					newY >= frame->Pixels()->Height()) {
					continue;
				}

				Col prevColor = frame->Pixels()->Get(newX, newY);
				frame->SetPixel(newX, newY, bgColor);

				if (prevColor == bgColor) {
					continue;
				}

				if (m_UndoByStroke) {
					RecordStrokeChange(frameI, newX, newY, prevColor, bgColor);
				} else {
					DrawAction action(
						newX, newY, prevColor, bgColor, frameI, player->Proj());
					player->Proj()->PushUndoableDrawAction(std::move(action));
				}
				actionPerformed = true;
			}
		}

		return actionPerformed;
	}

	bool EditorController::PaintBucket(Frame* frame, unsigned long frameI,
		ToolManager* toolManager, AnimationPlayer* player, int pixelX,
		int pixelY, bool mouseClicked) {
		if (!mouseClicked || pixelX < 0 || pixelX >= frame->Pixels()->Width() ||
			pixelY < 0 || pixelY >= frame->Pixels()->Height()) {
			return false;
		}

		const float* colOld = toolManager->Col();
		Col fillColor = {static_cast<unsigned char>(colOld[0] * 255.0f + 0.5f),
			static_cast<unsigned char>(colOld[1] * 255.0f + 0.5f),
			static_cast<unsigned char>(colOld[2] * 255.0f + 0.5f)};
		Col curPixelCol = frame->Pixels()->Get(pixelX, pixelY);

		if (curPixelCol == fillColor) {
			return false;
		}

		std::vector<std::pair<int, int>> pixelsChangedByBucketTool;
		FloodFill(pixelX, pixelY, curPixelCol, fillColor, frame,
			pixelsChangedByBucketTool);
		if (pixelsChangedByBucketTool.empty()) {
			return false;
		}

		FillAction action(curPixelCol, fillColor, frameI, player->Proj(),
			pixelsChangedByBucketTool);
		player->Proj()->PushUndoableFillAction(std::move(action));
		return true;
	}

	bool EditorController::PaintPicker(
		Frame* frame, ToolManager* toolManager, int pixelX, int pixelY) {
		if (!frame || !frame->Pixels() || !toolManager || pixelX < 0 ||
			pixelX >= frame->Pixels()->Width() || pixelY < 0 ||
			pixelY >= frame->Pixels()->Height()) {
			return false;
		}

		toolManager->SetCol(frame->Pixels()->Get(pixelX, pixelY));
		return true;
	}

	void EditorController::FloodFill(int x, int y, Col targetCol, Col fillCol,
		Frame* targetFrame, std::vector<std::pair<int, int>>& changed) {
		if (!targetFrame || !targetFrame->Pixels()) {
			return;
		}

		std::stack<std::pair<int, int>> pixelStack;
		pixelStack.push({x, y});

		while (!pixelStack.empty()) {
			auto [currentX, currentY] = pixelStack.top();
			pixelStack.pop();

			if (currentX < 0 || currentX >= targetFrame->Pixels()->Width() ||
				currentY < 0 || currentY >= targetFrame->Pixels()->Height()) {
				continue;
			}

			Col currentCol = targetFrame->Pixels()->Get(currentX, currentY);
			if (currentCol != targetCol || currentCol == fillCol) {
				continue;
			}

			changed.emplace_back(currentX, currentY);
			targetFrame->SetPixel(currentX, currentY, fillCol);

			pixelStack.push({currentX + 1, currentY});
			pixelStack.push({currentX - 1, currentY});
			pixelStack.push({currentX, currentY + 1});
			pixelStack.push({currentX, currentY - 1});
		}
	}

	void EditorController::RecordStrokeChange(
		unsigned long frameI, int x, int y, const Col& prev, const Col& next) {
		if (prev == next) {
			return;
		}

		if (!m_StrokeActive || m_StrokeFrameI != frameI) {
			m_StrokeActive = true;
			m_StrokeFrameI = frameI;
			m_StrokeChanges.clear();
			m_StrokeIndexByKey.clear();
		}

		const unsigned long long key =
			(static_cast<unsigned long long>(static_cast<unsigned int>(y))
				<< 32U) |
			static_cast<unsigned int>(x);
		auto it = m_StrokeIndexByKey.find(key);
		if (it == m_StrokeIndexByKey.end()) {
			m_StrokeIndexByKey[key] = m_StrokeChanges.size();
			m_StrokeChanges.push_back(StrokeAction::PixelChange{
				.x = x, .y = y, .prev = prev, .next = next});
		} else {
			m_StrokeChanges[it->second].next = next;
		}
	}

	void EditorController::FinalizeStroke(AnimationPlayer* player) {
		if (!m_StrokeActive) {
			return;
		}

		m_StrokeActive = false;
		if (!m_StrokeChanges.empty() && player && player->Proj()) {
			StrokeAction action(
				m_StrokeFrameI, player->Proj(), std::move(m_StrokeChanges));
			player->Proj()->PushUndoableStrokeAction(std::move(action));
		}
		m_StrokeChanges.clear();
		m_StrokeIndexByKey.clear();
	}
}  // namespace FuncDoodle
