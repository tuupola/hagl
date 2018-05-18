# Coding style for Copepod

This document is meant to define the coding style for Copepod, and
all subprojects of Copepod. This covers C and Assembly coding
conventions, *only*.

## Headers

* All files that are newly written, should have the MIT License clause
at the top of them.

* For files that are copied from another source, but contain an MIT
compatible license, the original license header shall be maintained.

* The Apache License clause for the top of files is as follows:

```no-highlight
/*

Copyright (c) 2018 Your Name

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
```

## Whitespace and Braces

* Code must be indented to 4 spaces, tabs should not be used.

* Do not add whitespace at the end of a line.

* Put space after keywords (for, if, return, switch, while).

* for, else, if, while statements must have braces around their
code blocks, i.e., do:

```
    if (x) {
        assert(0);
    } else {
        assert(0);
    }
```

Not:

```
    if (x)
        assert(0);
    else
        assert(0);
```

* Braces for statements must be on the same line as the statement.  Good:

```
    for (i = 0; i < 10; i++) {
        if (i == 5) {
            break;
        } else {
            continue;
        }
    }
```

Not:

```
    for (i = 0; i < 10; i++)
    { <-- brace must be on same line as for
        if (i == 5) {
            break;
        } <-- no new line between else
        else {
            continue;
        }
    }
```

* After a function declaration, the braces should be on a newline, i.e. do:

```
    static void *
    function(int var1, int var2)
    {
```

not:

```
    static void *
    function(int var1, int var2) {
```

## Line Length and Wrap

* Line length should never exceed 79 columns.

* When you have to wrap a long statement, put the operator at the end of the
  line.  i.e.:

```
    if (x &&
        y == 10 &&
        b)
```

Not:

```
    if (x
        && y == 10
        && b)
```

## Comments

* No C++ style comments allowed.

* When using a single line comment, put it above the line of code that you
intend to comment, i.e., do:

```
    /* check variable */
    if (a) {
```

Not:

```
    if (a) { /* check variable */
```


* All public APIs should be commented with Doxygen style comments describing
purpose, parameters and return values.  Private APIs need not be documented.


## Header files

* Header files must contain the following structure:
    * MIT (see above)
    * ```#ifdef``` aliasing, to prevent multiple includes
    * ```#include``` directives for other required header files
    * ```#ifdef __cplusplus``` wrappers to maintain C++ friendly APIs
    * Contents of the header file

* ```#ifdef``` aliasing, shall be in the following format, where
the package name is "os" and the file name is "callout.h":

```no-highlight
#ifndef _OS_CALLOUT_H
#define _OS_CALLOUT_H
```

* ```#include``` directives must happen prior to the cplusplus
wrapper.

* The cplusplus wrapper must have the following format, and precedes
any contents of the header file:

```no-highlight
#ifdef __cplusplus
#extern "C" {
##endif
```

## Naming

* Names of functions, structures and variables must be in all lowercase.

* Names should be as short as possible, but no shorter.

* Globally visible names must be prefixed with the name of the module,
followed by the '_' character, i.e.:

```
    os_callout_init(&c)
```

Not:

```
    callout_init(c)
```

## Functions

* No spaces after function names when calling a function, i.e, do:

```
    rc = function(a)
```

Not:

```
    rc = function (a)
```


* Arguments to function calls should have spaces between the comma, i.e. do:

```
    rc = function(a, b)
```

Not:

```
    rc = function(a,b)
```

* The function type must be on a line by itself preceding the function, i.e. do:

```
    static void *
    function(int var1, int var2)
    {
```

Not:

```
    static void *function(int var1, int var2)
    {
```

* In general, for functions that return values that denote success or error, 0
shall be success, and non-zero shall be the failure code.

## Variables and Macros

* Place all function-local variable definitions at the top of the function body, before any statements.

## Compiler Directives

* Code must compile cleanly with -Wall enabled.

