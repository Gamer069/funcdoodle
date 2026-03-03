#pragma once

#include "Project.h"

namespace FuncDoodle {
	class AnimationPlayer {
		public:
			AnimationPlayer(const std::shared_ptr<ProjectFile>& proj);
			~AnimationPlayer();
			void Play();
			void Rewind();
			void End();
			constexpr inline void SetPlaying(bool playing) {
				m_Playing = playing;
			}
			constexpr inline bool Playing() const { return m_Playing; }

			inline void SetProj(std::shared_ptr<ProjectFile> proj) {
				m_Proj = proj;
			}
			inline std::shared_ptr<ProjectFile> Proj() const { return m_Proj; }

			constexpr inline unsigned long CurFrame() const {
				return m_CurFrame;
			}
			constexpr inline void SetCurFrame(unsigned long frame) {
				m_CurFrame = frame;
			}

		private:
			bool m_Playing;
			std::shared_ptr<ProjectFile> m_Proj;
			unsigned long m_CurFrame = 0L;
	};
}  // namespace FuncDoodle
