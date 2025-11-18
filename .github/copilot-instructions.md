## Project Guidelines for Copilot

You are assisting a student from campus 42 who must follow **strict coding norms** when writing C code. Please follow these rules **at all times** when providing code suggestions, completing functions, or writing new code.

### 🚨 THE "NORME" (MANDATORY RULES)

1. ✅ Each function must be **no more than 25 lines long**.
   - Please **number the lines at the beginning of each line**, like this, to help me check:
     ```c
     void function(void)
     {
     1:    int i;
     2:
     3:    i = 0;
     4:    while (i)
     5:    {
     6:        code;
               // some comment
     7:    }
     8:    return ;
     }
     ```
   - Count **all visible lines inside the function**, including blank lines.
   - Do **not count** comments or the opening/closing braces `{}`.

2. ✅ Use only **`while` loops**.
   - ❌ Do **not** use `for` loops.

3. ✅ Maximum **80 columns per line**.
   - Wrap lines accordingly to avoid exceeding this limit.

4. ✅ A function must receive **no more than 4 parameters**.

5. ✅ No more than **5 variables per function**.

6. ✅ No more than **5 functions per file**.

7. ❌ Do **not** use **ternary operators** (`? :`).

8. ❌ Do **not** initialize and declare a variable on the same line.
   - ✅ `int i;`
   - ✅ `i = 0;`
   - ❌ `int i = 0;` ← This is forbidden.

9. ✅ Function and variable names must be **in English** and **self-descriptive**.

---

### ✨ Additional Coding Style Preferences

- Write **clean and modular C code**.
- Use **proper memory management**.
- Favor **readability** and **maintainability** over micro-optimizations.
- Please simulate and explain code **following the actual execution flow** ("CUIDADO CON EL FLUJO"):
  - Explanations or comments must **follow the order in which the code is executed**, like a debugger (e.g., starting from `main`, then going step by step into each function called).
  - Return to the calling context after each function ends.
  - This helps me reason through the program and debug it effectively.
- Avoid writing any unnecessary boilerplate unless explicitly requested.

---

### ⚠️ IMPORTANT

These rules are not flexible.
They are strictly enforced in the evaluation process, and failing to follow them can cause a project to be rejected.

Please help the user write high-quality C code **that strictly follows these constraints**.

To check that everything is working correctly, I want you to check whether the programme compiles ‘make re’ for each function change (or several functions if the change involves several) (be careful, there are several versions of this project; this is the miniRT1311 folder). I also want you to clean the folder ‘make clean’ and run the programme with the test ./miniRT ‘scene.rt’ to check that it is visually correct.