#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../src/utils.hpp"

class DeleteFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream ofs(existingFile);
        ofs << "test data";
        ofs.close();

        std::ofstream ofsReadOnly(readOnlyFile);
        ofsReadOnly << "test data";
        ofsReadOnly.close();
        std::filesystem::permissions(readOnlyFile, std::filesystem::perms::owner_read, std::filesystem::perm_options::remove);
    }

    void TearDown() override {
        if (std::filesystem::exists(existingFile)) {
            std::filesystem::remove(existingFile);
        }
        if (std::filesystem::exists(readOnlyFile)) {
            std::filesystem::permissions(readOnlyFile, std::filesystem::perms::owner_write, std::filesystem::perm_options::add);
            std::filesystem::remove(readOnlyFile);
        }
        if (std::filesystem::exists(nonExistingFile)) {
            std::filesystem::remove(nonExistingFile);
        }
        if (std::filesystem::exists(testDir)) {
            std::filesystem::remove_all(testDir);
        }
    }


    std::string existingFile = "test_file.txt";
    std::string readOnlyFile = "readonly_file.txt";
    std::string nonExistingFile = "non_existing_file.txt";
    std::string testDir = "test_dir";
};

TEST_F(DeleteFileTest, DeleteExistingFile) {
    EXPECT_TRUE(deleteFile(existingFile));
    EXPECT_FALSE(std::filesystem::exists(existingFile));
}

TEST_F(DeleteFileTest, DeleteNonExistingFile) {
    EXPECT_FALSE(deleteFile(nonExistingFile));
}

TEST_F(DeleteFileTest, DeleteDirectory) {
    std::filesystem::create_directory(testDir);
    EXPECT_FALSE(deleteFile(testDir));
    EXPECT_TRUE(std::filesystem::exists(testDir));
}

TEST_F(DeleteFileTest, DeleteReadOnlyFile) {
    EXPECT_TRUE(deleteFile(readOnlyFile));
    EXPECT_FALSE(std::filesystem::exists(readOnlyFile));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
