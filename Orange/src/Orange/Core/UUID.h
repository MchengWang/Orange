#pragma once

#include <xhash>

namespace Orange
{

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return o_UUID; }
	private:
		uint64_t o_UUID;
	};

}


namespace std
{

	template <>
	struct hash<Orange::UUID>
	{
		std::size_t operator()(const Orange::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}
