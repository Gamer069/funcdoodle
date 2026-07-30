#include "Test/ImTest.h"

static void RegisterTests_FuncDoodle() {
	ImTest("menu_open_close", [](ImTestCtx& tc) {
		tc.Popups.Open("new");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("New project##new"));
		tc.Popups.Close("new");
		tc.Ctx->Yield();
		IM_CHECK(!ImGui::IsPopupOpen("New project##new"));
	});


	ImTest("menu_show_keybinds", [](ImTestCtx& tc) {
		tc.Popups.Open("keybinds");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("Keybinds##keybinds"));
		tc.Popups.Close("keybinds");
		tc.Ctx->Yield();
	});


	ImTest("menu_open_preferences", [](ImTestCtx& tc) {
		tc.Popups.Open("pref");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("Preferences##pref"));
		tc.Popups.Close("pref");
		tc.Ctx->Yield();
	});


	ImTest("keybinds_table_headers", [](ImTestCtx& tc) {
		tc.Popups.Open("keybinds");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("Keybinds##keybinds"));
		tc.Popups.Close("keybinds");
		tc.Ctx->Yield();
	});


	ImTest("new_project_dialog_inputs", [](ImTestCtx& tc) {
		tc.Popups.Open("new");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("New project##new"));
		tc.Popups.Close("new");
		tc.Ctx->Yield();
	});


	ImTest("rotate_dialog", [](ImTestCtx& tc) {
		tc.Popups.Open("rotate");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("Rotate##rotate"));
		tc.Popups.Close("rotate");
		tc.Ctx->Yield();
	});


	ImTest("export_dialog", [](ImTestCtx& tc) {
		tc.Popups.Open("export");
		tc.Ctx->Yield();
		IM_CHECK(ImGui::IsPopupOpen("Export##export"));
		tc.Popups.Close("export");
		tc.Ctx->Yield();
	});


	ImTest("test_engine_window", [](ImTestCtx& tc) {
		(void)tc.Popups;
		tc.Ctx->SetRef("Dear ImGui Test Engine");
		ImGuiWindow* window = ImGui::FindWindowByName("Dear ImGui Test Engine");
		IM_CHECK(window != nullptr);
	});
}

int FuncDoodle_RegisterImTests() {
	if (s_TestEngine == nullptr)
		return 0;
	RegisterTests_FuncDoodle();
	return 0;
}
