# Instructions for AI assistants

Follow https://github.com/romkey/esphome-ai-coding-rules/blob/main/RULES.md.
With Claude Code, the `esphome-rules` plugin loads them for you.

Read docs/overview.md first. `main` is protected: work on a branch and
open a pull request; CI and Lint must pass before it can merge.

<!-- Project-specific notes go below. -->

This repo is a library: an external component in `components/sths34pf80/`
and a package in `packages/`. The STHS34PF80's I2C address is fixed at 0x5A;
never suggest a second address. Hardware facts come from
`datasheets/sths34pf80.pdf`.
