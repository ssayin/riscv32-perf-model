// SPDX-FileCopyrightText: 2022 - 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: MIT

#include "decoder/decoder.hpp"

namespace {
template <typename T> inline op make_store(uint32_t word, masks::store type) {
  T isn{word};
  return op{isn.imm, type, target::store, 0, isn.rs1, isn.rs2, false};
}

template <typename T> inline op make_load(uint32_t word, masks::load type) {
  T isn{word};
  return op{isn.imm, type, target::load, isn.rd, isn.rs, 0, false};
}

template <typename T> inline op make_csr(uint32_t word, masks::sys type) {
  T isn{word};
  return op{isn.csr, type, target::csr, isn.rd, isn.rs, 0, false};
}

template <typename T> inline op make_branch(uint32_t word, masks::branch type) {
  T isn{word};
  return op{isn.imm, type, target::branch, 0, isn.rs1, isn.rs2, false};
}

template <typename T> inline op make_reg_imm(uint32_t word, alu type) {
  T isn{word};
  return op{isn.imm, type, target::alu, isn.rd, isn.rs, 0, true};
}

op decode_load(uint32_t word);
op decode_store(uint32_t word);
op decode_branch(uint32_t word);
op decode_reg_imm(uint32_t word);
op decode_alu(uint32_t word);
op decode_sys(uint32_t word);
op decode_fence(uint32_t word);

} // namespace

op decode(uint32_t word) {
  switch (word) {
  case masks::ecall:
    return op{0, {}, target::ecall, 0, 0, 0, false};
  case masks::ebreak:
    return op{0, {}, target::ebreak, 0, 0, 0, false};
  case masks::mret:
    return op{0, {}, target::mret, 0, 0, 0, false};
  case masks::sret:
  case masks::wfi:
    return make_nop();
  default:
    break;
  }

  switch (masks::opcode{rv32::opc(word)}) {
    using enum masks::opcode;
  case auipc: {
    rv32_auipc isn{word};
    return op{isn.imm, alu::_auipc, target::alu, isn.rd, 0, 0, true, true};
  }
  case lui: {
    rv32_lui isn{word};
    return op{isn.imm, alu::_add, target::alu, isn.rd, 0, 0, true};
  }
  case jal: {
    rv32_jal isn{word};
    return op{isn.imm, alu::_jal, target::alu, isn.rd, 0, 0, true, true};
  }
  case jalr: {
    rv32_jalr isn{word};
    return op{isn.imm, alu::_jalr, target::alu, isn.rd, isn.rs, 0, true};
  }
  case load:
    return decode_load(word);
  case store:
    return decode_store(word);
  case branch:
    return decode_branch(word);
  case reg_imm:
    return decode_reg_imm(word);
  case reg_reg:
    return decode_alu(word);
  case sys:
    return decode_sys(word);
  case misc_mem:
    return decode_fence(word);
  default:
    return make_illegal();
  }
}

