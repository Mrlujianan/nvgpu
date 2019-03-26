/*
 * Copyright (c) 2011-2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * GK20A Graphics
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef GK20A_H
#define GK20A_H

struct gk20a;
struct fifo_gk20a;
struct channel_gk20a;
struct gr_gk20a;
struct sim_nvgpu;
struct gk20a_ce_app;
struct gk20a_ctxsw_ucode_segments;
struct gk20a_fecs_trace;
struct gk20a_ctxsw_trace;
struct nvgpu_mem_alloc_tracker;
struct dbg_profiler_object_data;
struct gk20a_debug_output;
struct nvgpu_clk_pll_debug_data;
struct nvgpu_nvhost_dev;
struct nvgpu_netlist_vars;
struct netlist_av_list;
struct nvgpu_gr_global_ctx_buffer_desc;
struct nvgpu_gr_fecs_trace;
struct nvgpu_gpu_ctxsw_trace_entry;
struct nvgpu_cpu_time_correlation_sample;
struct nvgpu_mem_sgt;
struct nvgpu_warpstate;
struct nvgpu_clk_session;
struct nvgpu_clk_arb;
#ifdef CONFIG_GK20A_CTXSW_TRACE
struct nvgpu_gpu_ctxsw_trace_filter;
#endif
struct priv_cmd_entry;
struct nvgpu_setup_bind_args;
struct nvgpu_clk_pmupstate;
struct perf_pmupstate;
struct vin_device_v20;
struct nvgpu_avfsvinobjs;
struct nvgpu_set_fll_clk;
struct boardobjgrp;
struct boardobjgrp_pmu_cmd;
struct boardobjgrpmask;
struct nvgpu_sgt;
struct nvgpu_sgl;
struct nvgpu_device_info;
struct nvgpu_gr_subctx;
struct nvgpu_gr_zbc;
struct nvgpu_gr_zbc_entry;
struct nvgpu_gr_zbc_query_params;
struct nvgpu_gr_zcull_info;
struct nvgpu_channel_hw_state;
struct nvgpu_engine_status_info;
struct nvgpu_pbdma_status_info;
struct nvgpu_gr_config;
enum nvgpu_nvlink_minion_dlcmd;
struct nvgpu_cbc;

#include <nvgpu/lock.h>
#include <nvgpu/thread.h>

#include <nvgpu/mm.h>
#include <nvgpu/as.h>
#include <nvgpu/log.h>
#include <nvgpu/pramin.h>
#include <nvgpu/acr.h>
#include <nvgpu/kref.h>
#include <nvgpu/falcon.h>
#include <nvgpu/pmu.h>
#include <nvgpu/atomic.h>
#include <nvgpu/barrier.h>
#include <nvgpu/rwsem.h>
#include <nvgpu/nvlink.h>
#include <nvgpu/sim.h>
#include <nvgpu/ecc.h>
#include <nvgpu/tsg.h>
#include <nvgpu/sec2.h>
#include <nvgpu/cbc.h>
#include <nvgpu/ltc.h>
#include <nvgpu/nvgpu_err.h>

#include "gk20a/clk_gk20a.h"
#include "gk20a/fifo_gk20a.h"
#include "gk20a/gr_gk20a.h"

#ifdef CONFIG_DEBUG_FS
struct railgate_stats {
	unsigned long last_rail_gate_start;
	unsigned long last_rail_gate_complete;
	unsigned long last_rail_ungate_start;
	unsigned long last_rail_ungate_complete;
	unsigned long total_rail_gate_time_ms;
	unsigned long total_rail_ungate_time_ms;
	unsigned long railgating_cycle_count;
};
#endif

#define MC_INTR_UNIT_DISABLE	false
#define MC_INTR_UNIT_ENABLE		true

#define GPU_LIT_NUM_GPCS	0
#define GPU_LIT_NUM_PES_PER_GPC 1
#define GPU_LIT_NUM_ZCULL_BANKS 2
#define GPU_LIT_NUM_TPC_PER_GPC 3
#define GPU_LIT_NUM_SM_PER_TPC  4
#define GPU_LIT_NUM_FBPS	5
#define GPU_LIT_GPC_BASE	6
#define GPU_LIT_GPC_STRIDE	7
#define GPU_LIT_GPC_SHARED_BASE 8
#define GPU_LIT_TPC_IN_GPC_BASE 9
#define GPU_LIT_TPC_IN_GPC_STRIDE 10
#define GPU_LIT_TPC_IN_GPC_SHARED_BASE 11
#define GPU_LIT_PPC_IN_GPC_BASE	12
#define GPU_LIT_PPC_IN_GPC_STRIDE 13
#define GPU_LIT_PPC_IN_GPC_SHARED_BASE 14
#define GPU_LIT_ROP_BASE	15
#define GPU_LIT_ROP_STRIDE	16
#define GPU_LIT_ROP_SHARED_BASE 17
#define GPU_LIT_HOST_NUM_ENGINES 18
#define GPU_LIT_HOST_NUM_PBDMA	19
#define GPU_LIT_LTC_STRIDE	20
#define GPU_LIT_LTS_STRIDE	21
#define GPU_LIT_NUM_FBPAS	22
#define GPU_LIT_FBPA_STRIDE	23
#define GPU_LIT_FBPA_BASE	24
#define GPU_LIT_FBPA_SHARED_BASE 25
#define GPU_LIT_SM_PRI_STRIDE	26
#define GPU_LIT_SMPC_PRI_BASE		27
#define GPU_LIT_SMPC_PRI_SHARED_BASE	28
#define GPU_LIT_SMPC_PRI_UNIQUE_BASE	29
#define GPU_LIT_SMPC_PRI_STRIDE		30
#define GPU_LIT_TWOD_CLASS	31
#define GPU_LIT_THREED_CLASS	32
#define GPU_LIT_COMPUTE_CLASS	33
#define GPU_LIT_GPFIFO_CLASS	34
#define GPU_LIT_I2M_CLASS	35
#define GPU_LIT_DMA_COPY_CLASS	36
#define GPU_LIT_GPC_PRIV_STRIDE	37
#define GPU_LIT_PERFMON_PMMGPCTPCA_DOMAIN_START 38
#define GPU_LIT_PERFMON_PMMGPCTPCB_DOMAIN_START 39
#define GPU_LIT_PERFMON_PMMGPCTPC_DOMAIN_COUNT  40
#define GPU_LIT_PERFMON_PMMFBP_LTC_DOMAIN_START 41
#define GPU_LIT_PERFMON_PMMFBP_LTC_DOMAIN_COUNT 42
#define GPU_LIT_PERFMON_PMMFBP_ROP_DOMAIN_START 43
#define GPU_LIT_PERFMON_PMMFBP_ROP_DOMAIN_COUNT 44

#define nvgpu_get_litter_value(g, v) (g)->ops.get_litter_value((g), v)

#define MAX_TPC_PG_CONFIGS      3

enum nvgpu_unit;

enum nvgpu_flush_op;
enum gk20a_mem_rw_flag;

struct _resmgr_context;
struct nvgpu_gpfifo_entry;

struct nvgpu_gpfifo_userdata {
	struct nvgpu_gpfifo_entry __user *entries;
	struct _resmgr_context *context;
};

/*
 * gpu_ops should only contain function pointers! Non-function pointer members
 * should go in struct gk20a or be implemented with the boolean flag API defined
 * in nvgpu/enabled.h
 */

/* index for FB fault buffer functions */
#define	NVGPU_FB_MMU_FAULT_NONREPLAY_REG_INDEX		0U
#define	NVGPU_FB_MMU_FAULT_REPLAY_REG_INDEX		1U
#define	NVGPU_FB_MMU_FAULT_BUF_DISABLED			0U
#define	NVGPU_FB_MMU_FAULT_BUF_ENABLED			1U

/* Parameters for init_elcg_mode/init_blcg_mode */
enum {
	ELCG_RUN,	/* clk always run, i.e. disable elcg */
	ELCG_STOP,	/* clk is stopped */
	ELCG_AUTO	/* clk will run when non-idle, standard elcg mode */
};

enum {
	BLCG_RUN,	/* clk always run, i.e. disable blcg */
	BLCG_AUTO	/* clk will run when non-idle, standard blcg mode */
};

