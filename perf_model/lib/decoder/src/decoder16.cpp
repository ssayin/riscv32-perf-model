// SPDX-FileCopyrightText: 2022 - 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: MIT

#include "decoder/decoder.hpp"

namespace {
op decode16_quad0(uint16_t word);
op decode16_quad1(uint16_t word);
op decode16_quad2(uint16_t word);

op decode16_quad2_extract_other(uint16_t word);

constexpr uint16_t quad_0 = 0b00;
constexpr uint16_t quad_1 = 0b01;
constexpr uint16_t quad_2 = 0b10;
constexpr uint16_t quad_3 = 0b11;

} // namespace

enum class quad0 : uint16_t {
  c_addi4spn  = 0b000,
  c_fld       = 0b001,
  c_lw        = 0b010,
  c_flw       = 0b011,
  c_reserved0 = 0b100,
  c_fsd       = 0b101,
  c_sw        = 0b110,
  c_fsw       = 0b111,
};

enum class quad1 : uint16_t {
  c_addi         = 0b000,
  c_jal          = 0b001,
  c_li           = 0b010,
  c_addi16sp_lui = 0b011,
  c_arith        = 0b100,
  c_j            = 0b101,
  c_beqz         = 0b110,
  c_bnez         = 0b111
};

// instr[11:10]
enum class arith {
  c_srli = 0b00,
  c_srai = 0b01,
  c_andi = 0b10,
  // C_SUB, C_XOR, C_OR, C_AND
  arith_no_imm = 0b11
};

// instr[6:5]
// [12:12] should be 0 for 32-bit OPS, 1 for 64-bit OPS
enum class arith_no_imm {
  c_sub = 0b00,
  c_xor = 0b01,
  c_or  = 0b10,
  c_and = 0b11
};

enum class quad2 : uint16_t {
  c_slli  = 0b000,
  c_fldsp = 0b001,
  c_lwsp  = 0b010,
  c_flwsp = 0b011,
  // C_JR, C_MV, C_EBREAK, C_JALR, C_ADD
  other   = 0b100,
  c_fsdsp = 0b101,
  c_swsp  = 0b110,
  c_fswsp = 0b111
};

op decode16(uint16_t word) {
  if ((word & 0xFFFF) == 0x0000) return make_illegal(true);
  switch (word & 0b11) {
  case quad_0:
    return decode16_quad0(word);
  case quad_1:
    return decode16_quad1(word);
  case quad_2:
    return decode16_quad2(word);
  case quad_3:
  default:
    return make_illegal(true);
  }
}

