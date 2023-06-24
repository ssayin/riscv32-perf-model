// SPDX-FileCopyrightText: 2022 - 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: MIT

#ifndef DECODER_DECODER_HPP
#define DECODER_DECODER_HPP

#include "common/common.hpp"

struct rv32 {
  constexpr static uint8_t funct7(uint32_t w) { return offset(w, 25U, 31U); }
  constexpr static uint8_t funct3(uint32_t w) { return offset(w, 12U, 14U); }
  constexpr static uint8_t rs2(uint32_t w) { return offset(w, 20U, 24U); }
  constexpr static uint8_t rs1(uint32_t w) { return offset(w, 15U, 19U); }
  constexpr static uint8_t opc(uint32_t w) { return offset(w, 0U, 6U); }
  constexpr static uint8_t rd(uint32_t w) { return offset(w, 7U, 11U); }

  constexpr static uint32_t imm_i(uint32_t x) {
    return static_cast<uint32_t>(static_cast<int32_t>(x) >> 20);
  }

  constexpr static uint32_t imm_s(uint32_t x) {
    return (offset(x, 7U, 11U) |
            static_cast<uint32_t>(static_cast<int32_t>(x & 0xFE000000) >> 20));
  }

  constexpr static uint32_t imm_b(uint32_t x) {
    return ((offset(x, 8U, 11U) << 1) | (offset(x, 25U, 30U) << 5) |
            (offset(x, 7U, 7U) << 11) |
            static_cast<uint32_t>(static_cast<int32_t>(x & masks::sign_bit) >>
                                  19)) &
           0xFFFFFFFE;
  }

  constexpr static uint32_t imm_u(uint32_t x) { return x & masks::type_u_imm; }

  constexpr static uint32_t imm_j(uint32_t x) {
    return (imm_i(x) & 0xFFF007FE) | (offset(x, 12U, 19U) << 12) |
           (offset(x, 20U, 20U) << 11);
  }

  constexpr static uint16_t csr(uint32_t x) { return offset(x, 20U, 31U); }

  constexpr static uint32_t rvc_imm11(uint16_t x) {
    return (offset(x, 2U, 2U) << 5) | (offset(x, 3U, 5U) << 1) |
           (offset(x, 6U, 6U) << 7) | (offset(x, 7U, 7U) << 6) |
           (offset(x, 8U, 8U) << 10) | (offset(x, 9U, 10U) << 8) |
           (offset(x, 11U, 11U) << 4) |
           static_cast<uint32_t>(
               static_cast<int32_t>((offset(x, 12U, 12U) << 31)) >> 20);
  }

  constexpr static uint8_t rvc_97(uint16_t x) {
    return offset(x, 7U, 9U) | 0b01000;
  }

  constexpr static uint8_t rvc_42(uint16_t x) {
    return offset(x, 2U, 4U) | 0b01000;
  }

  constexpr static uint8_t rvc_62(uint16_t x) { return offset(x, 2U, 6U); }

  //  constexpr static uint32_t rvc_imm5(uint16_t x) {}
  // constexpr static uint32_t rvc_uimm5(uint16_t x) {}

  //  constexpr static uint32_t rvc_imm9(uint16_t x) {}

  // constexpr static uint32_t rvc_uimm9(uint16_t x) {}

  // constexpr static uint32_t rvc_imm17(uint16_t x) {}

  // constexpr static uint32_t rvc_imm8(uint16_t x) {}
};

