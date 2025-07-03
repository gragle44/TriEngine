#include "EditorUtils.h"

#include "TriEngine.h"

#include "nfd.h"

std::filesystem::path OpenFileDialog(std::string_view initial_path, std::string_view filetype) {
	nfdchar_t* output;
	nfdresult_t result = NFD_OpenDialog(filetype.begin(), initial_path.data(), &output);

	std::string path;

	if (result == NFD_OKAY) {
		path = output;
		free(output);
	}

	else if (result == NFD_CANCEL) {
		TRI_CORE_TRACE("Canceled file dialog");
	}

	else if (result == NFD_ERROR) {
		TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
	}

	return path;
}

std::filesystem::path SaveFileDialog(std::string_view initial_path, std::string_view filetype) {
	nfdchar_t* output;
	nfdresult_t result = NFD_SaveDialog(filetype.begin(), initial_path.data(), &output);

	std::string path;

	if (result == NFD_OKAY) {
		path = output;
		free(output);
	}

	else if (result == NFD_CANCEL) {
		TRI_CORE_TRACE("Canceled file dialog");
	}

	else if (result == NFD_ERROR) {
		TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
	}

	return path;
}
