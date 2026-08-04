# Coding Rules

## Implementation

- Don't refactor unbroken code, or "improve" adjacent code, comments, or formatting the task didn't touch.
- If you notice unrelated dead code, mention it — don't delete it.
- Remove imports/variables/functions that **your** changes orphaned.
- Only touch comments on methods/fields you actually changed.
- Don't add speculative features, configurability, or abstractions for single-use code.
- **Trust internal code; validate only at system boundaries.** No defensive guards for impossible scenarios, conditions already handled by existing cleanup paths, or duplicate handling "just in case".
- When fixing a bug, address the root cause — do not add guards for theoretical edge cases.
- If the implementation feels overcomplicated for the requirement, rewrite it smaller.

### Readability

- Match existing style even if you would write it differently.
- Keep methods short and single-purpose (≤100 lines); extract helpers instead of nesting.
- Prefer strong domain types over loose primitives.
- No hidden side effects; helpers must not mutate shared state.
- Use meaningful names; avoid abbreviations except well-known acronyms (SQL, JSON, ID).
- Method names must describe the full scope of what they do.
- Put each top-level class in its own source or test file; never define multiple top-level classes in one file.
- Code must be self-explanatory; never restate what it does.
- Only comment a non-obvious **why**.

## Reliability

- No sensitive data in logs (PII, tokens, connection strings).
- Validate all user input at system boundaries.
- Always use parameterized queries — never string-concatenate user input into SQL.
- Let exceptions propagate to the boundary layer.
- Do not catch generic exceptions unless re-throwing or at a top-level boundary.
- Use domain-specific exception types where they add clarity.
- Concurrency-sensitive code must be thread-safe.
- Handle edge cases: nulls, empty collections, boundary values.

## Testing

- Provide one dedicated unit test class for every production class.
- Name tests `MethodName_Scenario_ExpectedResult`; one behavior per test.
- New features require unit tests; functional tests recommended.
- Bug fixes require a regression test.
- Refactors: existing tests must pass; add new tests if coverage gaps found.
- Tests must reference production constants directly — never mirror constants (limits, thresholds, retry counts) as `private const` in test files.
- For boundary-violation cases, use `<CONST> + 1`.
- For a deliberate "did this limit change?" pin-test, centralize it in one place (typically alongside the constant) — don't duplicate mirrors across test files.
- Use a per-test setup hook (e.g. `[TestInitialize]`, `@BeforeEach`) to construct shared collaborators and mocks once and apply their default (common-case) stubbing there; each test overrides only what differs. Don't repeat identical mock creation or default stub setup across test methods.
- Assert observable behavior — returned values, thrown exceptions, persisted state. Don't assert on interactions (call counts, "was X called with Y").
- For a shared code path reached through multiple entry points (overloads, wrappers), test once at one entry point; don't duplicate the same behavior across callers.
- Parameterize tests when cases differ only by input or expected value.
