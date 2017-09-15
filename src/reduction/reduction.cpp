// @author: trucnguyenlam@gmail.com
// @description:
//      reduction from rGURA to ARBAC-URA
// TODO:
//
// @changeLog:
//    2017.09.15   Update translation of spec 
//    2017.05.09   Initial version

#include "reduction.h"

using namespace VAC;

namespace {

std::string to_ARBACURA_precondition(PreconditionPtr p, rGURAPtr policy) {
    std::string ret = "";
    if (p->isTrue) {
        ret += "TRUE";
    } else {
        for (const auto & role : p->getPt()) {  // Pt
            ret += role->getAttribute() + "_" + role->getValue() + " & ";
        }
        for (const auto & role : p->getNt()) {  // Nt
            ret += "-" + role->getAttribute() + "_" + role->getValue() + " & ";
        }
        // Additional
        for (const auto & role : p->getPt()) {  // Pt
            if (policy->getAttribute(role->getAttribute())->isSingle())  {// FIXME: Potential nullptr
                // FIXME: Potential nullptr
                for (const auto& v : policy->getScope()->getDomain(role->getAttribute())->getValues()) {
                    if (v != role->getValue()) {
                        ret += "-" + role->getAttribute() + "_" + v + " & ";
                    }
                }
            }
        }
        if (ret.size() > 2) {
            ret.pop_back();
            ret.pop_back();
        }
    }
    return ret;
}

// Private
std::string to_ARBACURA_policy(rGURAPtr policy, bool debug) {
    std::string user_str, role_str, ua_str, ca_str, cr_str, spec_str;
    // 1. User and Administrative Role
    user_str = "USERS\n";
    for (const auto& u : policy->getUsers()) {
        user_str += u->getName() + '\n';
    }
    role_str = "ROLES\n";
    ua_str = "UA\n";
    int i = 0;
    for (const auto& r : policy->getAdmin_roles()) {
        role_str += r + '\n';
        // Add administrators to the list of users
        user_str += "ADMINUSER_" + std::to_string(i) + '\n';
        // Add administrator to ua
        ua_str += "<ADMINUSER_" + std::to_string(i) + ", " + r + ">\n";
        i++;
    }
    if (i == 0) {
        throw VAC::ReductionException("Administrative Roles is empty!");
    }

    user_str += ";\n";

    // 2. Scope
    for (auto it = policy->getScope()->getDomains().begin(); it != policy->getScope()->getDomains().end(); it++) {
        std::string attrname = it->first;
        for (const auto & v : it->second->getValues()) {
            role_str += attrname + "_" + v + '\n';
        }
    }

    // 3. UAttri
    for (const auto& u : policy->getUsers()) {
        for (const auto& a : u->getConfiguration()) {
            for (const auto& v : a->getValue()) {
                ua_str += "<" + u->getName() + ", " + a->getName() + "_" + v->getVal() + ">\n";
            }
        }
    }
    ua_str += ";\n";

    // 4. Can_assign
    ca_str = "CA\n";
    for (const auto& r : policy->getAssign_rules()) {
        ca_str += "<" + r->getAdmin() + ", ";
        ca_str += to_ARBACURA_precondition(r->getPrecondition(), policy);
        ca_str += ", " + r->getTarget()->getAttribute() + "_" + r->getTarget()->getValue() + ">\n";
    }

    // 5. Can_add
    for (const auto& r : policy->getAdd_rules()) {
        ca_str += "<" + r->getAdmin() + ", ";
        ca_str += to_ARBACURA_precondition(r->getPrecondition(), policy);
        ca_str += ", " + r->getTarget()->getAttribute() + "_" + r->getTarget()->getValue() + ">\n";
    }

    // 6. Can_delete
    cr_str = "CR\n";
    for (const auto & r : policy->getDelete_rules()) {
        cr_str += "<" + r->getAdmin() + ", " + r->getTarget()->getAttribute() + "_" + r->getTarget()->getValue() + ">\n";
    }
    cr_str += ";\n";
    // 7. Query
    spec_str = "SPEC role_Target;\n";

    // Add new target role to role_str
    role_str += "role_Target\n";
    role_str += ";\n";

    // add new rule to can_assign
    // pick any admin from AR (the first one)
    for (const auto& r : policy->getAdmin_roles()) {
        ca_str += "<" + r + ", ";
        ca_str += to_ARBACURA_precondition(policy->getQuery(), policy);
        ca_str += ", role_Target>\n";
        break;
    }    
    ca_str += ";\n";

    return (user_str + role_str + ua_str + ca_str + cr_str + spec_str);
}


} // Annonymous namespace


std::string Reduction::reduceRGURAPolicyToARBACURA(const std::string filename, bool debug) {
    std::ifstream stream;
    stream.open(filename);

    if (not stream.good()) {
        throw ParserException("Error: file" + std::string(filename) + " does not exist!");
    }

    antlr4::ANTLRInputStream input(stream);
    rGURALexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    // Create parser and parse the policy
    rGURAParser parser(&tokens);
    antlr4::tree::ParseTree * program = parser.file();

    // Work through parser tree to produce the model
    myrGURAListener listener;
    antlr4::tree::ParseTreeWalker mywalker;
    mywalker.walk(&listener, program);

    stream.close();

    // Get the policy from parsed tree (AST)
    rGURAPtr policy = listener.getPolicy();

    if (debug) {
        std::cout << policy->to_string();
    }

    std::string ret = "";
    ret = to_ARBACURA_policy(policy, debug);
    return ret;
}

