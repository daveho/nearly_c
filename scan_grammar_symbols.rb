#! /usr/bin/env ruby

# Copyright (c) 2021-2023, David H. Hovemeyer <david.hovemeyer@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR

# Generate grammar_symbols.h and grammar_symbols.cpp from grammar productions
# in parse.y

print "Generating grammar_symbols.h/grammar_symbols.cpp..."
STDOUT.flush

START = 1000
first = true
grammar_symbol_names = []
num_productions = 0
TOKEN_START = 258   # bison token types start at 258
first_token = true
num_tokens = 0

header_fh = File.open('grammar_symbols.h', 'w')
source_fh = File.open('grammar_symbols.cpp', 'w')

header_fh.print <<"EOF1"
#ifndef GRAMMAR_SYMBOLS_H
#define GRAMMAR_SYMBOLS_H

#include "treeprint.h"

//! @file
//! This header defines the GrammarSymbol enumeration, which defines
//! members for each symbol (terminal and nonterminal) used in the
//! grammar. These values are used as tags for the parse tree or
//! AST. It also defines the `node_tag_to_string` function which
//! converts a GrammarSymbol value to a string (useful for printing
//! a textual representation of a ndoe or tree.)

//! Grammar symbol enumeration.
enum GrammarSymbol {
EOF1

STDIN.each_line do |line|
  if m = line.match(/^([a-z_]+)(\s*\/\*.*|\s+)?$/)
    grammar_symbol_names.push(m[1])
    header_fh.print "  NODE_#{m[1]}"
    header_fh.print " = #{START}" if first
    header_fh.puts ","
    first = false
    num_productions += 1
  elsif m = line.match(/^%token(.*)/)
    line = m[1] # skip "%token" at beginning of line

    idx = line.index('>')
    if !idx.nil?
      # A type (%union member) was specified for these tokens,
      # skip to the actual token names
      line = line[idx+1..line.length-1].strip
    end

    line.split(/\s+/).each do |token|
      grammar_symbol_names.push(token)
      header_fh.print "  NODE_#{token}"
      header_fh.print " = #{TOKEN_START}" if first_token
      header_fh.puts ","
      first_token = false
      num_tokens += 1
    end
  end
end

header_fh.print <<"EOF2"
};

//! Get grammar symbol name corresponding to tag (enumeration value).
//! Useful for making sense of a parse tree node based on its tag value.
//! Note that this function doesn't return anything useful for AST
//! node tags, since they don't appear directly in the grammar.
//!
//! @param tag a GrammarSymbol value
//! @return a string with a textual representation of the GrammarSymbol
const char *get_grammar_symbol_name(int tag);

//! Print a parse tree.
class ParseTreePrint : public TreePrint {
public:
  ParseTreePrint();
  ~ParseTreePrint();

  //! Override to convert a parse node's tag to a string.
  //! @param tag parse node tag value
  //! @return the string representation of the parse node tag
  //!         (i.e., the GrammarSymbol)
  virtual std::string node_tag_to_string(int tag) const;
};

#endif // GRAMMAR_SYMBOLS_H
EOF2

source_fh.print <<"EOF3"
#include "grammar_symbols.h"

namespace {
const char *s_grammar_symbol_names[] = {
EOF3
grammar_symbol_names.each do |name|
  source_fh.puts "  \"#{name}\","
end

source_fh.print <<"EOF4"
};
}

const char *get_grammar_symbol_name(int tag) {
  if (tag < #{TOKEN_START}) {
    return NULL;
  }

  if (tag < #{START}) {
    // must be a token
    int which_token = tag - #{TOKEN_START};
    if (which_token >= #{num_tokens}) {
      return NULL;
    } else {
      return s_grammar_symbol_names[which_token];
    }
  }

  int which_production = tag - #{START};
  if (which_production >= #{num_productions}) {
    return NULL;
  }
  return s_grammar_symbol_names[#{num_tokens} + which_production];
}

ParseTreePrint::ParseTreePrint() {
}

ParseTreePrint::~ParseTreePrint() {
}

std::string ParseTreePrint::node_tag_to_string(int tag) const {
  return std::string(get_grammar_symbol_name(tag));
}
EOF4

header_fh.close
source_fh.close

puts "Done!"
