#include "scheduler_time_pfp.h"
#include "../slicing/slice_ue_repository.h"
#include "srsran/support/srsran_assert.h"

using namespace srsran;

static inline double invert_fixed(uint8_t fixed_prio)
{
  // LC prio nhỏ → đảo thành số lớn để pipeline ưu tiên trước.
  return scheduler_time_pfp::prio_base - static_cast<double>(fixed_prio);
}

scheduler_time_pfp::scheduler_time_pfp(const scheduler_ue_expert_config& expert_cfg_,
                                       du_cell_index_t                   cell_index_)
: cell_index(cell_index_),
  expert_cfg(expert_cfg_),
  params(std::get<time_pfp_scheduler_expert_config>(expert_cfg.strategy_cfg)) // Lấy từ variant
{}

double scheduler_time_pfp::compute_fixed_prio_dl(const slice_ue& u) const
{
  uint8_t best = params.default_prio;
  // Duyệt các LC của UE; chọn LC đang có pending DL newTx bytes với fixed_prio nhỏ nhất.
  for (auto lc : *u.logical_channels()) {
    if (not u.contains(lc->lcid)) continue;
    if (u.pending_dl_newtx_bytes(lc->lcid) == 0) continue;
    uint8_t p = params.lcid_prio[lc->lcid];
    if (p < best) best = p;
  }
  return invert_fixed(best);
}

double scheduler_time_pfp::compute_fixed_prio_ul(const slice_ue& u) const
{
  uint8_t best = params.default_prio;
  // Ở UL, pending tính theo LCG. Chọn min fixed_prio trong các LC thuộc LCG có pending.
  for (auto lc : *u.logical_channels()) {
    if (not u.contains(lc->lcid)) continue;
    auto lcg = u.get_lcg_id(lc->lcid);
    if (u.pending_ul_unacked_bytes(lcg) == 0) continue;
    uint8_t p = params.lcid_prio[lc->lcid];
    if (p < best) best = p;
  }
  return invert_fixed(best);
}

void scheduler_time_pfp::compute_ue_dl_priorities(slot_point, slot_point, span<ue_newtx_candidate> ue_candidates)
{
  for (auto& c : ue_candidates) {
    c.priority = compute_fixed_prio_dl(*c.ue);
  }
}

void scheduler_time_pfp::compute_ue_ul_priorities(slot_point, slot_point, span<ue_newtx_candidate> ue_candidates)
{
  for (auto& c : ue_candidates) {
    c.priority = compute_fixed_prio_ul(*c.ue);
  }
}
