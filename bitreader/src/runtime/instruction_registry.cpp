#include <cstdint>
#include "ruberoid/runtime/instruction_registry.hpp"

#include "ruberoid/runtime/instructions/brd.hpp"
#include "ruberoid/runtime/instructions/call.hpp"
#include "ruberoid/runtime/instructions/ctxc.hpp"
#include "ruberoid/runtime/instructions/ctxf.hpp"
#include "ruberoid/runtime/instructions/ret.hpp"
#include "ruberoid/runtime/instructions/slota.hpp"
#include "ruberoid/runtime/instructions/slotd.hpp"
#include "ruberoid/runtime/instructions/slote.hpp"
#include "ruberoid/runtime/instructions/slotf.hpp"
#include "ruberoid/runtime/instructions/baln.hpp"
#include "ruberoid/runtime/instructions/bskp.hpp"
#include "ruberoid/runtime/instructions/var.hpp"

using namespace rb::runtime;

namespace {
    template <typename... Types>
    struct id_type
    {
    };

    template<typename Instr>
    inline void register_helper(instruction_registry& registry, id_type<Instr>)
    {
        registry.register_instruction<Instr>();
    }

    template<typename Instr, typename... Rest>
    inline void register_helper(instruction_registry& registry, id_type<Instr, Rest...> idt)
    {
        register_helper<Instr>(registry, id_type<Instr>{});
        register_helper<Rest...>(registry, id_type<Rest...>{});
    }
}

void rb::runtime::instruction_registry::register_instructions()
{
    using namespace instructions;

    id_type<
        baln,
        bskp,
        brd<uint8_t>,
        brd<uint16_t>,
        brd<uint32_t>,
        brd<uint64_t>,
        brd<int8_t>,
        brd<int16_t>,
        brd<int32_t>,
        brd<int64_t>,
        call,
        ctxc,
        ctxf,
        ret,
        slota,
        slotd,
        slote,
        slotf,
        var
    > type_list;

    register_helper(*this, type_list);
}