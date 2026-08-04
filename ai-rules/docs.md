# Documentation Rules

- Never restate implementation logic in documentation; the code is the sole source of truth for all implementation logic.
- Require every documentation file to declare exactly one supported document `type` in YAML frontmatter.
- Treat PM documents as the source of truth for intended functionality, Context documents as the source of truth for implementation decisions, Operation documents as the source of truth for executable workflows, and the Glossary document as the source of truth for terminology.
- Treat every Markdown italic span in documentation as a glossary term reference; never use Markdown italics for emphasis or any other purpose.
- Update documentation whenever behavior, interfaces, configuration, workflows, constraints, or prerequisites change.
- Describe current behavior only; remove or revise stale, speculative, or contradictory guidance.
- Preserve the existing information architecture, terminology, tone, and formatting unless the task requires restructuring.
- Keep each concept in one canonical location; link to it instead of duplicating explanations.
- Write concise, task-oriented guidance that makes prerequisites, steps, expected outcomes, and relevant failure modes clear.
- Keep headings logically nested and update navigation, links, anchors, and cross-references when content moves.
- Keep commands and code examples minimal, copyable, and synchronized with the implementation.
- Verify technical claims, identifiers, defaults, and examples against the source of truth before declaring completion.
- Do not manually edit generated documentation; update its source and regenerate it.

## Document Types

- Use only the document types defined below; define a type and its required fields here before using it.

### Guide

```yaml
---
type: guide
---
```

- Use a Guide document for orientation and navigation that do not define product behavior, implementation decisions, or executable workflows.
- Keep Guide content concise and link to canonical PM, Context, Operation, and Glossary documents instead of repeating their content.
- Never place runnable command sequences in a Guide document.

### Operation

```yaml
---
type: operation
---
```

- Use an Operation document for executable setup, build, test, deployment, maintenance, and troubleshooting workflows.
- Keep each Operation document focused on a cohesive set of workflows for one system or component.
- State prerequisites and the required starting directory or system state before listing commands.
- Keep commands copyable, ordered, and synchronized with the configuration that defines them.
- State the expected outcome and document only actionable failure recovery that differs from the normal workflow.
- Keep workflow variants separate only when their prerequisites, commands, or outcomes differ.
- Store runnable command sequences in Operation documents, not in Guide, PM, Context, or Glossary documents.

### PM

```yaml
---
type: pm
---
```

- Use one PM document to describe the detailed functionality of one module.
- Define purpose, scope, non-goals, observable behavior, invariants, and meaningful boundaries at the capability level.
- Group equivalent cases; never require exhaustive input-output mappings unless each mapping is itself part of the external contract.
- Specify behavioral distinctions only when they affect users, compatibility, security, or data correctness.
- Use examples to clarify categories and boundaries, never as an exhaustive requirements list.
- Describe what the module must do, never how the code implements it.
- Record implementation decisions and behavior-neutral details in the corresponding Context document, not the PM document.

#### Acceptance Criteria

- Add an acceptance criterion only when it contributes a falsifiable condition, boundary, invariant, or behavioral distinction not already stated.
- Never restate the objective, story, or surrounding requirements as an acceptance criterion.
- Do not require every story or capability to have a separate acceptance-criteria section.
- Use Given/When/Then only when the initial state or trigger materially changes the expected behavior; otherwise use a direct declarative statement.
- Merge equivalent scenarios into one behavior category instead of listing permutations.
- Use a table when several operations or categories share the same contract dimensions.
- Keep each criterion concise and independently verifiable.

### Context

```yaml
---
type: context
pm: ./relative/path/to/pm.md
---
```

- Use one Context document to record implementation decisions made while realizing the referenced PM document.
- Set `pm` to a relative path from the Context document to an existing document with `type: pm`.
- Record each decision, its rationale, considered alternatives, trade-offs, and consequences.
- Reference the PM document and code instead of repeating their functionality or implementation logic.

### Glossary

```yaml
---
type: glossary
---
```

- Maintain exactly one Glossary document for all documentation.
- Define each term once under a level-two heading; the heading establishes its canonical spelling and capitalization.
- Do not define headings that differ only by letter case.
- Keep all term headings in case-insensitive alphabetical order by canonical term.
- Reference a glossary term in other documents as `*term*`.
- Resolve every `*term*` reference against a Glossary heading using case-insensitive matching; letter case never creates a distinct term.
- Keep term definitions conceptual; do not restate implementation logic.
