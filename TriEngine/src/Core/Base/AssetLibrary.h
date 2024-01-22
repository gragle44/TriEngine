#pragma once

#include "Core.h"
#include "Log.h"
#include <unordered_map>
#include <string>

namespace TriEngine {
	template <typename T>
	class AssetLibrary {
	public:
		AssetLibrary() = default;
		~AssetLibrary() = default;

		void Push(const std::string& name, Reference<T> asset) {
			if (Exists(name))
			{
				TRI_CORE_WARN("AssetLibrary::Load({0}) - overwriting asset with same name", name);
			}
			m_Assets[name] = asset;
		}

		void Pop(const std::string& name) {
			if (Exists(name)) {
				auto it = m_Assets.find(name);
				m_Assets.erase(it);
			}
		}

		void Clear() { m_Assets.clear(); }

		std::unordered_map<std::string, Reference<T>> GetAll() const { return m_Assets; };

		Reference<T> Get(const std::string& name) const {
			if (Exists(name)) {
				return m_Assets[name];
			}
			else {
				TRI_CORE_ERROR("Could not find asset {0}", name);
				return nullptr;
			}
		}

		bool Exists(const std::string& name) const {
			return m_Assets.find(name) != m_Assets.end();
		}

	private:
		mutable std::unordered_map<std::string, Reference<T>> m_Assets;
	};
}