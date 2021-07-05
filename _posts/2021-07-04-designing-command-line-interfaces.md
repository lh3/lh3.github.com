---
layout: post
title: "Designing a command-line interface"
description: ""
category: 
tags: []
---
{% include JB/setup %}

This post is inspired by Vince's [tweet][tweet]. It describes my thoughts on
the design of command-line interface (CLI). Note that this article doesn't
necessarily represent the best practices; it just shows my personal
preferences.

First of all, I need to clarify the terminology. For an example, in the command
line below
```
rm -f file.txt
```
`rm` is the the *command*. `file.txt` is the *command-line argument*, which is
required for `rm` to function. `-f` is the *command-line option*, which is
optional. Some call `file.txt` a positional option, but I think that is
inconsistent because `file.txt` is not optional.

In general, I think a good CLI should be intuitive, concise and easy to
remember and to type. Here are some more specific details in my mind when I
design CLI.

* I prefer to use command-line arguments and strive to make the default
setting (i.e. without any options) work well. I personally feel it is easier to
remember the positions of arguments than to remember the option letters,
especially when different tools often use the same option letter for distinct
meanings. The positions of arguments tend to be more consistent across tools.
For example, most mainstream aligners take the first argument as the
reference/database and the second as the query. Another benefit of using
arguments is that users can more easily specify multiple similar input sources.

* I use a command-line argument parser compatible with the getopt behavior.
With getopt, an option with one hypen like `-a` is called a short option and an
option with two hypens like `--foo` is called a long option. If you see `-ab`,
it may mean `-a b` or `-a -b`, depending on the definition of `-a`. I know this
looks complicated to new users, but it is the convention the vast majority of
unix tools have adopted for decades. Users will learn the getopt behavior one
way or another anyway. Implementing non-standard behaviors (e.g. parsing
`-ab` as a long option, requiring a space after each option or allowing spaces
in an option) is more likely to cause confusion. 

* I try to avoid long options for basic settings because long options are
harder to remember and take longer to type. At least for basic use cases, I
expect a user to type a command line most of time, not to copy-paste an
excessively long command line in his/her note book. Human-first.

* Accept the standard input (aka stdin) as much as possible. This will help to
connect different tools together. Note that certain applications may save a lot
of memory or may be greatly simplified if they read an input file more than
once. It is ok not to support stdin in this case.

* If possible, output the results to the standard output (aka stdout) and
output the error and messaging information to the standard error output
(aka stderr). Nonetheless, it is also preferred to have an `-o file.out` option
to output to an ordinary file in case stdout and stderr may get mixed.

* For a long-running tool, I will make it output a message to stderr when it
ends. For example, minimap2 outputs running time and peak memory to stderr in
the end. If you see this message, you will know minimap2 is not killed by the
system.

* Keep CLI backwardly compatible such that a pipeline using an earlier version
of the tool can update the tool to the latest version without any code changes.
Backward compatibility is essential to the long-term stability of a
command-line tool.

The above are just general directions for you to think about. Even myself
doesn't strictly follow them.

[tweet]: https://twitter.com/vsbuffalo/status/1411771531407990784
[wiki]: https://en.wikipedia.org/wiki/Command-line_interface
[clig]: https://clig.dev/