struct gpu_ops {
	struct {
		int (*determine_L2_size_bytes)(struct gk20a *gk20a);
		void (*set_zbc_color_entry)(struct gk20a *g,
					    u32 *color_val_l2,
					    u32 index);
		void (*set_zbc_depth_entry)(struct gk20a *g,
					    u32 depth_val,
					    u32 index);
		void (*set_zbc_s_entry)(struct gk20a *g,
					    u32 s_val,
					    u32 index);
		void (*set_enabled)(struct gk20a *g, bool enabled);
		void (*init_fs_state)(struct gk20a *g);
		void (*flush)(struct gk20a *g);
		bool (*pri_is_ltc_addr)(struct gk20a *g, u32 addr);
		bool (*is_ltcs_ltss_addr)(struct gk20a *g, u32 addr);
		bool (*is_ltcn_ltss_addr)(struct gk20a *g, u32 addr);
		void (*split_lts_broadcast_addr)(struct gk20a *g, u32 addr,
							u32 *priv_addr_table,
							u32 *priv_addr_table_index);
		void (*split_ltc_broadcast_addr)(struct gk20a *g, u32 addr,
							u32 *priv_addr_table,
							u32 *priv_addr_table_index);
		struct {
			void (*configure)(struct gk20a *g);
			void (*isr)(struct gk20a *g, u32 ltc);
			void (*en_illegal_compstat)(struct gk20a *g,
								bool enable);
		} intr;
		struct {
			int (*report_ecc_parity_err)(struct gk20a *g,
					u32 hw_id, u32 inst, u32 err_id,
					u64 err_addr, u64 count);
		} err_ops;
	} ltc;
	struct {
		void (*init)(struct gk20a *g, struct nvgpu_cbc *cbc);
		u64 (*get_base_divisor)(struct gk20a *g);
		int (*alloc_comptags)(struct gk20a *g,
					struct nvgpu_cbc *cbc);
		int (*ctrl)(struct gk20a *g, enum nvgpu_cbc_op op,
				u32 min, u32 max);
		u32 (*fix_config)(struct gk20a *g, int base);
	} cbc;
	struct {
		void (*isr_stall)(struct gk20a *g, u32 inst_id, u32 pri_base);
		u32 (*isr_nonstall)(struct gk20a *g, u32 inst_id, u32 pri_base);
		u32 (*get_num_pce)(struct gk20a *g);
		struct {
			int (*report_ce_err)(struct gk20a *g,
					u32 hw_id, u32 inst, u32 err_id,
					u32 status);
		} err_ops;
	} ce2;
	struct {
		u32 (*get_patch_slots)(struct gk20a *g);
		void (*access_smpc_reg)(struct gk20a *g, u32 quad, u32 offset);
		int (*handle_sw_method)(struct gk20a *g, u32 addr,
					 u32 class_num, u32 offset, u32 data);
		void (*set_alpha_circular_buffer_size)(struct gk20a *g,
					               u32 data);
		void (*set_circular_buffer_size)(struct gk20a *g, u32 data);
		void (*set_bes_crop_debug3)(struct gk20a *g, u32 data);
		void (*set_bes_crop_debug4)(struct gk20a *g, u32 data);
		bool (*is_valid_class)(struct gk20a *g, u32 class_num);
		bool (*is_valid_gfx_class)(struct gk20a *g, u32 class_num);
		bool (*is_valid_compute_class)(struct gk20a *g, u32 class_num);
		void (*get_sm_dsm_perf_regs)(struct gk20a *g,
						  u32 *num_sm_dsm_perf_regs,
						  u32 **sm_dsm_perf_regs,
						  u32 *perf_register_stride);
		void (*get_sm_dsm_perf_ctrl_regs)(struct gk20a *g,
						  u32 *num_sm_dsm_perf_regs,
						  u32 **sm_dsm_perf_regs,
						  u32 *perf_register_stride);
		void (*get_ovr_perf_regs)(struct gk20a *g,
						  u32 *num_ovr_perf_regs,
						  u32 **ovr_perf_regsr);
		void (*set_hww_esr_report_mask)(struct gk20a *g);
		void (*falcon_load_ucode)(struct gk20a *g,
				u64 addr_base,
				struct gk20a_ctxsw_ucode_segments *segments,
				u32 reg_offset);
		int (*load_ctxsw_ucode)(struct gk20a *g);
		void (*set_gpc_tpc_mask)(struct gk20a *g, u32 gpc_index);
		int (*alloc_obj_ctx)(struct channel_gk20a  *c,
				     u32 class_num, u32 flags);
		int (*decode_egpc_addr)(struct gk20a *g,
			u32 addr, enum ctxsw_addr_type *addr_type,
			u32 *gpc_num, u32 *tpc_num, u32 *broadcast_flags);
		void (*egpc_etpc_priv_addr_table)(struct gk20a *g, u32 addr,
			u32 gpc, u32 tpc, u32 broadcast_flags,
			u32 *priv_addr_table,
			u32 *priv_addr_table_index);
		bool (*is_tpc_addr)(struct gk20a *g, u32 addr);
		bool (*is_egpc_addr)(struct gk20a *g, u32 addr);
		bool (*is_etpc_addr)(struct gk20a *g, u32 addr);
		void (*get_egpc_etpc_num)(struct gk20a *g, u32 addr,
				u32 *gpc_num, u32 *tpc_num);
		u32 (*get_tpc_num)(struct gk20a *g, u32 addr);
		u32 (*get_egpc_base)(struct gk20a *g);
		void (*detect_sm_arch)(struct gk20a *g);
		int (*init_ctx_state)(struct gk20a *g);
		void (*free_gr_ctx)(struct gk20a *g,
				    struct vm_gk20a *vm, struct nvgpu_gr_ctx *gr_ctx);
		void (*powergate_tpc)(struct gk20a *g);
		int (*init_ctxsw_preemption_mode)(struct gk20a *g,
			  struct nvgpu_gr_ctx *gr_ctx, struct vm_gk20a *vm,
			  u32 class, u32 padding);
		void (*update_ctxsw_preemption_mode)(struct gk20a *g,
				struct nvgpu_gr_ctx *gr_ctx,
				struct nvgpu_gr_subctx *subctx);
		int (*update_smpc_ctxsw_mode)(struct gk20a *g,
				struct channel_gk20a *c,
				bool enable);
		int (*update_hwpm_ctxsw_mode)(struct gk20a *g,
				struct channel_gk20a *c,
				u64 gpu_va,
				u32 mode);
		void (*init_hwpm_pmm_register)(struct gk20a *g);
		void (*get_num_hwpm_perfmon)(struct gk20a *g, u32 *num_sys_perfmon,
				u32 *num_fbp_perfmon, u32 *num_gpc_perfmon);
		void (*set_pmm_register)(struct gk20a *g, u32 offset, u32 val,
				u32 num_chiplets, u32 num_perfmons);
		int (*dump_gr_regs)(struct gk20a *g,
				struct gk20a_debug_output *o);
		int (*update_pc_sampling)(struct channel_gk20a *ch,
					   bool enable);
		u32* (*get_rop_l2_en_mask)(struct gk20a *g);
		void (*init_sm_dsm_reg_info)(void);
		void (*init_ovr_sm_dsm_perf)(void);
		void (*init_cyclestats)(struct gk20a *g);
		int (*set_sm_debug_mode)(struct gk20a *g, struct channel_gk20a *ch,
					u64 sms, bool enable);
		void (*bpt_reg_info)(struct gk20a *g,
				struct nvgpu_warpstate *w_state);
		int (*handle_fecs_error)(struct gk20a *g,
				struct channel_gk20a *ch,
				struct gr_gk20a_isr_data *isr_data);
		int (*pre_process_sm_exception)(struct gk20a *g,
			u32 gpc, u32 tpc, u32 sm, u32 global_esr, u32 warp_esr,
			bool sm_debugger_attached,
			struct channel_gk20a *fault_ch,
			bool *early_exit, bool *ignore_debugger);
		u32 (*get_sm_hww_warp_esr)(struct gk20a *g,
						u32 gpc, u32 tpc, u32 sm);
		u32 (*get_sm_hww_global_esr)(struct gk20a *g,
						u32 gpc, u32 tpc, u32 sm);
		u64 (*get_sm_hww_warp_esr_pc)(struct gk20a *g, u32 offset);
		u32 (*get_sm_no_lock_down_hww_global_esr_mask)(struct gk20a *g);
		int  (*lock_down_sm)(struct gk20a *g, u32 gpc, u32 tpc, u32 sm,
				u32 global_esr_mask, bool check_errors);
		int  (*wait_for_sm_lock_down)(struct gk20a *g, u32 gpc, u32 tpc,
				u32 sm, u32 global_esr_mask, bool check_errors);
		void (*clear_sm_hww)(struct gk20a *g, u32 gpc, u32 tpc, u32 sm,
					 u32 global_esr);
		void (*get_esr_sm_sel)(struct gk20a *g, u32 gpc, u32 tpc,
					 u32 *esr_sm_sel);
		int (*handle_tpc_sm_ecc_exception)(struct gk20a *g,
			u32 gpc, u32 tpc,
			bool *post_event, struct channel_gk20a *fault_ch,
			u32 *hww_global_esr);
		int (*handle_sm_exception)(struct gk20a *g,
			u32 gpc, u32 tpc, u32 sm,
			bool *post_event, struct channel_gk20a *fault_ch,
			u32 *hww_global_esr);
		int (*handle_gcc_exception)(struct gk20a *g, u32 gpc, u32 tpc,
				bool *post_event, struct channel_gk20a *fault_ch,
				u32 *hww_global_esr);
		int (*handle_tex_exception)(struct gk20a *g, u32 gpc, u32 tpc,
						bool *post_event);
		int (*handle_tpc_mpc_exception)(struct gk20a *g,
					u32 gpc, u32 tpc, bool *post_event);
		int (*handle_gpc_gpccs_exception)(struct gk20a *g, u32 gpc,
						u32 gpc_exception);
		int (*handle_gpc_gpcmmu_exception)(struct gk20a *g, u32 gpc,
						u32 gpc_exception);
		int (*init_ecc)(struct gk20a *g);
		u32 (*get_lrf_tex_ltc_dram_override)(struct gk20a *g);
		int (*record_sm_error_state)(struct gk20a *g, u32 gpc, u32 tpc,
				u32 sm, struct channel_gk20a *fault_ch);
		int (*clear_sm_error_state)(struct gk20a *g,
				struct channel_gk20a *ch, u32 sm_id);
		int (*suspend_contexts)(struct gk20a *g,
				struct dbg_session_gk20a *dbg_s,
				int *ctx_resident_ch_fd);
		int (*resume_contexts)(struct gk20a *g,
				struct dbg_session_gk20a *dbg_s,
				int *ctx_resident_ch_fd);
		int (*set_preemption_mode)(struct channel_gk20a *ch,
				u32 graphics_preempt_mode,
				u32 compute_preempt_mode);
		int (*get_preemption_mode_flags)(struct gk20a *g,
		       struct nvgpu_preemption_modes_rec *preemption_modes_rec);
		int (*set_ctxsw_preemption_mode)(struct gk20a *g,
				struct nvgpu_gr_ctx *gr_ctx,
				struct vm_gk20a *vm, u32 class,
				u32 graphics_preempt_mode,
				u32 compute_preempt_mode);
		int (*set_boosted_ctx)(struct channel_gk20a *ch, bool boost);
		int (*init_sw_veid_bundle)(struct gk20a *g);
		int (*commit_inst)(struct channel_gk20a *c, u64 gpu_va);
		int (*trigger_suspend)(struct gk20a *g);
		int (*wait_for_pause)(struct gk20a *g, struct nvgpu_warpstate *w_state);
		int (*resume_from_pause)(struct gk20a *g);
		int (*clear_sm_errors)(struct gk20a *g);
		u32 (*tpc_enabled_exceptions)(struct gk20a *g);
		bool (*sm_debugger_attached)(struct gk20a *g);
		void (*suspend_single_sm)(struct gk20a *g,
				u32 gpc, u32 tpc, u32 sm,
				u32 global_esr_mask, bool check_errors);
		void (*suspend_all_sms)(struct gk20a *g,
				u32 global_esr_mask, bool check_errors);
		void (*resume_single_sm)(struct gk20a *g,
				u32 gpc, u32 tpc, u32 sm);
		void (*resume_all_sms)(struct gk20a *g);
		void (*init_gfxp_wfi_timeout_count)(struct gk20a *g);
		unsigned long (*get_max_gfxp_wfi_timeout_count)
					(struct gk20a *g);
		void (*ecc_init_scrub_reg)(struct gk20a *g);
		void (*fecs_host_int_enable)(struct gk20a *g);
		int (*handle_ssync_hww)(struct gk20a *g, u32 *ssync_esr);
		int (*handle_notify_pending)(struct gk20a *g,
					struct gr_gk20a_isr_data *isr_data);
		int (*handle_semaphore_pending)(struct gk20a *g,
					struct gr_gk20a_isr_data *isr_data);
		int (*add_ctxsw_reg_pm_fbpa)(struct gk20a *g,
				struct ctxsw_buf_offset_map_entry *map,
				struct netlist_aiv_list *regs,
				u32 *count, u32 *offset,
				u32 max_cnt, u32 base,
				u32 num_fbpas, u32 stride, u32 mask);
		int (*decode_priv_addr)(struct gk20a *g, u32 addr,
			      enum ctxsw_addr_type *addr_type,
			      u32 *gpc_num, u32 *tpc_num,
			      u32 *ppc_num, u32 *be_num,
			      u32 *broadcast_flags);
		int (*create_priv_addr_table)(struct gk20a *g,
					   u32 addr,
					   u32 *priv_addr_table,
					   u32 *num_registers);
		void (*split_fbpa_broadcast_addr)(struct gk20a *g, u32 addr,
					u32 num_fbpas,
					u32 *priv_addr_table,
					u32 *priv_addr_table_index);
		int (*init_sw_bundle64)(struct gk20a *g);
		int (*alloc_global_ctx_buffers)(struct gk20a *g);
		int (*commit_global_ctx_buffers)(struct gk20a *g,
			struct nvgpu_gr_ctx *gr_ctx, bool patch);
		u32 (*get_nonpes_aware_tpc)(struct gk20a *g, u32 gpc, u32 tpc);
		int (*get_offset_in_gpccs_segment)(struct gk20a *g,
			enum ctxsw_addr_type addr_type, u32 num_tpcs,
			u32 num_ppcs, u32 reg_list_ppc_count,
			u32 *__offset_in_segment);
		void (*set_debug_mode)(struct gk20a *g, bool enable);
		int (*init_gfxp_rtv_cb)(struct gk20a *g,
			  struct nvgpu_gr_ctx *gr_ctx, struct vm_gk20a *vm);
		void (*log_mme_exception)(struct gk20a *g);
		u32 (*get_ctx_spill_size)(struct gk20a *g);
		u32 (*get_ctx_pagepool_size)(struct gk20a *g);
		u32 (*get_ctx_betacb_size)(struct gk20a *g);
		u32 (*get_ctx_attrib_cb_size)(struct gk20a *g, u32 betacb_size);
		int (*halt_pipe)(struct gk20a *g);
		int (*reset)(struct gk20a *g);
		struct {
			u32 (*hw_get_fecs_header_size)(void);
			u32 (*hw_get_gpccs_header_size)(void);
			u32 (*hw_get_extended_buffer_segments_size_in_bytes)(void);
			u32 (*hw_extended_marker_size_in_bytes)(void);
			u32 (*hw_get_perf_counter_control_register_stride)(void);
			u32 (*hw_get_perf_counter_register_stride)(void);
			u32 (*get_main_image_ctx_id)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			u32 (*get_patch_count)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_patch_count)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u32 count);
			void (*set_patch_addr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*set_zcull_ptr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*set_zcull)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u32 mode);
			void (*set_zcull_mode_no_ctxsw)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			bool (*is_zcull_mode_separate_buffer)(u32 mode);
			void (*set_pm_ptr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*set_pm_mode)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u32 mode);
			void (*set_pm_smpc_mode)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, bool enable);
			u32 (*hw_get_pm_mode_no_ctxsw)(void);
			u32 (*hw_get_pm_mode_ctxsw)(void);
			u32 (*hw_get_pm_mode_stream_out_ctxsw)(void);
			void (*init_ctxsw_hdr_data)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_compute_preemption_mode_cta)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_compute_preemption_mode_cilp)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_graphics_preemption_mode_gfxp)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_cde_enabled)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_pc_sampling)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, bool enable);
			void (*set_priv_access_map_config_mode)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, bool allow_all);
			void (*set_priv_access_map_addr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*disable_verif_features)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			bool (*check_main_image_header_magic)(u8 *context);
			bool (*check_local_header_magic)(u8 *context);
			u32 (*get_num_gpcs)(u8 *context);
			u32 (*get_num_tpcs)(u8 *context);
			void (*get_extended_buffer_size_offset)(u8 *context,
				u32 *size, u32 *offset);
			void (*get_ppc_info)(u8 *context,
				u32 *num_ppcs, u32 *ppc_mask);
			u32 (*get_local_priv_register_ctl_offset)(u8 *context);
			u32 (*hw_get_ts_tag_invalid_timestamp)(void);
			u32 (*hw_get_ts_tag)(u64 ts);
			u64 (*hw_record_ts_timestamp)(u64 ts);
			u32 (*hw_get_ts_record_size_in_bytes)(void);
			bool (*is_ts_valid_record)(u32 magic_hi);
			u32 (*get_ts_buffer_aperture_mask)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*set_ts_num_records)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u32 num);
			void (*set_ts_buffer_ptr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr,
				u32 aperture_mask);
			void (*set_pmu_options_boost_clock_frequencies)(
				struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u32 boosted_ctx);
			void (*set_context_buffer_ptr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*set_full_preemption_ptr)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*set_full_preemption_ptr_veid0)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem, u64 addr);
			void (*set_type_per_veid_header)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
			void (*dump_ctxsw_stats)(struct gk20a *g,
				struct nvgpu_mem *ctx_mem);
		} ctxsw_prog;

		struct {
			u32 (*get_gpc_mask)(struct gk20a *g,
				struct nvgpu_gr_config *config);
			u32 (*get_gpc_tpc_mask)(struct gk20a *g,
				struct nvgpu_gr_config *config, u32 gpc_index);
			u32 (*get_tpc_count_in_gpc)(struct gk20a *g,
				struct nvgpu_gr_config *config, u32 gpc_index);
			u32 (*get_zcull_count_in_gpc)(struct gk20a *g,
				struct nvgpu_gr_config *config, u32 gpc_index);
			u32 (*get_pes_tpc_mask)(struct gk20a *g,
				struct nvgpu_gr_config *config, u32 gpc_index,
				u32 pes_index);
			u32 (*get_pd_dist_skip_table_size)(void);
			int (*init_sm_id_table)(struct nvgpu_gr_config *gr_config);
		} config;

		struct {
			u32 (*fecs_base_addr)(void);
			u32 (*gpccs_base_addr)(void);
			void (*dump_stats)(struct gk20a *g);
			u32 (*fecs_ctxsw_mailbox_size)(void);
			u32 (*get_fecs_ctx_state_store_major_rev_id)
							(struct gk20a *g);
		} falcon;
