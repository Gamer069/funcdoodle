#include "Player.h"

#include "Project.h"

#include <GLFW/glfw3.h>

#include <string>
#include <sstream>

namespace FuncDoodle {
    AnimationPlayer::AnimationPlayer(ProjectFile* proj) : m_Proj(proj) {}
    AnimationPlayer::~AnimationPlayer() {}

    void AnimationPlayer::Play() const {
      if (!m_Playing) return;

      double last = glfwGetTime();
      int frameCount = 0;
      double cur = glfwGetTime();
      frameCount++;
      if (cur - last >= 1.0) {
        std::cout << "FPS: " << frameCount << std::endl;
        std::stringstream title;
        title << "FuncDoodle -- " << m_Proj->AnimName() << " (" << frameCount << ")";
        glfwSetWindowTitle(m_Proj->Window(), title.str().c_str());
        frameCount = 0;
        last = cur;
      }
    }
}
