//  Copyright 2022 Google LLC.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

#define VECTOR_TYPE std::vector

template <typename T>
void _ExpectContents(VECTOR_TYPE<T> const& v, T const* arr, unsigned n) {
  ASSERT(v.size() == n);
  ASSERT(v.empty() == (v.size() == 0));  // NOLINT
  ASSERT(v.capacity() >= v.size());

  typename VECTOR_TYPE<T>::const_iterator it;
  typename VECTOR_TYPE<T>::size_type idx;
  for (it = v.begin(), idx = 0; it != v.end(); ++it, ++idx) {
    ASSERT(*it == arr[idx]);
    ASSERT(*it == v[idx]);  // this doesn't hurt
  }
  ASSERT(idx == n);
}

#define ExpectContents(v, a0, a...)                         \
  ({                                                        \
    const __typeof__(a0) _x[] = {a0, a};                    \
    _ExpectContents(v, _x, sizeof(_x) / sizeof(_x[0]));     \
  })

static void TestVectorDefaultCtor() {
  VECTOR_TYPE<int> v;
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
}


static void TestVectorCtorSize() {
  VECTOR_TYPE<int> v(42);
  ASSERT(!v.empty());
  ASSERT(v.size() == 42);
  ASSERT(v.capacity() >= 42);
  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
}

static void TestVectorCopyCtorEmpty() {
  VECTOR_TYPE<int> other;
  VECTOR_TYPE<int> v(other);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
}

static void TestVectorCopyCtorNonEmpty() {
  VECTOR_TYPE<unsigned int> other(42);
  VECTOR_TYPE<unsigned int>::size_type idx;
  VECTOR_TYPE<unsigned int>::iterator it;

  for (it = other.begin(), idx = 0; it != other.end(); ++it, ++idx) *it = idx;

  VECTOR_TYPE<unsigned int> v(other);
  ASSERT(!v.empty());
  ASSERT(v.size() == 42);
  ASSERT(v.capacity() >= 42);

  // check + alter 'v'
  for (it = v.begin(), idx = 0; it != v.end(); ++it, ++idx) {
    ASSERT(*it == idx);
    ASSERT(*it == other[idx]);
    *it = idx * 2;
  }

  // check alterations on 'v'
  for (it = v.begin(), idx = 0; it != v.end(); ++it, ++idx) {
    ASSERT(*it == idx * 2);
  }

  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());
  v.clear();
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= v.size());

  // make sure 'other' was not altered
  for (it = other.begin(), idx = 0; it != other.end(); ++it, ++idx)
    ASSERT(*it == idx);
}

static void TestRangeCtor() {
  uint8_t data[] = {1, 2, 3};
  std::vector<uint8_t> v(data, data + sizeof(data));
  ASSERT(v.size() == sizeof(data));
  ASSERT(!memcmp(v.data(), data, v.size()));
}

static void TestAt() {
  VECTOR_TYPE<int> v(1);
  v.at(0) = 42;
  ASSERT(v.at(0) == 42);
  v.at(0) = 43;
  ASSERT(v.at(0) == 43);
  v[0] = 44;
  ASSERT(v[0] == 44);
}

static void TestReserve() {
  VECTOR_TYPE<int> v;
  VECTOR_TYPE<int>::size_type sz;
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  sz = v.capacity();
  ASSERT(v.capacity() >= v.size());

  // make sure 0 does not change capacity
  v.reserve(0);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // expand
  v.reserve(1);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= 1);
  sz = v.capacity();

  // back to 0, check nothing changed
  v.reserve(0);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // expand
  v.reserve(1024);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= 1024);
  sz = v.capacity();

  // back to same value, check nothing changed
  v.reserve(1024);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // back to smaller value, check nothing changed
  v.reserve(123);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // back to 0, check nothing changed
  v.reserve(0);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // back to 1024, check nothing changed
  v.reserve(1024);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // changing to one more
  v.reserve(1025);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() >= 1025);
  ASSERT(v.capacity() >= sz);
}