struct rv32_jal {
  uint32_t imm;
  uint8_t  rd;
  rv32_jal(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

struct rv32_op32 {
  uint8_t rd;
  uint8_t rs1;
  uint8_t rs2;
  rv32_op32(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

using rv32_sll = rv32_op32;
using rv32_srl = rv32_op32;
using rv32_sra = rv32_op32;
using rv32_add = rv32_op32;
using rv32_sub = rv32_op32;

using rv32_slt  = rv32_op32;
using rv32_sltu = rv32_op32;
using rv32_xor  = rv32_op32;
using rv32_or   = rv32_op32;
using rv32_and  = rv32_op32;

using rv32_mulh   = rv32_op32;
using rv32_divu   = rv32_op32;
using rv32_mul    = rv32_op32;
using rv32_mulhsu = rv32_op32;
using rv32_mulhu  = rv32_op32;
using rv32_div    = rv32_op32;
using rv32_rem    = rv32_op32;
using rv32_remu   = rv32_op32;

struct rv32_opimm32 {
  uint32_t imm;
  uint8_t  rd;
  uint8_t  rs;
  rv32_opimm32(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

using rv32_addi  = rv32_opimm32;
using rv32_xori  = rv32_opimm32;
using rv32_ori   = rv32_opimm32;
using rv32_andi  = rv32_opimm32;
using rv32_slti  = rv32_opimm32;
using rv32_sltiu = rv32_opimm32;

using rv32_lb  = rv32_opimm32;
using rv32_lh  = rv32_opimm32;
using rv32_lw  = rv32_opimm32;
using rv32_lbu = rv32_opimm32;
using rv32_lhu = rv32_opimm32;

using rv32_jalr = rv32_opimm32;

struct rv32_opimm32_shamt {
  uint8_t rd;
  uint8_t rs;
  uint8_t imm;
  rv32_opimm32_shamt(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

using rv32_slli = rv32_opimm32_shamt;
using rv32_srli = rv32_opimm32_shamt;
using rv32_srai = rv32_opimm32_shamt;

struct rv32_lui {
  uint32_t imm;
  uint8_t  rd;
  rv32_lui(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

struct rv32_auipc {
  uint32_t imm;
  uint8_t  rd;
  rv32_auipc(uint32_t word) { unpack(word); }
  void unpack(uint32_t word) {
    rd  = rv32 ::rd(word);
    imm = rv32 ::imm_u(word);
  }
};

struct rv32_store {
  uint32_t imm;
  uint8_t  rs1;
  uint8_t  rs2;
  rv32_store(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

using rv32_sb = rv32_store;
using rv32_sh = rv32_store;
using rv32_sw = rv32_store;

struct rv32_branch {
  uint32_t imm;
  uint8_t  rs1;
  uint8_t  rs2;
  rv32_branch(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

using rv32_beq  = rv32_branch;
using rv32_bne  = rv32_branch;
using rv32_blt  = rv32_branch;
using rv32_bge  = rv32_branch;
using rv32_bltu = rv32_branch;
using rv32_bgeu = rv32_branch;

struct rv32_zicsr {
  uint32_t csr;
  uint8_t  rd;
  uint8_t  rs;
  rv32_zicsr(uint32_t word) { unpack(word); }
  void unpack(uint32_t word);
};

using rv32_csrrw  = rv32_zicsr;
using rv32_csrrs  = rv32_zicsr;
using rv32_csrrc  = rv32_zicsr;
using rv32_csrrwi = rv32_zicsr;
using rv32_csrrsi = rv32_zicsr;
using rv32_csrrci = rv32_zicsr;

struct rvc_ca {
  uint8_t rdrs1;
  uint8_t rs2;
  void    unpack(uint16_t w);
  rvc_ca(uint16_t w) { unpack(w); }
};

using rvc_and  = rvc_ca;
using rvc_or   = rvc_ca;
using rvc_xor  = rvc_ca;
using rvc_sub  = rvc_ca;
using rvc_addw = rvc_ca;
using rvc_subw = rvc_ca;

struct rvc_cr /* CR */ {
  uint8_t rdrs1;
  uint8_t rs2;
  void    unpack(uint16_t w);
  rvc_cr(uint16_t w) { unpack(w); }
};

using rvc_jr   = rvc_cr;
using rvc_jalr = rvc_cr;
using rvc_mv   = rvc_cr;
using rvc_add  = rvc_cr;

struct rvc_lwsp /* CI */ {
  uint8_t rdrs1;
  uint8_t imm;
  void    unpack(uint16_t w);
  rvc_lwsp(uint16_t w) { unpack(w); }
};

struct rvc_li /* CI */ {
  uint32_t imm;
  uint8_t  rdrs1;
  void     unpack(uint16_t w);
  rvc_li(uint16_t w) { unpack(w); }
};

struct rvc_lui /* CI */ {
  uint32_t imm;
  uint8_t  rdrs1;
  void     unpack(uint16_t w);
  rvc_lui(uint16_t w) { unpack(w); }
};

// ALSO EXTRACT C_NOP
struct rvc_addi /* CI */ {
  uint32_t imm;
  uint8_t  rdrs1;

  void unpack(uint16_t w);
  rvc_addi(uint16_t w) { unpack(w); }
};

struct rvc_addi16sp /* CI */ {
  uint32_t imm;
  uint8_t  rdrs1;

  void unpack(uint16_t w);
  rvc_addi16sp(uint16_t w) { unpack(w); }
};

struct rvc_slli /* CI */ {
  uint8_t rdrs1;
  uint8_t imm;
  void    unpack(uint16_t w);
  rvc_slli(uint16_t w) { unpack(w); }
};

struct rvc_swsp /* CSS */ {
  uint8_t rs2;
  uint8_t imm;
  void    unpack(uint16_t w);
  rvc_swsp(uint16_t w) { unpack(w); }
};

struct rvc_addi4spn /* CIW */ {
  uint16_t imm;
  uint8_t  rd;
  void     unpack(uint16_t w);
  rvc_addi4spn(uint16_t w) { unpack(w); }
};

struct rvc_lw /* CL */ {
  uint8_t imm;
  uint8_t rs1;
  uint8_t rd;
  void    unpack(uint16_t w);
  rvc_lw(uint16_t w) { unpack(w); }
};

struct rvc_sw /* CS */ {
  uint8_t imm;
  uint8_t rs1;
  uint8_t rs2;
  void    unpack(uint16_t w);
  rvc_sw(uint16_t w) { unpack(w); }
};

struct rvc_beqz {
  uint32_t ofs;
  uint8_t  rs1;

  void unpack(uint16_t w);
  rvc_beqz(uint16_t w) { unpack(w); }
};

struct rvc_bnez {
  uint32_t ofs;
  uint8_t  rs1;

  void unpack(uint16_t w);
  rvc_bnez(uint16_t w) { unpack(w); }
};

struct rvc_srli {
  uint8_t rs1;
  uint8_t ofs;
  void    unpack(uint16_t w);
  rvc_srli(uint16_t w) { unpack(w); }
};

struct rvc_srai {
  uint8_t rs1;
  uint8_t ofs;
  void    unpack(uint16_t w);
  rvc_srai(uint16_t w) { unpack(w); }
};

struct rvc_andi {
  uint32_t ofs;
  uint8_t  rs1;
  void     unpack(uint16_t w);
  rvc_andi(uint16_t w) { unpack(w); }
};

struct rvc_j /* CJ */ {
  uint32_t tgt;
  void     unpack(uint16_t w);
  rvc_j(uint16_t w) { unpack(w); }
};

struct rvc_jal {
  uint32_t tgt;
  void     unpack(uint16_t w);
  rvc_jal(uint16_t w) { unpack(w); }
};

inline void rvc_jal::unpack(uint16_t w) { tgt = rv32::rvc_imm11(w); }
inline void rvc_j::unpack(uint16_t w) { tgt = rv32::rvc_imm11(w); }

inline void rvc_andi::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  ofs = offset(w, 2U, 6U) |
        static_cast<uint32_t>(
            (static_cast<int32_t>(offset(w, 12U, 12U) << 31) >> 26));
}

inline void rvc_srai::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  ofs = offset(w, 2U, 6U) | offset(w, 12U, 12U) << 5;
}

inline void rvc_srli::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  ofs = offset(w, 2U, 6U) | offset(w, 12U, 12U) << 5;
}

inline void rvc_bnez::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  ofs = (offset(w, 2U, 2U) << 5) | (offset(w, 3U, 4U) << 1) |
        (offset(w, 5U, 6U) << 6) | (offset(w, 10U, 11U) << 3) |
        static_cast<uint32_t>(
            static_cast<int32_t>((offset(w, 12U, 12U) << 31)) >> 23);
}

inline void rvc_beqz::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  ofs = (offset(w, 2U, 2U) << 5) | (offset(w, 3U, 4U) << 1) |
        (offset(w, 5U, 6U) << 6) | (offset(w, 10U, 11U) << 3) |
        static_cast<uint32_t>(
            static_cast<int32_t>((offset(w, 12U, 12U) << 31)) >> 23);
}

inline void rvc_sw::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  rs2 = rv32::rvc_42(w);
  imm = (offset(w, 5U, 5U) << 6) | (offset(w, 6U, 6U) << 2) |
        (offset(w, 10U, 12U) << 3);
}

inline void rvc_lw::unpack(uint16_t w) {
  rs1 = rv32::rvc_97(w);
  rd  = rv32::rvc_42(w);
  imm = (offset(w, 5U, 5U) << 6) | (offset(w, 6U, 6U) << 2) |
        (offset(w, 10U, 12U) << 3);
}

inline void rvc_addi4spn::unpack(uint16_t w) {
  rd  = rv32::rvc_42(w);
  imm = (offset(w, 5U, 5U) << 3) | (offset(w, 6U, 6U) << 2) |
        (offset(w, 7U, 10U) << 6) | (offset(w, 11U, 12U) << 4);
}

inline void rvc_swsp::unpack(uint16_t w) {
  rs2 = offset(w, 2U, 6U);
  imm = (offset(w, 7U, 8U) << 6) | (offset(w, 9U, 12U) << 2);
}

inline void rvc_slli::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  // assert(rdrs1 != 0);
  imm = offset(w, 2U, 6U) | (offset(w, 12U, 12U) << 5);
}

inline void rvc_addi16sp::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  // assert(rdrs1 == 2);
  imm = (offset(w, 2U, 2U) << 5) | (offset(w, 3U, 4U) << 7) |
        (offset(w, 5U, 5U) << 6) | (offset(w, 6U, 6U) << 4) |
        static_cast<uint32_t>(
            static_cast<int32_t>((offset(w, 12U, 12U) << 31)) >> 22);
}

inline void rvc_addi::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  // C_NOP
  // assert(rdrs1 != 0);
  imm = offset(w, 2U, 6U) |
        static_cast<uint32_t>(static_cast<int32_t>(offset(w, 12U, 12U) << 31) >>
                              27);
}

inline void rvc_lui::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  // assert(rdrs1 != 0);
  // assert(rdrs1 != 2);
  imm = (offset(w, 2U, 6U) << 12) |
        static_cast<uint32_t>(static_cast<int32_t>(offset(w, 12U, 12U) << 31) >>
                              14);
}

inline void rvc_li::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  // assert(rdrs1 != 0);
  imm = offset(w, 2U, 6U) |
        static_cast<uint32_t>(static_cast<int32_t>(offset(w, 12U, 12U) << 31) >>
                              27);
}

inline void rvc_lwsp::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  // assert(rdrs1 != 0);
  imm = (offset(w, 12U, 12U) << 5) | (offset(w, 2U, 3U) << 6) |
        (offset(w, 4U, 5U) << 2);
}

inline void rvc_cr::unpack(uint16_t w) {
  rdrs1 = offset(w, 7U, 11U);
  rs2   = offset(w, 2U, 6U);
}

inline void rvc_ca::unpack(uint16_t w) {
  rdrs1 = rv32::rvc_97(w);
  rs2   = rv32::rvc_42(w);
}

inline void rv32_jal::unpack(uint32_t word) {
  rd  = rv32::rd(word);
  imm = rv32::imm_j(word);
}

inline void rv32_op32::unpack(uint32_t word) {
  rd  = rv32::rd(word);
  rs1 = rv32::rs1(word);
  rs2 = rv32::rs2(word);
}

inline void rv32_opimm32::unpack(uint32_t word) {
  rd  = rv32::rd(word);
  rs  = rv32::rs1(word);
  imm = rv32::imm_i(word);
}

inline void rv32_opimm32_shamt::unpack(uint32_t word) {
  rd  = rv32::rd(word);
  rs  = rv32::rs1(word);
  imm = rv32::rs2(word);
}

inline void rv32_lui::unpack(uint32_t word) {
  rd  = rv32 ::rd(word);
  imm = rv32 ::imm_u(word);
}

inline void rv32_store::unpack(uint32_t word) {
  rs1 = rv32::rs1(word);
  rs2 = rv32::rs2(word);
  imm = rv32::imm_s(word);
}

inline void rv32_branch::unpack(uint32_t word) {
  rs1 = rv32::rs1(word);
  rs2 = rv32::rs2(word);
  imm = rv32::imm_b(word);
}

inline void rv32_zicsr::unpack(uint32_t word) {
  rd  = rv32::rd(word);
  rs  = rv32::rs1(word);
  csr = rv32::csr(word);
}

constexpr inline op make_nop(bool compressed = false) {
  return op{0, alu::_add, target::alu, 0, 0, 0, true, false, compressed};
}

constexpr inline op make_illegal(bool compressed = false) {
  return op{0, {}, target::illegal, 0, 0, 0, false, false, compressed};
}

op decode(uint32_t word);
op decode16(uint16_t word);

#endif // DECODER_DECODER_HPP
