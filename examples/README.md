# Examples

Every `.yaml` here is a complete device config that CI compiles. All include
`common/base.yaml` (network, API, OTA, recovery, diagnostics) and hold their
own board and pin choices. Wiring is in the comment at the top of each file.

| File | Shows |
|------|-------|
| `esp32.yaml`, `esp32-c3.yaml`, `esp32-s3.yaml` | The package on each board |
| `advanced.yaml` | Every component option and entity, INT pin monitor, occupancy template |
| `home_automation.yaml` | Occupancy with hold time switching a light; Home Assistant events |
| `low_latency.yaml` | Fast polling, 8 Hz data rate, shortest filters, minimal averaging |
| `multiple_devices.yaml` | Two sensors on two I2C buses by including the package twice |
| `deep_sleep.yaml` | Sleeps until the sensor's INT pin wakes it |

To try one, from the repo root:

```bash
cp examples/secrets.yaml.example examples/secrets.yaml   # then edit it
docker compose run --rm esphome run examples/esp32.yaml
```

For your own devices, copy an example to `local-<name>.yaml`; those are
gitignored and skipped by CI.

Tuning: lower filter divisors and less averaging respond faster; higher ones
reject more noise. Presence detection needs a minute or two to settle after
power-on. See [../docs/configuration.md](../docs/configuration.md).
