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

void *BasicArena::alloc(size_t size) {
  return operator new(size);
}

void BasicArena::add_obj(ArenaObjectBase *aobj) {
  m_objects.push_back(aobj);
}
