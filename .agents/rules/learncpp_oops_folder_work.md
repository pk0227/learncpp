---
trigger: always_on
description: Mandatory steering guide and workflow rules for learncpp OOPs chapter folders
---

# Learncpp OOPs — Folder Work Steering Guide

> This file is the **mandatory reference** for any agent working on learncpp OOPs chapter folders.
> Read this **before starting any work** on a folder.

---

## 🎯 Goal

For each OOPs chapter folder, the job is to:
1. Produce a high-quality **`Readme.md`** from the existing **`Readme.txt`**
2. Apply all corrections and missing topics to **both `Readme.txt` and `Readme.md`**
3. Ensure **every topic in the corrected Readme.txt has a corresponding `.cpp` example** — implement any that are missing

The output must have:
- All original content **faithfully preserved** (no information loss)
- Technical **corrections** applied to **both files**
- **Missing topics** added to both files (only after confirming no upcoming folder covers them)
- All code examples **linked** with clickable paths in `Readme.md`
- Every topic that needs a code example **has one** — write and add it if missing
- Rich markdown **formatting** applied in `Readme.md`

---

## 📋 Mandatory Workflow (Do in Order)

### Step 1 — Read the Source
1. Read the **full `Readme.txt`** from top to bottom.
2. List all **sections** and **subsections** found in it.
3. List all **code example files** found in subfolders: `find . -name "*.cpp" | sort`
4. Note the **exact folder structure** so file links are correct.

### Step 2 — Identify What's Missing (Before Writing Anything)
For any topic that appears to be missing or underdeveloped in `Readme.txt`:
1. **Check the upcoming folders first** — scan the next 2–3 folder `Readme.txt` files for that topic.
2. If the topic is covered **in full depth** in an upcoming folder → **do NOT add it here**. Just add a brief contextual mention if needed.
3. If the topic is only **introduced** in an upcoming folder or not covered at all → **add it here**.
4. Document your decision with a short comment so it can be traced.

> **Rule**: An introductory mention in folder N and a full deep-dive in folder N+2 is **not duplication**.
> Duplication is when the same full explanation exists in two places.

### Step 3 — Apply Technical Corrections (to BOTH files)

> [!IMPORTANT]
> Every correction and every missing topic addition must be applied to **both `Readme.txt` and `Readme.md`**.
> `Readme.txt` is the source of truth — it must always reflect the corrected, complete content.
> `Readme.md` is the formatted version — it must match the corrected `Readme.txt` exactly.

Check the following common error categories:
- C++ standard version numbers:
  - `std::vector` → **C++98** (not C++03)
  - `std::array` → **C++11**
  - `std::ssize()` → **C++20**
  - `std::to_underlying` → **C++23**
- Statements about features that have evolved across standards (constexpr, ranges, etc.)
- Claims about behavior that differ between compilers (GCC vs Clang vs MSVC)
- Any statement that says "cannot" that was later relaxed in a newer standard

**Correction workflow**:
1. Find the error in `Readme.txt` → fix it in `Readme.txt` first
2. Then apply the same fix to the corresponding location in `Readme.md`
3. For added missing topics: add to `Readme.txt` first, then add formatted version to `Readme.md`

### Step 4 — Write the Readme.md

#### 4a. Structure
- Start with: `# Chapter N: [Title]`
- Include a **Table of Contents (TOC)**:
  - Must be placed right after the title and any high-level introduction/pillar list.
  - Enclosed between `---` horizontal rules.
  - Formatted as a numbered list with working markdown anchor links to all sections: `1. [Section Title](#anchor-slug)`
  - Anchor slugs must exactly match the section heading text (lowercased, spaces replaced by hyphens, special characters removed).
- Use `---` horizontal rules between major sections
- Section headings must match the `Readme.txt` section numbering exactly
- Every section must end with a **📁 Code Examples** subsection

#### 4b. Content Preservation (CRITICAL)
> ⚠️ **This is the most important rule.**

