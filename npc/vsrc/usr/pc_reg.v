`include "sysconfig.v"
// /** 纯组合逻辑电路
//  * 1.branch_pc：来自exc阶段
//  * 2.csr_pc：来自mem阶段
//  * 3.pc_next：输出给if阶段
// ×/

module pc_reg (
    input clk,
    input rst,
    input stall_valid_i,
    input flush_valid_i,
    input if_ram_valid_i,

    input  [`XLEN_BUS] branch_pc_i,        // branch pc,来自 exc
    input              branch_pc_valid_i,
    input  [`XLEN_BUS] clint_pc_i,         //trap pc,来自mem
    input              clint_pc_valid_i,   //trap pc valide,来自mem
    output             if_ram_valid_o,
    output [`XLEN_BUS] pc_o                //输出pc
);

  // trap 优先级高
  wire [`XLEN_BUS] _pc_next =  clint_pc_valid_i ? clint_pc_i : 
                     branch_pc_valid_i?branch_pc_i:_pc_current+4;

  reg [`XLEN_BUS] _pc_current;

  assign pc_o = _pc_current;


  wire _pc_reg_wen = ~stall_valid_i & ~rst;
  wire _flush_valid = flush_valid_i;
  wire [`XLEN_BUS] _pc_next_d = (_flush_valid) ? `PC_RESET_ADDR : _pc_next;

  regTemplate #(
      .WIDTH    (`XLEN),
      .RESET_VAL(`PC_RESET_ADDR)
  ) u_pc_dreg (
      .clk (clk),
      .rst (rst),
      .din (_pc_next_d),
      .dout(_pc_current),
      .wen (_pc_reg_wen)
  );

  wire _if_ram_valid_d = !(clint_pc_valid_i | branch_pc_valid_i);  // 是否允许 if 访存
  reg _if_ram_valid_q;

  regTemplate #(
      .WIDTH    (1),
      .RESET_VAL(1'b0)
  ) u_if_ram_valid (
      .clk (clk),
      .rst (rst),
      .din (_if_ram_valid_d),
      .dout(_if_ram_valid_q),
      .wen (1'b1)
  );
  assign if_ram_valid_o = _if_ram_valid_q;

endmodule
