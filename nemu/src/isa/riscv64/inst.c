/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include "common.h"
#include "debug.h"
#include "local-include/reg.h"
#include "stdio.h"
#include <assert.h>
#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/ifetch.h>
// #include <cstdint>
#include <isa.h>
#include <stdbool.h>
#define R(i) gpr(i)
#define Mr vaddr_read
#define Mw vaddr_write

enum {
  TYPE_I,
  TYPE_U,
  TYPE_S,
  TYPE_J,
  TYPE_R,
  TYPE_B,
  TYPE_N, // none
};

#define src1R(n)                                                               \
  do {                                                                         \
    *src1 = R(n);                                                              \
  } while (0) // 读取寄存器内容
#define src2R(n)                                                               \
  do {                                                                         \
    *src2 = R(n);                                                              \
  } while (0)
#define destR(n)                                                               \
  do {                                                                         \
    *dest = n;                                                                 \
  } while (0)
#define src1I(i)                                                               \
  do {                                                                         \
    *src1 = i;                                                                 \
  } while (0) // 读取立即数
#define src2I(i)                                                               \
  do {                                                                         \
    *src2 = i;                                                                 \
  } while (0)
#define destI(i)                                                               \
  do {                                                                         \
    *dest = i;                                                                 \
  } while (0)

static word_t immI(uint32_t i) { return SEXT(BITS(i, 31, 20), 12); }
static word_t immU(uint32_t i) { return SEXT(BITS(i, 31, 12), 20) << 12; }
static word_t immS(uint32_t i) {
  return (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7);
}
/* add by leesum */
static word_t immB(uint32_t i) {
  return (SEXT(BITS(i, 31, 31), 1) << 12 | BITS(i, 7, 7) << 11 |
          BITS(i, 30, 25) << 5 | BITS(i, 11, 8) << 1 | 0);
}
static word_t immJ(uint32_t i) {
  return (SEXT(BITS(i, 31, 31), 1) << 20 | BITS(i, 19, 12) << 12 |
          BITS(i, 20, 20) << 11 | BITS(i, 30, 21) << 1 | 0);
}

// static word_t immCSR(uint32_t i) {

//   word_t imm_temp = (word_t)BITS(i, 19, 15);

//   return imm_temp;
// }

/* 格式转换 */
#define S32(i) ((int32_t)i)
#define S64(i) ((int64_t)i)
#define S128(i) ((__int128_t)(int64_t)i) // 需要二次转换防止丢失数据
#define U32(i) ((uint32_t)i)
#define U64(i) ((uint64_t)i)
#define U128(i) ((__uint128_t)(uint64_t)i)

// BITS(i, 7, 0)
// BITS(i, 8, 8)
// BITS(i, 18, 9)
// BITS(i, 19, 19)
// (BITS(i, 19, 19), BITS(i, 7, 0), BITS(i, 8, 8), BITS(i, 18, 9), 0)
static void decode_operand(Decode *s, word_t *dest, word_t *src1, word_t *src2,
                           int type) {
  uint32_t i = s->isa.inst.val;
  int rd = BITS(i, 11, 7);
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
  destR(rd);
  switch (type) {
  case TYPE_I:
    src1R(rs1);
    src2I(immI(i));
    break;
  case TYPE_U:
    src1I(immU(i));
    break;
  case TYPE_S:
    destI(immS(i));
    src1R(rs1);
    src2R(rs2);
    break;
    /* add by leesum */
  case TYPE_J:
    src1I(immJ(i));
    break;
  case TYPE_R:
    src1R(rs1);
    src2R(rs2);
    break;
  case TYPE_B:
    destI(immB(i));
    src1R(rs1);
    src2R(rs2);
    break;
  default:
    break;
  }
}

