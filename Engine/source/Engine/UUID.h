#pragma once

// UUID from https://www.youtube.com/watch?v=O_0nUE4S8T8&ab_channel=TheCherno
// A lot simpler and smaller than creating a hash map


namespace Engine
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}


// Hashes the UUID
namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<Engine::UUID>
	{
		std::size_t operator() (const Engine::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}

