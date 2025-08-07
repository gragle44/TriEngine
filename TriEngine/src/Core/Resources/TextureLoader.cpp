#include "tripch.h"

#include "TextureLoader.h"

#include "ResourceArchive.h"

#include "Core/Projects/ProjectManager.h"
#include "ResourceManager.h"

#include "filesystem"
#include "stb_image.h"
#include "yaml-cpp/yaml.h"
#include "magic_enum.hpp"

namespace TriEngine {

	Reference<Resource> TextureLoader::Load(const ResourceMetadata& metadata)
	{
		std::filesystem::path path = ProjectManager::GetCurrent()->GetWorkingDirectory() / metadata.Filepath;
		stbi_set_flip_vertically_on_load(1);

		int32_t x, y, channels;
		uint8_t* data = stbi_load(path.string().c_str(), &x, &y, &channels, 0);

		int32_t size = x * y * channels;

		TRI_CORE_ASSERT(size, "Failed to load image file");

		ByteBuffer buffer;
		buffer.reserve(size);
		buffer.insert(buffer.end(), data, data + size);

		TextureSettings settings;
		settings.SizeX = x;
		settings.SizeY = y;

		std::string metadataPath = path.string() + ".meta";
		if (std::filesystem::exists(metadataPath)) {
			auto node = YAML::LoadFile(metadataPath);

			settings.Usage = magic_enum::enum_cast<TextureUsage>(node["Usage"].as<std::string>()).value();
			settings.Samples = node["Samples"].as<uint32_t>();
			settings.Filter = magic_enum::enum_cast<TextureFilter>(node["Filter"].as<std::string>()).value();
			settings.Wrap = magic_enum::enum_cast<TextureWrap>(node["Wrap"].as<std::string>()).value();
		}

		stbi_image_free(data);

		//Add constructor for taking in a buffer and settings
		Reference<Texture2D> texture = Texture2D::Create(buffer, settings);
		texture->MetaData = metadata;
		return texture;
	}
	void TextureLoader::Save(Reference<Resource> resource)
	{
		Reference<Texture2D> texture = std::reinterpret_pointer_cast<Texture2D>(resource);
		ResourceMetadata& metadata = texture->MetaData;
        std::filesystem::path metadataPath = ProjectManager::GetCurrent()->GetWorkingDirectory() / metadata.Filepath / ".meta";

        YAML::Emitter out;

		const TextureSettings& texSettings = texture->GetSettings();

		out << YAML::BeginMap;
		out << YAML::Key << "Usage" << YAML::Value << magic_enum::enum_name(texSettings.Usage).data();
		out << YAML::Key << "Filter" << YAML::Value << magic_enum::enum_name(texSettings.Filter).data();
		out << YAML::Key << "Wrap" << YAML::Value << magic_enum::enum_name(texSettings.Wrap).data();
		out << YAML::Key << "Samples" << YAML::Value << texSettings.Samples;
		out << YAML::EndMap;

		std::ofstream fout(metadataPath);
		fout << out.c_str();
	}

	Reference<Resource> TextureLoader::LoadBinary(const ResourceMetadata& metadata)
	{
		ArchivedFileHeader header;
		TextureSettings settings;
		ByteBuffer buffer;

		std::filesystem::path archivePath = ProjectManager::GetCurrent()->GetAbsolutePath(std::format("data/{}.pck", metadata.ArchiveIndex));
		std::ifstream archive(archivePath, std::ios::binary);
		archive.seekg(metadata.ArchiveOffset, std::ios::beg);

		archive.read(reinterpret_cast<char*>(&header), sizeof(header));
		archive.read(reinterpret_cast<char*>(&settings), sizeof(settings));
		uint32_t textureSize = (header.EncryptedSize - sizeof(header)) - sizeof(settings);
		buffer.resize(textureSize);
		archive.read(reinterpret_cast<char*>(buffer.data()), textureSize);

		archive.close();

		Reference<Texture2D> texture = Texture2D::Create(buffer, settings);
		texture->MetaData = metadata;

		return texture;
	}

	void TextureLoader::SaveBinary(Reference<Resource> resource, std::ostream& stream)
	{
		Reference<Texture2D> texture = std::dynamic_pointer_cast<Texture2D>(resource);
		ResourceMetadata& metadata = texture->MetaData;
		const TextureSettings& texSettings = texture->GetSettings();

		const uint32_t size = sizeof(ArchivedFileHeader) + sizeof(TextureSettings) + texture->GetData().size();

		ArchivedFileHeader header;
		header.Type = texture->MetaData.Type;
		header.Size = size;
		header.CompressedSize = size;
		header.EncryptedSize = size;

		stream.write(reinterpret_cast<char*>(&header), sizeof(header));
		stream.write(reinterpret_cast<const char*>(&texSettings), sizeof(TextureSettings));
		stream.write(reinterpret_cast<char*>(texture->GetData().data()), texture->GetData().size());
	}
}