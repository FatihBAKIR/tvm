#include <iostream>

#include <tvm/meta.hpp>
#include <tvm/vm_state.hpp>
#include <tvm/tvm_types.hpp>
#include <tvm/traits.hpp>
#include <tvm/instr_traits.hpp>
#include <tvm/exec/decoding.hpp>
#include <tvm/exec/execution.hpp>
#include <tvm/instructions.hpp>
#include <tvm/exec/isa.hpp>

#include <fstream>
#include <vector>

using ISA = list <ins<0x01, add>, ins<0x02, movi>, ins<0x03, exit_ins>>;

template <uint8_t N>
constexpr tvm::executor get_executor(opcode_t<N> c)
{
    constexpr auto lookup = tvm::gen_lookup<ISA>::value();
    return lookup.data[c.opcode];
}

constexpr uint8_t exec_one(vm_state *state, uint32_t instr)
{
    return get_executor(get_opcode(instr))(state, instr);
}

struct ptr_fetcher
{
    constexpr uint32_t fetch(uint16_t pc);
    uint8_t* m_base;
};

struct executor
{
    ptr_fetcher m_fetcher;
    vm_state m_state;

    constexpr executor(ptr_fetcher fetcher)
            : m_fetcher(std::move(fetcher)), m_state{}
    {}

    constexpr void exec_one();
    constexpr void exec();
};

constexpr uint32_t ptr_fetcher::fetch(uint16_t pc)
{
    auto p = m_base + pc;
    uint32_t res = 0;
    res |= *p++; res <<= 8;
    res |= *p++; res <<= 8;
    res |= *p++; res <<= 8;
    res |= *p++;
    return res;
}

constexpr void executor::exec_one()
{
    m_state.pc += ::exec_one(&m_state, m_fetcher.fetch(m_state.pc));
}

constexpr void executor::exec()
{
    while (m_state.registers[15] != 0xDEAD)
    {
        exec_one();
    }
}

int main() {
    std::ifstream in("a.out", std::ios::binary);
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());

    constexpr uint8_t prog[14] = {
            0x04, 0x00, 0x00, 0x40, // mov %r0, 2
            0x04, 0x20, 0x00, 0x80, // mov %r1, 4
            0x02, 0x02,             // add %r0, %r1
            0x06, 0x00, 0x00, 0x00  // exit
    };

    ptr_fetcher fetch{contents.data()};
    executor exec(fetch);
    exec.exec();

    std::cout << exec.m_state.registers[0] << '\n';

    return 0;
}