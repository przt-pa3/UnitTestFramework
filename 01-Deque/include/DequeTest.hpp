#include "utf/UnitTestFramework.hpp"
#include "test/Deque.hpp"
#include "test/Palindrome.hpp"
#include <random>
#include <deque>

// Framework Check
DeclareTest(module, FrameworkCheck)
 
// ArrayDeque Test
DeclareTest(module, ArrayDequeConstruction)
DeclareTest(module, ArrayDequePushFront)
DeclareTest(module, ArrayDequePushBack)
DeclareTest(module, ArrayDequePushAndResize)
DeclareTest(module, ArrayDequeRandomPushIndexing)
DeclareTest(module, ArrayDequeRemoveFront)
DeclareTest(module, ArrayDequeRemoveBack)
DeclareTest(module, ArrayDequeRemoveInEmpty)
DeclareTest(module, ArrayDequeEmpty)

// ListDeque Test
DeclareTest(module, ListDequeConstruction)
DeclareTest(module, ListDequePushFront)
DeclareTest(module, ListDequePushBack)
DeclareTest(module, ListDequeRemoveFront)
DeclareTest(module, ListDequeRemoveBack)
DeclareTest(module, ListDequeIndexing)

// Palindrome Test
DeclareTest(module, PalindromeArrayDeque)
DeclareTest(module, PalindromeListDeque)