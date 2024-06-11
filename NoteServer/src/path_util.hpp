#pragma once
#include <filesystem>
#include <string>


std::filesystem::path get_path_to_dir(const std::string& user_name = "test_user") {
    std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path parent_path = current_path.parent_path();

    std::filesystem::path user_path = parent_path / "user_files";
    std::filesystem::create_directory(user_path);

    std::filesystem::path user_name_path = user_path / user_name;
    std::filesystem::create_directory(user_name_path);

    return user_name_path;
}

