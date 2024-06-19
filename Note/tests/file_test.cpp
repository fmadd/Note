#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "../src/utils.hpp"

namespace fs = std::filesystem;

class CountFilesTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directory(testDir);
    }

    void TearDown() override {
        fs::remove_all(testDir);
    }

    std::string testDir = "test_directory";
};

TEST_F(CountFilesTest, NoFiles) {
    EXPECT_EQ(countFilesInDirectory(testDir, ".txt"), 0);
}

TEST_F(CountFilesTest, OnlyMatchingExtensionFiles) {
    std::ofstream(testDir + "/file1.txt");
    std::ofstream(testDir + "/file2.txt");

    EXPECT_EQ(countFilesInDirectory(testDir, ".txt"), 2);
}

TEST_F(CountFilesTest, MixedExtensions) {
    std::ofstream(testDir + "/file1.txt");
    std::ofstream(testDir + "/file2.cpp");
    std::ofstream(testDir + "/file3.txt");
    std::ofstream(testDir + "/file4.cpp");

    EXPECT_EQ(countFilesInDirectory(testDir, ".txt"), 2);
}

TEST_F(CountFilesTest, NoMatchingExtension) {
    std::ofstream(testDir + "/file1.cpp");
    std::ofstream(testDir + "/file2.cpp");

    EXPECT_EQ(countFilesInDirectory(testDir, ".txt"), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}