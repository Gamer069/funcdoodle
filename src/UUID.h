#pragma once
#include <array>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <memory.h>

namespace FuncDoodle {
	class UUID {
		public:
			UUID(unsigned char* bytes) { m_Bytes = bytes; }
			UUID() { m_Bytes = new unsigned char[16]{0}; }
			inline unsigned char* Bytes() { return m_Bytes; }
			const char* ToString() const;
			bool operator==(const UUID& other) const;
			bool operator!=(const UUID& other) const;
			static UUID GenV4();
			static UUID GenV6();
			static UUID FromString(const char* str);
			friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);
			struct Hash {
				size_t operator()(const UUID& uuid) const {
					static const size_t FNV_PRIME = 1099511628211ULL;
					static const size_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
					size_t hash = FNV_OFFSET_BASIS;
					for (size_t i = 0; i < 16; ++i) {
						hash ^= static_cast<size_t>(uuid.m_Bytes[i]);
						hash *= FNV_PRIME;
					}
					return hash;
				}
			};
			bool operator<(const UUID& other) const {
				return memcmp(m_Bytes, other.m_Bytes, 16) < 0;
			}

			bool operator>(const UUID& other) const {
				return memcmp(m_Bytes, other.m_Bytes, 16) > 0;
			}

			bool operator<=(const UUID& other) const {
				return memcmp(m_Bytes, other.m_Bytes, 16) <= 0;
			}

			bool operator>=(const UUID& other) const {
				return memcmp(m_Bytes, other.m_Bytes, 16) >= 0;
			}
			UUID operator+(const UUID& other) {
				if (this->m_Bytes == nullptr || other.m_Bytes == nullptr) {
					return UUID(); // Return a null UUID if either is null
				}
				unsigned char* resultBytes = new unsigned char[16];
				for (int i = 0; i < 16; ++i) {
					resultBytes[i] = this->m_Bytes[i] ^ other.m_Bytes[i]; // example operation: XOR
				}
				return UUID(resultBytes);
			}

			// Subtraction operator (example)
			UUID operator-(const UUID& other) {
				if (this->m_Bytes == nullptr || other.m_Bytes == nullptr) {
					return UUID(); // Return a null UUID if either is null
				}
				unsigned char* resultBytes = new unsigned char[16];
				for (int i = 0; i < 16; ++i) {
					resultBytes[i] = this->m_Bytes[i] - other.m_Bytes[i]; // example operation: subtraction.
				}
				return UUID(resultBytes);
			}
		private:
			// it's definitely 16 though
			unsigned char* m_Bytes;
	};
}  // namespace FuncDoodle
