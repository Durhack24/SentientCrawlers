#pragma once
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

class ResourceManager
{
public:
	static void AddSearchPath(std::filesystem::path path);
	static void FindResources();

	static std::filesystem::path GetResourcePath();

	template <typename... Strs>
	static std::string GetResourcePath(const Strs&... dirs)
	{
		auto ret = GetResourcePath();

		(ret /= ... /= dirs);
		return ret.string();
	}

protected:
	static std::vector<std::filesystem::path> searchPaths;
	static std::filesystem::path resourcePath;
	
	static std::optional<std::filesystem::path> FindResources(std::filesystem::path path);
};