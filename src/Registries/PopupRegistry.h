/**
 * @file PopupRegistry.h
 * @brief Central registry for UI popup state management.
 *
 * This file defines the PopupRegistry class, which manages the open/close state
 * of UI popups within FuncDoodle.
 *
 * Responsibilities include:
 * - Registering popup identifiers
 * - Opening and closing individual popups
 * - Querying popup visibility state
 * - Bulk closing operations (all or all except one)
 *
 * Popups are stored in a map keyed by string_view identifiers.
 *
 * @note Popup state is stored as simple boolean flags.
 * @warning string_view keys must reference stable string storage elsewhere.
 */

#pragma once

#include <unordered_map>
#include <string_view>
#include <functional>

#include "UI/Gui.h"

namespace FuncDoodle {
	/**
	 * @struct PopupEntry
	 * @brief Struct to store popup state like animation progress and if its open
	 *
	 * @see PopupRegistry
	 */
	struct PopupEntry {
		/**
		 * @var AnimationProgress
		 * @brief Progress of opening/closing animations, -1 if none is present.
		 */
		double AnimationProgress = -1;
		
		/**
		 * @var IsOpen
		 * @brief Whether or not the popup is open.
		 */
		bool IsOpen = false;

		/**
		 * @var SizeReady
		 * @brief Whether or not size is ready to get.
		 */
		bool SizeReady = false;

		/**
		 * @var FullSize
		 * @brief Full size of the popup, (-1, -1) if none.
		 */
		ImVec2 FullSize = {-1, -1};

		/**
		 * @var Pos
		 * @brief Position of popup, (-1, -1) if none.
		 */
		ImVec2 Pos = {-1, -1};
	};

	/**
	 * @class PopupRegistry
	 * @brief Stores all popups used in FuncDoodle
	 *
	 * Handles fetching and setting popup state.
	 *
	 * @see KeybindsRegistry
	 */
	class PopupRegistry {
		public:
		PopupRegistry() = default;
		~PopupRegistry() = default;

		/**
		 * @fn Register
		 * @brief Registers a popup identifier in the registry.
		 *
		 * @param id Popup identifier to track.
		 */
		void Register(std::string_view id);
		/**
		 * @fn Open
		 * @brief Marks a popup as open.
		 *
		 * @param id Popup identifier to open.
		 */
		void Open(std::string_view id);
		/**
		 * @fn Close
		 * @brief Marks a popup as closed.
		 *
		 * @param id Popup identifier to close.
		 */
		void Close(std::string_view id);

		/**
		 * @fn Popup
		 * @brief Renders a popup using a lambda.
		 *
		 * @param humanReadable The part before #.
		 * @param id The part after #.
		 * @param dt The current deltaTime.
		 * @param popup The lambda to render with.
		 */
		void Popup(std::string_view humanReadable, std::string_view id, double dt, std::function<void()> popup);

		/**
		 * @fn Modal
		 * @brief Renders a popup MODAL using a lambda.
		 *
		 * @see Popup
		 *
		 * @param humanReadable The part before #.
		 * @param id The part after #.
		 * @param dt The current deltaTime.
		 * @param popup The lambda to render with.
		 */
		void Modal(std::string_view humanReadable, std::string_view id, double dt, std::function<void()> popup);

		/**
		 * @fn IsOpen
		 * @brief Returns whether a popup is currently open.
		 *
		 * @param id Popup identifier to query.
		 * @return Whether the popup is open.
		 */
		[[nodiscard]] bool IsOpen(std::string_view id) const;

		/**
		 * @fn Get
		 * @brief Returns direct access to a popup open-state flag.
		 *
		 * @param id Popup identifier to query.
		 * @return Pointer to the popup flag, or nullptr if missing.
		 */
		bool* Get(std::string_view id);
		/**
		 * @fn CloseAll
		 * @brief Closes every registered popup.
		 */
		void CloseAll();
		/**
		 * @fn CloseAllExcept
		 * @brief Closes every popup except one.
		 *
		 * @param exception Popup identifier to keep open.
		 */
		void CloseAllExcept(std::string_view exception);

		private:
		std::unordered_map<std::string_view, PopupEntry> m_Popups;
	};
}  // namespace FuncDoodle
