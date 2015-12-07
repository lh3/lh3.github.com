---
layout: post
title: "Bioconda: the best bio-software package manager so far"
description: ""
category: 
tags: []
---
{% include JB/setup %}

### Getting Started

Firstly, a few basic concepts. [Conda][conda] is a portable package manager
primarily for Python and precompiled binaries. Miniconda is the base system of
conda. It includes a standard python and a few required dependencies such as
readline and sqlite. In conda, a *channel* contains a set of software
typically managed by the same group. [Bioconda][biocd] is a channel of conda
focusing on bioinformatics software. The following shows how to install and
use conda.

    # Download the miniconda installation script for Python2
    wget https://repo.continuum.io/miniconda/Miniconda-latest-Linux-x86_64.sh
    # Install conda; it will ask a few questions, including the installation path
    sh Miniconda-latest-Linux-x86_64.sh
    . ~/.bashrc   # or relogin to get PATH updated
    # A few examples
    conda info
    conda search -c bioconda bwa
    conda install -c bioconda bwa
    conda list

In this example, option `-c bioconda` specifies that the package comes from
the bioconda channel; otherwise the default channel is used.

### Why conda?

Firstly, conda ships precompiled binaries, not source code. In my experience,
shipping source code has not worked well on "managed" Linux clusters.
Compiling source code is error prone, space wasting and time consuming.

Secondly, conda assumes the users do not have the root permission. Of course
not all software can be installed without the root permission, but most
enduser applications should not require the privilege.

Thirdly, conda is self contained. It puts all files in one root directory. It
does not taint other system paths. Conda installs its own dependencies. Unless
you want to build new packages, you do not need compilers or an existing
python installation.

Fourthly, conda ships portable binaries. It uses [rpath][rpath] to make sure
non-default dynamic libraries are loaded from a fixed relative path, not from
the system paths. Rpath solves one of the portability difficulties. The other
is libc, which can be solved by compiling on an old Linux system.

### What does conda lack?

I could be wrong -- it seems to me that conda does not provide a fully
automated system to build on old systems. The package maintainers need to find
a machine/VM/docker image with an old Linux system and have reasonable skills
to create portable packages. I think a missing step is to allow package
maintainers to (either manually or automatically) build tools on a CentOS5 AWS
instance.

Another missing link is to allow maintainers to bypass the conda build
process. For example, if I can build a portable package in my own way, it
would be good to let me add a package without having to go through `conda
build`. Conda could provide a script to check the structure of a package and
test it on a CentOS5 machine.

A third problem is documentation. Conda is in fact simple, but its
documentation is complex. It is too verbose for a beginner like me. I need to
read through a lot of pages to understand the basis of conda. As a minor
complaint, I do not like the documentation generator conda is using. I prefer
the entire documentation or a sufficiently long section to be contained in a
single web page such that I can go back and forth easily. The conda
documentation has many separate pages, making navigation quite difficult.
Ok, maybe it is just me.

Due to these problems, I suspect it *might* not be easy for every tool
developer to contribute to conda. Bioconda is currently maintained by several
experienced developers, but we need more to push it further. Automation is the
key, in my opinion.

### My thoughts

I have tried a few package management systems such as [Linuxbrew][lbrew] and
[Gentoo Prefix][gentoo], and checked [Guix][guix]. Conda is the closest to the
system in my mind and in serveral ways better. It is promising. I really hope
it can be a success.

[conda]: http://conda.pydata.org/docs/
[biocd]: https://bioconda.github.io
[rpath]: https://en.wikipedia.org/wiki/Rpath
[lbrew]: https://github.com/Homebrew/linuxbrew
[gentoo]: https://wiki.gentoo.org/wiki/Project:Prefix
[guix]: http://www.gnu.org/software/guix/

### Appendix: my summary of Loman's survey

Nick Loman conducted a [survey][sur], where part of the questions are about
the difficulties in data analysis and running software. The answers are in
free text. I have read through all of them and classified them into several
categories. In the end, I [collected][rst] 233 non-duplicate replies that have
answers to the related questions. The leading difficulty is installation
problems (132/233; category 3), followed by insufficient computing resources
(88/257; 7 and 8), lack of interoperability (72/233; category 5) and bad
documentations (69/233; category 6). Not surprisingly, software installation
is less an issue to skilled researchers (27/62 for skill level 8 or above),
but 44% is still a large percentage and the fraction of junior
bioinformaticians is probably larger in the community that has not
participated the survey.

The software installation problem is real.

[sur]: http://figshare.com/articles/Bioinformatics_infrastructure_and_training_summary/1572287
[rst]: https://gist.github.com/lh3/f49eb49168ce8b841958
