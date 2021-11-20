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

#include <cstddef>
#include <cassert>
#include <vector>

class ArenaObjectBase {
protected:
  ArenaObjectBase();

public:
  virtual ~ArenaObjectBase();

  virtual void on_destroy() = 0;
  virtual void *get_obj() = 0;
};

template<typename ObjType>
class ArenaObject : public ArenaObjectBase {
private:
  ObjType *m_obj;

public:
  ArenaObject(ObjType *obj) : m_obj(obj) { }
  virtual ~ArenaObject() { }

  virtual void on_destroy() {
    // Just call the destructor: the Arena is responsible for
    // freeing the memory.
    m_obj->~ObjType();
  }

  virtual void *get_obj() { return static_cast<void *>(m_obj); }
};

// An Arena is a mechanism for allocating objects of varying types so
// that it is possible to delete all of the objects at once.
// Objects allocated this way should *not* try to explicitly
// delete other objects that are allocated in the same Arena.
//
// This is an abstract base class. The concrete implementations
// are BasicArena and ChunkedArena.
class Arena {
private:
  // copy constructor and assignment operator are prohibited
  Arena(const Arena &);
  Arena &operator=(const Arena &);

public:
  Arena();
  virtual ~Arena();

  // Allocate an object of an arbitrary type with arbitrary constructor
  // arguments. The object will be registered as being part of the Arena,
  // and when the Arena is destroyed, the object will be destroyed
  // (i.e., its destructor will be called), and its memory will be
  // automatically deallocated.
  template<typename ObjType, typename... Args>
  ObjType *create(Args... args) {
    // Allocate and initialize the object
    void *obj_buf = alloc(sizeof(ObjType), alignof(ObjType));
    ObjType *obj = new(obj_buf) ObjType(args...);

    // Allocate and initialize the ArenaObject that will handle calling
    // the object's destructor
    void *aobj_buf = alloc(sizeof(ArenaObject<ObjType>), alignof(ArenaObject<ObjType>));
    ArenaObjectBase *aobj = new(aobj_buf) ArenaObject<ObjType>(obj);

    // Register the object as being allocated within the Arena
    add_obj(aobj);

    return obj;
  }

protected:
  virtual void *alloc(size_t size, size_t align) = 0;
  virtual void add_obj(ArenaObjectBase *aobj) = 0;
};

// BasicArena uses operator new and operator delete to allocate
// memory.
class BasicArena : public Arena {
private:
  std::vector<ArenaObjectBase *> m_objects;

public:
  BasicArena();
  virtual ~BasicArena();

protected:
  virtual void *alloc(size_t size, size_t align);
  virtual void add_obj(ArenaObjectBase *aobj);
};

// ChunkedArena allocates large-ish "chunks" of memory, and then
// allocates objects (and ArenaObjects needed as metadata)
// within the chunks. In theory this is more efficient (in both
// time and space) than using operator new and operator delete
// for each allocation.
class ChunkedArena : public Arena {
public:
  // Size of initial chunk.
  // Total amount of reserved memory doubles each time
  // we run out of space.  I.e., by default, additional chunks are
  // allocated as 64KB, 128KB, 256KB, etc.
  static const size_t DEFAULT_INITIAL_SIZE = 64*1024;

private:
  struct Chunk {
    char *m_buf;
    size_t m_capacity, m_used;

    Chunk(size_t capacity);
    ~Chunk();
    size_t get_start_offset(size_t align);
    bool can_alloc(size_t size, size_t align);
    char *alloc(size_t size, size_t align);
  };

  size_t m_total_size;
  std::vector<Chunk *> m_chunks;
  std::vector<ArenaObjectBase *> m_objects;

public:
  // Note that the initial chunk size must be at least as large as
  // the largest object which will be allocated in the arena.
  ChunkedArena(size_t initial_size = DEFAULT_INITIAL_SIZE);
  virtual ~ChunkedArena();

protected:
  virtual void *alloc(size_t size, size_t align);
  virtual void add_obj(ArenaObjectBase *aobj);
};

#endif // ARENA_H
