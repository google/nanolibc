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

/* Unit tests for std::string */

#include <stdio.h>
#include <string.h>

#include <string>
#include <utility>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

namespace {

// Just test we can assign Hello, World!.
void TestAssign() {
  std::string s1 = "Hello, World!";
  std::string s2 = s1;
  ASSERT(s1 == s2);
  ASSERT(s1 == "Hello, World!");
  std::string s3(4, 'x');
  printf("TestAssign passed\n");
}

// Test constructors.
void TestConstructors() {
  // Test default constructor.
  std::string s;
  ASSERT(s.empty());
  ASSERT(s == "");        // NOLINT
  ASSERT(s.size() == 0);  // NOLINT
  const char kData[] = "xxxx";
  std::string s1(4, 'x');
  ASSERT(s1.size() == 4);
  ASSERT(s1 == kData);
  ASSERT(!strcmp(s1.c_str(), kData));
  std::string s2(kData, sizeof(kData) - 1);
  ASSERT(s2 == kData);
  const char kLongData[] = "xxxx\x00";
  // Construction from C strings look for a terminating '\0'.
  std::string temp(kLongData);
  ASSERT(s2 == kLongData);
  const char kHello[] = "Constructor test passed";
  std::string s3(kHello);
  ASSERT(s3 == kHello);
  std::string s4(s3);
  ASSERT(s4 == s3);
  std::string s5 = s4;
  ASSERT(s5 == kHello);
  // Test copy constructor.
  std::string s6(s5);
  ASSERT(s6 == kHello);
  ASSERT(s6.data() != s5.data());
  // NOLINTBEGIN
  // Test move constructor.
  std::string s7 = std::move(s6);
  ASSERT(s7 == kHello);
  ASSERT(s6.empty());
  // NOLINTEND
  // Record in golden test that printing c_str() does not change.
  printf("%s\n", s3.c_str());
  printf("TesstConstructors passed\n");
}

// Test various methods of std::string.
void TestMethods() {
  std::string s1("test");
  ASSERT(!s1.empty());
  ASSERT(s1.substr(2, 2) == "st");
  ASSERT(s1.substr(4, 100).empty());
  s1.clear();
  ASSERT(s1.empty());
  std::string s2("Hello, World!");
  ASSERT(!strcmp(s2.data(), s2.c_str()));
  ASSERT(strlen(s2.data()) == s2.size());
  std::string s3("Hello, ");
  s3.append("World!");
  ASSERT(s3 == "Hello, World!");
  ASSERT(s3.size() ==
         std::string("Hello, ").size() + std::string("World!").size());
  printf("TestMethods passed\n");
}

// Test string indexing operations.
void TestIndexing() {
  std::string s1("Hello, World!");
  ASSERT(s1[0] == 'H' && s1[s1.size() - 1] == '!');
  // Violating the abstraction here: we know there is a trailing '\0' in this
  // implementation.
  ASSERT(s1[s1.size()] == '\0');
  s1[0] = 'h';
  s1[7] = 'w';
  ASSERT(s1 == "hello, world!");
  ASSERT(s1.at(0) == 'h');
  ASSERT(s1.at(1) == 'e');
  ASSERT(s1.at(7) == 'w');
  s1.at(s1.size() - 1) = '.';
  ASSERT(s1 == "hello, world.");
  printf("TestIndexing passed\n");
}

// Test relational operators
void TestRelationalOperators() {
  // From smallest to largest.
  std::string s1("a");
  std::string s2("aa");
  std::string s3("b");
  std::string s4("b\x00", 2);
  ASSERT(s1 == s1);
  ASSERT(s1 != s2);
  ASSERT(s1 < s2);
  ASSERT(s2 < s3);
  ASSERT(s3 < s4);
  ASSERT(s4 > s3);
  ASSERT(s3 > s2);
  ASSERT(s2 > s1);
  ASSERT(s1 <= s2);
  ASSERT(s2 <= s3);
  ASSERT(s3 <= s4);
  ASSERT(s4 >= s3);
  ASSERT(s3 >= s2);
  ASSERT(s2 >= s1);
  ASSERT(s1 <= s1);
  ASSERT(s1 >= s1);
  const char cstr1[] = "aa";
  ASSERT(cstr1 == s2);
  ASSERT(cstr1 != s3);
  ASSERT(cstr1 <= s2);
  ASSERT(cstr1 <= s3);
  ASSERT(cstr1 >= s2);
  ASSERT(cstr1 >= s1);
  ASSERT(cstr1 < s3);
  ASSERT(cstr1 > s1);
  printf("TestRelationalOperators passed\n");
}

// Test the resize method.
void TestResize() {
  std::string s;
  s.resize(4);
  ASSERT(s == std::string(4, '\0'));
  s[0] = 'x';
  s[1] = 'x';
  s[2] = 'x';
  s[3] = 'x';
  s.resize(6);
  ASSERT(s == std::string("xxxx\x00\x00", 6));
  s.resize(3);
  ASSERT(s == std::string("xxx", 3));
  s.resize(4);
  ASSERT(s == std::string("xxx\x00", 4));
  printf("TestResize passed\n");
}

// Test the addition operator.
void TestAdditionOperator() {
  ASSERT(std::string("a") + "b" == "ab");
  // From smallest to largest.
  std::string a("a");
  std::string b("aa");
  std::string c("b");
  std::string d("b\x00", 2);
  ASSERT(a + b == "aaa");
  ASSERT(a + b + c == "aaab");
  ASSERT(a + b + c + d == std::string("aaabb\x00", 6));
  std::string sum;
  ASSERT(sum.empty());
  sum += a;
  sum += b;
  sum += c;
  sum += d;
  ASSERT(sum == a + b + c + d);
  std::string nothing;
  ASSERT(nothing.empty());
  ASSERT(nothing + nothing == nothing);
  ASSERT(a + nothing == a);
  ASSERT(nothing + a == a);
  ASSERT("Hello, " + std::string("World!") == "Hello, World!");
  printf("TestAdditionOperator passed\n");
}

// Test iterating over string characters.
void TestIterators() {
  std::string s("Hello, World!");
  size_t i = 0;
  for (char c : s) {
    ASSERT(c == s[i]);
    i++;
  }
  ASSERT(i == s.size());
  auto it = s.begin();
  i = 0;
  while (it != s.end()) {
    ASSERT(*it == s[i]);
    it++;
    i++;
  }
  ASSERT(static_cast<size_t>(it - s.begin()) == s.size());
  printf("TestIterators passed\n");
}

// Since C++11, the string::data() returned pointer can be written to legally.
void TestWriteData() {
  std::string s = "test";
  char* p = s.data();
  p[0] = 'T';
  p[3] = 'T';
  ASSERT(s == "TesT");
  printf("TesstWriteData  passed\n");
}

// Test the replace method.  It must behave well even when an attacker controls
// the inputs.
void TestReplace() {
  std::string buf("hello");
  std::string& result = buf.replace(0, 1, "H");
  ASSERT(buf == "Hello");
  // Make sure we returned *this.
  ASSERT(result.data() == buf.data());
  buf.replace(5, std::string::npos, ", World!");
  ASSERT(buf == "Hello, World!");
  buf.replace(7, 1, std::string("great big w"));
  ASSERT(buf == "Hello, great big world!");
  buf.replace(7, 10, "");
  ASSERT(buf == "Hello, world!");
  buf.replace(6, 0, "");
  ASSERT(buf == "Hello, world!");
  buf.replace(0, 0, "!");
  ASSERT(buf == "!Hello, world!");
  printf("TestReplace passed\n");
}

// Test any supported std::to_string() functions.
void TestToString() {
  ASSERT(std::to_string(0xdeadbeef) == "3735928559");
  printf("TestToString passed\n");
}

}  // namespace

int main() {
  TestAssign();
  TestConstructors();
  TestMethods();
  TestResize();
  TestRelationalOperators();
  TestAdditionOperator();
  TestIndexing();
  TestIterators();
  TestWriteData();
  TestReplace();
  TestToString();
  printf("PASSED\n");
  return 0;
}
