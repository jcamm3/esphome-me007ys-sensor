
## CHANGELOG.md
```markdown
# Changelog

## v1.2.0 — 2025-08-11
### Added
- `raw_mm` diagnostic sensor to expose raw millimeter frames after checksum verification.
- `raw_mm_policy` option:
  - `all` — publish every valid-checksum frame (default)
  - `valid_only` — publish only when in-range; NaN on too_close/too_far
- Robustness controls:
  - `guard_cm` — ignore band above `min_valid_cm`
  - `max_step_cm` — drop unrealistic single-frame jumps
  - `require_consecutive` — require N similar frames before publish

### Changed
- Upper bound handling simplified: distance ≥ `max_valid_cm` → publish NaN + `too_far` (no `too_far_behavior`).
- README updated with synced inches example (Option A).
