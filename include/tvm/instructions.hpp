//
// Created by fatih on 4/19/18.
//

#pragma once

#include <tvm/vm_state.hpp>
#include <tvm/tvm_types.hpp>
#include <tvm/instr_traits.hpp>

struct add
{
    constexpr void operator()(vm_state* state, reg_ind_t<> r1, reg_ind_t<> r2)
    {
        state->registers[r1.index] += state->registers[r2.index];
    }
};

template<>
struct instr_name<add>
{
    static constexpr auto value() { return "add"; }
};

struct movi
{
    constexpr void operator()(vm_state* state, reg_ind_t<> r1, operand_t<16> op)
    {
        state->registers[r1.index] = (uint16_t)op.operand;
    }
};

template<>
struct instr_name<movi>
{
    static constexpr auto value() { return "movi"; }
};
