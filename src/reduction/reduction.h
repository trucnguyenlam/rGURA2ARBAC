#pragma once

#include "rGURAModel.h"
#include "parser/rGURALexer.h"
#include "parser/rGURAParser.h"
#include "parser/myrGURAListener.h"

namespace VAC {
class Reduction {
  public:
    std::string reduceRGURAPolicy(const std::string filename, bool);
  private:
    std::string to_ARBACURA_precondition(PreconditionPtr p, rGURAPtr policy) const ;
    std::string to_ARBACURA_policy(rGURAPtr policy, bool) const;

};

} // VAC


