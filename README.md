# LibMessageStudio

This is a decompilation of the MessageStudio library.

The objective is to recreate the MessageStudio library for 3DS as accurately as possible.

Note that some names (especially for inlined, templated functions) are just plain guesses.

## Folder Structure

* **/LIBRARY_ROOT/libms/**

*    |____ **include/LMS** - Headers used in *LibMessageStudio*.

*    |____ **src** - Source code for *LibMessageStudio*.

## Message Files

* **msbt** / **MsgStdBn** - Standard Message file.
* **msbf** / **MsgFlwBn** - Message flow binary.
* **msbp** / **MsgPrjBn** - Project file.

Presets and features for more games can be added if desired.

## Building

Building this project requires:

- ARM C++ Complier (ARMCC) Version 4.0/4.1/5.0 [which can be found here.](https://github.com/RE-Pepper/data/releases/tag/dasdasdsa) 

## Contributing

### Non-inlined functions
When **implementing non-inlined functions**, please compare the assembly output against the original function and make it match the original code. At this scale, that is pretty much the only reliable way to ensure accuracy and functional equivalency.

However, given the large number of functions, certain kinds of small differences can be ignored when a function would otherwise be equivalent:

* Regalloc differences.

* Instruction reorderings when it is obvious the function is still semantically equivalent (e.g. two add/mov instructions that operate on entirely different registers being reordered)

When ignoring minor differences, add a `// NOT_MATCHING: explanation` comment and explain what does not match.

### Header utilities or inlined functions
For **header-only utilities** (like container classes), use pilot/debug builds, assertion messages and common sense to try to undo function inlining. For example, if you see the same assertion appear in many functions and the file name is a header file, or if you see identical snippets of code in many different places, chances are that you are dealing with an inlined function. In that case, you should refactor the inlined code into its own function.

Also note that introducing inlined functions is sometimes necessary to get the desired codegen.

If a function is inlined, you should try as hard as possible to make it match perfectly. For inlined functions, it is better to use weird code or small hacks to force a match as differences would otherwise appear in every single function that inlines the non-matching code, which drastically complicates matching other functions. If a hack is used, wrap it inside a `#ifdef MATCHING_HACK_CTR` (see above for a list of defines).

### Tentative PR Contributing rules
The `ctrdecomp` organization follows a set of standards to maintain consistency and quality across our projects. To help contributors meet these standards, our team has established the following guidelines:

* **All code must be submitted through the GitHub Pull Request process.**

* **Code must not be obtained from illegal or unauthorized material.** If such material is detected, the contribution **will not** be accepted.

* **Use of AI must be disclosed.** Contributors must disclose when and where they use AI.

* **All code must be reviewed by a human before submission.** Contributors are responible for reviewing to match styling, errors, etc.

* **Decompiled code should be reasonably representative of how the original source code may have been written. Avoid excessive or unnecessary pointer arithmetic when the underlying data is clearly identifiable as a struct or class.** In general, a raw Ghidra decompilation that merely compiles is not sufficient for PR acceptance; the code should be properly cleaned up, structured, and made readable.

* **Most functions should have a corresponding Doxygen documentation comment above its top-most declaration.** Most one-line functions are exempt here, but generally over 2-3 lines is a decent rule of thumb.

* **All code must be C++03-compliant.**