#ifdef CONFIG_GK20A_CTXSW_TRACE
		struct {
			int (*init)(struct gk20a *g);
			int (*max_entries)(struct gk20a *,
				struct nvgpu_gpu_ctxsw_trace_filter *filter);
			int (*flush)(struct gk20a *g);
			int (*poll)(struct gk20a *g);
			int (*enable)(struct gk20a *g);
			int (*disable)(struct gk20a *g);
			bool (*is_enabled)(struct gk20a *g);
			int (*reset)(struct gk20a *g);
			int (*bind_channel)(struct gk20a *g,
				struct nvgpu_mem *inst_block,
				struct nvgpu_gr_subctx *subctx,
				struct nvgpu_gr_ctx *gr_ctx, pid_t pid, u32 vmid);
			int (*unbind_channel)(struct gk20a *g,
				struct nvgpu_mem *inst_block);
			int (*deinit)(struct gk20a *g);
			int (*alloc_user_buffer)(struct gk20a *g,
						void **buf, size_t *size);
			int (*free_user_buffer)(struct gk20a *g);
			int (*mmap_user_buffer)(struct gk20a *g,
						struct vm_area_struct *vma);
			int (*set_filter)(struct gk20a *g,
				struct nvgpu_gpu_ctxsw_trace_filter *filter);
			u32 (*get_buffer_full_mailbox_val)(void);
			int (*get_read_index)(struct gk20a *g);
			int (*get_write_index)(struct gk20a *g);
			int (*set_read_index)(struct gk20a *g, int index);
			void (*vm_dev_write)(struct gk20a *g, u8 vmid,
				u32 *vm_update_mask,
				struct nvgpu_gpu_ctxsw_trace_entry *entry);
			void (*vm_dev_update)(struct gk20a *g, u32 vm_update_mask);
		} fecs_trace;
