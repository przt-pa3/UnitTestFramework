#include "utf/UnitTestFramework.hpp"
#include "test/HashTable.hpp"

#include <random>
#include <vector>

// Framework Check
DeclareTest(module, FrameworkCheck)

// HashTable Check
DeclareTest(module, SimpleTest_Linear)
DeclareTest(module, SimpleTest_Quad)
DeclareTest(module, EnlargeInsert_Linear)
DeclareTest(module, EnlargeInsert_Quad)
DeclareTest(module, EnlargeRemove_Linear)
DeclareTest(module, EnlargeRemove_Quad)
DeclareTest(module, RandomTest_Linear)
DeclareTest(module, RandomTest_Quad)
DeclareTest(module, ProbeTest_Linear)
DeclareTest(module, ProbeTest_Quad)

