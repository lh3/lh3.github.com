---
layout: post
title: "What high-performance language to learn?"
description: ""
category: 
tags: []
---
{% include JB/setup %}

In the past couple of months, I have been asked several times about what language(s) to learn
if someone wants to write high-performance programs.
This is a sensitive topic that often triggers heated debate
partly because many fast languages share similar features and are comparable in performance.
My general take is that for small research projects only involving a few developers,
the choice of programming languages is personal.
Nevertheless, if you hold a gun to head and force me to recommend a single high-performance language,
I will probably say [Rust][rust].
Here is my thought.

### Why not Python or R?

Python and R are the most popular programming lauguages in computational biology.
However, they are slow unless you can find libraries written in other efficient languages.
How slow? Often [~50 times slower][benchmark] than a C/C++ implementation.
This means a one-minute task in C/C++ is turned into one hour and
a one-hour task is turned into two days.
When you have a new method with the bottleneck falling in to pure Python/R code,
the inefficiency of Python/R may limit your stretch.

Another practical problem, at least with Python, is the deployment of your tools.
To run a Python program, users have to install dependencies on their machines.
This is slow and sometimes problematic when dependencies of different packages conflictive with each other.
With C/C++, it is possible, though takes efforts, to compile portable binaries that do not require users to install dependencies.
Properly distributed C/C++ tools are easier to install and use.

### Why Rust?

Rust is a mature memory-safe programming language with little/no compromise on performance.
It is as efficient as C/C++ and almost free of memory-related errors.
Unlike C/C++, Rust comes with its own package manager, which greatly simplifies the reuse of existing libraries;
Rust also makes it easy to create portable executables, improving user experiences.
Rust has been used [for Linux kernel development][linux-rust] and endorsed [by Google][google-rust], [by the CTO of Microsoft Azure][ms-rust] and
surprisingly, even [by White House][white-house].
You can easily find articles on the Internet about how good Rust is.
I will save some words here.

### The caveat

Rust is associated with a steep learning curve.
Its secure memory model makes it challenging to implement certain data structures.
Even implementing a [doubly linked list][rust-dlist] or graph may take a lot more efforts than in C/C++.
Furthermore, the Rust memory model brings new concepts that are rarely seen in other common programming languages and take time to digest.
I have written five short programs in Rust but I am still at the bottom of the learning curve.

Nevertheless, I want to note that mastering C from scratch might not be easier.
Although C looks simpler, getting the best out of C without frequent [segmentation fault][segfault] may take years of practice.
Modern C++ is a very different language from C.
It is a behemoth with a myriad of modern features and decades of legacy packed in one of the most complex languages.
Mastering C++ takes more efforts.

### Other contenders

Rust is not the only language that matches the performance of C/C++ without memory-related bugs.
If you do not need the best possible performance and feel Rust is difficult to learn, Go may be a decent second choice.
As to other high-performance languages, some have not reached the v1.0 milestone and are not stable;
some have a small community, which will make learning harder;
some, in my humble opinion, are not designed well;
some (e.g. Swift and C#) are popular in industry but are rarely used in our field for historical reasons.
I like playing with these languages, but I wouldn't recommend them to general computational biologists.

### Am I switching to Rust?

Not in the near future.
I enjoy the freedom of [shooting myself in the foot][shoot].

[rust]: https://www.rust-lang.org/
[google-rust]: https://security.googleblog.com/2024/03/secure-by-design-googles-perspective-on.html
[white-house]: https://www.whitehouse.gov/oncd/briefing-room/2024/02/26/memory-safety-statements-of-support/
[ms-rust]: https://twitter.com/markrussinovich/status/1571995117233504257
[linux-rust]: https://www.kernel.org/doc/html/next/rust/index.html
[rust-dlist]: https://rust-unofficial.github.io/too-many-lists/
[segfault]: https://en.wikipedia.org/wiki/Segmentation_fault
[shoot]: https://www.stroustrup.com/quotes.html
[benchmark]: https://benchmarksgame-team.pages.debian.net/benchmarksgame/fastest/python3-gcc.html
