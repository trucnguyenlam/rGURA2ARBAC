#pragma once

#include "rGURAModel.h"
#include "parser/rGURALexer.h"
#include "parser/rGURAParser.h"
#include "parser/myrGURAListener.h"

namespace VAC {

class ReductionException: public ParserException {
 public:
  ReductionException(const std::string &msg = "") : ParserException(msg) {};
};

class Reduction {
  public:
    std::string reduceRGURAPolicyToARBACURA(const std::string filename, bool);
};

} // VAC