#endif

		struct {
			int (*add_color)(struct gk20a *g,
				struct nvgpu_gr_zbc_entry *color_val,
				u32 index);
			int (*add_depth)(struct gk20a *g,
				struct nvgpu_gr_zbc_entry *depth_val,
				u32 index);
			int (*set_table)(struct gk20a *g,
				struct nvgpu_gr_zbc *zbc,
				struct nvgpu_gr_zbc_entry *zbc_val);
			int (*query_table)(struct gk20a *g,
				struct nvgpu_gr_zbc *zbc,
				struct nvgpu_gr_zbc_query_params *query_params);
			int (*add_stencil)(struct gk20a *g,
				struct nvgpu_gr_zbc_entry *s_val, u32 index);
			u32 (*get_gpcs_swdx_dss_zbc_c_format_reg)(
				struct gk20a *g);
			u32 (*get_gpcs_swdx_dss_zbc_z_format_reg)(
				struct gk20a *g);
		} zbc;

		struct {
			int (*init_zcull_hw)(struct gk20a *g,
					struct nvgpu_gr_zcull *gr_zcull,
					struct nvgpu_gr_config *gr_config);
			int (*bind_ctxsw_zcull)(struct gk20a *g,
						struct channel_gk20a *c,
						u64 zcull_va,
						u32 mode);
			int (*get_zcull_info)(struct gk20a *g,
				struct nvgpu_gr_config *gr_config,
				struct nvgpu_gr_zcull *gr_zcull,
				struct nvgpu_gr_zcull_info *zcull_params);
			void (*program_zcull_mapping)(struct gk20a *g,
						u32 zcull_alloc_num,
						u32 *zcull_map_tiles);
		} zcull;

		struct {
			void (*align_regs_perf_pma)(u32 *offset);
			u32 (*get_active_fbpa_mask)(struct gk20a *g);
		} hwpm_map;

		struct {
			u32 (*get_fbp_en_mask)(struct gk20a *g);
			void (*lg_coalesce)(struct gk20a *g, u32 data);
			void (*su_coalesce)(struct gk20a *g, u32 data);
			void (*pes_vsc_stream)(struct gk20a *g);
			void (*gpc_mmu)(struct gk20a *g);
			void (*fifo_access)(struct gk20a *g, bool enable);
			void (*get_access_map)(struct gk20a *g,
				      u32 **whitelist, int *num_entries);
			u32 (*get_sm_id_size)(void);
			int (*sm_id_config)(struct gk20a *g, u32 *tpc_sm_id,
					    struct nvgpu_gr_config *gr_config);
			void (*sm_id_numbering)(struct gk20a *g,
						u32 gpc, u32 tpc, u32 smid);
			void (*tpc_mask)(struct gk20a *g,
					 u32 gpc_index, u32 pes_tpc_mask);
			int (*rop_mapping)(struct gk20a *g,
				struct nvgpu_gr_config *gr_config);
			int (*fs_state)(struct gk20a *g);
			void (*pd_tpc_per_gpc)(struct gk20a *g,
				struct nvgpu_gr_config *gr_config);
			void (*pd_skip_table_gpc)(struct gk20a *g,
				struct nvgpu_gr_config *gr_config);
			void (*cwd_gpcs_tpcs_num)(struct gk20a *g,
						  u32 gpc_count,
						  u32 tpc_count);
			int (*wait_empty)(struct gk20a *g);
			int (*wait_idle)(struct gk20a *g);
			int (*wait_fe_idle)(struct gk20a *g);
			int (*fe_pwr_mode_force_on)(struct gk20a *g,
				bool force_on);
			void (*override_context_reset)(struct gk20a *g);
			int (*preemption_state)(struct gk20a *g,
				u32 gfxp_wfi_timeout_count,
				bool gfxp_wfi_timeout_unit_usec);
			void (*fe_go_idle_timeout)(struct gk20a *g,
				bool enable);
			void (*load_method_init)(struct gk20a *g,
				struct netlist_av_list *sw_method_init);
			void (*commit_global_timeslice)(struct gk20a *g);
			u32 (*get_rtv_cb_size)(struct gk20a *g);
			void (*commit_rtv_cb)(struct gk20a *g, u64 addr,
				struct nvgpu_gr_ctx *gr_ctx, bool patch);
			void (*commit_gfxp_rtv_cb)(struct gk20a *g,
				struct nvgpu_gr_ctx *gr_ctx, bool patch);
			u32 (*get_bundle_cb_default_size)(struct gk20a *g);
			u32 (*get_min_gpm_fifo_depth)(struct gk20a *g);
			u32 (*get_bundle_cb_token_limit)(struct gk20a *g);
			u32 (*get_attrib_cb_default_size)(struct gk20a *g);
			u32 (*get_alpha_cb_default_size)(struct gk20a *g);
			u32 (*get_attrib_cb_gfxp_default_size)(struct gk20a *g);
			u32 (*get_attrib_cb_gfxp_size)(struct gk20a *g);
			u32 (*get_attrib_cb_size)(struct gk20a *g,
				u32 tpc_count);
			u32 (*get_alpha_cb_size)(struct gk20a *g,
				u32 tpc_count);
			u32 (*get_global_attr_cb_size)(struct gk20a *g,
				u32 tpc_count, u32 max_tpc);
			u32 (*get_global_ctx_cb_buffer_size)(struct gk20a *g);
			u32 (*get_global_ctx_pagepool_buffer_size)(
				struct gk20a *g);
			void (*commit_global_bundle_cb)(struct gk20a *g,
				struct nvgpu_gr_ctx *ch_ctx, u64 addr, u64 size,
				bool patch);
			u32 (*pagepool_default_size)(struct gk20a *g);
			void (*commit_global_pagepool)(struct gk20a *g,
				struct nvgpu_gr_ctx *ch_ctx, u64 addr, u32 size,
				bool patch, bool global_ctx);
			void (*commit_global_attrib_cb)(struct gk20a *g,
				struct nvgpu_gr_ctx *ch_ctx, u32 tpc_count,
				u32 max_tpc, u64 addr, bool patch);
			void (*commit_global_cb_manager)(struct gk20a *g,
				struct nvgpu_gr_config *config,
				struct nvgpu_gr_ctx *gr_ctx, bool patch);
		} init;

		struct {
			void (*enable_hww_exceptions)(struct gk20a *g);
			void (*enable_interrupts)(struct gk20a *g, bool enable);
			void (*enable_exceptions)(struct gk20a *g,
					struct nvgpu_gr_config *gr_config,
					bool enable);
			void (*enable_gpc_exceptions)(struct gk20a *g,
					struct nvgpu_gr_config *gr_config);
		} intr;

		u32 (*get_ctxsw_checksum_mismatch_mailbox_val)(void);

		struct {
			int (*report_ecc_parity_err)(struct gk20a *g,
					u32 hw_id, u32 inst, u32 err_id,
					u64 err_addr,
					u64 err_count);
			int (*report_gr_err)(struct gk20a *g,
					u32 hw_id, u32 inst, u32 err_id,
					struct gr_err_info *err_info);
			int (*report_ctxsw_err)(struct gk20a *g,
					u32 hw_id, u32 err_id,
					void *data);
		} err_ops;
	} gr;
	struct {
		void (*init_hw)(struct gk20a *g);
		void (*cbc_configure)(struct gk20a *g, struct nvgpu_cbc *cbc);
		void (*init_fs_state)(struct gk20a *g);
		void (*init_uncompressed_kind_map)(struct gk20a *g);
		void (*init_kind_attr)(struct gk20a *g);
		void (*set_mmu_page_size)(struct gk20a *g);
		bool (*set_use_full_comp_tag_line)(struct gk20a *g);
		u32 (*mmu_ctrl)(struct gk20a *g);
		u32 (*mmu_debug_ctrl)(struct gk20a *g);
		u32 (*mmu_debug_wr)(struct gk20a *g);
		u32 (*mmu_debug_rd)(struct gk20a *g);

		/*
		 * Compression tag line coverage. When mapping a compressible
		 * buffer, ctagline is increased when the virtual address
		 * crosses over the compression page boundary.
		 */
		u64 (*compression_page_size)(struct gk20a *g);

		/*
		 * Minimum page size that can be used for compressible kinds.
		 */
		unsigned int (*compressible_page_size)(struct gk20a *g);

		/*
		 * Compressible kind mappings: Mask for the virtual and physical
		 * address bits that must match.
		 */
		u64 (*compression_align_mask)(struct gk20a *g);

		void (*dump_vpr_info)(struct gk20a *g);
		void (*dump_wpr_info)(struct gk20a *g);
		int (*vpr_info_fetch)(struct gk20a *g);
		void (*read_wpr_info)(struct gk20a *g, u64 *wpr_base, u64 *wpr_size);
		bool (*is_debug_mode_enabled)(struct gk20a *g);
		void (*set_debug_mode)(struct gk20a *g, bool enable);
		int (*tlb_invalidate)(struct gk20a *g, struct nvgpu_mem *pdb);
		void (*hub_isr)(struct gk20a *g);
		void (*handle_replayable_fault)(struct gk20a *g);
		int (*mem_unlock)(struct gk20a *g);
		int (*init_nvlink)(struct gk20a *g);
		int (*enable_nvlink)(struct gk20a *g);
		void (*enable_hub_intr)(struct gk20a *g);
		void (*disable_hub_intr)(struct gk20a *g);
		int (*init_fbpa)(struct gk20a *g);
		void (*handle_fbpa_intr)(struct gk20a *g, u32 fbpa_id);
		void (*write_mmu_fault_buffer_lo_hi)(struct gk20a *g, u32 index,
			u32 addr_lo, u32 addr_hi);
		void (*write_mmu_fault_buffer_get)(struct gk20a *g, u32 index,
			u32 reg_val);
		void (*write_mmu_fault_buffer_size)(struct gk20a *g, u32 index,
			u32 reg_val);
		void (*write_mmu_fault_status)(struct gk20a *g, u32 reg_val);
		u32 (*read_mmu_fault_buffer_get)(struct gk20a *g, u32 index);
		u32 (*read_mmu_fault_buffer_put)(struct gk20a *g, u32 index);
		u32 (*read_mmu_fault_buffer_size)(struct gk20a *g, u32 index);
		void (*read_mmu_fault_addr_lo_hi)(struct gk20a *g,
			u32 *addr_lo, u32 *addr_hi);
		void (*read_mmu_fault_inst_lo_hi)(struct gk20a *g,
			u32 *inst_lo, u32 *inst_hi);
		u32 (*read_mmu_fault_info)(struct gk20a *g);
		u32 (*read_mmu_fault_status)(struct gk20a *g);
		int (*mmu_invalidate_replay)(struct gk20a *g,
			u32 invalidate_replay_val);
		bool (*mmu_fault_pending)(struct gk20a *g);
		bool (*is_fault_buf_enabled)(struct gk20a *g, u32 index);
		void (*fault_buf_set_state_hw)(struct gk20a *g,
				 u32 index, u32 state);
		void (*fault_buf_configure_hw)(struct gk20a *g, u32 index);
		size_t (*get_vidmem_size)(struct gk20a *g);
		int (*apply_pdb_cache_war)(struct gk20a *g);
		struct {
			int (*report_ecc_parity_err)(struct gk20a *g,
					u32 hw_id, u32 inst,
					u32 err_id, u64 err_addr,
					u64 err_cnt);
		} err_ops;
	} fb;
	struct {
		u32 (*falcon_base_addr)(void);
	} nvdec;
	struct {
		void (*slcg_bus_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_ce2_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_chiplet_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_ctxsw_firmware_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_fb_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_fifo_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_gr_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_ltc_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_perf_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_priring_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_pmu_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_therm_load_gating_prod)(struct gk20a *g, bool prod);
		void (*slcg_xbar_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_bus_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_ce_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_ctxsw_firmware_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_fb_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_fifo_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_gr_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_ltc_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_pwr_csb_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_pmu_load_gating_prod)(struct gk20a *g, bool prod);
		void (*blcg_xbar_load_gating_prod)(struct gk20a *g, bool prod);
		void (*pg_gr_load_gating_prod)(struct gk20a *g, bool prod);
	} cg;
	struct {
		int (*setup_sw)(struct gk20a *g);
		void (*cleanup_sw)(struct gk20a *g);
		int (*init_fifo_setup_hw)(struct gk20a *g);
		int (*alloc_inst)(struct gk20a *g, struct channel_gk20a *ch);
		void (*free_inst)(struct gk20a *g, struct channel_gk20a *ch);
		int (*preempt_channel)(struct gk20a *g, struct channel_gk20a *ch);
		int (*preempt_tsg)(struct gk20a *g, struct tsg_gk20a *tsg);
		int (*enable_tsg)(struct tsg_gk20a *tsg);
		void (*disable_tsg)(struct tsg_gk20a *tsg);
		int (*tsg_verify_channel_status)(struct channel_gk20a *ch);
		void (*tsg_verify_status_ctx_reload)(struct channel_gk20a *ch);
		void (*tsg_verify_status_faulted)(struct channel_gk20a *ch);
		void (*trigger_mmu_fault)(struct gk20a *g,
				unsigned long engine_ids);
		void (*get_mmu_fault_info)(struct gk20a *g, u32 mmu_fault_id,
			struct mmu_fault_info *mmfault);
		void (*get_mmu_fault_desc)(struct mmu_fault_info *mmfault);
		void (*get_mmu_fault_client_desc)(
					struct mmu_fault_info *mmfault);
		void (*get_mmu_fault_gpc_desc)(struct mmu_fault_info *mmfault);
		void (*apply_pb_timeout)(struct gk20a *g);
		int (*tsg_set_timeslice)(struct tsg_gk20a *tsg, u32 timeslice);
		u32 (*default_timeslice_us)(struct gk20a *g);
		int (*force_reset_ch)(struct channel_gk20a *ch,
					u32 err_code, bool verbose);
		int (*tsg_bind_channel)(struct tsg_gk20a *tsg,
				struct channel_gk20a *ch);
		int (*tsg_unbind_channel)(struct channel_gk20a *ch);
		int (*tsg_open)(struct tsg_gk20a *tsg);
		void (*tsg_release)(struct tsg_gk20a *tsg);
		int (*init_pbdma_info)(struct fifo_gk20a *f);
		int (*init_engine_info)(struct fifo_gk20a *f);
		u32 (*get_engines_mask_on_id)(struct gk20a *g,
			u32 id, bool is_tsg);
		void (*free_channel_ctx_header)(struct channel_gk20a *ch);
		void (*dump_channel_status_ramfc)(struct gk20a *g,
				struct gk20a_debug_output *o,
				struct nvgpu_channel_dump_info *info);
		int (*is_preempt_pending)(struct gk20a *g, u32 id,
			unsigned int id_type);
		int (*reset_enable_hw)(struct gk20a *g);
		void (*teardown_ch_tsg)(struct gk20a *g, u32 act_eng_bitmask,
			u32 id, unsigned int id_type, unsigned int rc_type,
			 struct mmu_fault_info *mmfault);
		void (*teardown_mask_intr)(struct gk20a *g);
		void (*teardown_unmask_intr)(struct gk20a *g);
		void (*init_eng_method_buffers)(struct gk20a *g,
						struct tsg_gk20a *tsg);
		void (*deinit_eng_method_buffers)(struct gk20a *g,
						struct tsg_gk20a *tsg);
		u32 (*get_preempt_timeout)(struct gk20a *g);
		void (*post_event_id)(struct tsg_gk20a *tsg, int event_id);
		void (*ch_abort_clean_up)(struct channel_gk20a *ch);
		int (*channel_suspend)(struct gk20a *g);
		int (*channel_resume)(struct gk20a *g);
		void (*set_error_notifier)(struct channel_gk20a *ch, u32 error);
		void (*ring_channel_doorbell)(struct channel_gk20a *c);
		u64 (*usermode_base)(struct gk20a *g);
		u32 (*doorbell_token)(struct channel_gk20a *c);
		int (*init_pdb_cache_war)(struct gk20a *g);
		void (*deinit_pdb_cache_war)(struct gk20a *g);
		int (*set_sm_exception_type_mask)(struct channel_gk20a *ch,
				u32 exception_mask);
		u32 (*runlist_busy_engines)(struct gk20a *g, u32 runlist_id);
		bool (*find_pbdma_for_runlist)(struct fifo_gk20a *f,
				u32 runlist_id, u32 *pbdma_id);
		int (*init_ce_engine_info)(struct fifo_gk20a *f);
		struct {
			int (*report_host_err)(struct gk20a *g,
					u32 hw_id, u32 inst, u32 err_id,
					u32 intr_info);
		} err_ops;

		void (*intr_0_enable)(struct gk20a *g, bool enable);
		void (*intr_0_isr)(struct gk20a *g);
		void (*intr_1_enable)(struct gk20a *g, bool enable);
		u32  (*intr_1_isr)(struct gk20a *g);
		bool (*handle_sched_error)(struct gk20a *g);
		void (*ctxsw_timeout_enable)(struct gk20a *g, bool enable);
		bool (*handle_ctxsw_timeout)(struct gk20a *g);

	} fifo;
	struct {
		int (*setup)(struct channel_gk20a *ch, u64 gpfifo_base,
				u32 gpfifo_entries, u64 pbdma_acquire_timeout,
				u32 flags);
		void (*capture_ram_dump)(struct gk20a *g,
				struct channel_gk20a *ch,
				struct nvgpu_channel_dump_info *info);
		int (*commit_userd)(struct channel_gk20a *ch);
		u32 (*get_syncpt)(struct channel_gk20a *ch);
		void (*set_syncpt)(struct channel_gk20a *ch, u32 syncpt);
	} ramfc;
	struct {
		int (*reschedule)(struct channel_gk20a *ch, bool preempt_next);
		int (*reschedule_preempt_next_locked)(struct channel_gk20a *ch,
				bool wait_preempt);
		int (*update_for_channel)(struct gk20a *g, u32 runlist_id,
				struct channel_gk20a *ch, bool add,
				bool wait_for_finish);
		int (*reload)(struct gk20a *g, u32 runlist_id,
				bool add, bool wait_for_finish);
		int (*set_interleave)(struct gk20a *g, u32 id,
					u32 runlist_id, u32 new_level);
		u32 (*count_max)(void);
		u32 (*entry_size)(struct gk20a *g);
		u32 (*length_max)(struct gk20a *g);
		void (*get_tsg_entry)(struct tsg_gk20a *tsg, u32 *runlist);
		void (*get_ch_entry)(struct channel_gk20a *ch, u32 *runlist);
		void (*hw_submit)(struct gk20a *g, u32 runlist_id,
			u32 count, u32 buffer_index);
		int (*wait_pending)(struct gk20a *g, u32 runlist_id);
		void (*write_state)(struct gk20a *g, u32 runlists_mask,
				u32 runlist_state);
	} runlist;
	struct {
		int (*setup_sw)(struct gk20a *g);
		void (*cleanup_sw)(struct gk20a *g);
		int (*init_mem)(struct gk20a *g, struct channel_gk20a *c);
		u32 (*gp_get)(struct gk20a *g, struct channel_gk20a *c);
		void (*gp_put)(struct gk20a *g, struct channel_gk20a *c);
		u64 (*pb_get)(struct gk20a *g, struct channel_gk20a *c);
		u32 (*entry_size)(struct gk20a *g);
	} userd;

	struct {
		bool (*is_fault_engine_subid_gpc)(struct gk20a *g,
					 u32 engine_subid);
	} engine;

	struct {
		unsigned int (*handle_pbdma_intr_0)(struct gk20a *g,
					u32 pbdma_id, u32 pbdma_intr_0,
					u32 *handled, u32 *error_notifier);
		unsigned int (*handle_pbdma_intr_1)(struct gk20a *g,
					u32 pbdma_id, u32 pbdma_intr_1,
					u32 *handled, u32 *error_notifier);
		u32 (*get_pbdma_signature)(struct gk20a *g);
		void (*dump_pbdma_status)(struct gk20a *g,
				struct gk20a_debug_output *o);
		u32 (*pbdma_acquire_val)(u64 timeout);
		u32 (*read_pbdma_data)(struct gk20a *g, u32 pbdma_id);
		void (*reset_pbdma_header)(struct gk20a *g, u32 pbdma_id);
		u32 (*device_fatal_0_intr_descs)(void);
		u32 (*channel_fatal_0_intr_descs)(void);
		u32 (*restartable_0_intr_descs)(void);
	} pbdma;

	struct {
#ifdef CONFIG_TEGRA_GK20A_NVHOST
		int (*alloc_syncpt_buf)(struct channel_gk20a *c,
				u32 syncpt_id, struct nvgpu_mem *syncpt_buf);
		void (*free_syncpt_buf)(struct channel_gk20a *c,
				struct nvgpu_mem *syncpt_buf);
		void (*add_syncpt_wait_cmd)(struct gk20a *g,
					struct priv_cmd_entry *cmd, u32 off,
					u32 id, u32 thresh, u64 gpu_va);
		u32 (*get_syncpt_wait_cmd_size)(void);
		void (*add_syncpt_incr_cmd)(struct gk20a *g,
			bool wfi_cmd, struct priv_cmd_entry *cmd,
			u32 id, u64 gpu_va);
		u32 (*get_syncpt_incr_cmd_size)(bool wfi_cmd);
		int (*get_sync_ro_map)(struct vm_gk20a *vm,
				u64 *base_gpuva, u32 *sync_size);
		u32 (*get_syncpt_incr_per_release)(void);
#endif
		u32 (*get_sema_wait_cmd_size)(void);
		u32 (*get_sema_incr_cmd_size)(void);
		void (*add_sema_cmd)(struct gk20a *g,
			struct nvgpu_semaphore *s, u64 sema_va,
			struct priv_cmd_entry *cmd,
			u32 off, bool acquire, bool wfi);
	} sync;
	struct {
		void (*bind)(struct channel_gk20a *ch);
		void (*unbind)(struct channel_gk20a *ch);
		void (*enable)(struct channel_gk20a *ch);
		void (*disable)(struct channel_gk20a *ch);
		u32 (*count)(struct gk20a *g);
		void (*read_state)(struct gk20a *g, struct channel_gk20a *ch,
				struct nvgpu_channel_hw_state *state);
		void (*force_ctx_reload)(struct channel_gk20a *ch);
		void (*reset_faulted)(struct gk20a *g, struct channel_gk20a *ch,
				bool eng, bool pbdma);
		int (*set_syncpt)(struct channel_gk20a *ch);
	} channel;
	struct {
		bool (*check_ctxsw_timeout)(struct tsg_gk20a *tsg,
					bool *verbose, u32 *ms);
	} tsg;
	struct {
		void (*read_engine_status_info) (struct gk20a *g,
			u32 engine_id, struct nvgpu_engine_status_info *status);
		void (*dump_engine_status)(struct gk20a *g,
				struct gk20a_debug_output *o);
	} engine_status;
	struct {
		void (*read_pbdma_status_info) (struct gk20a *g,
			u32 engine_id, struct nvgpu_pbdma_status_info *status);
	} pbdma_status;
	struct pmu_v {
		u32 (*get_pmu_cmdline_args_size)(struct nvgpu_pmu *pmu);
		void (*set_pmu_cmdline_args_cpu_freq)(struct nvgpu_pmu *pmu,
			u32 freq);
		void (*set_pmu_cmdline_args_trace_size)(struct nvgpu_pmu *pmu,
			u32 size);
		void (*set_pmu_cmdline_args_trace_dma_base)(
				struct nvgpu_pmu *pmu);
		void (*config_pmu_cmdline_args_super_surface)(
				struct nvgpu_pmu *pmu);
		void (*set_pmu_cmdline_args_trace_dma_idx)(
			struct nvgpu_pmu *pmu, u32 idx);
		void * (*get_pmu_cmdline_args_ptr)(struct nvgpu_pmu *pmu);
		u32 (*get_pmu_allocation_struct_size)(struct nvgpu_pmu *pmu);
		void (*set_pmu_allocation_ptr)(struct nvgpu_pmu *pmu,
				void **pmu_alloc_ptr, void *assign_ptr);
		void (*pmu_allocation_set_dmem_size)(struct nvgpu_pmu *pmu,
				void *pmu_alloc_ptr, u16 size);
		u16 (*pmu_allocation_get_dmem_size)(struct nvgpu_pmu *pmu,
				void *pmu_alloc_ptr);
		u32 (*pmu_allocation_get_dmem_offset)(struct nvgpu_pmu *pmu,
				void *pmu_alloc_ptr);
		u32 * (*pmu_allocation_get_dmem_offset_addr)(
				struct nvgpu_pmu *pmu, void *pmu_alloc_ptr);
		void (*pmu_allocation_set_dmem_offset)(struct nvgpu_pmu *pmu,
				void *pmu_alloc_ptr, u32 offset);
		void * (*pmu_allocation_get_fb_addr)(
				struct nvgpu_pmu *pmu, void *pmu_alloc_ptr);
		u32 (*pmu_allocation_get_fb_size)(
				struct nvgpu_pmu *pmu, void *pmu_alloc_ptr);
		void (*get_pmu_init_msg_pmu_queue_params)(
				u32 id,
				void *pmu_init_msg,
				u32 *index, u32 *offset, u32 *size);
		void *(*get_pmu_msg_pmu_init_msg_ptr)(
				struct pmu_init_msg *init);
		u16 (*get_pmu_init_msg_pmu_sw_mg_off)(
			union pmu_init_msg_pmu *init_msg);
		u16 (*get_pmu_init_msg_pmu_sw_mg_size)(
			union pmu_init_msg_pmu *init_msg);
		u32 (*get_pmu_perfmon_cmd_start_size)(void);
		int (*get_perfmon_cmd_start_offsetofvar)(
				enum pmu_perfmon_cmd_start_fields field,
				u32 *offset);
		void (*perfmon_start_set_cmd_type)(struct pmu_perfmon_cmd *pc,
				u8 value);
		void (*perfmon_start_set_group_id)(struct pmu_perfmon_cmd *pc,
				u8 value);
		void (*perfmon_start_set_state_id)(struct pmu_perfmon_cmd *pc,
				u8 value);
		void (*perfmon_start_set_flags)(struct pmu_perfmon_cmd *pc,
				u8 value);
		u8 (*perfmon_start_get_flags)(struct pmu_perfmon_cmd *pc);
		u32 (*get_pmu_perfmon_cmd_init_size)(void);
		int (*get_perfmon_cmd_init_offsetofvar)(
				enum pmu_perfmon_cmd_start_fields field,
				u32 *offset);
		void (*perfmon_cmd_init_set_sample_buffer)(
				struct pmu_perfmon_cmd *pc, u16 value);
		void (*perfmon_cmd_init_set_dec_cnt)(
				struct pmu_perfmon_cmd *pc, u8 value);
		void (*perfmon_cmd_init_set_base_cnt_id)(
				struct pmu_perfmon_cmd *pc, u8 value);
		void (*perfmon_cmd_init_set_samp_period_us)(
				struct pmu_perfmon_cmd *pc, u32 value);
		void (*perfmon_cmd_init_set_num_cnt)(struct pmu_perfmon_cmd *pc,
				u8 value);
		void (*perfmon_cmd_init_set_mov_avg)(struct pmu_perfmon_cmd *pc,
				u8 value);
		void *(*get_pmu_seq_in_a_ptr)(
				struct pmu_sequence *seq);
		void *(*get_pmu_seq_out_a_ptr)(
				struct pmu_sequence *seq);
		void (*set_pmu_cmdline_args_secure_mode)(struct nvgpu_pmu *pmu,
			u8 val);
		u32 (*get_perfmon_cntr_sz)(struct nvgpu_pmu *pmu);
		void * (*get_perfmon_cntr_ptr)(struct nvgpu_pmu *pmu);
		void (*set_perfmon_cntr_ut)(struct nvgpu_pmu *pmu, u16 ut);
		void (*set_perfmon_cntr_lt)(struct nvgpu_pmu *pmu, u16 lt);
		void (*set_perfmon_cntr_valid)(struct nvgpu_pmu *pmu, u8 val);
		void (*set_perfmon_cntr_index)(struct nvgpu_pmu *pmu, u8 val);
		void (*set_perfmon_cntr_group_id)(struct nvgpu_pmu *pmu,
				u8 gid);

		u8 (*pg_cmd_eng_buf_load_size)(struct pmu_pg_cmd *pg);
		void (*pg_cmd_eng_buf_load_set_cmd_type)(struct pmu_pg_cmd *pg,
				u8 value);
		void (*pg_cmd_eng_buf_load_set_engine_id)(struct pmu_pg_cmd *pg,
				u8 value);
		void (*pg_cmd_eng_buf_load_set_buf_idx)(struct pmu_pg_cmd *pg,
				u8 value);
		void (*pg_cmd_eng_buf_load_set_pad)(struct pmu_pg_cmd *pg,
				u8 value);
		void (*pg_cmd_eng_buf_load_set_buf_size)(struct pmu_pg_cmd *pg,
				u16 value);
		void (*pg_cmd_eng_buf_load_set_dma_base)(struct pmu_pg_cmd *pg,
				u32 value);
		void (*pg_cmd_eng_buf_load_set_dma_offset)(struct pmu_pg_cmd *pg,
				u8 value);
		void (*pg_cmd_eng_buf_load_set_dma_idx)(struct pmu_pg_cmd *pg,
				u8 value);
		struct {
			int (*boardobjgrp_pmucmd_construct_impl)
				(struct gk20a *g,
				struct boardobjgrp *pboardobjgrp,
				struct boardobjgrp_pmu_cmd *cmd, u8 id, u8 msgid,
				u16 hdrsize, u16 entrysize, u16 fbsize, u32 ss_offset,
				u8 rpc_func_id);
			int (*boardobjgrp_pmuset_impl)(struct gk20a *g,
				struct boardobjgrp *pboardobjgrp);
			int (*boardobjgrp_pmugetstatus_impl)(struct gk20a *g,
				struct boardobjgrp *pboardobjgrp,
				struct boardobjgrpmask *mask);
			int (*is_boardobjgrp_pmucmd_id_valid)(struct gk20a *g,
				struct boardobjgrp *pboardobjgrp,
				struct boardobjgrp_pmu_cmd *cmd);
		} boardobj;
		struct {
			u32 (*get_vbios_clk_domain)(u32 vbios_domain);
			int (*clk_avfs_get_vin_cal_data)(struct gk20a *g,
				struct nvgpu_avfsvinobjs *pvinobjs,
				struct vin_device_v20 *pvindev);
			u32 (*clk_vf_change_inject_data_fill)(struct gk20a *g,
				struct nv_pmu_clk_rpc *rpccall,
				struct nvgpu_set_fll_clk *setfllclk);
			int (*clk_set_boot_clk)(struct gk20a *g);
		}clk;
	} pmu_ver;
	struct {
		int (*get_netlist_name)(struct gk20a *g, int index, char *name);
		bool (*is_fw_defined)(void);
	} netlist;
	struct {
		u64 (*gmmu_map)(struct vm_gk20a *vm,
				u64 map_offset,
				struct nvgpu_sgt *sgt,
				u64 buffer_offset,
				u64 size,
				u32 pgsz_idx,
				u8 kind_v,
				u32 ctag_offset,
				u32 flags,
				enum gk20a_mem_rw_flag rw_flag,
				bool clear_ctags,
				bool sparse,
				bool priv,
				struct vm_gk20a_mapping_batch *batch,
				enum nvgpu_aperture aperture);
		void (*gmmu_unmap)(struct vm_gk20a *vm,
				u64 vaddr,
				u64 size,
				u32 pgsz_idx,
				bool va_allocated,
				enum gk20a_mem_rw_flag rw_flag,
				bool sparse,
				struct vm_gk20a_mapping_batch *batch);
		int (*vm_bind_channel)(struct vm_gk20a *vm,
				struct channel_gk20a *ch);
		int (*fb_flush)(struct gk20a *g);
		void (*l2_invalidate)(struct gk20a *g);
		int (*l2_flush)(struct gk20a *g, bool invalidate);
		void (*cbc_clean)(struct gk20a *g);
		void (*set_big_page_size)(struct gk20a *g,
					  struct nvgpu_mem *mem, u32 size);
		u32 (*get_big_page_sizes)(void);
		u32 (*get_default_big_page_size)(void);
		u32 (*get_iommu_bit)(struct gk20a *g);
		int (*init_mm_setup_hw)(struct gk20a *g);
		bool (*is_bar1_supported)(struct gk20a *g);
		int (*init_bar2_vm)(struct gk20a *g);
		void (*remove_bar2_vm)(struct gk20a *g);
		const struct gk20a_mmu_level *
			(*get_mmu_levels)(struct gk20a *g, u32 big_page_size);
		void (*init_pdb)(struct gk20a *g, struct nvgpu_mem *inst_block,
				struct vm_gk20a *vm);
		u64 (*gpu_phys_addr)(struct gk20a *g,
				     struct nvgpu_gmmu_attrs *attrs, u64 phys);
		int (*alloc_inst_block)(struct gk20a *g,
					struct nvgpu_mem *inst_block);
		void (*init_inst_block)(struct nvgpu_mem *inst_block,
				struct vm_gk20a *vm, u32 big_page_size);
		bool (*mmu_fault_pending)(struct gk20a *g);
		void (*fault_info_mem_destroy)(struct gk20a *g);
		void (*mmu_fault_disable_hw)(struct gk20a *g);
		u32 (*get_kind_invalid)(void);
		u32 (*get_kind_pitch)(void);
		u32 (*get_flush_retries)(struct gk20a *g,
							enum nvgpu_flush_op op);
		u64 (*bar1_map_userd)(struct gk20a *g, struct nvgpu_mem *mem, u32 offset);
		int (*vm_as_alloc_share)(struct gk20a *g, struct vm_gk20a *vm);
		void (*vm_as_free_share)(struct vm_gk20a *vm);
	} mm;
	/*
	 * This function is called to allocate secure memory (memory
	 * that the CPU cannot see). The function should fill the
	 * context buffer descriptor (especially fields destroy, sgt,
	 * size).
	 */
	int (*secure_alloc)(struct gk20a *g,
				struct nvgpu_gr_global_ctx_buffer_desc *desc,
				size_t size);
	struct {
		void (*exit)(struct gk20a *g, struct nvgpu_mem *mem,
			struct nvgpu_sgl *sgl);
		u32 (*data032_r)(u32 i);
	} pramin;
	struct {
		int (*init_therm_setup_hw)(struct gk20a *g);
		void (*init_elcg_mode)(struct gk20a *g, u32 mode, u32 engine);
		void (*init_blcg_mode)(struct gk20a *g, u32 mode, u32 engine);
		int (*elcg_init_idle_filters)(struct gk20a *g);
#ifdef CONFIG_DEBUG_FS
		void (*therm_debugfs_init)(struct gk20a *g);
#endif
		int (*get_internal_sensor_curr_temp)(struct gk20a *g, u32 *temp_f24_8);
		void (*get_internal_sensor_limits)(s32 *max_24_8,
							s32 *min_24_8);
		int (*configure_therm_alert)(struct gk20a *g, s32 curr_warn_temp);
		void (*throttle_enable)(struct gk20a *g, u32 val);
		u32 (*throttle_disable)(struct gk20a *g);
		void (*idle_slowdown_enable)(struct gk20a *g, u32 val);
		u32 (*idle_slowdown_disable)(struct gk20a *g);
	} therm;
	struct {
		bool (*is_pmu_supported)(struct gk20a *g);
		u32 (*falcon_base_addr)(void);
		int (*pmu_setup_hw_and_bootstrap)(struct gk20a *g);
		int (*pmu_nsbootstrap)(struct nvgpu_pmu *pmu);
		int (*pmu_init_perfmon)(struct nvgpu_pmu *pmu);
		int (*pmu_perfmon_start_sampling)(struct nvgpu_pmu *pmu);
		int (*pmu_perfmon_stop_sampling)(struct nvgpu_pmu *pmu);
		int (*pmu_perfmon_get_samples_rpc)(struct nvgpu_pmu *pmu);
		int (*pmu_setup_elpg)(struct gk20a *g);
		u32 (*pmu_get_queue_head)(u32 i);
		u32 (*pmu_get_queue_head_size)(void);
		u32 (*pmu_get_queue_tail_size)(void);
		u32 (*pmu_get_queue_tail)(u32 i);
		int (*pmu_reset)(struct gk20a *g);
		int (*pmu_queue_head)(struct gk20a *g, u32 queue_id,
				      u32 queue_index, u32 *head, bool set);
		int (*pmu_queue_tail)(struct gk20a *g, u32 queue_id,
				      u32 queue_index, u32 *tail, bool set);
		void (*pmu_msgq_tail)(struct nvgpu_pmu *pmu,
			u32 *tail, bool set);
		u32 (*pmu_mutex_size)(void);
		int (*pmu_mutex_acquire)(struct nvgpu_pmu *pmu,
			u32 id, u32 *token);
		int (*pmu_mutex_release)(struct nvgpu_pmu *pmu,
			u32 id, u32 *token);
		bool (*pmu_is_interrupted)(struct nvgpu_pmu *pmu);
		void (*pmu_isr)(struct gk20a *g);
		void (*pmu_init_perfmon_counter)(struct gk20a *g);
		void (*pmu_pg_idle_counter_config)(struct gk20a *g, u32 pg_engine_id);
		u32  (*pmu_read_idle_counter)(struct gk20a *g, u32 counter_id);
		u32  (*pmu_read_idle_intr_status)(struct gk20a *g);
		void (*pmu_clear_idle_intr_status)(struct gk20a *g);
		void (*pmu_reset_idle_counter)(struct gk20a *g, u32 counter_id);
		void (*pmu_dump_elpg_stats)(struct nvgpu_pmu *pmu);
		void (*pmu_dump_falcon_stats)(struct nvgpu_pmu *pmu);
		void (*pmu_enable_irq)(struct nvgpu_pmu *pmu, bool enable);
		int (*init_wpr_region)(struct gk20a *g);
		int (*load_lsfalcon_ucode)(struct gk20a *g, u32 falconidmask);
		void (*write_dmatrfbase)(struct gk20a *g, u32 addr);
		int (*pmu_elpg_statistics)(struct gk20a *g, u32 pg_engine_id,
			struct pmu_pg_stats_data *pg_stat_data);
		int (*pmu_pg_init_param)(struct gk20a *g, u32 pg_engine_id);
		int (*pmu_pg_set_sub_feature_mask)(struct gk20a *g,
			u32 pg_engine_id);
		u32 (*pmu_pg_supported_engines_list)(struct gk20a *g);
		u32 (*pmu_pg_engines_feature_list)(struct gk20a *g,
			u32 pg_engine_id);
		bool (*pmu_is_lpwr_feature_supported)(struct gk20a *g,
			u32 feature_id);
		int (*pmu_lpwr_enable_pg)(struct gk20a *g, bool pstate_lock);
		int (*pmu_lpwr_disable_pg)(struct gk20a *g, bool pstate_lock);
		int (*pmu_pg_param_post_init)(struct gk20a *g);
		void (*dump_secure_fuses)(struct gk20a *g);
		int (*reset_engine)(struct gk20a *g, bool do_reset);
		bool (*is_engine_in_reset)(struct gk20a *g);
		void (*handle_ext_irq)(struct gk20a *g, u32 intr);
		void (*set_irqmask)(struct gk20a *g);
		void (*update_lspmu_cmdline_args)(struct gk20a *g);
		void (*setup_apertures)(struct gk20a *g);
		u32 (*get_irqdest)(struct gk20a *g);
		int (*alloc_super_surface)(struct gk20a *g,
			struct nvgpu_mem *super_surface, u32 size);
		bool (*is_debug_mode_enabled)(struct gk20a *g);
		void (*secured_pmu_start)(struct gk20a *g);
		struct {
			int (*report_ecc_parity_err)(struct gk20a *g,
					u32 hw_id, u32 inst,
					u32 err_id, u64 err_addr,
					u64 err_cnt);
			int (*report_pmu_err)(struct gk20a *g,
				u32 hw_id, u32 err_id, u32 status,
				u32 pmu_err_type);
		} err_ops;
		void (*create_ssmd_lookup_table)(struct nvgpu_pmu *pmu);
		void (*save_zbc)(struct gk20a *g, u32 entries);
		void (*pmu_clear_bar0_host_err_status)(struct gk20a *g);
	} pmu;
	struct {
		int (*init_debugfs)(struct gk20a *g);
		void (*disable_slowboot)(struct gk20a *g);
		int (*init_clk_support)(struct gk20a *g);
		void (*suspend_clk_support)(struct gk20a *g);
		u32 (*get_crystal_clk_hz)(struct gk20a *g);
		int (*clk_domain_get_f_points)(struct gk20a *g,
			u32 clkapidomain, u32 *pfpointscount,
			u16 *pfreqpointsinmhz);
		int (*clk_get_round_rate)(struct gk20a *g, u32 api_domain,
			unsigned long rate_target, unsigned long *rounded_rate);
		int (*get_clk_range)(struct gk20a *g, u32 api_domain,
			u16 *min_mhz, u16 *max_mhz);
		unsigned long (*measure_freq)(struct gk20a *g, u32 api_domain);
		u32 (*get_rate_cntr)(struct gk20a *g, struct namemap_cfg *c);
		unsigned long (*get_rate)(struct gk20a *g, u32 api_domain);
		int (*set_rate)(struct gk20a *g, u32 api_domain, unsigned long rate);
		unsigned long (*get_fmax_at_vmin_safe)(struct gk20a *g);
		u32 (*get_ref_clock_rate)(struct gk20a *g);
		int (*predict_mv_at_hz_cur_tfloor)(struct clk_gk20a *clk,
			unsigned long rate);
		unsigned long (*get_maxrate)(struct gk20a *g, u32 api_domain);
		int (*prepare_enable)(struct clk_gk20a *clk);
		void (*disable_unprepare)(struct clk_gk20a *clk);
		int (*get_voltage)(struct clk_gk20a *clk, u64 *val);
		int (*get_gpcclk_clock_counter)(struct clk_gk20a *clk, u64 *val);
		int (*pll_reg_write)(struct gk20a *g, u32 reg, u32 val);
		int (*get_pll_debug_data)(struct gk20a *g,
				struct nvgpu_clk_pll_debug_data *d);
		int (*mclk_init)(struct gk20a *g);
		void (*mclk_deinit)(struct gk20a *g);
		int (*mclk_change)(struct gk20a *g, u16 val);
		bool split_rail_support;
		bool support_clk_freq_controller;
		bool support_pmgr_domain;
		bool support_lpwr_pg;
		int (*perf_pmu_vfe_load)(struct gk20a *g);
		bool support_clk_freq_domain;
		bool support_vf_point;
		u8 lut_num_entries;
	} clk;
	struct {
		int (*arbiter_clk_init)(struct gk20a *g);
		bool (*check_clk_arb_support)(struct gk20a *g);
		u32 (*get_arbiter_clk_domains)(struct gk20a *g);
		int (*get_arbiter_f_points)(struct gk20a *g,u32 api_domain,
				u32 *num_points, u16 *freqs_in_mhz);
		int (*get_arbiter_clk_range)(struct gk20a *g, u32 api_domain,
				u16 *min_mhz, u16 *max_mhz);
		int (*get_arbiter_clk_default)(struct gk20a *g, u32 api_domain,
				u16 *default_mhz);
		void (*clk_arb_run_arbiter_cb)(struct nvgpu_clk_arb *arb);
		/* This function is inherently unsafe to call while
		 *  arbiter is running arbiter must be blocked
		 *  before calling this function */
		u32 (*get_current_pstate)(struct gk20a *g);
		void (*clk_arb_cleanup)(struct nvgpu_clk_arb *arb);
	} clk_arb;
	struct {
		int (*handle_pmu_perf_event)(struct gk20a *g, void *pmu_msg);
		bool support_changeseq;
		bool support_vfe;
	} pmu_perf;
	struct {
		int (*exec_regops)(struct gk20a *g,
			    struct channel_gk20a *ch,
			    struct nvgpu_dbg_reg_op *ops,
			    u64 num_ops,
			    bool is_profiler,
			    bool *is_current_ctx);
		const struct regop_offset_range* (
				*get_global_whitelist_ranges)(void);
		u64 (*get_global_whitelist_ranges_count)(void);
		const struct regop_offset_range* (
				*get_context_whitelist_ranges)(void);
		u64 (*get_context_whitelist_ranges_count)(void);
		const u32* (*get_runcontrol_whitelist)(void);
		u64 (*get_runcontrol_whitelist_count)(void);
		const u32* (*get_qctl_whitelist)(void);
		u64 (*get_qctl_whitelist_count)(void);
	} regops;
	struct {
		void (*intr_mask)(struct gk20a *g);
		void (*intr_enable)(struct gk20a *g);
		void (*intr_unit_config)(struct gk20a *g,
				bool enable, bool is_stalling, u32 mask);
		void (*isr_stall)(struct gk20a *g);
		bool (*is_intr_hub_pending)(struct gk20a *g, u32 mc_intr);
		bool (*is_intr_nvlink_pending)(struct gk20a *g, u32 mc_intr);
		bool (*is_stall_and_eng_intr_pending)(struct gk20a *g,
					u32 act_eng_id, u32 *eng_intr_pending);
		u32 (*intr_stall)(struct gk20a *g);
		void (*intr_stall_pause)(struct gk20a *g);
		void (*intr_stall_resume)(struct gk20a *g);
		u32 (*intr_nonstall)(struct gk20a *g);
		void (*intr_nonstall_pause)(struct gk20a *g);
		void (*intr_nonstall_resume)(struct gk20a *g);
		u32 (*isr_nonstall)(struct gk20a *g);
		void (*enable)(struct gk20a *g, u32 units);
		void (*disable)(struct gk20a *g, u32 units);
		void (*reset)(struct gk20a *g, u32 units);
		bool (*is_enabled)(struct gk20a *g, enum nvgpu_unit unit);
		bool (*is_intr1_pending)(struct gk20a *g, enum nvgpu_unit unit, u32 mc_intr_1);
		void (*log_pending_intrs)(struct gk20a *g);
		void (*fbpa_isr)(struct gk20a *g);
		u32 (*reset_mask)(struct gk20a *g, enum nvgpu_unit unit);
		void (*fb_reset)(struct gk20a *g);
		void (*ltc_isr)(struct gk20a *g);
	} mc;
	struct {
		void (*show_dump)(struct gk20a *g,
				struct gk20a_debug_output *o);
	} debug;
