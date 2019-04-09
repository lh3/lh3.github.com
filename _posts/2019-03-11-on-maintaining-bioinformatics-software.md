---
layout: post
title: "On maintaining bioinformatics software"
description: ""
category: 
tags: []
---
{% include JB/setup %}

In 2006, Ruiqiang Li, the now CEO of Novogene, said to me in Chinese: "you
can't maintain the [TreeFam][treefam] database forever". Considering TreeFam as
my most significant work at the time, I said "I think I can". Some of you know
what happened next: one year later, I started to develop tools for next-gen
sequencing data and forgot about TreeFam almost completely. Few can constantly
and single-handedly maintain their own software or databases.

A database has to receive constant updates to stay relevant, but a software
package is much different. Along with TreeFam, I also developed a companion
tool, [TreeBeST][treebest] (written in C++ by the way). It is [used by Ensembl
Compara][compara] to build gene trees and still serves the community. A few
months ago when I needed to view and compare trees, I [downloaded][tbdl] the
binary of TreeBeST compiled in 2007. It still works. A software package can
survive without the attention of its developer.

Currently I have [74 repos][myrepo] at GitHub. Most of them are my own projects
in a state similar to TreeBeST. They still work for the specific tasks they
were designed for but they rarely receive code changes any more. This is how I
maintain my personal projects: I try *not* to maintain them. To achieve the
goal, 1) I strive to simplify the user interface to reduce users' questions. 2)
I make my projects independent of each other and of external libraries to avoid
changes in dependencies failing my own projects. 3) If I perceive significant
changes to an existing code base, I more often duplicate the code and start
fresh (e.g. fermi vs fermi2 vs fermi-lite, and minimap vs minimap2). This way
I can forget about compatibility and freely cut [technical debts][tdebt]
without affecting the stability of the previous versions. These strategies have
enabled me to switch projects from time to time without leaving unmanageable
messes behind.

Many fundamental tools in bioinformatics (e.g. BLAST, samtools/htslib and GATK)
have thrived only due to continuous efforts by a stable development team, but
in reality, much more tools are coded in short term by individual developers
who lack sustainable resources. In the latter case, developing simple, clean
and self-consistent tools without need for maintenance is often the best way
towards maintenance, just as Richard Durbin, my postdoc advisor, [said in an
interview][interview]: "a key thing is that software or a data format does a
clean job correctly, that it works ... support isn't a critical thing, in a
strange way. Rather, it's the lack of need for support that's important".
Think about this when you develop your next coding project.

[treefam]: http://www.treefam.org/
[treebest]: https://github.com/Ensembl/treebest
[compara]: https://useast.ensembl.org/info/genome/compara/homology_method.html
[tbdl]: https://sourceforge.net/projects/treesoft/files/treebest/1.9.2/
[myrepo]: https://github.com/lh3?tab=repositories
[tdebt]: https://en.wikipedia.org/wiki/Technical_debt
[interview]: https://www.nature.com/articles/nbt.2721
