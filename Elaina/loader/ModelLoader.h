#pragma once
namespace Elaina
{
	class CNode;
	class CModelLoader
	{
	public:
		static std::shared_ptr<CNode> loadGltfFile(const std::string& vFileName);
	};
}