#ifdef NVGPU_DEBUGGER
	struct {
		void (*post_events)(struct channel_gk20a *ch);
		int (*dbg_set_powergate)(struct dbg_session_gk20a *dbg_s,
					bool disable_powergate);
		bool (*check_and_set_global_reservation)(
				struct dbg_session_gk20a *dbg_s,
				struct dbg_profiler_object_data *prof_obj);
		bool (*check_and_set_context_reservation)(
				struct dbg_session_gk20a *dbg_s,
				struct dbg_profiler_object_data *prof_obj);
		void (*release_profiler_reservation)(
				struct dbg_session_gk20a *dbg_s,
				struct dbg_profiler_object_data *prof_obj);
	} debugger;
#endif
	struct {
		void (*enable_membuf)(struct gk20a *g, u32 size,
			u64 buf_addr, struct nvgpu_mem *inst_block);
		void (*disable_membuf)(struct gk20a *g);
		void (*membuf_reset_streaming)(struct gk20a *g);
		u32 (*get_membuf_pending_bytes)(struct gk20a *g);
		void (*set_membuf_handled_bytes)(struct gk20a *g,
			u32 entries, u32 entry_size);
		bool (*get_membuf_overflow_status)(struct gk20a *g);
		u32 (*get_pmm_per_chiplet_offset)(void);
	} perf;
	struct {
		int (*perfbuf_enable)(struct gk20a *g, u64 offset, u32 size);
		int (*perfbuf_disable)(struct gk20a *g);
	} perfbuf;

	u32 (*get_litter_value)(struct gk20a *g, int value);
	void (*chip_init_gpu_characteristics)(struct gk20a *g);

	struct {
		void (*init_hw)(struct gk20a *g);
		void (*isr)(struct gk20a *g);
		int (*bar1_bind)(struct gk20a *g, struct nvgpu_mem *bar1_inst);
		int (*bar2_bind)(struct gk20a *g, struct nvgpu_mem *bar1_inst);
		u32 (*set_bar0_window)(struct gk20a *g, struct nvgpu_mem *mem,
			struct nvgpu_sgt *sgt, struct nvgpu_sgl *sgl,
			u32 w);
		u32 (*read_sw_scratch)(struct gk20a *g, u32 index);
		void (*write_sw_scratch)(struct gk20a *g, u32 index, u32 val);
	} bus;

	struct {
		void (*isr)(struct gk20a *g);
		int (*read_ptimer)(struct gk20a *g, u64 *value);
		int (*get_timestamps_zipper)(struct gk20a *g,
			u32 source_id, u32 count,
			struct nvgpu_cpu_time_correlation_sample *samples);
		struct {
			int (*report_timeout_err)(struct gk20a *g,
					u32 hw_id, u32 inst, u32 err_id,
					u32 err_addr, u32 error_code);
		} err_ops;
	} ptimer;

	struct {
		int (*init)(struct gk20a *g);
		int (*preos_wait_for_halt)(struct gk20a *g);
		void (*preos_reload_check)(struct gk20a *g);
		int (*devinit)(struct gk20a *g);
		int (*preos)(struct gk20a *g);
		int (*verify_devinit)(struct gk20a *g);
	} bios;

