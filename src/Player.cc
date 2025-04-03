#include "Player.h"

#include "Project.h"

#include <GLFW/glfw3.h>

#include "Gui.h"

namespace FuncDoodle {
	AnimationPlayer::AnimationPlayer(ProjectFile* proj) : m_Proj(proj) {
		m_Playing = false;
	}
	AnimationPlayer::~AnimationPlayer() {}

	void AnimationPlayer::Play() {
		if (!m_Playing)
			return;
		float dt = ImGui::GetIO().DeltaTime;
		float fps = static_cast<float>(m_Proj->AnimFPS());
		static float timeElapsed = 0.0f;
		timeElapsed += dt;
		m_CurFrame = static_cast<unsigned long>(timeElapsed * fps) %
					 m_Proj->AnimFrameCount();
	}
	void AnimationPlayer::Rewind() {
		m_CurFrame = 0;
	}
	void AnimationPlayer::End() {
		m_CurFrame =
			m_Proj->AnimFrameCount() -
			1;	// -1 cos cpp ain't stupid and uses chad 0 based indexing unlike
				// python, stupid lua, and other stupid languages
	}
}  // namespace FuncDoodle
