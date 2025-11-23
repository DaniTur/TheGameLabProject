#pragma once
#include <string>

namespace Engine 
{
	class UUID {
	public:

		UUID();
		UUID(uint64_t uuid);

		operator uint64_t() const { return m_UUID; }

		std::string ToString() const { return std::to_string(m_UUID); }

	private:

		uint64_t m_UUID;
	};

}

// The Engine::UUID class should be "hashable" to be implemented as a key of unordered maps/ unordered set(which are implented as hash tables)
//	Object requirements:
//	Hash (with Key as the function call argument type)
//	DefaultConstructible
//	CopyAssignable
//	Swappable
template <>
struct std::hash<Engine::UUID> {
	std::size_t operator()(const Engine::UUID& uuid) const {
		return std::hash<uint64_t>()((uint64_t)uuid);
	}
};