#include "utf/UnitTestFramework.hpp"
#include "test/RBTree.hpp"

#include <random>
#include <vector>

// Framework Check
DeclareTest(module, FrameworkCheck)

// RBTree Check
DeclareTest(module, BlackBalance)
DeclareTest(module, InorderTraversal)
DeclareTest(module, LeftLean)
DeclareTest(module, RemoveMax)
DeclareTest(module, RemoveMin)
DeclareTest(module, BlackBalanceAfterDeletion)
DeclareTest(module, InorderTraversalAfterDeletion)
