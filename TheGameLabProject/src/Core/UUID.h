#pragma once
#include <string>

namespace Engine 
{
	class UUID {
	public:

		UUID();
		UUID(uint64_t uuid);

		std::string ToString() const {
			return std::to_string(m_UUID);
		}

	private:

		uint64_t m_UUID;
	};

}