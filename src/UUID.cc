#include "UUID.h"

#include <cstring>

#include <random>

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
	UUID UUID::GenV4() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);

		unsigned char* bytes = new unsigned char[16]{};

		for (int i = 0; i < 16; ++i) {
			bytes[i] = static_cast<unsigned char>(dis(gen));
		}

		bytes[6] = (bytes[6] & 0x0f) | 0x40;
		bytes[8] = (bytes[8] & 0x3f) | 0x80;
		UUID uuid(bytes);
		return uuid;
	}
	UUID UUID::GenV6() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);

		unsigned char* bytes = new unsigned char[16]{};

		for (int i = 0; i < 16; ++i) {
			bytes[i] = static_cast<unsigned char>(dis(gen));
		}

		bytes[6] = (bytes[6] & 0x0f) | 0x60; // Version 6
		bytes[8] = (bytes[8] & 0x3f) | 0x80; // Variant RFC 4122

		UUID uuid(bytes);
		return uuid;
	}
}
