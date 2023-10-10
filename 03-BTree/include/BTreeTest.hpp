#include "utf/UnitTestFramework.hpp"
#include "test/BTree.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

// Framework Check
DeclareTest(module, FrameworkCheck)

// BTree Check
DeclareTest(module, 2_4_InorderTraversal)
DeclareTest(module, InorderTraversal)
DeclareTest(module, NodeUtilization)
DeclareTest(module, PerfectBalance)
DeclareTest(module, Delete)
DeclareTest(module, TraversalAfterDeletion)
DeclareTest(module, NodeUtilizationAfterDeletion)
