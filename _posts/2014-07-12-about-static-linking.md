---
layout: post
title: "About static linking"
description: ""
category: 
tags: []
---
{% include JB/setup %}

One of the things I hate most about Linux is to compile software. Sometimes it
is a nightmare: lack of root permission, requirement of new gcc, dependencies
on huge or weird libraries, etc. Whenever these happens, I ask myself: why not
just distribute statically linked binaries such that they can run on most Linux
distributions?  I knew a few reasons, but only today I took the question a
little more serious and did a google search. The following two links are
quite useful: [static linking vs. dynamic linking][svsd] and [static linking
considered harmful][harm].

In summary, static linking has the following disadvantages: more likely to be
attacked, not receiving patches in dynamic libraries, more memory hungry, not
truly static, sometimes not flexible and potentially violating GPL. I buy all
these arguments. However, for tools in bioinformatics, these are not big
concerns because most bioinformatics tools:

* are not system utilities and are not security-critical.

* are only linked to small dynamic libraries. Statically linking the tools will
  not cost much memory.

* do not often use glibc features that have to be dynamically linked.

* are distributed under a license compatible with LGPL.

Most command-line bioinformatics tools can be statically linked without
problems. And I think we should create a repository for precompiled
bioinformatics tools. This will at least make my life much easier. What about
you?

[svsd]: http://stackoverflow.com/questions/1993390/static-linking-vs-dynamic-linking
[harm]: http://www.akkadia.org/drepper/no_static_linking.html

