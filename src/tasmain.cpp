//
// Created by fatih on 4/20/18.
//

#include <fstream>
#include <tvm/as/scanner.hpp>
#include <iostream>
#include <tvm/instructions.hpp>
#include <tvm/as/grammar.hpp>
#include <tvm/as/parser.hpp>
#include <tvm/as/ast_printer.hpp>
#include <tvm/as/isa_description.hpp>
#include <tvm/as/codegen.hpp>

using ISA = list <ins<0x01, add>, ins<0x02, movi>>;
using isa_t = isa_map<ISA>;
static constexpr isa_t isa{};

auto isa_descr = tvm::describe(ISA{});

void print_instr(tvm::instr_data& inst)
{
    std::cout << inst.mnemonic() << std::hex << " (0x" << (int)inst.get_opcode() << ")" << std::dec << '\n';
    for (auto& opc : inst.get_operands())
    {
        std::cout << "\tOp: " << (int)opc.type << " " << (int)opc.bits << " bits" << '\n';
    }
}

int main()
{
    for (auto& [name, id] : isa_descr)
    {
        print_instr(*id);
    }

    std::ifstream prog("../bc/add.tcs");
    tvm::as::scanner s{prog};

    std::ifstream dup("../bc/add.tcs");
    tvm::as::parser p{ dup, s.begin() };

    // TODO: where do commas go?
    auto parsed = p.parse_program();
    std::cout << parsed.size() << '\n';

    //tvm::as::codegen dg{parsed, isa_descr};
    //dg.generate();

    for (auto& elem : parsed)
    {
        mpark::visit(tvm::as::print_ast{std::cout, 0}, elem);
        std::cout << '\n';
    }
}