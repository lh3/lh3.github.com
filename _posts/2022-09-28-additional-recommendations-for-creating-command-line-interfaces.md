---
layout: post
title: "A few suggestions for creating command line interfaces"
description: ""
category: 
tags: []
---
{% include JB/setup %}

[Command-line interface][cli], or CLI in brief, specifies how a user interacts
with a program on the command line. [Torsten Seemann][tseemann] wrote a [good
article][giga-cli] on creating CLI. This blog post adds a few more suggestions.

#### 1. Keep the backward compatibility of CLI as much as possible

Backward compatibility here means users can upgrade and run a tool without
changing the command lines they used in the past. This implies we should not
remove or change the meaning of an existing option. It is ok to add new
options. Backward compatibility, in my opinion, is the most important factor in
CLI design and outweighs all the following points.

#### 2. Human-first: command lines are meant for a human to type

It is important to keep CLI simple such that a human can remember the basic
syntax and type a command line without reading the full manual or looking back
through the bash history. For this goal, the tool should only require
indispensable input (e.g. input files) and it should set sensible default
values good for general use cases.

What are good default values has to be analyzed on a case-by-case basis. I only
mention one example. A common parameter used by high-performance tools is the
number of parallel threads. In my opinion, a tool should not attempt to use all
available CPUs by default because this default behavior may greatly impact many
users in a cluster environment. Some of my tools use 3 or 4 threads by default.
Defaulting to one thread is perhaps more common.

Command-line tools are also invoked in shell scripts or workflow scripts. In
this case, we do not repeatedly type command lines. An explicit and verbose CLI
may help to reduce typos and is preferred. It is worth considering such use
cases. Nonetheless, the human-first principle is still more important.

#### 3. If possible, read/write a file as a data stream

With file streaming, we read or write a file without jumping back and forth
using something like `seek()` calls. Streaming is essential to unix pipes and
will make the tool work nicely with others. By convention, it is also preferred
to support a single dash `-` for standard input/output, but this is not that
important in unix as we can use `/dev/stdin` or `/dev/stdout` as long as the
tool supports file streaming.

A corallary is not to guess the input/output file formats by
file extensions because data streams do not have file extensions. We may use
[named pipe][named-pipe] but it is awkward.

#### 4. Print useful information to the standard error output

It would be good for a tool to print the version number and the full command
line in use. I often find this is helpful when going back to old analysis. I
recommend to print something like "Done!" when the tool finishes. This lets
users know the tool has not crashed in the middle. Printing progress is also
convenient for a long running job as users may get a rough estimate about how
long the job will take.

All these messages should be printed to the [standard error output][stderr],
not to the [standard output][stdout]. The standard output is meant to be used
for piping (see suggestion 3). In addition, in many languages, text outputted
to the standard output is bufferred by default for efficiency. It may not be
written to the output when the tool is interrupted. The standard error output
is usually not bufferred and is more useful for logging and debugging.

#### 5. Use a getopt-compatible library to parse command-line options

This is a minor point. The unix/GNU getopt convention allows both short options
and long options with multiple variations (see [this article][unix-cli] for
details). Most unix tools, except gcc, follow this convention. The standard
libraries in many languages also support it. A tool adopting different
behaviors will increase the chance of misuses. Some may argue the unix
convention is confusing but breaking the convention is worse.

[cli]: https://en.wikipedia.org/wiki/Command-line_interface
[tseemann]: https://www.doherty.edu.au/people/associate-professor-torsten-seemann
[giga-cli]: https://academic.oup.com/gigascience/article/2/1/2047-217X-2-15/2656133
[unix-cli]: https://nullprogram.com/blog/2020/08/01/
[named-pipe]: https://en.wikipedia.org/wiki/Named_pipe
[stderr]: https://en.wikipedia.org/wiki/Standard_streams#Standard_error_(stderr)
[stdout]: https://en.wikipedia.org/wiki/Standard_streams#Standard_output_(stdout)