namespace {
op decode16_quad0(uint16_t word) {
  switch (static_cast<quad0>(offset(word, 13U, 15U))) {
  case quad0::c_addi4spn: {
    rvc_addi4spn isn{word};
    return op{isn.imm, alu::_add, target::alu, isn.rd, 2, 0, true, false, true};
  }
  case quad0::c_lw: {
    rvc_lw isn{word};
    return op{isn.imm, masks::load::lw, target::load, isn.rd, isn.rs1,
              0,       false,           false,        true};
  }
  case quad0::c_sw: {
    rvc_sw isn{word};
    return op{isn.imm, masks::store::sw, target::store, 0,
              isn.rs1, isn.rs2,          false,         false,
              true};
  }
  default:
    return make_illegal(true);
  }
}

op decode16_quad1(uint16_t word) {
  switch (static_cast<quad1>(offset(word, 13U, 15U))) {
  case quad1::c_addi: {
    rvc_addi isn{word};
    return op{isn.imm, alu::_add, target::alu, isn.rdrs1, isn.rdrs1,
              0,       true,      false,       true};
  }
  case quad1::c_jal: {
    rvc_jal isn{word};
    return op{isn.tgt, alu::_jal, target::alu, 1, 0, 0, true, true, true};
  }
  case quad1::c_li: {
    rvc_li isn{word};
    return op{isn.imm, alu::_add, target::alu, isn.rdrs1, 0,
              0,       true,      false,       true};
  }

  case quad1::c_addi16sp_lui: {
    // C_ADDI16SP
    if (offset(word, 7U, 11U) == 2) {
      rvc_addi16sp isn{word};
      return op{isn.imm, alu::_add, target::alu, 2, 2, 0, true, false, true};
      // C_LUI
    } else {
      rvc_lui isn{word};
      return op{isn.imm, alu::_add, target::alu, isn.rdrs1, 0,
                0,       true,      false,       true};
    }
  }
  case quad1::c_arith: {
    switch (static_cast<arith>(offset(word, 10U, 11U))) {
    case arith::c_srli: {
      rvc_srli isn{word};
      return op{isn.ofs, alu::_srl, target::alu, isn.rs1, isn.rs1,
                0,       true,      false,       true};
    }
    case arith::c_srai: {
      rvc_srai isn{word};
      return op{isn.ofs, alu::_sra, target::alu, isn.rs1, isn.rs1,
                0,       true,      false,       true};
    }
    case arith::c_andi: {
      rvc_andi isn{word};
      return op{isn.ofs, alu::_and, target::alu, isn.rs1, isn.rs1,
                0,       true,      false,       true};
    }
    case arith::arith_no_imm: {
      switch (static_cast<arith_no_imm>(offset(word, 5U, 6U))) {
      case arith_no_imm::c_sub: {
        rvc_sub isn{word};
        return op{0,       alu::_sub, target::alu, isn.rdrs1, isn.rdrs1,
                  isn.rs2, false,     false,       true};
      }
      case arith_no_imm::c_xor: {
        rvc_xor isn{word};
        return op{0,       alu::_xor, target::alu, isn.rdrs1, isn.rdrs1,
                  isn.rs2, false,     false,       true};
      }
      case arith_no_imm::c_or: {
        rvc_or isn{word};
        return op{0,       alu::_or, target::alu, isn.rdrs1, isn.rdrs1,
                  isn.rs2, false,    false,       true};
      }
      case arith_no_imm::c_and: {
        rvc_and isn{word};
        return op{0,       alu::_and, target::alu, isn.rdrs1, isn.rdrs1,
                  isn.rs2, false,     false,       true};
      }
      default:
        return make_illegal(true);
      }
    }
    default:
      return make_illegal(true);
    }
  }
  case quad1::c_j: {
    rvc_j isn{word};
    return op{isn.tgt, alu::_jal, target::alu, 0, 0, true, true, true};
  }
  case quad1::c_beqz: {
    rvc_beqz isn{word};
    return op{
        isn.ofs, masks::branch::beq, target::branch, 0, isn.rs1, 0, true, false,
        true};
  }
  case quad1::c_bnez: {
    rvc_bnez isn{word};
    return op{
        isn.ofs, masks::branch::bne, target::branch, 0, isn.rs1, 0, true, false,
        true};
  }
  default:
    return make_illegal(true);
  }
}

op decode16_quad2(uint16_t word) {
  switch (static_cast<quad2>(offset(word, 13U, 15U))) {
  case quad2::c_slli: {
    rvc_slli isn{word};
    return op{isn.imm,   alu::_sll, target::alu, isn.rdrs1, isn.rdrs1,
              isn.rdrs1, true,      false,       true};
  }
    // Floating point RVC for RV32
  case quad2::c_fldsp:
  case quad2::c_flwsp:
  case quad2::c_fsdsp:
  case quad2::c_fswsp:
    return make_illegal(true);

  case quad2::c_lwsp: {
    rvc_lwsp isn{word};
    return op{isn.imm, masks::load::lw, target::load, isn.rdrs1, 2,
              0,       false,           false,        true};
  }
  case quad2::other:
    return decode16_quad2_extract_other(word);

  case quad2::c_swsp: {
    rvc_swsp isn{word};
    return op{isn.imm, masks::store::sw, target::store, 0,
              2,       isn.rs2,          false,         false,
              true};
  }
  default:
    return make_illegal(true);
  }
}

op decode16_quad2_extract_other(uint16_t word) {
  auto bit_12   = offset(word, 12U, 12U);
  auto bit_6_2  = offset(word, 2U, 6U);
  auto bit_11_7 = offset(word, 7U, 11U);
  if (bit_12 == 0b0) {
    if (bit_6_2 == 0U && bit_11_7 != 0U) /*jr*/ {
      rvc_jr isn{word};
      return op{0,       alu::_jalr, target::alu, 0,   isn.rdrs1,
                isn.rs2, false,      false,       true};
    } else if (bit_6_2 != 0U && bit_11_7 != 0U) /*mv*/ {
      rvc_mv isn{word};
      return op{0,       alu::_add, target::alu, isn.rdrs1, 0,
                isn.rs2, false,     false,       true};
    }
    // reserved
    return make_nop(true);
  } else if (bit_12 == 0b1) {
    if (bit_6_2 == 0 && bit_11_7 == 0) /* ebreak*/ {
      return op{0, {}, target::ebreak, 0, 0, 0, false, false, true};
    } else if (bit_6_2 == 0 && bit_11_7 != 0) /*jalr*/ {
      rvc_jalr isn{word};
      return op{0,       alu::_jalr, target::alu, 1,   isn.rdrs1,
                isn.rs2, false,      false,       true};
    }

    else if (bit_6_2 != 0 && bit_11_7 != 0) /*add*/ {
      rvc_add isn{word};
      return op{0,       alu::_add, target::alu, isn.rdrs1, isn.rdrs1,
                isn.rs2, false,     false,       true};
    }
    // reserved
    return make_nop(true);
  }

  return make_illegal(true);
}

} // namespace
