// Copyright (c) 2021, David H. Hovemeyer <david.hovemeyer@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef ARENA_H
#define ARENA_H

#include <vector>

class ArenaObjectBase {
protected:
  ArenaObjectBase();

public:
  virtual ~ArenaObjectBase();

  virtual void on_destroy() = 0;
};

template<typename ObjType>
class ArenaObject : public ArenaObjectBase {
private:
  ObjType *m_obj;

public:
  ArenaObject(ObjType *obj) : m_obj(obj) { }
  virtual ~ArenaObject() { }

  virtual void on_destroy() {
    delete m_obj;
  }
};

// An Arena is a mechanism for allocating objects of varying types so
// that it is possible to delete all of the objects at once.
// Objects allocated this way should *not* try to explicitly
// delete other objects that are allocated in the same Arena.
class Arena {
private:
  std::vector<ArenaObjectBase *> m_objects;

  // copy constructor and assignment operator are prohibited
  Arena(const Arena &);
  Arena &operator=(const Arena &);

public:
  Arena();
  ~Arena();

  // Allocate an object of an arbitrary type with arbitrary constructor
  // arguments. The object will be registered as being part of the Arena,
  // and its memory will be de-allocated when the Arena is destroyed.
  template<typename ObjType, typename... Args>
  ObjType *create(Args... args) {
    ObjType *obj = new ObjType(args...);
    ArenaObjectBase *aobj = new ArenaObject<ObjType>(obj);
    m_objects.push_back(aobj);
    return obj;
  }
};


#endif // ARENA_H
