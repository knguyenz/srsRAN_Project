#pragma once

#include "scheduler_policy.h"
#include "srsran/scheduler/config/scheduler_expert_config.h"

namespace srsran {

/// Preemptive Fixed Priority: UE/LC có fixed priority. Số nhỏ = ưu tiên cao.
class scheduler_time_pfp final : public scheduler_policy
{
public:
  scheduler_time_pfp(const scheduler_ue_expert_config& expert_cfg_, du_cell_index_t cell_index_);

  void add_ue(du_ue_index_t) override {}
  void rem_ue(du_ue_index_t) override {}

  void compute_ue_dl_priorities(slot_point               pdcch_slot,
                                slot_point               pdsch_slot,
                                span<ue_newtx_candidate> ue_candidates) override;

  void compute_ue_ul_priorities(slot_point               pdcch_slot,
                                slot_point               pusch_slot,
                                span<ue_newtx_candidate> ue_candidates) override;

  void save_dl_newtx_grants(span<const dl_msg_alloc>) override {}
  void save_ul_newtx_grants(span<const ul_sched_info>) override {}

private:
  const du_cell_index_t            cell_index;
  const scheduler_ue_expert_config expert_cfg;
  // Bạn có thể để tham chiếu hoặc by-value. Ở đây chọn by-value cho đơn giản & an toàn.
  time_pfp_scheduler_expert_config params;

  static constexpr double prio_base = 1000.0;

  // Tính fixed-priority cho UE ở DL/UL dựa trên LC đang có dữ liệu.
  double compute_fixed_prio_dl(const slice_ue& u) const;
  double compute_fixed_prio_ul(const slice_ue& u) const;
};

} // namespace srsran