static void TestResize() {
  VECTOR_TYPE<int> v;
  VECTOR_TYPE<int>::size_type sz;
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  sz = v.capacity();
  ASSERT(v.capacity() >= v.size());

  // make sure 0 does not change capacity
  v.resize(0);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // expand
  v.resize(1);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1);
  ASSERT(v.capacity() >= 1);
  sz = v.capacity();
  v[0] = 42;
  ASSERT(v[0] == 42);

  // back to 0, check nothing changed
  v.resize(0);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // back to 1, check that v[0] was reset
  v.resize(1);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1);
  ASSERT(v.capacity() == sz);
  ASSERT(v[0] == 0);

  // expand
  v.resize(1024);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1024);
  ASSERT(v.capacity() >= 1024);
  sz = v.capacity();
  v[0] = 43;
  ASSERT(v[0] == 43);
  v[1023] = 44;
  ASSERT(v[1023] == 44);

  // back to same value, check nothing changed
  v.resize(1024);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1024);
  ASSERT(v.capacity() == sz);
  ASSERT(v[0] == 43);
  ASSERT(v[1023] == 44);
  v[0] = 45;
  ASSERT(v[0] == 45);
  v[1023] = 46;
  ASSERT(v[1023] == 46);

  // shrink to smaller value, check nothing changed
  v.resize(123);
  ASSERT(!v.empty());
  ASSERT(v.size() == 123);
  ASSERT(v.capacity() == sz);
  ASSERT(v[0] == 45);
  v[0] = 47;
  ASSERT(v[0] == 47);
  v[122] = 48;
  ASSERT(v[122] == 48);

  // grow to larger value, check copy worked
  v.resize(1024 * 1024);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1024 * 1024);
  ASSERT(v.capacity() >= v.size());
  sz = v.capacity();
  ASSERT(v[0] == 47);
  ASSERT(v[122] == 48);
  v[0] = 49;
  ASSERT(v[0] == 49);
  ASSERT(v[122] == 48);
  v[1024 * 1024 - 1] = 51;
  ASSERT(v[1024 * 1024 - 1] == 51);
  ASSERT(v[122] == 48);
  v[122] = 50;
  ASSERT(v[122] == 50);

  // back to 0, check nothing changed
  v.resize(0);
  ASSERT(v.empty());
  ASSERT(v.size() == 0);  // NOLINT
  ASSERT(v.capacity() == sz);

  // back to 1024, check nothing changed
  v.resize(1024);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1024);
  ASSERT(v.capacity() == sz);
  v[0] = 52;
  ASSERT(v[0] == 52);
  v[1023] = 53;
  ASSERT(v[1023] == 53);

  // changing to one more
  v.resize(1025);
  ASSERT(!v.empty());
  ASSERT(v.size() == 1025);
  ASSERT(v.capacity() >= 1025);
  ASSERT(v.capacity() >= sz);
  ASSERT(v[0] == 52);
  ASSERT(v[1023] == 53);
  v[0] = 54;
  ASSERT(v[0] == 54);
  v[1024] = 55;
  ASSERT(v[1024] == 55);
  ASSERT(v[1023] == 53);
}