namespace {

op decode_load(uint32_t word) {
  switch (masks::load{rv32::funct3(word)}) {
  case masks::load::lb:
    return make_load<rv32_lb>(word, masks::load::lb);
  case masks::load::lh:
    return make_load<rv32_lh>(word, masks::load::lh);
  case masks::load::lw:
    return make_load<rv32_lw>(word, masks::load::lw);
  case masks::load::lbu:
    return make_load<rv32_lbu>(word, masks::load::lbu);
  case masks::load::lhu:
    return make_load<rv32_lhu>(word, masks::load::lhu);
  default:
    return make_illegal();
  }
}

op decode_store(uint32_t word) {
  switch (masks::store{rv32::funct3(word)}) {
  case masks::store::sb:
    return make_store<rv32_sb>(word, masks::store::sb);
  case masks::store::sh:
    return make_store<rv32_sh>(word, masks::store::sh);
  case masks::store::sw:
    return make_store<rv32_sw>(word, masks::store::sw);
  default:
    return make_illegal();
  }
}

op decode_alu_and_remu(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_and isn{word};
    return op{0, alu::_and, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_remu isn{word};
    return op{0, alu::_remu, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_or_rem(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_or isn{word};
    return op{0, alu::_or, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_rem isn{word};
    return op{0, alu::_rem, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_xor_div(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_xor isn{word};
    return op{0, alu::_xor, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_div isn{word};
    return op{0, alu::_div, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_add_sub_mul(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_add isn{word};
    return op{0, alu::_add, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_mul isn{word};
    return op{0, alu::_mul, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x20: {
    rv32_sub isn{word};
    return op{0, alu::_sub, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_sll_mulh(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_sll isn{word};
    return op{0, alu::_sll, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_mulh isn{word};
    return op{0, alu::_mulh, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_srl_sra_divu(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_srl isn{word};
    return op{0, alu::_srl, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_divu isn{word};
    return op{0, alu::_divu, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x20: {
    rv32_sra isn{word};
    return op{0, alu::_sra, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_slt_mulhsu(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_slt isn{word};
    return op{0, alu::_slt, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_mulhsu isn{word};
    return op{0, alu::_mulhsu, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu_sltu_mulhu(uint32_t word) {
  switch (rv32::funct7(word)) {
  case 0x0: {
    rv32_sltu isn{word};
    return op{0, alu::_sltu, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  case 0x1: {
    rv32_mulhu isn{word};
    return op{0, alu::_mulhu, target::alu, isn.rd, isn.rs1, isn.rs2, false};
  }
  default:
    return make_illegal();
  }
}

op decode_alu(uint32_t word) {
  switch (masks::reg_reg{rv32::funct3(word)}) {
    using enum masks::reg_reg;
  case and_remu:
    return decode_alu_and_remu(word);
  case or_rem:
    return decode_alu_or_rem(word);
  case xor_div:
    return decode_alu_xor_div(word);
  case add_sub_mul:
    return decode_alu_add_sub_mul(word);
  case sll_mulh:
    return decode_alu_sll_mulh(word);
  case srl_sra_divu:
    return decode_alu_srl_sra_divu(word);
  case slt_mulhsu:
    return decode_alu_slt_mulhsu(word);
  case sltu_mulhu:
    return decode_alu_sltu_mulhu(word);
  default:
    return make_illegal();
  }
}

op decode_reg_imm(uint32_t word) {
  switch (masks::reg_imm{rv32::funct3(word)}) {
    using enum masks::reg_imm;
  case addi:
    return make_reg_imm<rv32_addi>(word, alu::_add);
  case slti:
    return make_reg_imm<rv32_slti>(word, alu::_slt);
  case xori:
    return make_reg_imm<rv32_xori>(word, alu::_xor);
  case ori:
    return make_reg_imm<rv32_ori>(word, alu::_or);
  case andi:
    return make_reg_imm<rv32_andi>(word, alu::_and);
  case slli:
    return make_reg_imm<rv32_slli>(word, alu::_sll);

  case srli_srai: {
    constexpr int srli = 0x0;
    constexpr int srai = 0x20;
    switch (rv32::funct7(word)) {
    case srli:
      return make_reg_imm<rv32_srli>(word, alu::_srl);
    case srai:
      return make_reg_imm<rv32_srai>(word, alu::_sra);
    default:
      return make_illegal();
    }
  }

  case sltiu: {
    rv32_sltiu isn{word};
    return op{isn.imm, alu::_sltu, target::alu, isn.rd, isn.rs, 0, true};
  }
  default:
    return make_illegal();
  }
}

op decode_branch(uint32_t word) {
  switch (masks::branch{rv32::funct3(word)}) {
  case masks::branch::beq:
    return make_branch<rv32_beq>(word, masks::branch::beq);
  case masks::branch::bne:
    return make_branch<rv32_bne>(word, masks::branch::bne);
  case masks::branch::blt:
    return make_branch<rv32_blt>(word, masks::branch::blt);
  case masks::branch::bltu:
    return make_branch<rv32_bltu>(word, masks::branch::bltu);
  case masks::branch::bge:
    return make_branch<rv32_bge>(word, masks::branch::bge);
  case masks::branch::bgeu:
    return make_branch<rv32_bgeu>(word, masks::branch::bgeu);
  default:
    return make_illegal();
  }
}

op decode_fence(uint32_t word) { return make_nop(); }

op decode_sys(uint32_t word) {
  switch (masks::sys{rv32::funct3(word)}) {
  case masks::sys::csrrw:
    return make_csr<rv32_csrrw>(word, masks::sys::csrrw);
  case masks::sys::csrrs:
    return make_csr<rv32_csrrs>(word, masks::sys::csrrs);
  case masks::sys::csrrc:
    return make_csr<rv32_csrrc>(word, masks::sys::csrrc);
  case masks::sys::csrrwi:
    return make_csr<rv32_csrrwi>(word, masks::sys::csrrwi);
  case masks::sys::csrrsi:
    return make_csr<rv32_csrrsi>(word, masks::sys::csrrsi);
  case masks::sys::csrrci:
    return make_csr<rv32_csrrci>(word, masks::sys::csrrci);
  default:
    return make_illegal();
  }
}
} // namespace
