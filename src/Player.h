#pragma once

#include "Project.h"

namespace FuncDoodle {
	class AnimationPlayer {
		public:
			AnimationPlayer(ProjectFile* proj);
			~AnimationPlayer();
			void Play();
			void Rewind();
			void End();
			constexpr inline void SetPlaying(bool playing) {
				m_Playing = playing;
			}
			constexpr inline bool Playing() const { return m_Playing; }
			constexpr inline void SetProj(ProjectFile* proj) { m_Proj = proj; }
			constexpr inline ProjectFile* Proj() const { return m_Proj; }
			constexpr inline unsigned long CurFrame() const { return m_CurFrame; }
			constexpr inline void SetCurFrame(unsigned long frame) {
				m_CurFrame = frame;
			}

		private:
			bool m_Playing;
			ProjectFile* m_Proj;
			unsigned long m_CurFrame = 0L;
	};
}  // namespace FuncDoodle
