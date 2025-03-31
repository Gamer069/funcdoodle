#pragma once
#include <iostream>
#include <random>
#include <string>
#include <array>
#include <ostream>

namespace FuncDoodle {
	class UUID {
		public:
			UUID(unsigned char* bytes) { m_Bytes = bytes; }
			inline unsigned char* Bytes() { return m_Bytes; }
			const char* ToString() const;
			bool operator==(const UUID& other) const;
			bool operator!=(const UUID& other) const;
			static UUID GenV4();
			static UUID GenV6();
			friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);
		private:
			// it's definitely 16 though
			unsigned char* m_Bytes;
	};
}  // namespace FuncDoodle
