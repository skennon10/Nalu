#include <gtest/gtest.h>
#include <limits>

#include <nalu_make_unique.h>

#include <memory>
#include <vector>

#include "../include/element_promotion/ElementDescription.h"
#include "UnitTestUtils.h"

namespace {

TEST(ElementDescriptionTestQuad, node_map_P1)
{
  std::vector<int> stkNodeMap =
  {
      0, 1, // bottom row of nodes
      3, 2  // top row of nodes
  };

  auto elem = sierra::nalu::ElementDescription::create(2,1);
  EXPECT_EQ(stkNodeMap.size(), elem->nodeMap.size());
  for (unsigned j = 0; j < stkNodeMap.size(); ++j) {
    EXPECT_EQ(stkNodeMap[j], elem->nodeMap[j]);
  }
}

TEST(ElementDescriptionTestQuad, node_map_P2)
{
  std::vector<int> stkNodeMap =
  {
      0, 4, 1, // bottom row of nodes
      7, 8, 5, // middle row of nodes
      3, 6, 2  // top row of nodes
  };

  auto elem = sierra::nalu::ElementDescription::create(2,2);
  EXPECT_EQ(stkNodeMap.size(), elem->nodeMap.size());
  for (unsigned j = 0; j < stkNodeMap.size(); ++j) {
    EXPECT_EQ(stkNodeMap[j], elem->nodeMap[j]);
  }
}

TEST(ElementDescriptionTestQuad, node_map_P4)
{
  std::vector<int> stkNodeMap =
  {
      0, 4, 5, 6, 1,
      15, 16, 17, 18, 7,
      14,19,20,21,8,
      13,22,23,24,9,
      3,12,11,10,2
  };

  auto elem = sierra::nalu::ElementDescription::create(2,4);
  EXPECT_EQ(stkNodeMap.size(), elem->nodeMap.size());
  for (unsigned j = 0; j < stkNodeMap.size(); ++j) {
    EXPECT_EQ(stkNodeMap[j], elem->nodeMap[j]);
  }
}

TEST(ElementDescriptionTestQuad, side_ordinal_map_P2)
{
  std::vector<std::vector<int>> sideNodeMap =
  {
      {0, 1, 4},
      {1, 2, 5},
      {2, 3, 6},
      {3, 0, 7}
  };

  auto elem = sierra::nalu::ElementDescription::create(2,2);
  EXPECT_EQ(4u, elem->sideOrdinalMap.size());
  for (unsigned j = 0; j < 4; ++j) {
    EXPECT_EQ(3u, elem->sideOrdinalMap[j].size());
    for (unsigned i = 0; i < 3u; ++i) {
      EXPECT_EQ(sideNodeMap[j][i], elem->sideOrdinalMap[j][i]);
    }
  }
}

TEST(ElementDescriptionTestQuad, side_ordinal_map_P4)
{
  std::vector<std::vector<int>> sideNodeMap =
  {
      {0, 1, 4, 5, 6},
      {1, 2, 7, 8, 9},
      {2, 3, 10, 11, 12},
      {3, 0, 13, 14, 15}
  };

  auto elem = sierra::nalu::ElementDescription::create(2,4);
  EXPECT_EQ(4u, elem->sideOrdinalMap.size());
  for (unsigned j = 0; j < 4; ++j) {
    EXPECT_EQ(5u, elem->sideOrdinalMap[j].size());
    for (unsigned i = 0; i < 5; ++i) {
      EXPECT_EQ(sideNodeMap[j][i], elem->sideOrdinalMap[j][i]);
    }
  }
}


TEST(ElementDescriptionTestHex, node_map_P2)
{
  // NOTE: the center node has been moved to being numbered last
  // this is just to avoid a re-mapping when doing static condensation
  std::vector<int> stkNodeMap =
  {
      0, 8, 1,
      11, 20, 9,
      3, 10, 2,
      12, 24, 13,
      22, 26, 23,
      15, 25, 14,
      4, 16, 5,
      19, 21, 17,
      7, 18, 6
  };

  auto elem = sierra::nalu::ElementDescription::create(3, 2);
  EXPECT_EQ(stkNodeMap.size(), elem->nodeMap.size());
  for (unsigned j = 0; j < stkNodeMap.size(); ++j) {
    EXPECT_EQ(stkNodeMap[j], elem->nodeMap[j]);
  }
}

TEST(ElementDescriptionTestHex, node_map_P3)
{
  std::vector<int> intendedNodeMap =
  {
      0, 8, 9, 1,
      15, 32, 34, 10,
      14, 33, 35, 11,
      3, 13, 12, 2,
      16, 48, 49, 18,
      40, 56, 57, 44,
      42, 58, 59, 45,
      22, 53, 52, 20,
      17, 50, 51, 19,
      41, 60, 61, 46,
      43, 62, 63, 47,
      23, 55, 54, 21,
      4, 24, 25, 5,
      31, 36, 37, 26,
      30, 38, 39, 27,
      7, 29, 28, 6
  };

  auto elem = sierra::nalu::ElementDescription::create(3, 3);
  EXPECT_EQ(intendedNodeMap.size(), elem->nodeMap.size());
  for (unsigned j = 0; j < intendedNodeMap.size(); ++j) {
    EXPECT_EQ(intendedNodeMap[j], elem->nodeMap[j]);
  }
}

TEST(ElementDescriptionTestHex, side_ordinal_map_P2)
{
  std::vector<std::vector<int>> sideNodeMap =
  {
    {0, 1, 5, 4, 8, 13, 16, 12, 24},
    {1, 2, 6, 5, 9, 14, 17, 13, 23},
    {2, 3, 7, 6, 10, 15, 18, 14, 25},
    {0, 4, 7, 3, 12, 19, 15, 11, 22},
    {0, 3, 2, 1, 11, 10, 9, 8, 20},
    {4, 5, 6, 7, 16, 17, 18, 19, 21}
  };

  auto elem = sierra::nalu::ElementDescription::create(3, 2);

  EXPECT_EQ(6u, elem->sideOrdinalMap.size());
  for (int side_ordinal = 0; side_ordinal < elem->numBoundaries; ++side_ordinal) {
    auto side_node_ordinals = elem->side_node_ordinals(side_ordinal);
    auto expected_side_node_ordinals = sideNodeMap.at(side_ordinal);
    for (unsigned n = 0; n < expected_side_node_ordinals.size(); ++n) {
      EXPECT_EQ(side_node_ordinals[n], expected_side_node_ordinals.at(n));
    }
  }
}

TEST(ElementDescriptionTestHex, side_ordinal_map_P3)
{
  std::vector<std::vector<int>> sideNodeMap =
  {
      { 0, 1, 5, 4, 8, 9, 18, 19, 25, 24, 17, 16, 48, 49, 50, 51 },
      { 1, 2, 6, 5, 10, 11, 20, 21, 27, 26, 19, 18, 44, 45, 46, 47 },
      { 2, 3, 7, 6, 12, 13, 22, 23, 29, 28, 21, 20, 52, 53, 54, 55 },
      { 0, 4, 7, 3, 16, 17, 31, 30, 23, 22, 14, 15, 40, 41, 42, 43 },
      { 0, 3, 2, 1, 15, 14, 13, 12, 11, 10, 9, 8, 32, 33, 34, 35 },
      { 4, 5, 6, 7, 24, 25, 26, 27, 28, 29, 30, 31, 36, 37, 38, 39 }
  };

  auto elem = sierra::nalu::ElementDescription::create(3, 3);
  EXPECT_EQ(6u, elem->sideOrdinalMap.size());
  for (int side_ordinal = 0; side_ordinal < elem->numBoundaries; ++side_ordinal) {
    auto side_node_ordinals = elem->side_node_ordinals(side_ordinal);
    auto expected_side_node_ordinals = sideNodeMap.at(side_ordinal);
    for (unsigned n = 0; n < expected_side_node_ordinals.size(); ++n) {
      EXPECT_EQ(side_node_ordinals[n], expected_side_node_ordinals.at(n));
    }
  }
}

}
//auto sideNodeOrdinals = elem->sideOrdinalMap;
//for (int j = 0; j < 6; ++j) {
//  std::cout << "Face: " << j << "| {";
//  for (int k = 0; k < elem->nodesPerSide; ++k) {
//    std::cout << sideNodeOrdinals.at(j).at(k) << ", ";
//  }
//  std:: cout << "}" << std::endl;
//}
