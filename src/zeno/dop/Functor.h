#pragma once


#include <zeno/ztd/memory.h>
#include <zeno/ztd/map.h>
#include <functional>
#include <typeindex>
#include <vector>
#include <any>
#include <map>


ZENO_NAMESPACE_BEGIN
namespace dop {


struct FuncContext {
    std::vector<ztd::generic_ptr> inputs;
    std::vector<ztd::generic_ptr> outputs;
};


using Functor = std::function<void(FuncContext *)>;
using FuncSignature = std::vector<std::type_index>;


struct FuncOverloads {
    std::map<FuncSignature, Functor> functors;

    void invoke(FuncContext *ctx) const;
};


ztd::map<std::string, FuncOverloads> &overloading_table();
void add_overloading(const char *kind, Functor func, FuncSignature const &sig);


}
ZENO_NAMESPACE_END
