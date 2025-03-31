#pragma once
#include <array>
#include <iostream>
#include <ostream>
#include <random>
#include <string>

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
			static UUID FromString(const char* str);
			friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);

		private:
			// it's definitely 16 though
			unsigned char* m_Bytes;
	};
}  // namespace FuncDoodle