static void TestInsertOne() {
  // Test insert at head on empty vec
  {
    VECTOR_TYPE<int> v;
    v.insert(v.begin(), 42);
    ExpectContents(v, 42);
  }
  // Test insert at tail on empty vec
  {
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    ExpectContents(v, 42);
  }
  // Test insert several at head
  {
    VECTOR_TYPE<int> v;
    v.insert(v.begin(), 42);
    v.insert(v.begin(), 43);
    v.insert(v.begin(), 44);
    v.insert(v.begin(), 45);

    ExpectContents(v, 45, 44, 43, 42);

    // Insert 24 inbetween 44 and 43
    VECTOR_TYPE<int>::iterator it = v.begin();
    it++;
    it++;
    it = v.insert(it, 24);
    ExpectContents(v, 45, 44, 24, 43, 42);

    // Insert 23 before our 24
    it = v.insert(it, 23);
    ExpectContents(v, 45, 44, 23, 24, 43, 42);

    // Insert 22 before our last
    it++;
    it++;
    it++;
    it = v.insert(it, 22);
    ExpectContents(v, 45, 44, 23, 24, 43, 22, 42);

    // Insert 21 at end
    it++;
    it++;
    it = v.insert(it, 21);
    ExpectContents(v, 45, 44, 23, 24, 43, 22, 42, 21);

    // Insert 20 at head
    it = v.insert(v.begin(), 20);
    ExpectContents(v, 20, 45, 44, 23, 24, 43, 22, 42, 21);

    // Insert 19 where we left, at head
    v.insert(it, 19);
    ExpectContents(v, 19, 20, 45, 44, 23, 24, 43, 22, 42, 21);
  }

  // Test insert several at tail
  {
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    v.insert(v.end(), 43);
    v.insert(v.end(), 44);
    v.insert(v.end(), 45);

    ExpectContents(v, 42, 43, 44, 45);

    // Insert 24 inbetween 43 and 44
    VECTOR_TYPE<int>::iterator it = v.begin();
    it++;
    it++;
    it = v.insert(it, 24);
    ExpectContents(v, 42, 43, 24, 44, 45);

    // Insert 23 before our 24
    it = v.insert(it, 23);
    ExpectContents(v, 42, 43, 23, 24, 44, 45);

    // Insert 22 before our last
    it++;
    it++;
    it++;
    it = v.insert(it, 22);
    ExpectContents(v, 42, 43, 23, 24, 44, 22, 45);

    // Insert 21 at end
    it++;
    it++;
    it = v.insert(it, 21);
    ExpectContents(v, 42, 43, 23, 24, 44, 22, 45, 21);
  }

  // Test insert head/tail "random"
  {
    VECTOR_TYPE<int> v;
    v.insert(v.begin(), 42);
    v.insert(v.end(), 43);
    v.insert(v.begin(), 44);
    v.insert(v.end(), 45);
    v.insert(v.begin(), 46);

    ExpectContents(v, 46, 44, 42, 43, 45);
  }
}

static void TestInsertIteratorEmpty() {
  // Insert an empty vector in another empty (begin)
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.begin(), other.begin(), other.end());
    ASSERT(v.empty());
    ASSERT(v.size() == 0);  // NOLINT
  }

  // Insert an empty vector in another empty (end)
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.end(), other.begin(), other.end());
    ASSERT(v.empty());
    ASSERT(v.size() == 0);  // NOLINT
  }

  // Insert an empty vector in another non-empty (begin)
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    v.insert(v.end(), 43);
    v.insert(v.begin(), other.begin(), other.end());
    ExpectContents(v, 42, 43);
  }

  // Insert an empty vector in another non-empty (end)
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    v.insert(v.end(), 43);
    v.insert(v.end(), other.begin(), other.end());
    ExpectContents(v, 42, 43);
  }

  // Insert an empty vector in another non-empty (inbetween)
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    VECTOR_TYPE<int>::iterator it = v.insert(v.end(), 43);
    v.insert(it, other.begin(), other.end());
    ExpectContents(v, 42, 43);
  }
}

static void TestInsertIterator() {
  VECTOR_TYPE<int> other;
  other.insert(other.end(), 1);
  other.insert(other.end(), 2);
  other.insert(other.end(), 3);
  ExpectContents(other, 1, 2, 3);

  // Insert an empty vector in another empty (begin)
  {
    VECTOR_TYPE<int> v;
    v.insert(v.begin(), other.begin(), other.end());
    ASSERT(!v.empty());
    ExpectContents(v, 1, 2, 3);
  }

  // Insert an empty vector in another empty (end)
  {
    VECTOR_TYPE<int> v;
    v.insert(v.end(), other.begin(), other.end());
    ExpectContents(v, 1, 2, 3);
  }

  // Insert an empty vector in another non-empty (begin)
  {
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    v.insert(v.end(), 43);
    v.insert(v.begin(), other.begin(), other.end());
    ExpectContents(v, 1, 2, 3, 42, 43);
  }

  // Insert an empty vector in another non-empty (end)
  {
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    v.insert(v.end(), 43);
    v.insert(v.end(), other.begin(), other.end());
    ExpectContents(v, 42, 43, 1, 2, 3);
  }

  // Insert an empty vector in another non-empty (inbetween)
  {
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 42);
    VECTOR_TYPE<int>::iterator it = v.insert(v.end(), 43);
    v.insert(it, other.begin(), other.end());
    ExpectContents(v, 42, 1, 2, 3, 43);
  }

  ExpectContents(other, 1, 2, 3);
}

