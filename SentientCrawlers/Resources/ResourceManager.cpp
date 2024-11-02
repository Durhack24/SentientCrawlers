#include "ResourceManager.h"

#include <iostream>

// Initialize static members
std::vector<std::filesystem::path> ResourceManager::searchPaths = { std::filesystem::current_path() };
std::filesystem::path ResourceManager::resourcePath = {};

void ResourceManager::AddSearchPath(std::filesystem::path path)
{
	if (!std::filesystem::is_directory(path))
		path = path.parent_path();
	searchPaths.push_back(path);
}

void ResourceManager::FindResources()
{
	for (const std::filesystem::path& searchPath : searchPaths)
	{
		auto foundResourcePath = FindResources(searchPath);
		if (foundResourcePath.has_value())
		{
			resourcePath = foundResourcePath.value();
			return;
		}
	}

	std::cerr << "Resource path not found!\n";
	std::terminate();
}

std::filesystem::path ResourceManager::GetResourcePath()
{
	return resourcePath;
}

std::optional<std::filesystem::path> ResourceManager::FindResources(std::filesystem::path path)
{
	for (;;)
	{
		// Check if cetra_resources directory exists
		if (std::filesystem::exists(path / "sc_resources"))
			return path / "sc_resources";

		// Get parent path
		std::filesystem::path parentPath = path.parent_path();

		// Check if path has a reached top-level directory
		if (path == parentPath)
			break;

		path = parentPath;
	}

	return std::nullopt;
}
