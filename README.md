# The TI-BASIC Compiler

The TI-BASIC Compiler is all about compiling TI-BASIC code on your computer
into an 8XP file ready for transferring to your TI-83/TI-83+/TI-84 calculator.

Being able to type your programs instead of try and use the calculator's keypad
is much easier :)

## How to run the compiler?

For most situations, just "drag and drop" the text file containing your code
on top of the `tibasic.exe` file. A black window will appear for a split second
and you should have an 8XP file next to your text file.

If you feel comfortable using a command line, you can run `tibasic.exe` without
any arguments to get usage information.

## Important Note!

There are a few symbols that could not be directly transferred to the language and were replaced instead with simpler tokens. Also note some of these are 'shorthand' versions of supported tokens:

- `&` --> `and`
- `|` --> <code>&nbsp;or&nbsp;</code> (including spaces!)
- `~` --> <code>&nbsp;xor&nbsp;</code> (including spaces!)
- theta --> `[theta]`
- powers --> `^` (for instance, 5 squared would be `5^2`)
- `e` constant --> `[e]`
- Store --> `->` (same for such things as `>DMS`: `->DMS`)
- Roots --> `[root]^` (for instance, square root would be `[root]^2`)

All Picture variables (`Pic0` - `Pic9`) and String variables (`Str0` - `Str9`) are CAPITALIZED (ie. `STR0`)

Other shorthand versions for more obscure functions exist, check `src/tokens.cpp` for the mapping.

Happy programming!

## How to build the compiler?

You will need [CMake 3.15](https://cmake.org/download/) or above and a C++ compiler.

For Windows with Visual Studio, the CMake project can usually be loaded directly into the IDE. See [this tutorial](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170) for more.

On OSX or Linux, you will need to run:

```shell
$ mkdir build && cd build
$ cmake ..
$ make
```

The compiler will be built and the compiler can be found at `build/src/tibasic`.
