
`include "sysconfig.v"

// Burst types
`define AXI_BURST_TYPE_FIXED 2'b00               //突发类型  FIFO
`define AXI_BURST_TYPE_INCR 2'b01               //ram  
`define AXI_BURST_TYPE_WRAP 2'b10
// Access permissions
`define AXI_PROT_UNPRIVILEGED_ACCESS 3'b000
`define AXI_PROT_PRIVILEGED_ACCESS 3'b001
`define AXI_PROT_SECURE_ACCESS 3'b000
`define AXI_PROT_NON_SECURE_ACCESS 3'b010
`define AXI_PROT_DATA_ACCESS 3'b000
`define AXI_PROT_INSTRUCTION_ACCESS 3'b100
// Memory types (AR)
`define AXI_ARCACHE_DEVICE_NON_BUFFERABLE 4'b0000
`define AXI_ARCACHE_DEVICE_BUFFERABLE 4'b0001
`define AXI_ARCACHE_NORMAL_NON_CACHEABLE_NON_BUFFERABLE 4'b0010
`define AXI_ARCACHE_NORMAL_NON_CACHEABLE_BUFFERABLE 4'b0011
`define AXI_ARCACHE_WRITE_THROUGH_NO_ALLOCATE 4'b1010
`define AXI_ARCACHE_WRITE_THROUGH_READ_ALLOCATE 4'b1110
`define AXI_ARCACHE_WRITE_THROUGH_WRITE_ALLOCATE 4'b1010
`define AXI_ARCACHE_WRITE_THROUGH_READ_AND_WRITE_ALLOCATE 4'b1110
`define AXI_ARCACHE_WRITE_BACK_NO_ALLOCATE 4'b1011
`define AXI_ARCACHE_WRITE_BACK_READ_ALLOCATE 4'b1111
`define AXI_ARCACHE_WRITE_BACK_WRITE_ALLOCATE 4'b1011
`define AXI_ARCACHE_WRITE_BACK_READ_AND_WRITE_ALLOCATE 4'b1111
// Memory types (AW)
`define AXI_AWCACHE_DEVICE_NON_BUFFERABLE 4'b0000
`define AXI_AWCACHE_DEVICE_BUFFERABLE 4'b0001
`define AXI_AWCACHE_NORMAL_NON_CACHEABLE_NON_BUFFERABLE 4'b0010
`define AXI_AWCACHE_NORMAL_NON_CACHEABLE_BUFFERABLE 4'b0011
`define AXI_AWCACHE_WRITE_THROUGH_NO_ALLOCATE 4'b0110
`define AXI_AWCACHE_WRITE_THROUGH_READ_ALLOCATE 4'b0110
`define AXI_AWCACHE_WRITE_THROUGH_WRITE_ALLOCATE 4'b1110
`define AXI_AWCACHE_WRITE_THROUGH_READ_AND_WRITE_ALLOCATE 4'b1110
`define AXI_AWCACHE_WRITE_BACK_NO_ALLOCATE 4'b0111
`define AXI_AWCACHE_WRITE_BACK_READ_ALLOCATE 4'b0111
`define AXI_AWCACHE_WRITE_BACK_WRITE_ALLOCATE 4'b1111
`define AXI_AWCACHE_WRITE_BACK_READ_AND_WRITE_ALLOCATE 4'b1111

`define AXI_SIZE_BYTES_1 3'b000                //突发宽度一个数据的宽度
`define AXI_SIZE_BYTES_2 3'b001
`define AXI_SIZE_BYTES_4 3'b010
`define AXI_SIZE_BYTES_8 3'b011
`define AXI_SIZE_BYTES_16 3'b100
`define AXI_SIZE_BYTES_32 3'b101
`define AXI_SIZE_BYTES_64 3'b110
`define AXI_SIZE_BYTES_128 3'b111


