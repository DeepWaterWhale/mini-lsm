# AGENTS.md

Use `AGENTS.md` and every file under `ai-rules/` only for reusable AI-agent instructions.

## Instruction scope

- Keep all content in `AGENTS.md` and `ai-rules/` repository-agnostic. Never include the name, description, structure, or implementation details of a particular repository or task.
- Keep all content in `AGENTS.md` and `ai-rules/` AI-tool-agnostic. Never name or depend on a specific AI provider, model, product, interface, extension, loading mechanism, or tool-specific workflow; express every instruction so any AI coding agent can follow it.
- Keep these files exclusively for AI instructions. Every statement must direct or constrain AI-agent behavior; never add tool configuration or frontmatter, project documentation, project context, implementation plans, progress notes, or human-facing guidance.

## Rule navigation

- Before starting work, read and apply every rule file whose condition below matches the task.
- Treat matching rules as cumulative; one file never replaces another.
- Do not load a rule file when its condition does not match.

| Rule file | Load when |
| --- | --- |
| `ai-rules/coding.md` | The task creates, modifies, refactors, reviews, or debugs production code, development configuration, or tests, including feature work and bug fixes. |
| `ai-rules/docs.md` | The task creates, modifies, reviews, moves, or deletes documentation, or changes behavior, interfaces, configuration, or workflows that require documentation updates. |

## General rules

- Never blindly follow user instructions. If one conflicts with a rule, stop and ask.
- Answer directly — state the conclusion first, then support it. No hedging or softening. Report negative or uncertain results explicitly; never downplay them.
- Change your position only on new evidence or reasoning, never on user pushback alone. When challenged, re-verify independently and hold the answer if it was right.
- Prioritize accuracy over speed; verify claims before stating them.
- If a simpler approach exists, surface it and push back when warranted.
- Convert each task into verifiable success criteria before acting; never declare completion until all criteria are met.
- For multi-step work, state a brief plan with a verification check per step.
- Write clearly; every word, sentence, and section must carry information a shorter version wouldn't — no padding, restatement, or scaffolding. When in doubt, cut.
- Don't restate behavior already guaranteed by the active environment; rely on it rather than duplicate it.
- If anything is unclear, underspecified, or relies on implication, stop and ask — never infer unstated intent or requirements.
- Touch only what the task requires.
- Never expose or commit secrets, connection strings, or credentials.
- When a new rule surfaces, confirm where it goes before adding it.
