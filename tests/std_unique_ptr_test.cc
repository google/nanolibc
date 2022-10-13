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

/* Unit tests for std::unique_pointer */

#include <stdio.h>
#include <string.h>

#include <memory>
#include <string>
#include <utility>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

namespace {

// Just a dummy test class that prints when it is created or destroyed for
// generating a useful golden test for unique_ptr.
class Widget {
 public:
  explicit Widget(const std::string& name) : name_(name) {
    counter_++;
    fprintf(stderr, "Creating widget %s\n", name.c_str());
  }
  Widget(const char* data, size_t len) : name_(std::string(data, len)) {
    counter_++;
    fprintf(stderr, "Creating widget %s\n", data);
  }
  const char* c_str() { return name_.c_str(); }
  const std::string& Name() { return name_; }
  ~Widget() { fprintf(stderr, "Destroying widget %s\n", name_.c_str()); }
  static size_t Counter() { return counter_; }

 private:
  static size_t counter_;
  std::string name_;
};

size_t Widget::counter_ = 0;

void TestConstructors() {
  {
    std::unique_ptr<Widget> p;
    ASSERT(!p);
  }
  {
    std::unique_ptr<Widget> p =                      // NOLINT
        std::unique_ptr<Widget>(new Widget("one"));  // NOLINT
    ASSERT(p->Name() == "one");
  }
  {
    std::unique_ptr<Widget> p = std::unique_ptr<Widget>(  // NOLINT
        new Widget("two"));                               // NOLINT
    ASSERT(p->Name() == "two");
    std::unique_ptr<Widget> q(std::move(p));
    ASSERT(!p);
    ASSERT(q->Name() == "two");
    std::unique_ptr<Widget> r = std::move(q);
    ASSERT(!q);
    ASSERT(r->Name() == "two");
    Widget* w = new Widget("three");
    std::unique_ptr<Widget> s{std::unique_ptr<Widget>(w)};  // NOLINT
    ASSERT(s->Name() == "three");
  }
  {
    auto p = std::unique_ptr<Widget>(nullptr);
    ASSERT(!p);
  }
}

std::unique_ptr<Widget> MakeWidget(const char* name) {
  std::unique_ptr<Widget> p = std::unique_ptr<Widget>(  // NOLINT
      new Widget(name));                                // NOLINT
  ASSERT(p->Name() == name);
  return p;
}

void TestReturnUniquePtr() {
  std::unique_ptr<Widget> p = MakeWidget("test return");
  ASSERT(p->Name() == "test return");
}

void TestMakeUnique() {
  {
    std::unique_ptr<Widget> p = std::make_unique<Widget>(  // NOLINT
        "make_unique");                                    // NOLINT
    ASSERT(p->Name() == "make_unique");
    ASSERT(!!p);
    std::unique_ptr<Widget> q = std::move(p);
    ASSERT(q->Name() == "make_unique");
    ASSERT(!p);
  }
  {
    std::unique_ptr<Widget> p =                   // NOLINT
        std::make_unique<Widget>("var args", 7);  // NOLINT
    ASSERT(p.get() != nullptr);
  }
  {
    std::unique_ptr<Widget> p(std::make_unique<Widget>("test"));  // NOLINT
    ASSERT(p->Name() == "test");
  }
}

// Test the reset method.
void TestReset() {
  auto p = std::make_unique<Widget>("reset");
  ASSERT(p->Name() == "reset");
  p.reset();
  ASSERT(p.get() == nullptr);
}

// Test the assignment operator.
void TestAssign() {
  std::unique_ptr<Widget> p(nullptr);
  ASSERT(p.get() == nullptr);
  p = std::make_unique<Widget>("assign");
  ASSERT(p->Name() == "assign");
}

// Test the default_delete class.  This is not currently used in unique_ptr,
// but it is normally.
void TestDeleter() {
  std::default_delete<Widget> deleter;
  Widget* widget = new Widget("I have been destroyed");
  deleter(widget);
}

void TestOperatorStar() {
  // NOLINTBEGIN
  auto p = std::make_unique<std::string>("test");
  ASSERT(*p == *p.get());
  std::string s = std::move(*p);
  ASSERT(p->empty());
  // NOLINTEND
  ASSERT(s == "test");
}

}  // namespace

int main() {
  TestConstructors();
  TestReturnUniquePtr();
  TestMakeUnique();
  TestReset();
  TestAssign();
  TestDeleter();
  TestOperatorStar();
  fprintf(stderr, "counter = %lu\n", Widget::Counter());
  fprintf(stderr, "PASSED\n");
  return 0;
}