#if defined(CONFIG_GK20A_CYCLE_STATS)
	struct {
		int (*enable_snapshot)(struct channel_gk20a *ch,
				struct gk20a_cs_snapshot_client *client);
		void (*disable_snapshot)(struct gr_gk20a *gr);
		int (*check_data_available)(struct channel_gk20a *ch,
						u32 *pending,
						bool *hw_overflow);
		void (*set_handled_snapshots)(struct gk20a *g, u32 num);
		u32 (*allocate_perfmon_ids)(struct gk20a_cs_snapshot *data,
				       u32 count);
		u32 (*release_perfmon_ids)(struct gk20a_cs_snapshot *data,
				      u32 start,
				      u32 count);
		int (*detach_snapshot)(struct channel_gk20a *ch,
				struct gk20a_cs_snapshot_client *client);
		bool (*get_overflow_status)(struct gk20a *g);
		u32 (*get_pending_snapshots)(struct gk20a *g);
	} css;
#endif
	struct {
		int (*get_speed)(struct gk20a *g, u32 *xve_link_speed);
		int (*set_speed)(struct gk20a *g, u32 xve_link_speed);
		void (*available_speeds)(struct gk20a *g, u32 *speed_mask);
		u32 (*xve_readl)(struct gk20a *g, u32 reg);
		void (*xve_writel)(struct gk20a *g, u32 reg, u32 val);
		void (*disable_aspm)(struct gk20a *g);
		void (*reset_gpu)(struct gk20a *g);
#if defined(CONFIG_PCI_MSI)
		void (*rearm_msi)(struct gk20a *g);
#endif
		void (*enable_shadow_rom)(struct gk20a *g);
		void (*disable_shadow_rom)(struct gk20a *g);
		u32 (*get_link_control_status)(struct gk20a *g);
	} xve;
	struct {
		void (*reset)(struct nvgpu_falcon *flcn);
		void (*set_irq)(struct nvgpu_falcon *flcn, bool enable,
				u32 intr_mask, u32 intr_dest);
		bool (*clear_halt_interrupt_status)(struct nvgpu_falcon *flcn);
		bool (*is_falcon_cpu_halted)(struct nvgpu_falcon *flcn);
		bool (*is_falcon_idle)(struct nvgpu_falcon *flcn);
		bool (*is_falcon_scrubbing_done)(struct nvgpu_falcon *flcn);
		int (*copy_from_dmem)(struct nvgpu_falcon *flcn,
				      u32 src, u8 *dst, u32 size, u8 port);
		int (*copy_to_dmem)(struct nvgpu_falcon *flcn,
				    u32 dst, u8 *src, u32 size, u8 port);
		int (*copy_from_imem)(struct nvgpu_falcon *flcn,
				      u32 src, u8 *dst, u32 size, u8 port);
		int (*copy_to_imem)(struct nvgpu_falcon *flcn,
				    u32 dst, u8 *src, u32 size, u8 port,
				    bool sec, u32 tag);
		u32 (*mailbox_read)(struct nvgpu_falcon *flcn,
				    u32 mailbox_index);
		void (*mailbox_write)(struct nvgpu_falcon *flcn,
				      u32 mailbox_index, u32 data);
		int (*bootstrap)(struct nvgpu_falcon *flcn,
				 u32 boot_vector);
		void (*dump_falcon_stats)(struct nvgpu_falcon *flcn);
		void (*get_falcon_ctls)(struct nvgpu_falcon *flcn,
					u32 *sctl, u32 *cpuctl);
		u32 (*get_mem_size)(struct nvgpu_falcon *flcn,
			enum falcon_mem_type mem_type);
		u8 (*get_ports_count)(struct nvgpu_falcon *flcn,
			enum falcon_mem_type mem_type);
	} falcon;
	struct {
		void (*enable_priv_ring)(struct gk20a *g);
		void (*isr)(struct gk20a *g);
		void (*decode_error_code)(struct gk20a *g, u32 error_code);
		void (*set_ppriv_timeout_settings)(struct gk20a *g);
		u32 (*enum_ltc)(struct gk20a *g);
		u32 (*get_gpc_count)(struct gk20a *g);
		u32 (*get_fbp_count)(struct gk20a *g);
	} priv_ring;
	struct {
		int (*check_priv_security)(struct gk20a *g);
		bool (*is_opt_ecc_enable)(struct gk20a *g);
		bool (*is_opt_feature_override_disable)(struct gk20a *g);
		u32 (*fuse_status_opt_fbio)(struct gk20a *g);
		u32 (*fuse_status_opt_fbp)(struct gk20a *g);
		u32 (*fuse_status_opt_rop_l2_fbp)(struct gk20a *g, u32 fbp);
		u32 (*fuse_status_opt_gpc)(struct gk20a *g);
		u32 (*fuse_status_opt_tpc_gpc)(struct gk20a *g, u32 gpc);
		void (*fuse_ctrl_opt_tpc_gpc)(struct gk20a *g, u32 gpc, u32 val);
		u32 (*fuse_opt_sec_debug_en)(struct gk20a *g);
		u32 (*fuse_opt_priv_sec_en)(struct gk20a *g);
		u32 (*read_vin_cal_fuse_rev)(struct gk20a *g);
		int (*read_vin_cal_slope_intercept_fuse)(struct gk20a *g,
							     u32 vin_id, u32 *slope,
							     u32 *intercept);
		int (*read_vin_cal_gain_offset_fuse)(struct gk20a *g,
							     u32 vin_id, s8 *gain,
							     s8 *offset);
		int (*read_gcplex_config_fuse)(struct gk20a *g, u32 *val);
	} fuse;
	struct {
		u32 (*get_link_reset_mask)(struct gk20a *g);
		int (*init)(struct gk20a *g);
		int (*discover_ioctrl)(struct gk20a *g);
		int (*discover_link)(struct gk20a *g);
		int (*rxdet)(struct gk20a *g, u32 link_id);
		void (*get_connected_link_mask)(u32 *link_mask);
		void (*set_sw_war)(struct gk20a *g, u32 link_id);
		/* API */
		int (*link_early_init)(struct gk20a *g, unsigned long mask);
		struct {
			int (*setup_pll)(struct gk20a *g,
					unsigned long link_mask);
			int (*data_ready_en)(struct gk20a *g,
					unsigned long link_mask, bool sync);
			u32 (*get_link_state)(struct gk20a *g, u32 link_id);
			enum nvgpu_nvlink_link_mode (*get_link_mode)(
					struct gk20a *g,
					u32 link_id);
			int (*set_link_mode)(struct gk20a *g, u32 link_id,
					enum nvgpu_nvlink_link_mode mode);
			u32 (*get_rx_sublink_state)(struct gk20a *g,
					u32 link_id);
			u32 (*get_tx_sublink_state)(struct gk20a *g,
					u32 link_id);
			enum nvgpu_nvlink_sublink_mode (*get_sublink_mode)(
					struct gk20a *g, u32 link_id,
					bool is_rx_sublink);
			int (*set_sublink_mode)(struct gk20a *g, u32 link_id,
					bool is_rx_sublink,
					enum nvgpu_nvlink_sublink_mode mode);
		} link_mode_transitions;
		int (*interface_init)(struct gk20a *g);
		int (*interface_disable)(struct gk20a *g);
		int (*reg_init)(struct gk20a *g);
		int (*shutdown)(struct gk20a *g);
		int (*early_init)(struct gk20a *g);
		int (*speed_config)(struct gk20a *g);
		struct {
			u32 (*base_addr)(struct gk20a *g);
			bool (*is_running)(struct gk20a *g);
			int (*is_boot_complete)(struct gk20a *g,
						bool *boot_cmplte);
			u32 (*get_dlcmd_ordinal)(struct gk20a *g,
					enum nvgpu_nvlink_minion_dlcmd dlcmd);
			int (*send_dlcmd)(struct gk20a *g, u32 link_id,
					enum nvgpu_nvlink_minion_dlcmd dlcmd,
					bool sync);
			void (*clear_intr)(struct gk20a *g);
			void (*init_intr)(struct gk20a *g);
			void (*enable_link_intr)(struct gk20a *g, u32 link_id,
								bool enable);
			void (*falcon_isr)(struct gk20a *g);
			void (*isr)(struct gk20a *g);
		} minion;
		struct {
			void (*common_intr_enable)(struct gk20a *g,
							unsigned long mask);
			void (*init_nvlipt_intr)(struct gk20a *g, u32 link_id);
			void (*enable_link_intr)(struct gk20a *g, u32 link_id,
							bool enable);
			void (*init_mif_intr)(struct gk20a *g, u32 link_id);
			void (*mif_intr_enable)(struct gk20a *g, u32 link_id,
							bool enable);
			void (*dlpl_intr_enable)(struct gk20a *g, u32 link_id,
							bool enable);
			void (*isr)(struct gk20a *g);
		} intr;
	} nvlink;
	struct {
		u32 (*get_nvhsclk_ctrl_e_clk_nvl)(struct gk20a *g);
		void (*set_nvhsclk_ctrl_e_clk_nvl)(struct gk20a *g, u32 val);
		u32 (*get_nvhsclk_ctrl_swap_clk_nvl)(struct gk20a *g);
		void (*set_nvhsclk_ctrl_swap_clk_nvl)(struct gk20a *g, u32 val);
		int (*device_info_parse_enum)(struct gk20a *g,
						u32 table_entry,
						u32 *engine_id, u32 *runlist_id,
						u32 *intr_id, u32 *reset_id);
		int (*device_info_parse_data)(struct gk20a *g,
						u32 table_entry, u32 *inst_id,
						u32 *pri_base, u32 *fault_id);
		u32 (*get_num_engine_type_entries)(struct gk20a *g,
						u32 engine_type);
		int (*get_device_info)(struct gk20a *g,
					struct nvgpu_device_info *dev_info,
					u32 engine_type, u32 inst_id);
		bool (*is_engine_gr)(struct gk20a *g, u32 engine_type);
		bool (*is_engine_ce)(struct gk20a *g, u32 engine_type);
		u32 (*get_ce_inst_id)(struct gk20a *g, u32 engine_type);
		u32 (*get_max_gpc_count)(struct gk20a *g);
		u32 (*get_max_tpc_per_gpc_count)(struct gk20a *g);
		u32 (*get_max_fbps_count)(struct gk20a *g);
		u32 (*get_max_fbpas_count)(struct gk20a *g);
		u32 (*get_max_ltc_per_fbp)(struct gk20a *g);
		u32 (*get_max_lts_per_ltc)(struct gk20a *g);
	} top;
	struct {
		void (*secured_sec2_start)(struct gk20a *g);
		void (*enable_irq)(struct nvgpu_sec2 *sec2, bool enable);
		bool (*is_interrupted)(struct nvgpu_sec2 *sec2);
		void (*isr)(struct gk20a *g);
		void (*msgq_tail)(struct gk20a *g, struct nvgpu_sec2 *sec2,
			u32 *tail, bool set);
		u32 (*falcon_base_addr)(void);
		int (*sec2_reset)(struct gk20a *g);
		int (*sec2_copy_to_emem)(struct gk20a *g, u32 dst,
					 u8 *src, u32 size, u8 port);
		int (*sec2_copy_from_emem)(struct gk20a *g,
					   u32 src, u8 *dst, u32 size, u8 port);
		int (*sec2_queue_head)(struct gk20a *g,
				       u32 queue_id, u32 queue_index,
				       u32 *head, bool set);
		int (*sec2_queue_tail)(struct gk20a *g,
				       u32 queue_id, u32 queue_index,
				       u32 *tail, bool set);
	} sec2;
	struct {
		u32 (*falcon_base_addr)(void);
		void (*falcon_setup_boot_config)(struct gk20a *g);
		int (*gsp_reset)(struct gk20a *g);
	} gsp;
	void (*semaphore_wakeup)(struct gk20a *g, bool post_events);
};

