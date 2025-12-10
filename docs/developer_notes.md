# Developer Notes – Test Infrastructure (Temporary Limitations)

## Config Singleton State Leakage Between Tests

### Problem Summary
The global `Config` singleton persists internal state across multiple test cases.
When tests are executed in one test binary (as required by CTest), the state
from previous tests affects subsequent ones — resulting in non-deterministic
failures, especially on CI runners.

### CI Symptoms Observed
- `threads()` value not matching expected defaults in some tests.
- Logging subsystem reinitializing with stale configuration.
- HTTP ingest tests failing unpredictably due to shared state or logger teardown.

### Temporary Quick-Fix (v0.7.1)
To stabilize CI/CD pipelines before the major 1.0.0 milestone:
- Each test suite explicitly resets baseline state using `cfg.loadDefaults()`.
- No changes were made to production code (Config remains a strict singleton).
- This solution is non-invasive and sufficient for 0.7.x releases.

### Planned Full Fix (v1.0.0+)
A robust test harness will be introduced, including:
- Singleton isolation or a test-only reset facility.
- DI-based configuration for logger and global services.
- Mocked HTTP ingest layer.
- Proper test fixtures and per-suite sandboxing.
- Separation of unit tests and integration tests.

This work will fully eliminate the need for the temporary workaround.
