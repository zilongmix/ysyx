// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtop__Syms.h"


void Vtop___024root__traceChgSub0(Vtop___024root* vlSelf, VerilatedVcd* tracep);

void Vtop___024root__traceChgTop0(void* voidSelf, VerilatedVcd* tracep) {
    Vtop___024root* const __restrict vlSelf = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    {
        Vtop___024root__traceChgSub0((&vlSymsp->TOP), tracep);
    }
}

void Vtop___024root__traceChgSub0(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode + 1);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
            tracep->chgQData(oldp+0,(vlSelf->top__DOT__u_pc__DOT___pc_current),64);
            tracep->chgIData(oldp+2,((IData)(vlSelf->top__DOT__u_fetch__DOT__mem_data)),32);
            tracep->chgCData(oldp+3,((((((IData)(vlSelf->top__DOT__u_dcode__DOT___R_type) 
                                         | (IData)(vlSelf->top__DOT__u_dcode__DOT___I_type)) 
                                        | (IData)(vlSelf->top__DOT__u_dcode__DOT___type_store)) 
                                       | (IData)(vlSelf->top__DOT__u_dcode__DOT___type_branch))
                                       ? (0x1fU & (IData)(
                                                          (vlSelf->top__DOT__u_fetch__DOT__mem_data 
                                                           >> 0xfU)))
                                       : 0U)),5);
            tracep->chgCData(oldp+4,(((((IData)(vlSelf->top__DOT__u_dcode__DOT___R_type) 
                                        | (IData)(vlSelf->top__DOT__u_dcode__DOT___type_store)) 
                                       | (IData)(vlSelf->top__DOT__u_dcode__DOT___type_branch))
                                       ? (0x1fU & (IData)(
                                                          (vlSelf->top__DOT__u_fetch__DOT__mem_data 
                                                           >> 0x14U)))
                                       : 0U)),5);
            tracep->chgCData(oldp+5,(vlSelf->top__DOT__u_dcode__DOT___rd_idx),5);
            tracep->chgQData(oldp+6,(vlSelf->top__DOT__u_dcode__DOT___imm_data),64);
            tracep->chgCData(oldp+8,(vlSelf->top__DOT__u_dcode__DOT___alu_op),5);
            tracep->chgCData(oldp+9,(vlSelf->top__DOT__u_dcode__DOT___mem_op),4);
            tracep->chgCData(oldp+10,(vlSelf->top__DOT__u_dcode__DOT___exc_op),5);
            tracep->chgCData(oldp+11,(vlSelf->top__DOT__u_dcode__DOT___pc_op),3);
            tracep->chgQData(oldp+12,(vlSelf->top__DOT__rs1_data),64);
            tracep->chgQData(oldp+14,(vlSelf->top__DOT__rs2_data),64);
            tracep->chgQData(oldp+16,((((IData)(vlSelf->top__DOT__u_memory__DOT___unsigned) 
                                        | (IData)(vlSelf->top__DOT__u_memory__DOT___signed))
                                        ? ((IData)(vlSelf->top__DOT__u_memory__DOT___signed)
                                            ? ((IData)(vlSelf->top__DOT__u_memory__DOT___ls8byte)
                                                ? (
                                                   ((- (QData)((IData)(
                                                                       (1U 
                                                                        & (IData)(
                                                                                (vlSelf->top__DOT__u_memory__DOT___mem_read 
                                                                                >> 7U)))))) 
                                                    << 8U) 
                                                   | (QData)((IData)(
                                                                     (0xffU 
                                                                      & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read)))))
                                                : ((IData)(vlSelf->top__DOT__u_memory__DOT___ls16byte)
                                                    ? 
                                                   (((- (QData)((IData)(
                                                                        (1U 
                                                                         & (IData)(
                                                                                (vlSelf->top__DOT__u_memory__DOT___mem_read 
                                                                                >> 0xfU)))))) 
                                                     << 0x10U) 
                                                    | (QData)((IData)(
                                                                      (0xffffU 
                                                                       & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read)))))
                                                    : 
                                                   ((IData)(vlSelf->top__DOT__u_memory__DOT___ls32byte)
                                                     ? 
                                                    (((QData)((IData)(
                                                                      (- (IData)(
                                                                                (1U 
                                                                                & (IData)(
                                                                                (vlSelf->top__DOT__u_memory__DOT___mem_read 
                                                                                >> 0x1fU))))))) 
                                                      << 0x20U) 
                                                     | (QData)((IData)(vlSelf->top__DOT__u_memory__DOT___mem_read)))
                                                     : vlSelf->top__DOT__u_memory__DOT___mem_read)))
                                            : ((IData)(vlSelf->top__DOT__u_memory__DOT___unsigned)
                                                ? ((IData)(vlSelf->top__DOT__u_memory__DOT___ls8byte)
                                                    ? (QData)((IData)(
                                                                      (0xffU 
                                                                       & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read))))
                                                    : 
                                                   ((IData)(vlSelf->top__DOT__u_memory__DOT___ls16byte)
                                                     ? (QData)((IData)(
                                                                       (0xffffU 
                                                                        & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read))))
                                                     : 
                                                    ((IData)(vlSelf->top__DOT__u_memory__DOT___ls32byte)
                                                      ? (QData)((IData)(vlSelf->top__DOT__u_memory__DOT___mem_read))
                                                      : vlSelf->top__DOT__u_memory__DOT___mem_read)))
                                                : 0ULL))
                                        : vlSelf->top__DOT__u_execute__DOT___alu_out)),64);
            tracep->chgQData(oldp+18,(vlSelf->top__DOT__u_execute__DOT___alu_out),64);
            tracep->chgQData(oldp+20,(((IData)(vlSelf->top__DOT__u_memory__DOT___signed)
                                        ? ((IData)(vlSelf->top__DOT__u_memory__DOT___ls8byte)
                                            ? (((- (QData)((IData)(
                                                                   (1U 
                                                                    & (IData)(
                                                                              (vlSelf->top__DOT__u_memory__DOT___mem_read 
                                                                               >> 7U)))))) 
                                                << 8U) 
                                               | (QData)((IData)(
                                                                 (0xffU 
                                                                  & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read)))))
                                            : ((IData)(vlSelf->top__DOT__u_memory__DOT___ls16byte)
                                                ? (
                                                   ((- (QData)((IData)(
                                                                       (1U 
                                                                        & (IData)(
                                                                                (vlSelf->top__DOT__u_memory__DOT___mem_read 
                                                                                >> 0xfU)))))) 
                                                    << 0x10U) 
                                                   | (QData)((IData)(
                                                                     (0xffffU 
                                                                      & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read)))))
                                                : ((IData)(vlSelf->top__DOT__u_memory__DOT___ls32byte)
                                                    ? 
                                                   (((QData)((IData)(
                                                                     (- (IData)(
                                                                                (1U 
                                                                                & (IData)(
                                                                                (vlSelf->top__DOT__u_memory__DOT___mem_read 
                                                                                >> 0x1fU))))))) 
                                                     << 0x20U) 
                                                    | (QData)((IData)(vlSelf->top__DOT__u_memory__DOT___mem_read)))
                                                    : vlSelf->top__DOT__u_memory__DOT___mem_read)))
                                        : ((IData)(vlSelf->top__DOT__u_memory__DOT___unsigned)
                                            ? ((IData)(vlSelf->top__DOT__u_memory__DOT___ls8byte)
                                                ? (QData)((IData)(
                                                                  (0xffU 
                                                                   & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read))))
                                                : ((IData)(vlSelf->top__DOT__u_memory__DOT___ls16byte)
                                                    ? (QData)((IData)(
                                                                      (0xffffU 
                                                                       & (IData)(vlSelf->top__DOT__u_memory__DOT___mem_read))))
                                                    : 
                                                   ((IData)(vlSelf->top__DOT__u_memory__DOT___ls32byte)
                                                     ? (QData)((IData)(vlSelf->top__DOT__u_memory__DOT___mem_read))
                                                     : vlSelf->top__DOT__u_memory__DOT___mem_read)))
                                            : 0ULL))),64);
            tracep->chgBit(oldp+22,(((IData)(vlSelf->top__DOT__u_memory__DOT___unsigned) 
                                     | (IData)(vlSelf->top__DOT__u_memory__DOT___signed))));
            tracep->chgQData(oldp+23,((((3U == (IData)(vlSelf->top__DOT__u_dcode__DOT___pc_op))
                                         ? vlSelf->top__DOT__rs1_data
                                         : vlSelf->top__DOT__u_pc__DOT___pc_current) 
                                       + ((4U == (IData)(vlSelf->top__DOT__u_dcode__DOT___pc_op))
                                           ? 4ULL : 
                                          ((((1U == (IData)(vlSelf->top__DOT__u_dcode__DOT___pc_op)) 
                                             | (2U 
                                                == (IData)(vlSelf->top__DOT__u_dcode__DOT___pc_op))) 
                                            | (3U == (IData)(vlSelf->top__DOT__u_dcode__DOT___pc_op)))
                                            ? vlSelf->top__DOT__u_dcode__DOT___imm_data
                                            : 0ULL)))),64);
            tracep->chgQData(oldp+25,(vlSelf->top__DOT__u_fetch__DOT__mem_data),64);
            tracep->chgQData(oldp+27,(vlSelf->top__DOT__u_rv64reg__DOT__rf[0]),64);
            tracep->chgQData(oldp+29,(vlSelf->top__DOT__u_rv64reg__DOT__rf[1]),64);
            tracep->chgQData(oldp+31,(vlSelf->top__DOT__u_rv64reg__DOT__rf[2]),64);
            tracep->chgQData(oldp+33,(vlSelf->top__DOT__u_rv64reg__DOT__rf[3]),64);
            tracep->chgQData(oldp+35,(vlSelf->top__DOT__u_rv64reg__DOT__rf[4]),64);
            tracep->chgQData(oldp+37,(vlSelf->top__DOT__u_rv64reg__DOT__rf[5]),64);
            tracep->chgQData(oldp+39,(vlSelf->top__DOT__u_rv64reg__DOT__rf[6]),64);
            tracep->chgQData(oldp+41,(vlSelf->top__DOT__u_rv64reg__DOT__rf[7]),64);
            tracep->chgQData(oldp+43,(vlSelf->top__DOT__u_rv64reg__DOT__rf[8]),64);
            tracep->chgQData(oldp+45,(vlSelf->top__DOT__u_rv64reg__DOT__rf[9]),64);
            tracep->chgQData(oldp+47,(vlSelf->top__DOT__u_rv64reg__DOT__rf[10]),64);
            tracep->chgQData(oldp+49,(vlSelf->top__DOT__u_rv64reg__DOT__rf[11]),64);
            tracep->chgQData(oldp+51,(vlSelf->top__DOT__u_rv64reg__DOT__rf[12]),64);
            tracep->chgQData(oldp+53,(vlSelf->top__DOT__u_rv64reg__DOT__rf[13]),64);
            tracep->chgQData(oldp+55,(vlSelf->top__DOT__u_rv64reg__DOT__rf[14]),64);
            tracep->chgQData(oldp+57,(vlSelf->top__DOT__u_rv64reg__DOT__rf[15]),64);
            tracep->chgQData(oldp+59,(vlSelf->top__DOT__u_rv64reg__DOT__rf[16]),64);
            tracep->chgQData(oldp+61,(vlSelf->top__DOT__u_rv64reg__DOT__rf[17]),64);
            tracep->chgQData(oldp+63,(vlSelf->top__DOT__u_rv64reg__DOT__rf[18]),64);
            tracep->chgQData(oldp+65,(vlSelf->top__DOT__u_rv64reg__DOT__rf[19]),64);
            tracep->chgQData(oldp+67,(vlSelf->top__DOT__u_rv64reg__DOT__rf[20]),64);
            tracep->chgQData(oldp+69,(vlSelf->top__DOT__u_rv64reg__DOT__rf[21]),64);
            tracep->chgQData(oldp+71,(vlSelf->top__DOT__u_rv64reg__DOT__rf[22]),64);
            tracep->chgQData(oldp+73,(vlSelf->top__DOT__u_rv64reg__DOT__rf[23]),64);
            tracep->chgQData(oldp+75,(vlSelf->top__DOT__u_rv64reg__DOT__rf[24]),64);
            tracep->chgQData(oldp+77,(vlSelf->top__DOT__u_rv64reg__DOT__rf[25]),64);
            tracep->chgQData(oldp+79,(vlSelf->top__DOT__u_rv64reg__DOT__rf[26]),64);
            tracep->chgQData(oldp+81,(vlSelf->top__DOT__u_rv64reg__DOT__rf[27]),64);
            tracep->chgQData(oldp+83,(vlSelf->top__DOT__u_rv64reg__DOT__rf[28]),64);
            tracep->chgQData(oldp+85,(vlSelf->top__DOT__u_rv64reg__DOT__rf[29]),64);
            tracep->chgQData(oldp+87,(vlSelf->top__DOT__u_rv64reg__DOT__rf[30]),64);
            tracep->chgQData(oldp+89,(vlSelf->top__DOT__u_rv64reg__DOT__rf[31]),64);
            tracep->chgQData(oldp+91,(vlSelf->top__DOT__u_execute__DOT___alu_in1),64);
            tracep->chgQData(oldp+93,(vlSelf->top__DOT__u_execute__DOT___alu_in2),64);
            tracep->chgBit(oldp+95,(((((((((9U == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op)) 
                                           | (0xfU 
                                              == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op))) 
                                          & (IData)(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___isSLT)) 
                                         | (((0xaU 
                                              == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op)) 
                                             | (0x11U 
                                                == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op))) 
                                            & (IData)(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___isCF))) 
                                        | ((0xdU == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op)) 
                                           & (0U == 
                                              ((vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___add_out[0U] 
                                                | vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___add_out[1U]) 
                                               | vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___add_out[2U])))) 
                                       | ((0xeU == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op)) 
                                          & (0U != 
                                             ((vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___add_out[0U] 
                                               | vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___add_out[1U]) 
                                              | vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___add_out[2U])))) 
                                      | ((0x10U == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op)) 
                                         & (~ (IData)(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___isSLT)))) 
                                     | ((0x12U == (IData)(vlSelf->top__DOT__u_dcode__DOT___alu_op)) 
                                        & (~ (IData)(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___isCF))))));
            tracep->chgQData(oldp+96,(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___shifter_in1_inv),64);
            tracep->chgQData(oldp+98,(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___shifter_res),64);
            tracep->chgQData(oldp+100,(vlSelf->top__DOT__u_execute__DOT__u_alu__DOT___srl_res),64);
        }
        tracep->chgBit(oldp+102,(vlSelf->clk));
        tracep->chgBit(oldp+103,(vlSelf->rst));
    }
}

void Vtop___024root__traceCleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    Vtop___024root* const __restrict vlSelf = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    {
        vlSymsp->__Vm_activity = false;
        vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
        vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    }
}