struct nvgpu_bios_ucode {
	u8 *bootloader;
	u32 bootloader_phys_base;
	u32 bootloader_size;
	u8 *ucode;
	u32 phys_base;
	u32 size;
	u8 *dmem;
	u32 dmem_phys_base;
	u32 dmem_size;
	u32 code_entry_point;
};

struct nvgpu_bios {
	u32 vbios_version;
	u8 vbios_oem_version;

	u8 *data;
	size_t size;

	struct nvgpu_bios_ucode devinit;
	struct nvgpu_bios_ucode preos;

	u8 *devinit_tables;
	u32 devinit_tables_size;
	u8 *bootscripts;
	u32 bootscripts_size;

	u8 mem_strap_data_count;
	u16 mem_strap_xlat_tbl_ptr;

	u32 condition_table_ptr;

	u32 devinit_tables_phys_base;
	u32 devinit_script_phys_base;

	struct bit_token *perf_token;
	struct bit_token *clock_token;
	struct bit_token *virt_token;
	u32 expansion_rom_offset;
	u32 base_rom_size;

	u32 nvlink_config_data_offset;
};

struct nvgpu_gpu_params {
	/* GPU architecture ID */
	u32 gpu_arch;
	/* GPU implementation ID */
	u32 gpu_impl;
	/* GPU revision ID */
	u32 gpu_rev;
	/* sm version */
	u32 sm_arch_sm_version;
	/* sm instruction set */
	u32 sm_arch_spa_version;
	u32 sm_arch_warp_count;
};

struct gk20a {
	void (*free)(struct gk20a *g);
	struct nvgpu_nvhost_dev *nvhost_dev;

