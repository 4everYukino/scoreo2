#include "rtlib/path_normalize.h"

#include <gtest/gtest.h>

#include <string>

TEST(PathNormalizeTest, CollapsesDotAndRepeatedSeparators)
{
    std::string path = "foo//./bar///baz";
    path_normalize(path, '/');
    EXPECT_EQ(path, "foo/bar/baz");
}

TEST(PathNormalizeTest, ResolvesParentSegmentsInRelativePaths)
{
    std::string path = "alpha/beta/../gamma";
    path_normalize(path, '/');
    EXPECT_EQ(path, "alpha/gamma");
}

TEST(PathNormalizeTest, PreservesLeadingParentSegmentsWhenRelative)
{
    std::string path = "../../alpha/../beta";
    path_normalize(path, '/');
    EXPECT_EQ(path, "../../beta");
}

TEST(PathNormalizeTest, DoesNotWalkAboveRootForAbsolutePaths)
{
    std::string path = "/../../alpha//beta/..";
    path_normalize(path, '/');
    EXPECT_EQ(path, "/alpha");
}

TEST(PathNormalizeTest, KeepsRootAsSingleSeparator)
{
    std::string path = "////";
    path_normalize(path, '/');
    EXPECT_EQ(path, "/");
}

TEST(PathNormalizeTest, ReducesCurrentDirectoryToEmptyRelativePath)
{
    std::string path = "././.";
    path_normalize(path, '/');
    EXPECT_TRUE(path.empty());
}