Every **subsection** from `Readme.txt` must appear in `Readme.md`. Do NOT:
- Merge separate subsections into one bullet point
- Replace a detailed reasoning chain with just its conclusion
- Omit a sub-bullet because it seems obvious
- Skip a "why?" explanation and only keep the "what"
- Skip intro/motivating sentences that set context for a section

Specifically preserve:
- **All definitional sentences** (verbatim meaning, not loose paraphrase)
- **All reasoning chains** (especially "why can't we do X? because Y")
- **All rule-of-thumb and best-practice statements**
- **All "Note:" and "NOTE:" items** from Readme.txt — convert to GitHub alerts
- **All comparison tables** from Readme.txt — convert ASCII tables to markdown tables
- **Motivating intro sentences** at the start of each section (the "why this matters" context)

#### 4c. Formatting Rules
Apply the following to ALL content:

| Element | Rule |
|---|---|
| **Key terms** | Bold on first definition: `**copy semantics**`, `**size_type**` |
| **Function names / types** | Always use backtick code: `` `push_back()` ``, `` `std::vector<T>` `` |
| `[!WARNING]` alert | UB, crashes, data races, dangling pointers, silent wrong behavior |
| `[!IMPORTANT]` alert | Rules the reader must not forget, critical facts |
| `[!TIP]` alert | Best practices, recommended approaches |
| `[!CAUTION]` alert | Things that silently corrupt behavior (e.g., `resize()` for stack) |
| `[!NOTE]` alert | Context, non-obvious details, non-critical background notes |
| **Code blocks** | Always specify language: ` ```cpp ` |
| **Comparisons** | Convert to markdown tables with bold column headers |
| **Math/formulas** | Use LaTeX `$...$` or `$$...$$` where it improves clarity |

#### 4d. Enhancements (Beyond the Original)
You MAY add content not in `Readme.txt` **only if**:
- It is a **direct technical enhancement** removing ambiguity (e.g., a formula, a C++ standard clarification)
- It is a **technical correction** required for accuracy
- It is a **missing topic** confirmed absent in all upcoming folders
- It **fills an implied gap** (e.g., thread-safety risk that the text implies but doesn't state)

You MUST NOT:
- Add opinions or tangential content
- Change the teaching order of topics
- Rename sections from what Readme.txt uses

#### 4e. Inline Code Snippets (MANDATORY)
In addition to linking standalone `.cpp` files in `📁 Code Examples`, **you must include short, focused inline C++ code snippets directly in `Readme.md`**:
- **When to include:** Wherever key syntax, definitions, language features, constructors, idiom patterns, or code comparisons are explained in text (e.g. member initializer list syntax, factory functions, default/delete constructors, `this` chaining, static member definitions, friend declarations, ref-qualifiers, vector initialization methods).
- **Purpose:** Allows the reader to immediately see and understand the concrete C++ syntax in context without needing to open the external `.cpp` files.
- **Rules for inline snippets:**
  - Keep snippets short and focused (5–15 lines).
  - Always use standard code block syntax: ` ```cpp ... ``` `.
  - Include brief inline comments highlighting the critical mechanics (e.g., `// binds to rvalue`, `// prevents implicit conversion`).
  - Do NOT replace prose explanations with code snippets alone — keep both the explanation and the snippet together.

### Step 5 — Link Code Examples
For every section, add a `### 📁 Code Examples for Section N` subsection:
- One bullet per `.cpp` file in the corresponding subfolder
- Format:
  ```
  - [`subfolder/filename.cpp`](file:///absolute/path/to/file.cpp): What this file demonstrates.
  ```
- The description must say **what the file demonstrates**, not just repeat the filename
- Verify every linked file **actually exists** before writing the link

### Step 6 — Check and Implement Missing Code Examples

> [!IMPORTANT]
> The corrected `Readme.txt` and `Readme.md` are the **authoritative list of topics**.
> Every major topic or concept that benefits from a code demonstration **must have a `.cpp` file**.
> If a topic is documented but has no example file, you must **write and add the example**.