	/*
	 * Used by <nvgpu/enabled.h>. Do not access directly!
	 */
	unsigned long *enabled_flags;

	nvgpu_atomic_t usage_count;

	struct nvgpu_mutex ctxsw_disable_lock;
	int ctxsw_disable_count;

	struct nvgpu_ref refcount;

	const char *name;

	bool gpu_reset_done;
	bool power_on;
	bool suspended;
	bool sw_ready;

	u64 log_mask;
	u32 log_trace;

	struct nvgpu_mutex tpc_pg_lock;

	struct nvgpu_gpu_params params;

	/*
	 * Guards access to hardware when usual gk20a_{busy,idle} are skipped
	 * for submits and held for channel lifetime but dropped for an ongoing
	 * gk20a_do_idle().
	 */
	struct nvgpu_rwsem deterministic_busy;

	struct nvgpu_netlist_vars *netlist_vars;
	bool netlist_valid;

	struct nvgpu_falcon fecs_flcn;
	struct nvgpu_falcon gpccs_flcn;
	struct nvgpu_falcon nvdec_flcn;
	struct nvgpu_falcon minion_flcn;
	struct nvgpu_falcon gsp_flcn;
	struct clk_gk20a clk;
	struct fifo_gk20a fifo;
	struct nvgpu_nvlink_dev nvlink;
	struct gr_gk20a gr;
	struct sim_nvgpu *sim;
	struct mm_gk20a mm;
	struct nvgpu_pmu pmu;
	struct nvgpu_acr *acr;
	struct nvgpu_ecc ecc;
	struct nvgpu_clk_pmupstate *clk_pmu;
	struct perf_pmupstate *perf_pmu;
	struct pmgr_pmupstate *pmgr_pmu;
	struct therm_pmupstate *therm_pmu;
	struct nvgpu_sec2 sec2;

#ifdef CONFIG_DEBUG_FS
	struct railgate_stats pstats;
#endif
	u32 gr_idle_timeout_default;
	bool timeouts_disabled_by_user;

	unsigned int ch_wdt_init_limit_ms;
	u32 ctxsw_timeout_period_ms;

	struct nvgpu_mutex power_lock;

	/* Channel priorities */
	u32 timeslice_low_priority_us;
	u32 timeslice_medium_priority_us;
	u32 timeslice_high_priority_us;
	u32 min_timeslice_us;
	u32 max_timeslice_us;
	bool runlist_interleave;

	struct nvgpu_mutex cg_pg_lock;
	bool slcg_enabled;
	bool blcg_enabled;
	bool elcg_enabled;
	bool elpg_enabled;
	bool aelpg_enabled;
	bool can_elpg;
	bool mscg_enabled;
	bool forced_idle;
	bool forced_reset;
	bool allow_all;

	u32 ptimer_src_freq;

	int railgate_delay;
	u8 ldiv_slowdown_factor;
	unsigned int aggressive_sync_destroy_thresh;
	bool aggressive_sync_destroy;

	/* Debugfs knob for forcing syncpt support off in runtime. */
	u32 disable_syncpoints;

	bool support_ls_pmu;

	bool is_virtual;

	bool has_cde;

	u32 emc3d_ratio;

	struct gk20a_ctxsw_ucode_info ctxsw_ucode_info;

	/*
	 * A group of semaphore pools. One for each channel.
	 */
	struct nvgpu_semaphore_sea *sema_sea;

	/* held while manipulating # of debug/profiler sessions present */
	/* also prevents debug sessions from attaching until released */
	struct nvgpu_mutex dbg_sessions_lock;
	int dbg_powergating_disabled_refcount; /*refcount for pg disable */
	/*refcount for timeout disable */
	nvgpu_atomic_t timeouts_disabled_refcount;

	/* must have dbg_sessions_lock before use */
	struct nvgpu_dbg_reg_op *dbg_regops_tmp_buf;
	u32 dbg_regops_tmp_buf_ops;

	/* For perfbuf mapping */
	struct {
		struct dbg_session_gk20a *owner;
		u64 offset;
	} perfbuf;

	/* For profiler reservations */
	struct nvgpu_list_node profiler_objects;
	bool global_profiler_reservation_held;
	int profiler_reservation_count;

	void (*remove_support)(struct gk20a *g);

	u64 pg_ingating_time_us;
	u64 pg_ungating_time_us;
	u32 pg_gating_cnt;

	struct nvgpu_spinlock mc_enable_lock;

	struct gk20a_as as;

	struct nvgpu_mutex client_lock;
	int client_refcount; /* open channels and ctrl nodes */

	struct gpu_ops ops;
	u32 mc_intr_mask_restore[4];
	/*used for change of enum zbc update cmd id from ver 0 to ver1*/
	u8 pmu_ver_cmd_id_zbc_table_update;
	bool pmu_lsf_pmu_wpr_init_done;
	u32 pmu_lsf_loaded_falcon_id;

	/* Needed to keep track of deferred interrupts */
	nvgpu_atomic_t hw_irq_stall_count;
	nvgpu_atomic_t hw_irq_nonstall_count;

	struct nvgpu_cond sw_irq_stall_last_handled_cond;
	nvgpu_atomic_t sw_irq_stall_last_handled;

	struct nvgpu_cond sw_irq_nonstall_last_handled_cond;
	nvgpu_atomic_t sw_irq_nonstall_last_handled;

	int irqs_enabled;
	int irq_stall; /* can be same as irq_nonstall in case of PCI */
	int irq_nonstall;

	/*
	 * The deductible memory size for max_comptag_mem (in MBytes)
	 * Usually close to memory size that running system is taking
	*/
	u32 comptag_mem_deduct;

	u32 max_comptag_mem; /* max memory size (MB) for comptag */

	u32 ltc_streamid;

	struct nvgpu_cbc *cbc;
	struct nvgpu_ltc *ltc;

	struct gk20a_worker {
		struct nvgpu_thread poll_task;
		nvgpu_atomic_t put;
		struct nvgpu_cond wq;
		struct nvgpu_list_node items;
		struct nvgpu_spinlock items_lock;
		struct nvgpu_mutex start_lock;
	} channel_worker, clk_arb_worker;

	struct {
		void (*open)(struct channel_gk20a *ch);
		void (*close)(struct channel_gk20a *ch);
		void (*work_completion_signal)(struct channel_gk20a *ch);
		void (*work_completion_cancel_sync)(struct channel_gk20a *ch);
		bool (*os_fence_framework_inst_exists)(struct channel_gk20a *ch);
		int (*init_os_fence_framework)(
			struct channel_gk20a *ch, const char *fmt, ...);
		void (*signal_os_fence_framework)(struct channel_gk20a *ch);
		void (*destroy_os_fence_framework)(struct channel_gk20a *ch);
		int (*copy_user_gpfifo)(struct nvgpu_gpfifo_entry *dest,
				struct nvgpu_gpfifo_userdata userdata,
				u32 start, u32 length);
		int (*alloc_usermode_buffers)(struct channel_gk20a *c,
			struct nvgpu_setup_bind_args *args);
		void (*free_usermode_buffers)(struct channel_gk20a *c);
	} os_channel;

	struct gk20a_scale_profile *scale_profile;
	unsigned long last_freq;

	struct gk20a_ctxsw_trace *ctxsw_trace;
	struct nvgpu_gr_fecs_trace *fecs_trace;

	bool mmu_debug_ctrl;

	u32 tpc_fs_mask_user;

	u32 tpc_pg_mask;
	bool can_tpc_powergate;

	u32 valid_tpc_mask[MAX_TPC_PG_CONFIGS];

	struct nvgpu_bios bios;
	bool bios_is_init;

	struct nvgpu_clk_arb *clk_arb;

	struct nvgpu_mutex clk_arb_enable_lock;

	nvgpu_atomic_t clk_arb_global_nr;

	struct gk20a_ce_app *ce_app;

	bool ltc_intr_en_illegal_compstat;

	/* PCI device identifier */
	u16 pci_vendor_id, pci_device_id;
	u16 pci_subsystem_vendor_id, pci_subsystem_device_id;
	u16 pci_class;
	u8 pci_revision;

	/*
	 * PCI power management: i2c device index, port and address for
	 * INA3221.
	 */
	u32 ina3221_dcb_index;
	u32 ina3221_i2c_address;
	u32 ina3221_i2c_port;
	bool hardcode_sw_threshold;

	/* PCIe power states. */
	bool xve_l0s;
	bool xve_l1;

	/* Current warning temp in sfxp24.8 */
	s32 curr_warn_temp;

#if defined(CONFIG_PCI_MSI)
	/* Check if msi is enabled */
	bool msi_enabled;
#endif
#ifdef CONFIG_NVGPU_TRACK_MEM_USAGE
	struct nvgpu_mem_alloc_tracker *vmallocs;
	struct nvgpu_mem_alloc_tracker *kmallocs;
#endif

	/* The minimum VBIOS version supported */
	u32 vbios_min_version;
	u32 vbios_compatible_version;

	/* memory training sequence and mclk switch scripts */
	u32 mem_config_idx;

	u64 dma_memory_used;

#if defined(CONFIG_TEGRA_GK20A_NVHOST)
	u64		syncpt_unit_base;
	size_t		syncpt_unit_size;
	u32		syncpt_size;
#endif
	struct nvgpu_mem syncpt_mem;

	struct nvgpu_list_node boardobj_head;
	struct nvgpu_list_node boardobjgrp_head;

	struct nvgpu_mem pdb_cache_war_mem;
};

static inline bool nvgpu_is_timeouts_enabled(struct gk20a *g)
{
	return nvgpu_atomic_read(&g->timeouts_disabled_refcount) == 0;
}

static inline u32 gk20a_get_gr_idle_timeout(struct gk20a *g)
{
	return nvgpu_is_timeouts_enabled(g) ?
		g->gr_idle_timeout_default : UINT_MAX;
}

#define MULTICHAR_TAG(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
enum BAR0_DEBUG_OPERATION {
	BARO_ZERO_NOP = 0,
	OP_END = MULTICHAR_TAG('D', 'O', 'N', 'E'),
	BAR0_READ32 = MULTICHAR_TAG('0', 'R', '3', '2'),
	BAR0_WRITE32 = MULTICHAR_TAG('0', 'W', '3', '2'),
};

struct share_buffer_head {
	enum BAR0_DEBUG_OPERATION operation;
/* size of the operation item */
	u32 size;
	u32 completed;
	u32 failed;
	u64 context;
	u64 completion_callback;
};

struct gk20a_cyclestate_buffer_elem {
	struct share_buffer_head	head;
/* in */
	u64 p_data;
	u64 p_done;
	u32 offset_bar0;
	u16 first_bit;
	u16 last_bit;
/* out */
/* keep 64 bits to be consistent */
	u64 data;
};

/* operations that will need to be executed on non stall workqueue */
#define GK20A_NONSTALL_OPS_WAKEUP_SEMAPHORE	BIT32(0)
#define GK20A_NONSTALL_OPS_POST_EVENTS		BIT32(1)

/* register accessors */
void __nvgpu_check_gpu_state(struct gk20a *g);
void __gk20a_warn_on_no_regs(void);

/* classes that the device supports */
/* TBD: get these from an open-sourced SDK? */
enum {
	FERMI_TWOD_A              = 0x902D,
	KEPLER_INLINE_TO_MEMORY_A = 0xA040,
	KEPLER_DMA_COPY_A         = 0xA0B5,
};

#define GK20A_BAR0_IORESOURCE_MEM	0U
#define GK20A_BAR1_IORESOURCE_MEM	1U
#define GK20A_SIM_IORESOURCE_MEM	2U

void gk20a_busy_noresume(struct gk20a *g);
void gk20a_idle_nosuspend(struct gk20a *g);
int __must_check gk20a_busy(struct gk20a *g);
void gk20a_idle(struct gk20a *g);
int __gk20a_do_idle(struct gk20a *g, bool force_reset);
int __gk20a_do_unidle(struct gk20a *g);

int nvgpu_can_busy(struct gk20a *g);
int gk20a_wait_for_idle(struct gk20a *g);

#define NVGPU_GPU_ARCHITECTURE_SHIFT 4U

/* constructs unique and compact GPUID from nvgpu_gpu_characteristics
 * arch/impl fields */
#define GK20A_GPUID(arch, impl) ((u32) ((arch) | (impl)))

#define GK20A_GPUID_GK20A   0x000000EAU
#define GK20A_GPUID_GM20B   0x0000012BU
#define GK20A_GPUID_GM20B_B 0x0000012EU
#define NVGPU_GPUID_GP10B   0x0000013BU
#define NVGPU_GPUID_GV11B   0x0000015BU
#define NVGPU_GPUID_GV100   0x00000140U
#define NVGPU_GPUID_TU104   0x00000164U

void gk20a_init_gpu_characteristics(struct gk20a *g);

int gk20a_prepare_poweroff(struct gk20a *g);
int gk20a_finalize_poweron(struct gk20a *g);

void nvgpu_wait_for_deferred_interrupts(struct gk20a *g);

struct gk20a * __must_check gk20a_get(struct gk20a *g);
void gk20a_put(struct gk20a *g);

bool nvgpu_has_syncpoints(struct gk20a *g);

#endif /* GK20A_H */
