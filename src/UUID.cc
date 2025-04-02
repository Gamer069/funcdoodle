#include "UUID.h"

#include <cstring>

#include <random>

#include "MacroUtils.h"

namespace FuncDoodle {
	std::ostream& operator<<(std::ostream& os, const UUID& obj) {
		os << obj.ToString();
		return os;
	}
	const char* UUID::ToString() const {
		static char str[37];
		str[0] = '\0';
		char temp[3];
		for (int i = 0; i < 16; i++) {
			sprintf(temp, "%02x", m_Bytes[i]);
			if (i == 4 || i == 6 || i == 8 || i == 10) {
				strcat(str, "-");
			}
			strcat(str, temp);
		}
		return str;
	}
	bool UUID::operator!=(const UUID& other) const {
		return !(*this == other);
	}
	bool UUID::operator==(const UUID& other) const {
		return other.ToString() == this->ToString();
	}
	UUID UUID::Gen() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);

		std::array<unsigned char, 16> bytes{};

		for (int i = 0; i < 16; ++i) {
			bytes[i] = static_cast<unsigned char>(dis(gen));
		}

		bytes[6] = (bytes[6] & 0x0f) | 0x40;
		bytes[8] = (bytes[8] & 0x3f) | 0x80;
		UUID uuid(bytes);
		return uuid;
	}
	UUID UUID::FromString(const char* str) {
		char* buf = (char*)malloc(strlen(str) + 1);
		strcpy(buf, str);
		char* tok = strtok(buf, "-");
		int i = 0;
		std::array<unsigned char, 16> bytes;
		while (tok != nullptr) {
			unsigned int cur = 0;
			if (sscanf(&tok[i], "%02x", &cur) != 1) {
				FUNC_FATAL(
					"Failed to convert string to UUID, failed to scan number");
			}
			bytes[i] = cur;
			++i;
			tok = strtok(nullptr, "-");
		}
		return UUID(bytes);
	}
}  // namespace FuncDoodle