**How to check:**
1. For each section in the corrected `Readme.txt`, identify all **distinct concepts** taught.
2. Cross-reference against the existing `.cpp` files in the subfolder.
3. For any concept that has **no corresponding example**, create one:
   - Name it following the existing naming convention: `N_descriptive_name.cpp`
   - Place it in the correct topic subfolder
   - The file must compile cleanly with `g++ -std=c++20`
   - It must demonstrate **only** the concept it is named for (focused, not a kitchen sink)
   - Add a comment block at the top explaining what it demonstrates
4. After creating new files, add their links to the `📁 Code Examples` subsection in `Readme.md`

**What qualifies as needing a `.cpp` file:**
- A new concept introduced for the first time in this section (e.g., `emplace_back`, `reserve()`)
- A common pitfall or bug pattern (e.g., `resize()` for stack corrupts data)
- A best-practice alternative that is recommended (e.g., `data()` indexing, `std::ssize()`)
- A comparison between two approaches (e.g., `push_back` vs `emplace_back`)

**What does NOT need its own `.cpp` file:**
- Pure conceptual definitions with no code aspect (e.g., "what is a container?")
- Topics fully shown by another existing example in the same section
- Topics that are only briefly mentioned and covered in a later dedicated folder

### Step 7 — Final Verification Checklist
Before finishing, verify:
- [ ] All corrections applied to **both `Readme.txt` and `Readme.md`**
- [ ] All missing topics added to **both `Readme.txt` and `Readme.md`**
- [ ] Every subsection from corrected `Readme.txt` has a corresponding heading in `Readme.md`
- [ ] Every NOTE/IMPORTANT from `Readme.txt` appears as a GitHub alert in `Readme.md`
- [ ] Every reasoning chain is preserved (not just the conclusion)
- [ ] Every motivating intro sentence per section is present
- [ ] Every major concept in corrected `Readme.txt` has a `.cpp` example file
- [ ] Every `.cpp` file (existing + new) is linked in the `📁 Code Examples` subsection
- [ ] All new `.cpp` files compile cleanly: `g++ -std=c++20 file.cpp -o /tmp/test`
- [ ] No topic is duplicated at full-depth in an upcoming folder
- [ ] All technical facts verified (std versions, behavior descriptions)
- [ ] All key terms bolded on first occurrence in `Readme.md`
- [ ] Table of Contents with working anchor links to all sections included at the top of `Readme.md`
- [ ] Short, focused inline code snippets (` ```cpp `) included in `Readme.md` wherever key syntax, idioms, or patterns are discussed

---

## 🚫 Common Mistakes to Avoid

| Mistake | Why It's Wrong |
|---|---|
| Replacing "Copy 2: can we elide? No. Can we move? No (lvalue)." with just "use const ref" | Loses the **reasoning** — the point is to understand *why*, not just *what* |
| Dropping "favor range-based for over index loops" sentence | That is a **best-practice rule**, not decoration |
| Dropping "auto sync benefit" (auto deduces updated type automatically) | That is a specific **teaching point** for maintainability |
| Dropping motivating intro sentences like "one of the bigger problems with..." | Those set context — they are **part of the lesson** |
| Adding `constexpr std::vector` without C++20 transient-allocation caveat | Technically misleading |
| Summarizing a detailed reasoning chain as one bold line | **Never summarize**; preserve and format |
| Adding a topic without checking upcoming folders | May cause **duplication** |
| Skipping a NOTE that says something "is non-binding" | **Behavioral detail** — must be preserved |
| Merging "Why differentiate length vs capacity?" into the length section | It's a **separate teaching point** about design intent |

---

## 📁 Expected Output per Folder

```
<chapter_folder>/
├── Readme.txt             ← source of truth: corrected + missing topics added
├── Readme.md              ← formatted mirror of corrected Readme.txt
└── <topic_subfolder>/
    ├── 1_existing.cpp     ← pre-existing example
    ├── 2_existing.cpp     ← pre-existing example
    └── 3_new_concept.cpp  ← NEW: added if topic had no example
```

> [!NOTE]
> `Readme.txt` is **NOT read-only**. It must be updated to reflect corrections and added topics.
> The rule is: **`Readme.txt` and `Readme.md` must always be in sync** after your work is done.

---

## 📌 Known Technical Facts (Verified)

| Fact | Correct Value |
|---|---|
| `std::vector` introduced in | **C++98** |
| `std::array` introduced in | **C++11** |
| `std::ssize()` introduced in | **C++20** |
| `std::to_underlying()` introduced in | **C++23** |
| `std::format` introduced in | **C++20** (requires GCC 13+ or Clang 14+) |
| `std::views::reverse` introduced in | **C++20** |
| `emplace_back` aggregate init support | **C++20** |
| `std::vector` constexpr support | **C++20** (transient allocations only) |
| Growth factor — GCC/Clang | **×2** (doubles capacity) |
| Growth factor — MSVC | **×1.5** |

---

## 📌 Duplication Reference (Folder 16 → Upcoming)

| Topic | Folder 16 | Upcoming Folder | Decision |
|---|---|---|---|
| Iterator invalidation (reallocation context) | Brief warning ✅ | Folder 18 (full deep-dive) | Keep both — complementary scope |
| Move semantics (intro + vector return) | Intro applied ✅ | Folder 22 (full theory) | Keep both — introductory vs deep |
| Iterators (begin/end/cbegin etc.) | Not needed | Folder 18 (dedicated) | Don't add to 16 |
| Smart pointers | Not needed | Folder 22 (dedicated) | Don't add to 16 |

---

## 📌 Lessons from Folder 16 Review (Claude Sonnet 4.6 vs Gemini Flash 3.8)

The following 11 items were found **missing** from Gemini's output when Claude reviewed it:

1. Copy 2 / Copy 4 detailed reasoning (why we can't elide, why we can't move, why out-param is ugly)
2. "When temporary is destroyed, no data left to clean up" — move semantics sentence
3. "Leave signed/unsigned warnings OFF" anti-pattern warning (Section 7)
4. `arr.data()[index]` advocacy bullet list — 4 reasons why it's the best option (Section 7)
5. "Favor range-based for over index loops" best-practice rule (Section 8)
6. `auto` maintainability benefit — type stays in sync if element type changes (Section 8)
7. Motivating intro sentence for Section 9 — "arrays don't tell you the meaning of the index"
8. "Why differentiate length and capacity?" — design rationale (Section 10)
9. "Stacks in C++ use general containers, not a dedicated stack type" context (Section 11)
10. `vector<bool>` basic usage subsection (Section 12)
11. "Why `vector<bool>` is discouraged" subsection (no opt-out, differs from other specializations)

**Root cause of all 11**: The model treated source content as something to *summarize and reformat*
rather than *preserve and enhance*. The fix is to treat every sentence in Readme.txt as deliberate teaching content.

---

## 📊 Chapter Progress Tracker

| Folder | Status | Readme.txt | Readme.md | TOC & Links | Code Snippets | Code Examples Verified |
|---|---|:---:|:---:|:---:|:---:|:---:|
| **14_Introduction_to_classes** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 10 added | ✅ 22 files linked |
| **15_More_on_Classes** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 8 added | ✅ 15 files linked |
| **16_Dynamic_arrays_std_vector** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ Complete | ✅ 15 files linked |
| **17_Fixed-size_arrays...** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 25+ added | ✅ 14 files linked (6 new) |
| **18_Iterators...** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 6+ added | ✅ 8 files linked (1 new) |
| **19_Dynamic_Allocation...** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 10+ added | ✅ 5 files linked (4 new) |
| **20_Functions** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 12+ added | ✅ 11 files linked (1 new) |
| **21_Operator_Overloading** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 15+ added | ✅ 21 files verified / linked |
| **22_Move_Semantics_n_Smart_Pointers** | ✅ Completed | ✅ Synced | ✅ Formatted | ✅ Complete | ✅ 15+ added | ✅ 19 files verified / linked |
| **23_Object_Relationships** | ⏳ Up Next | Pending review | Not created yet | Pending | Pending | Under construction |

---

*Steering file created: 2026-09-09 | Updated: 2026-09-10*
