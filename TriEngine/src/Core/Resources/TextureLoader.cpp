#include "tripch.h"

#include "TextureLoader.h"

#include "filesystem"
#include "stb_image.h"
#include "yaml-cpp/yaml.h"
#include "magic_enum.hpp"

namespace TriEngine {

	Reference<Resource> TextureLoader::Load(ResourceMetadata& metadata)
	{
		stbi_set_flip_vertically_on_load(1);

		int32_t x, y, channels;
		uint8_t* data = stbi_load(metadata.Filepath.c_str(), &x, &y, &channels, 0);

		int32_t size = x * y * channels;

		ByteBuffer buffer;
		buffer.reserve(size);
		buffer.insert(buffer.end(), data, data + size);

		TextureSettings settings;
		settings.SizeX = x;
		settings.SizeY = y;

		std::string metadataPath = metadata.Filepath + ".meta";
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
		std::string metadataPath = metadata.Filepath + ".meta";

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
}