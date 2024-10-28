#pragma once

#include "Project.h"

namespace FuncDoodle {
  class AnimationPlayer {
    public:
        AnimationPlayer(ProjectFile* proj);
        ~AnimationPlayer();
        void Play() const;
        constexpr inline void SetPlaying(bool playing) { m_Playing = playing; }
        constexpr inline bool Playing() const { return m_Playing; }
        constexpr inline void SetProj(ProjectFile* proj) { m_Proj = proj; }
        constexpr inline ProjectFile* Proj() const { return m_Proj; }
    private:
        bool m_Playing;
        ProjectFile* m_Proj;
  };
}