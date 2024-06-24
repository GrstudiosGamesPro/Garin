#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <imgui.h>
#include <filesystem>

#include <cstddef>
#include <windows.h>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

class FileManager
{
public:
    template <typename T>
    static T get_file(const std::string &path);

    static std::string read_file(const std::string &file_name);
    static void write_file(const std::string &file_name, const std::string &content);

    static std::vector<fs::directory_entry> get_files_noT(const std::string &path);
    static std::string get_execute_path();
};
