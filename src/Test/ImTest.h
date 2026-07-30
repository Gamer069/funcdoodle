/**
 * @file ImTest.h
 * @brief Helper for registering ImGui test engine tests with popup access.
 *
 * Provides the ImTest() function which eliminates boilerplate around
 * IM_REGISTER_TEST and automatically injects a PopupRegistry reference
 * into each test via ImTestCtx.
 *
 * @see ImTestCtx
 * @see PopupRegistry
 */

#pragma once

#include "Core/App.h"
#include "Test/Test.h"
#include "imgui_te_context.h"

/**
 * @struct ImTestCtx
 * @brief Bundles the ImGui test context with a popup registry reference.
 *
 * Passed to every ImTest() lambda so tests can drive the UI and open/close
 * popups without repeating the GetPopups() lookup themselves.
 *
 * @see ImTest
 * @see PopupRegistry
 */
struct ImTestCtx {
	ImGuiTestContext* Ctx;
	FuncDoodle::PopupRegistry& Popups;
};

/**
 * @fn GetApp
 * @brief Returns the active FuncDoodle application instance.
 *
 * Tries the ImGui UserData pointer first, then falls back to the singleton.
 *
 * @return Pointer to the running Application.
 */
inline FuncDoodle::Application* GetApp() {
	auto* app =
		static_cast<FuncDoodle::Application*>(ImGui::GetIO().UserData);
	if (!app) {
		app = FuncDoodle::Application::Get();
	}
	return app;
}

/**
 * @fn GetPopups
 * @brief Returns the application's popup registry.
 *
 * Convenience shorthand for GetApp()->GetUiManager().GetPopups().
 *
 * @return Reference to the global PopupRegistry.
 */
inline FuncDoodle::PopupRegistry& GetPopups() {
	return GetApp()->GetUiManager().GetPopups();
}

/**
 * @brief Registers a test under the "funcdoodle" group with automatic popup
 * injection.
 *
 * Stores the user-provided callable via heap allocation and wires it into
 * the ImGui test engine via UserData. The trampoline in TestFunc constructs
 * an ImTestCtx with the current ImGuiTestContext and PopupRegistry, then
 * forwards it to the user's function.
 *
 * Example usage:
 * @code
 *   ImTest("my_test", [](ImTestCtx& tc) {
 *       tc.Popups.Open("pref");
 *       tc.Ctx->Yield();
 *       IM_CHECK(ImGui::IsPopupOpen("Preferences##pref"));
 *   });
 * @endcode
 *
 * @tparam F        Callable type taking ImTestCtx&.
 * @param  name     Short test name (e.g. "menu_open_close").
 * @param  testFunc Lambda or function object.
 * @return Pointer to the registered ImGuiTest for further customization.
 */
template <typename F>
inline ImGuiTest* ImTest(const char* name, F testFunc) {
	auto* stored = new F(std::move(testFunc));
	ImGuiTest* t = IM_REGISTER_TEST(s_TestEngine, "funcdoodle", name);
	t->UserData = stored;
	t->GuiFunc = [](ImGuiTestContext*) {};
	t->TestFunc = [](ImGuiTestContext* ctx) {
		auto& func = *static_cast<F*>(ctx->Test->UserData);
		ImTestCtx tc{ctx, GetPopups()};
		func(tc);
	};
	return t;
}

