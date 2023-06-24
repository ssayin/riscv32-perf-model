// SPDX-FileCopyrightText: 2022 - 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: MIT

#ifndef RISCV32SIM_COMMON_HPP
#define RISCV32SIM_COMMON_HPP

#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

template <typename... Ts> struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts> overload(Ts...) -> overload<Ts...>;

template <class T>
concept UnsignedIntegral =
    std::is_unsigned<T>::value && std::is_integral<T>::value;

template <typename T>
concept Enum = std::is_enum<T>::value;

template <typename T>
concept MemoryModel = requires(T mem, uint32_t addr, void *ptr, int64_t size) {
                        mem.write(addr, ptr, size);
                      };

constexpr auto to_int(Enum auto val) {
  return static_cast<std::underlying_type_t<decltype(val)>>(val);
}

constexpr auto fillbits(UnsignedIntegral auto bitcount) {
  return (1U << bitcount) - 1U;
}

constexpr auto offset(UnsignedIntegral auto inst, UnsignedIntegral auto low,
                      UnsignedIntegral auto high) {
  return (inst >> low) & fillbits(high - low + 1U);
}

enum class target : uint8_t {
  load,
  store,
  alu,
  branch,
  csr,
  mret,
  illegal,
  ebreak,
  ecall
};

enum class alu : uint8_t {
  _and,
  _or,
  _xor,
  _add,
  _sub,
  _sll,
  _srl,
  _sra,
  _slt,
  _sltu,
  _mul,
  _mulh,
  _mulhsu,
  _mulhu,
  _div,
  _divu,
  _rem,
  _remu,
  _auipc,
  _jal,
  _jalr,
};

namespace masks {
enum class opcode : uint8_t {
  branch   = 0b1100011,
  load     = 0b0000011,
  store    = 0b0100011,
  reg_imm  = 0b0010011,
  reg_reg  = 0b0110011,
  misc_mem = 0b0001111,
  sys      = 0b1110011,
  lui      = 0b0110111,
  auipc    = 0b0010111,
  jal      = 0b1101111,
  jalr     = 0b1100111,
};

enum class branch : uint8_t {
  beq  = 0b000,
  bne  = 0b001,
  blt  = 0b100,
  bge  = 0b101,
  bltu = 0b110,
  bgeu = 0b111
};

enum class load : uint8_t {
  lb  = 0b000,
  lh  = 0b001,
  lw  = 0b010,
  lbu = 0b100,
  lhu = 0b101
};

enum class store : uint8_t {
  sb = 0b000,
  sh = 0b001,
  sw = 0b010,
};

enum class reg_imm : uint8_t {
  addi      = 0b000,
  slti      = 0b010,
  sltiu     = 0b011,
  xori      = 0b100,
  ori       = 0b110,
  andi      = 0b111,
  slli      = 0b001,
  srli_srai = 0b101,
};

enum class reg_reg : uint8_t {
  add_sub_mul  = 0b000,
  sll_mulh     = 0b001,
  slt_mulhsu   = 0b010,
  sltu_mulhu   = 0b011,
  xor_div      = 0b100,
  srl_sra_divu = 0b101,
  or_rem       = 0b110,
  and_remu     = 0b111,
};

enum class misc_mem : uint8_t {
  fence  = 0b000,
  fencei = 0b001,
};

enum class sys : uint8_t {
  other  = 0b000,
  csrrw  = 0b001,
  csrrs  = 0b010,
  csrrc  = 0b011,
  csrrwi = 0b101,
  csrrsi = 0b110,
  csrrci = 0b111,
};
} // namespace masks

using op_type = std::variant<std::monostate, alu, masks::load, masks::store,
                             masks::branch, masks::sys>;
struct op {
  uint32_t imm;
  op_type  opt; // 2 bytes
  target   tgt;
  uint8_t  rd;
  uint8_t  rs1;
  uint8_t  rs2;
  bool     has_imm;
  bool     use_pc        = false;
  bool     is_compressed = false;
};

struct program_counter {
  program_counter() = default;
  explicit program_counter(uint32_t x) : pc_next{x + 4}, pc{x} {}
  void set(uint32_t x) { pc_next = x; }
  void update() { pc = pc_next; }

  explicit operator uint32_t() const { return pc; }

  uint32_t pc_next = 0;
  uint32_t pc      = 0;
};

namespace masks {
static constexpr uint32_t sign_bit   = 0x80000000U;
static constexpr uint32_t lsb_zero   = 0xFFFFFFFEU;
static constexpr uint32_t msb_zero   = 0x7FFFFFFFU;
static constexpr uint32_t type_u_imm = 0xFFFFF000U;

namespace tvec {
static constexpr uint32_t type      = 0x00000002U;
static constexpr uint32_t base_addr = 0xFFFFFFFCU;
} // namespace tvec

static constexpr uint32_t ecall  = 0x73U;
static constexpr uint32_t ebreak = 0x9002U;
static constexpr uint32_t mret   = 0x30200073U;
static constexpr uint32_t sret   = 0x10200073U;
static constexpr uint32_t wfi    = 0x10500073U;

} // namespace masks

struct csr_change {
  uint16_t index;
  uint32_t prev;
  uint32_t next;
};

struct gpr_change {
  uint8_t  index;
  uint32_t prev;
  uint32_t next;
};

struct hart_state {
  op                      dec;
  program_counter         pc;
  uint32_t                instr;
  std::vector<csr_change> csr_staged;
  std::vector<gpr_change> gpr_staged;
  hart_state(uint32_t pc) : pc{pc} {}
};

#endif /* end of include guard: RISCV32SIM_COMMON_HPP */