static word_t calc_div(word_t src1, word_t src2, word_t width, bool sign) {

  Assert(width == 32 || width == 64, "width err : %lu", width);
  Log("src1:%lx,src2:%lx\n", src1, src2);
  if ((S32(src2) == 0 && width == 32) || (S64(src2) == 0 && width == 64)) {
    return -1;
  } else if (width == 64 && S64(src1) == INT64_MIN && src2 == -1 && sign) {
    return INT64_MIN;
  } else if (width == 32 && S32(src1) == INT32_MIN && src2 == -1 && sign) {
    return INT32_MIN;
  }

  switch (width) {
  case 32:
    return sign ? SEXT(S32(src1) / S32(src2), 32)
                : SEXT(U32(src1) / U32(src2), 32);
  case 64:
    return sign ? S64(src1) / S64(src2) : U64(src1) / U64(src2);
  default:
    panic("width err");
  }
}

static word_t calc_rem(word_t src1, word_t src2, word_t width, bool sign) {
  Assert(width == 32 || width == 64, "width err : %lu", width);

  //   Log("src1 %lx\n", src1);
  //   Log("src2 %lx\n", src2);
  if ((S32(src2) == 0 && width == 32)) {
    return S32(src1);
  } else if ((S64(src2) == 0 && width == 64)) {
    return S64(src1);

  } else if (width == 64 && S64(src1) == INT64_MIN && src2 == -1 && sign) {
    return 0;
  } else if (width == 32 && S32(src1) == INT32_MIN && S32(src2) == -1 && sign) {
    return 0;
  }

  switch (width) {
  case 32:
    return sign ? SEXT(S32(src1) % S32(src2), 32)
                : SEXT(U32(src1) % U32(src2), 32);
  case 64:
    return sign ? S64(src1) % S64(src2) : U64(src1) % U64(src2);
  default:
    panic("width err");
  }
}

