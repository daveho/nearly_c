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

#include "arena.h"

////////////////////////////////////////////////////////////////////////
// ArenaObjectBase member functions
////////////////////////////////////////////////////////////////////////

ArenaObjectBase::ArenaObjectBase() {
}

ArenaObjectBase::~ArenaObjectBase() {
}

////////////////////////////////////////////////////////////////////////
// Arena member functions
////////////////////////////////////////////////////////////////////////

Arena::Arena() {
}

Arena::~Arena() {
}

////////////////////////////////////////////////////////////////////////
// BasicArena member functions
////////////////////////////////////////////////////////////////////////

BasicArena::BasicArena() {
}

BasicArena::~BasicArena() {
  for (auto i = m_objects.begin(); i != m_objects.end(); i++) {
    ArenaObjectBase *aobj = *i;

    // get raw buffers for the allocated object and the ArenaObject
    void *obj_buf = aobj->get_obj();
    void *aobj_buf = static_cast<void *>(aobj);

    // destroy the allocated object
    aobj->on_destroy();

    // delete the memory used by the allocated and ArenaObject buffers
    operator delete(obj_buf);
    operator delete(aobj_buf);
  }
}

void *BasicArena::alloc(size_t size, size_t /* align */) {
  return operator new(size);
}

void BasicArena::add_obj(ArenaObjectBase *aobj) {
  m_objects.push_back(aobj);
}

////////////////////////////////////////////////////////////////////////
// ChunkedArena::Chunk member functions
////////////////////////////////////////////////////////////////////////

ChunkedArena::Chunk::Chunk(size_t capacity)
  : m_buf(static_cast<char *>(operator new(capacity)))
  , m_capacity(capacity)
  , m_used(0U) {
}

ChunkedArena::Chunk::~Chunk() {
  operator delete(static_cast<void *>(m_buf));
}

// Figure out the offset where an allocation of a buffer
// (with specified alignment) would start
size_t ChunkedArena::Chunk::get_start_offset(size_t align) {
  // alignment must be a power of 2
  assert(align != 0U);
  assert((align & (align - 1U)) == 0U);

  // see where next allocation would start (based on current number
  // of bytes used, and alignment requirements of allocation)
  size_t next_start = m_used;
  size_t misaligned_by = next_start & (align - 1U);
  if (misaligned_by > 0) {
    next_start += (align - misaligned_by);
  }
  assert((next_start & (align - 1U)) == 0U);

  return next_start;
}

// Determine whether an allocation with specified size and alignment
// would succeed
bool ChunkedArena::Chunk::can_alloc(size_t size, size_t align) {
  size_t end = get_start_offset(align) + size;
  return end <= m_capacity;
}

// Allocate a buffer with specified size and alignment: should only
// be called if can_alloc() returned true
char *ChunkedArena::Chunk::alloc(size_t size, size_t align) {
  assert(can_alloc(size, align));
  size_t next_start = get_start_offset(align);
  size_t end = next_start + size;
  m_used = end;
  return m_buf + next_start;
}

////////////////////////////////////////////////////////////////////////
// ChunkedArena member functions
////////////////////////////////////////////////////////////////////////

ChunkedArena::ChunkedArena(size_t initial_size)
  : m_total_size(initial_size) {
  m_chunks.push_back(new Chunk(initial_size));
}

ChunkedArena::~ChunkedArena() {
  // run object destructors
  for (auto i = m_objects.begin(); i != m_objects.end(); i++) {
    ArenaObjectBase *aobj = *i;
    aobj->on_destroy();
  }

  // free the chunks (thus freeling all of the memory)
  for (auto i = m_chunks.begin(); i != m_chunks.end(); i++) {
    Chunk *chunk = *i;
    delete chunk;
  }
}

void *ChunkedArena::alloc(size_t size, size_t align) {
  Chunk *current = m_chunks.back();

  if (!current->can_alloc(size, align)) {
    // Create a new chunk
    Chunk *next_chunk = new Chunk(m_total_size * 2);
    m_chunks.push_back(next_chunk);
    m_total_size += next_chunk->m_capacity;
    current = next_chunk;
    assert(current->can_alloc(size, align));
  }

  return current->alloc(size, align);
}

void ChunkedArena::add_obj(ArenaObjectBase *aobj) {
  m_objects.push_back(aobj);
}
