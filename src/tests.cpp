//
// Created by fatih on 4/20/18.
//

#include <tvm/instr_traits.hpp>
#include <tvm/tvm_types.hpp>
#include <tvm/exec/decoding.hpp>

namespace tvm
{
    static_assert(decode_one<opcode_t<7>, 1>(0x0FF).opcode == 0x7F, "");
    static_assert(decode_one<opcode_t<7>, 2>(0x0FF).opcode == 0x3F, "");

    static_assert(std::is_same<offsets<list< opcode_t<7> >>::type, std::index_sequence<0>>{}, "");
    static_assert(std::is_same<offsets<list< opcode_t<7>, reg_ind_t<> >>::type, std::index_sequence<4, 0>>{}, "");
    static_assert(std::is_same<offsets<list< opcode_t<7>, reg_ind_t<>, reg_ind_t<> >>::type, std::index_sequence<8, 4, 0>>{}, "");

    static_assert(decode(list<opcode_t<7>>{}, 0xFF) == std::tuple<opcode_t<7>>{ {0x7F} }, "");
    static_assert(decode(list<opcode_t<7>, reg_ind_t<>>{}, 0xFF) == std::tuple<opcode_t<7>, reg_ind_t<>>{ {0x0F}, {0x0F} }, "");
}