static void TestAssign() {
  // empty <- empty
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v = other;
    ASSERT(v.empty());
    ASSERT(v.size() == 0);  // NOLINT
  }

  // non-empty <- empty
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 1);
    v.insert(v.end(), 2);
    v.insert(v.end(), 3);
    ExpectContents(v, 1, 2, 3);
    v = other;
    ASSERT(v.empty());
    ASSERT(v.size() == 0);  // NOLINT
  }

  // empty <- non-empty
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    other.insert(other.end(), 1);
    other.insert(other.end(), 2);
    other.insert(other.end(), 3);
    ExpectContents(other, 1, 2, 3);
    v = other;
    ExpectContents(v, 1, 2, 3);
    ExpectContents(other, 1, 2, 3);

    // make sure 'other' is independent
    VECTOR_TYPE<int>::iterator it = v.begin();
    it++;
    v.insert(it, 99);
    ExpectContents(v, 1, 99, 2, 3);
    ExpectContents(other, 1, 2, 3);
  }

  // non-empty <- non-empty
  {
    VECTOR_TYPE<int> other;
    VECTOR_TYPE<int> v;
    v.insert(v.end(), 41);
    v.insert(v.end(), 42);
    v.insert(v.end(), 43);
    other.insert(other.end(), 1);
    other.insert(other.end(), 2);
    other.insert(other.end(), 3);
    ExpectContents(v, 41, 42, 43);
    ExpectContents(other, 1, 2, 3);
    v = other;
    ExpectContents(v, 1, 2, 3);
    ExpectContents(other, 1, 2, 3);

    // make sure 'other' is independent
    VECTOR_TYPE<int>::iterator it = v.begin();
    it++;
    v.insert(it, 99);
    ExpectContents(v, 1, 99, 2, 3);
    ExpectContents(other, 1, 2, 3);
  }

  // Test move constructor/operator.
  {
    VECTOR_TYPE<int> v1, v2;
    v1.push_back(42);
    v2 = std::move(v1);
    ExpectContents(v2, 42);
    ASSERT(v1.empty());  // NOLINT
    VECTOR_TYPE<std::unique_ptr<std::string>> str_vect;
    auto str_ptr = std::make_unique<std::string>("test");
    str_vect.push_back(std::move(str_ptr));
    ASSERT(!str_ptr);
    ASSERT(str_vect.size() == 1);
    const std::string& s = *str_vect[0].get();
    ASSERT(s == "test");
    // NOLINTBEGIN
    // This gives a compiler error without the std::move.
    auto str_vect2 = std::move(str_vect);
    ASSERT(str_vect.empty());
    // NOLINTEND
    ASSERT(str_vect2.size() == 1 && *(str_vect2[0].get()) == "test");
  }

  // Test copy constructor/operator.
  {
    VECTOR_TYPE<std::string> str_vect;
    str_vect.push_back("test");
    ASSERT(str_vect.size() == 1 && str_vect[0] == "test");
    // This gives a compiler error without the std::move.
    auto str_vect2 = str_vect;
    ASSERT(str_vect.size() == 1 && str_vect[0] == "test");
    ASSERT(str_vect2.size() == 1 && str_vect2[0] == "test");
  }
}

// Test the erase method.
void TestErase() {
  VECTOR_TYPE<std::unique_ptr<std::string>> str_vect;
  auto str_ptr1 = std::make_unique<std::string>("Hello");
  auto str_ptr2 = std::make_unique<std::string>("World!");
  str_vect.push_back(std::move(str_ptr1));
  str_vect.push_back(std::move(str_ptr2));
  ASSERT(!str_ptr1);
  ASSERT(!str_ptr2);
  ASSERT(str_vect.size() == 2);
  str_vect.erase(str_vect.begin());
  ASSERT(str_vect.size() == 1);
  const std::string& s = *str_vect[0].get();
  ASSERT(!s.empty());
  ASSERT(s == "World!");
  // Verify the element just past the end was cleared.
  ASSERT(!*str_vect.end());
}

int main() {
  TestVectorDefaultCtor();
  TestVectorCtorSize();
  TestVectorCopyCtorEmpty();
  TestVectorCopyCtorNonEmpty();
  TestRangeCtor();
  TestAt();
  TestReserve();
  TestResize();
  TestInsertOne();
  TestInsertIteratorEmpty();
  TestInsertIterator();
  TestAssign();
  TestErase();
  printf("That's all, folks!\n");
  return 0;
}