module axi_rw #(
    parameter RW_DATA_WIDTH  = 64,
    parameter RW_ADDR_WIDTH  = 32,
    parameter AXI_DATA_WIDTH = 64,
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_ID_WIDTH   = 4,
    parameter AXI_STRB_WIDTH = AXI_DATA_WIDTH / 8,
    parameter AXI_USER_WIDTH = 1
) (
    input clock,
    input reset,

    /* arb<-->axi */
    // 读通道
    input [`NPC_ADDR_BUS] arb_read_addr_i,
    input arb_raddr_valid_i,  // 是否发起读请求
    input [7:0] arb_rmask_i,  // 数据掩码
    input [3:0] arb_rsize_i,
    input [7:0] arb_rlen_i,  // 突发传输大小
    output [`XLEN_BUS] arb_rdata_o,  // 读数据返回mem
    output arb_rdata_ready_o,  // 读数据是否有效
    //写通道
    input [`NPC_ADDR_BUS] arb_write_addr_i,  // mem 阶段的 write
    input arb_write_valid_i,
    input [7:0] arb_wmask_i,
    input [3:0] arb_wsize_i,
    input [7:0] arb_wlen_i,  // 突发传输大小
    input [`XLEN_BUS] arb_wdata_i,
    output arb_wdata_ready_o,  // 数据是否已经写入

    /* axi master */
    // Advanced eXtensible Interface
    // 写地址
    input                         axi_aw_ready_i,
    output                        axi_aw_valid_o,
    output [  AXI_ADDR_WIDTH-1:0] axi_aw_addr_o,
    output [                 2:0] axi_aw_prot_o,
    output [    AXI_ID_WIDTH-1:0] axi_aw_id_o,
    output [  AXI_USER_WIDTH-1:0] axi_aw_user_o,
    output [                 7:0] axi_aw_len_o,
    output [                 2:0] axi_aw_size_o,
    output [                 1:0] axi_aw_burst_o,
    output                        axi_aw_lock_o,
    output [                 3:0] axi_aw_cache_o,
    output [                 3:0] axi_aw_qos_o,
    output [                 3:0] axi_aw_region_o,
    //写数据
    input                         axi_w_ready_i,
    output                        axi_w_valid_o,
    output [  AXI_DATA_WIDTH-1:0] axi_w_data_o,
    output [AXI_DATA_WIDTH/8-1:0] axi_w_strb_o,
    output                        axi_w_last_o,
    output [  AXI_USER_WIDTH-1:0] axi_w_user_o,
    //写响应
    output                        axi_b_ready_o,
    input                         axi_b_valid_i,
    input  [                 1:0] axi_b_resp_i,
    input  [    AXI_ID_WIDTH-1:0] axi_b_id_i,
    input  [  AXI_USER_WIDTH-1:0] axi_b_user_i,
    //读地址
    input                         axi_ar_ready_i,
    output                        axi_ar_valid_o,
    output [  AXI_ADDR_WIDTH-1:0] axi_ar_addr_o,
    output [                 2:0] axi_ar_prot_o,
    output [    AXI_ID_WIDTH-1:0] axi_ar_id_o,
    output [  AXI_USER_WIDTH-1:0] axi_ar_user_o,
    output [                 7:0] axi_ar_len_o,
    output [                 2:0] axi_ar_size_o,
    output [                 1:0] axi_ar_burst_o,
    output                        axi_ar_lock_o,
    output [                 3:0] axi_ar_cache_o,
    output [                 3:0] axi_ar_qos_o,
    output [                 3:0] axi_ar_region_o,
    //读数据
    output                        axi_r_ready_o,
    input                         axi_r_valid_i,
    input  [                 1:0] axi_r_resp_i,
    input  [  AXI_DATA_WIDTH-1:0] axi_r_data_i,
    input                         axi_r_last_i,
    input  [    AXI_ID_WIDTH-1:0] axi_r_id_i,
    input  [  AXI_USER_WIDTH-1:0] axi_r_user_i
);



  //握手信号
  wire axi_ar_handshake = axi_ar_valid_o & axi_ar_ready_i;
  wire axi_aw_handshake = axi_aw_valid_o & axi_aw_ready_i;
  wire axi_r_handshake = axi_r_valid_i & axi_r_ready_o;
  wire axi_w_handshake = axi_w_valid_o & axi_w_ready_i;
  wire axi_b_handshake = axi_b_valid_i & axi_b_ready_o;


  // ------------------State Machine------------------TODO

  /************************ 写通道状态切换 ************************/
  localparam AXI_WSTATE_LEN = 3;
  localparam AXI_WRST = 3'd0;
  localparam AXI_WIDLE = 3'd1;
  localparam AXI_WADDR_WDATA = 3'd2;  // axi4  写地址写数据同时发送


  wire [2:0 ]to_aw_size = ({3{arb_wsize_i[0]}}&`AXI_SIZE_BYTES_1)
                             | ({3{arb_wsize_i[1]}}&`AXI_SIZE_BYTES_2)
                             | ({3{arb_wsize_i[2]}}&`AXI_SIZE_BYTES_4)
                             | ({3{arb_wsize_i[3]}}&`AXI_SIZE_BYTES_8);


  reg [AXI_WSTATE_LEN-1:0] axi_wstate;
  reg _arb_wdata_ready_o;

  // 写地址缓存
  reg [`NPC_ADDR_BUS] aw_addr;
  reg aw_valid;
  reg [7:0] aw_len;  // 突发长度 AxLEN[7:0] + 1,0 表示不突发
  reg [2:0] aw_size;  // 突发大小 = 2^AxSIZE 
  // 写数据缓存
  reg [`XLEN_BUS] w_data;
  reg [7:0] w_strb;
  reg w_valid;
  reg w_last;

  // 写响应缓存
  reg b_ready;

  // // 握手缓存
  // reg                       axi_aw_handshake_buff;
  // reg                       axi_w_handshake_buff;
  // reg                       axi_b_handshake_buff;

  always @(posedge clock) begin
    if (reset) begin
      axi_wstate <= AXI_WRST;
      aw_valid <= `FALSE;
      w_valid <= `FALSE;
      w_last <= `FALSE;
      b_ready <= `FALSE;
      _arb_wdata_ready_o <= `FALSE;
    end else begin
      case (axi_wstate)
        AXI_WRST: begin
          axi_wstate <= AXI_WIDLE;
        end
        AXI_WIDLE: begin
          _arb_wdata_ready_o <= `FALSE;
          if (arb_write_valid_i & ~_arb_wdata_ready_o) begin : arb_write
            // 同时写数据和地址
            /* aw 通道 */
            axi_wstate <= AXI_WADDR_WDATA;
            aw_valid <= `TRUE;
            aw_addr <= arb_write_addr_i;
            aw_len <= 0;  // 不突发
            aw_size <= to_aw_size;
            /* w 通道 */
            w_valid <= `TRUE;
            w_last <= `TRUE;  // 表示最后一个数据，目前不实现突发传输
            //对于Narrow Burst，无论是读写请求，数据都出现在[RW]DATA对应访问地址%总线宽度的位置
            // wstrb wdata 与 data bus 的对齐处理在 mem 阶段处理
            w_strb <= arb_wmask_i;
            w_data <= arb_wdata_i;
            /* b 通道 */
            b_ready <= `TRUE;  // 默认为高
          end else begin
            axi_wstate <= AXI_WIDLE;
            aw_valid <= `FALSE;
            w_valid <= `FALSE;
            w_last <= `FALSE;
          end
        end
        AXI_WADDR_WDATA: begin
          // TODO 不支持突发传送
          if (axi_aw_handshake) begin
            aw_valid <= `FALSE;  // 握手成功后拉低 valid
          end
          if (axi_w_handshake) begin
            w_valid <= `FALSE;  // 握手成功后拉低 valid
            w_last  <= `FALSE;  // wlast 与 wvalid 一同拉低
          end
          if (axi_b_handshake) begin
            axi_wstate <= AXI_WIDLE;  // 一次写事务结束
            b_ready <= `FALSE;  // todo:为低表示一次写事务完毕,测试使用
            _arb_wdata_ready_o <= `TRUE;  // 通知 arb 写完成
          end

        end
        default: begin
          axi_wstate <= AXI_WIDLE;
        end
      endcase
    end
  end

  /************************ 读通道状态切换 ************************/
  localparam AXI_RSTATE_LEN = 3;
  localparam AXI_RRST = 3'd0;
  localparam AXI_RIDLE = 3'd1;
  localparam AXI_RADDR = 3'd2;
  localparam AXI_RDATA = 3'd3;


  wire [2:0 ]to_ar_size = ({3{arb_rsize_i[0]}}&`AXI_SIZE_BYTES_1)
                             | ({3{arb_rsize_i[1]}}&`AXI_SIZE_BYTES_2)
                             | ({3{arb_rsize_i[2]}}&`AXI_SIZE_BYTES_4)
                             | ({3{arb_rsize_i[3]}}&`AXI_SIZE_BYTES_8);


  reg [AXI_RSTATE_LEN-1:0] axi_rstate;
  reg _arb_rdata_ready_o;
  reg [`XLEN_BUS] _arb_rdata_o;

  reg ar_valid;
  reg [AXI_ADDR_WIDTH-1:0] ar_addr;
  reg [7:0] ar_len;  // 突发长度 AxLEN[7:0] + 1,0 表示不突发
  reg [2:0] ar_size;  // 突发大小 = 2^AxSIZE 
  reg r_ready;

  always @(posedge clock) begin
    if (reset) begin
      axi_rstate <= AXI_RRST;
      ar_valid <= `FALSE;
      ar_addr <= 0;
      ar_len <= 0;
      ar_size <= 0;
      r_ready <= `FALSE;
      _arb_rdata_ready_o <= `FALSE;
    end else begin
      case (axi_rstate)
        AXI_RRST: begin
          axi_rstate <= AXI_RIDLE;
        end
        AXI_RIDLE: begin
          _arb_rdata_ready_o <= `FALSE;
          // arb_raddr_valid_i & ~_arb_rdata_ready_o 为 arb 发出了读请求，且当前周期不为读数据返回周期
          // 当 _arb_rdata_ready_o = `TRUE 时，读数据返回，且 下一个读地址在下一个周期才会来到
          // 避免重复度读请求，_arb_rdata_ready_o = `TRUE 时，不能发生读请求
          if (arb_raddr_valid_i & ~_arb_rdata_ready_o) begin
            axi_rstate <= AXI_RADDR;
            /* ar 通道 */
            // cache miss 时,或者访问外设时,地址一定时对齐的
            ar_addr <= arb_read_addr_i;
            ar_valid <= `TRUE;
            ar_size <= to_ar_size;
            ar_len <= arb_rlen_i;  // TODO :测试突发传输
          end else begin
            axi_rstate <= AXI_RIDLE;
          end
        end
        AXI_RADDR: begin : wait_for_ar_handshake
          if (axi_ar_handshake) begin : wait_for_ar_handshake
            axi_rstate <= AXI_RDATA;
            ar_valid <= `FALSE;  // 地址握手成功后拉低
            ar_addr <= 0;
            r_ready <= `TRUE;  // 准备接收读数据
          end
        end
        AXI_RDATA: begin
          if (axi_r_handshake) begin : wait_for_r_handshake
            if (axi_r_last_i) begin  // 最后一个数据传输完成
              axi_rstate <= AXI_RIDLE;
              r_ready <= `FALSE;  // 数据握手成功后拉低
            end
            _arb_rdata_o <= axi_r_data_i;
            _arb_rdata_ready_o <= `TRUE;
          end else begin
            _arb_rdata_ready_o <= `FALSE;
          end
        end
        // AXI_RDATA: begin
        //   if (axi_r_handshake) begin : wait_for_r_handshake
        //     axi_rstate <= AXI_RIDLE;
        //     if (axi_r_resp_i == 2'b00) begin : R_RESP_OKAY
        //       _arb_rdata_o <= axi_r_data_i;
        //       _arb_rdata_ready_o <= `TRUE;
        //     end
        //     r_ready <= `FALSE;  // 数据握手成功后拉低
        //   end
        // end
        default: begin
          axi_rstate <= AXI_RIDLE;
        end
      endcase
    end
  end

  /********************类 sram 接口数据返回**************************/

  assign arb_rdata_o = _arb_rdata_o;
  assign arb_rdata_ready_o = _arb_rdata_ready_o;
  assign arb_wdata_ready_o = _arb_wdata_ready_o;


  // ------------------Write Transaction------------------

  wire [  AXI_ID_WIDTH-1:0] axi_id = {AXI_ID_WIDTH{1'b0}};
  wire [AXI_USER_WIDTH-1:0] axi_user = {AXI_USER_WIDTH{1'b0}};

  // 写地址通道  ��下没有备注初始化信号的都可能是你需要产生和用到的
  assign axi_aw_valid_o = aw_valid;
  assign axi_aw_addr_o = aw_addr;
  assign axi_aw_prot_o    = `AXI_PROT_UNPRIVILEGED_ACCESS | `AXI_PROT_SECURE_ACCESS | `AXI_PROT_DATA_ACCESS;  //初始化信号即可
  assign axi_aw_id_o = axi_id;  //初始化信号即可
  assign axi_aw_user_o = axi_user;  //初始化信号即可
  assign axi_aw_len_o = aw_len;
  assign axi_aw_size_o = aw_size;
  assign axi_aw_burst_o = `AXI_BURST_TYPE_INCR;
  assign axi_aw_lock_o = 1'b0;  //初始化信号即可
  assign axi_aw_cache_o = `AXI_AWCACHE_WRITE_BACK_READ_AND_WRITE_ALLOCATE;  //初始化信号即可
  assign axi_aw_qos_o = 4'h0;  //初始化信号即可
  assign axi_aw_region_o = 4'h0;  //初始化信号即可

  // 写数据通道
  assign axi_w_valid_o = w_valid;
  assign axi_w_data_o = w_data;
  assign axi_w_strb_o = w_strb;
  assign axi_w_last_o = w_last;
  assign axi_w_user_o = axi_user;  //初始化信号即可

  // 写应答通道
  assign axi_b_ready_o = b_ready;

  // ------------------Read Transaction------------------



  // Read address channel signals
  assign axi_ar_valid_o = ar_valid;  // leesum
  assign axi_ar_addr_o = ar_addr;  // leesum
  assign axi_ar_prot_o    = `AXI_PROT_UNPRIVILEGED_ACCESS | `AXI_PROT_SECURE_ACCESS | `AXI_PROT_DATA_ACCESS;  //初始化信号即可
  assign axi_ar_id_o = axi_id;  //初始化信号即可                        
  assign axi_ar_user_o = axi_user;  //初始化信号即可
  assign axi_ar_len_o = ar_len;  // leesum
  assign axi_ar_size_o = ar_size;  // leesum
  assign axi_ar_burst_o = `AXI_BURST_TYPE_INCR;
  assign axi_ar_lock_o = 1'b0;  //初始化信号即可
  assign axi_ar_cache_o   = `AXI_ARCACHE_NORMAL_NON_CACHEABLE_NON_BUFFERABLE;                                 //初始化信号即可
  assign axi_ar_qos_o = 4'h0;  //初始化信号即可

  // Read data channel signals
  assign axi_r_ready_o = r_ready;  //leesum

endmodule