static int decode_exec(Decode *s) {
  word_t dest = 0, src1 = 0, src2 = 0;
  s->dnpc = s->snpc;

  int csr_imm = BITS(s->isa.inst.val, 19, 15);

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* body */)                           \
  {                                                                            \
    decode_operand(s, &dest, &src1, &src2, concat(TYPE_, type));               \
    __VA_ARGS__;                                                               \
  }

  INSTPAT_START();
  /* op */
  INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add, R,
          R(dest) = src1 + src2);
  INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub, R,
          R(dest) = src1 - src2);
  INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor, R,
          R(dest) = src1 ^ src2);
  INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or, R,
          R(dest) = src1 | src2);
  INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and, R,
          R(dest) = src1 & src2);
  INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll, R,
          R(dest) = U64(src1) << BITS(src2, 5, 0));
  INSTPAT("0000000 ????? ????? 101 ????? 01100 11", srl, R,
          R(dest) = U64(src1) >> BITS(src2, 5, 0));
  INSTPAT("0100000 ????? ????? 101 ????? 01100 11", sra, R,
          R(dest) = S64(src1) >> BITS(src2, 5, 0));
  INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt, R,
          R(dest) = (S64(src1) < S64(src2)) ? 1 : 0);
  INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu, R,
          R(dest) = (U64(src1) < U64(src2)) ? 1 : 0);

  /* op-imm */
  INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi, I,
          R(dest) = src1 + src2);
  INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori, I,
          R(dest) = src1 ^ src2);
  INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori, I,
          R(dest) = src1 | src2);
  INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi, I,
          R(dest) = src1 & src2);
  INSTPAT("000000? ????? ????? 001 ????? 00100 11", slli, I,
          R(dest) = U64(src1) << BITS(src2, 5, 0)); // rv64
  INSTPAT("000000? ????? ????? 101 ????? 00100 11", srli, I,
          R(dest) = U64(src1) >> BITS(src2, 5, 0)); // rv64
  INSTPAT("010000? ????? ????? 101 ????? 00100 11", srai, I,
          R(dest) = S64(src1) >> BITS(src2, 5, 0)); // rv64
  INSTPAT("??????? ????? ????? 010 ????? 00100 11", slti, I,
          R(dest) = (S64(src1) < S64(src2)) ? 1 : 0);
  INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu, I,
          R(dest) = U64(src1) < U64(src2));

  /* store and load */
  INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb, I,
          R(dest) = SEXT(Mr(src1 + src2, 1), 8));
  INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh, I,
          R(dest) = SEXT(Mr(src1 + src2, 2), 16));
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw, I,
          R(dest) = SEXT(Mr(src1 + src2, 4), 32));
  INSTPAT("??????? ????? ????? 011 ????? 00000 11", ld, I,
          R(dest) = Mr(src1 + src2, 8)); // rv64
  INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu, I,
          R(dest) = Mr(src1 + src2, 1));
  INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu, I,
          R(dest) = Mr(src1 + src2, 2));
  INSTPAT("??????? ????? ????? 110 ????? 00000 11", lwu, I,
          R(dest) = Mr(src1 + src2, 4)); // rv64
  INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb, S,
          Mw(src1 + dest, 1, src2));
  INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh, S,
          Mw(src1 + dest, 2, src2));
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw, S,
          Mw(src1 + dest, 4, src2));
  INSTPAT("??????? ????? ????? 011 ????? 01000 11", sd, S,
          Mw(src1 + dest, 8, src2)); // rv64

  /* branch */
  INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq, B,
          if (src1 == src2) s->dnpc = s->pc + dest);
  INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne, B,
          if (src1 != src2) s->dnpc = s->pc + dest);
  INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt, B,
          if (S64(src1) < S64(src2)) s->dnpc = s->pc + dest);
  INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge, B,
          if (S64(src1) >= S64(src2)) s->dnpc = s->pc + dest);
  INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu, B,
          if (U64(src1) < U64(src2)) s->dnpc = s->pc + dest);
  INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu, B,
          if (U64(src1) >= U64(src2)) s->dnpc = s->pc + dest);

  /*RV64I Base Instruction Set (in addition to RV32I)*/
  INSTPAT("??????? ????? ????? 000 ????? 00110 11", addiw, I,
          R(dest) = SEXT(BITS(src1 + src2, 31, 0), 32));
  INSTPAT("000000? ????? ????? 001 ????? 00110 11", slliw, I,
          R(dest) = SEXT(U32(src1) << BITS(src2, 4, 0), 32));
  INSTPAT("000000? ????? ????? 101 ????? 00110 11", srliw, I,
          R(dest) = SEXT(U32(src1) >> BITS(src2, 4, 0), 32));
  INSTPAT("010000? ????? ????? 101 ????? 00110 11", sraiw, I,
          R(dest) = SEXT(S32(src1) >> BITS(src2, 4, 0), 32));
  INSTPAT("010000? ????? ????? 101 ????? 00110 11", sraiw, I,
          R(dest) = SEXT(S32(src1) >> BITS(src2, 4, 0), 32));
  INSTPAT("0000000 ????? ????? 000 ????? 01110 11", addw, R,
          R(dest) = SEXT(BITS(src1 + src2, 31, 0), 32));
  INSTPAT("0100000 ????? ????? 000 ????? 01110 11", subw, R,
          R(dest) = SEXT(BITS(src1 - src2, 31, 0), 32));
  INSTPAT("0000000 ????? ????? 001 ????? 01110 11", sllw, R,
          R(dest) = SEXT(U32(src1) << BITS(src2, 4, 0), 32));
  INSTPAT("0000000 ????? ????? 101 ????? 01110 11", srlw, R,
          R(dest) = SEXT(U32(src1) >> BITS(src2, 4, 0), 32)); // 很奇怪
  INSTPAT("0100000 ????? ????? 101 ????? 01110 11", sraw, R,
          R(dest) = SEXT(S32(src1) >> BITS(src2, 4, 0), 32));

  /* mul */
  union {
    __int128_t mul128;
    struct {
      uint64_t l;
      uint64_t h;
    } sep;
  } mulTemp;

  mulTemp.mul128 = 0;

  INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul, R,
          R(dest) = src1 * src2);
  INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh, R,
          mulTemp.mul128 = S128(src1) * S128(src2);
          R(dest) = mulTemp.sep.h);
  INSTPAT("0000001 ????? ????? 010 ????? 01100 11", mulhsu, R,
          mulTemp.mul128 = S128(src1) * U128(src2);
          R(dest) = mulTemp.sep.h);
  INSTPAT("0000001 ????? ????? 011 ????? 01100 11", mulhu, R,
          mulTemp.mul128 = U128(src1) * U128(src2);
          R(dest) = mulTemp.sep.h);
  INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div, R,
          R(dest) = calc_div(src1, src2, 64, true));
  INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu, R,
          R(dest) = calc_div(src1, src2, 64, false));
  INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem, R,
          R(dest) = calc_rem(src1, src2, 64, true));
  INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu, R,
          R(dest) = calc_rem(src1, src2, 64, false));

  // rv64
  INSTPAT("0000001 ????? ????? 000 ????? 01110 11", mulw, R,
          R(dest) = SEXT(BITS(S32(src1) * S32(src2), 31, 0), 32));
  INSTPAT("0000001 ????? ????? 100 ????? 01110 11", divw, R,
          R(dest) = calc_div(src1, src2, 32, true));
  INSTPAT("0000001 ????? ????? 101 ????? 01110 11", divuw, R,
          R(dest) = calc_div(src1, src2, 32, false));
  INSTPAT("0000001 ????? ????? 110 ????? 01110 11", remw, R,
          R(dest) = calc_rem(src1, src2, 32, true));
  INSTPAT("0000001 ????? ????? 111 ????? 01110 11", remuw, R,
          R(dest) = calc_rem(src1, src2, 32, false));
  /* other */
  INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal, J, R(dest) = s->pc + 4;
          s->dnpc = s->pc + src1);
  INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr, I,
          R(dest) = s->pc + 4;
          s->dnpc = (src1 + src2) & (~U64(1)));

  INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui, U, R(dest) = src1);
  INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc, U,
          R(dest) = src1 + s->pc);

  INSTPAT("??????? ????? ????? 010 ????? 11100 11", csrrs, I,
          word_t t = csr(src2);
          csr(src2) = t | src1; R(dest) = t);
  INSTPAT("??????? ????? ????? 001 ????? 11100 11", csrrw, I,
          word_t t = csr(src2);
          csr(src2) = src1; R(dest) = t);
  INSTPAT("??????? ????? ????? 011 ????? 11100 11", csrrc, I,
          word_t t = csr(src2);
          csr(src2) = t & ~src1; R(dest) = t;);
  INSTPAT("??????? ????? ????? 101 ????? 11100 11", csrrwi, I,
          word_t t = csr(src2);
          csr(src2) = csr_imm; R(dest) = t);
  INSTPAT("??????? ????? ????? 110 ????? 11100 11", csrrsi, I,
          word_t t = csr(src2);
          csr(src2) = t | csr_imm; R(dest) = t);
  INSTPAT("??????? ????? ????? 111 ????? 11100 11", csrrci, I,
          word_t t = csr(src2);
          csr(src2) = t & ~csr_imm; R(dest) = t);
  //
  INSTPAT("0000000 00000 00000 000 00000 11100 11", ecall, I,
          s->dnpc = isa_raise_intr(11, s->pc);); //  trap 操作
  INSTPAT("0011000 00010 00000 000 00000 11100 11", mret, R,
          s->dnpc = isa_intr_ret()); // 软件实现 +4 操作,区分异常和中断

  INSTPAT("0000000 00000 00000 001 00000 00011 11", fencei, I, ); // nop
  INSTPAT("0000??? ????? 00000 000 00000 00011 11", fence, I, );  // nop

  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak, N,
          NEMUTRAP(s->pc, R(10))); // R(10) is $a0
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv, N, INV(s->pc));

  INSTPAT_END();
  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst.val = inst_fetch(&s->snpc, 4); // 在其中实现 pc 自增
  return decode_exec(s);
}
