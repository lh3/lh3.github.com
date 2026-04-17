---
layout: post
title: "The AI Rewrite Dilemma"
description: ""
category: 
tags: []
---
{% include JB/setup %}

There have been multiple ongoing efforts (see Appendix below), and **debates**,
regarding porting bioinformatics tools to Rust. These include two rewrites of
minimap2. Here are some of my current reflections, which may change in the
future.

**From a** ***purely technical*** **standpoint, I believe AI rewrites,
especially in Rust, benefit the field.** Rust will not only speed up
bioinformatics tools in Python and R often by an order of magnitude, but will
also greatly simplify deployment.  With Rust (and a few other modern
languages), non-standard dependencies are compiled into the final binary. This
eliminates the need to install dependencies at runtime, freeing us from
[dependency hell][dep-hell]. Bioinformatics would be a more enjoyable field if
more tools were written in Rust.

Rust is notorious for its steep learning curve. It has historically been
unrealistic to ask biologists to develop tools in Rust. However, **coding
agents such as [Claude Code][claude-code] and [Codex][codex] have transformed
code translation.** Since last December, a developer familiar with these
agents could port a small tool in hours or one of medium complexity (e.g.
minimap2) in days, probably tens of times faster than porting by hand. This is
happening.

***Legally speaking,*** **permission for rewriting is governed by the software
license.** Maintainers of *open-source* projects can't stop others from
rewriting their work, no matter whether they like the action or not. One
defensive strategy is to fight fire with fire: preemptively rewrite your own
tool in Rust by yourself such that another Rust rewrite by a third party would
be of little added value. I don't necessarily like the idea but I could not
think of a more effective solution from the maintainer perspective.

For tool rewriters, please be aware that **not all maintainers are comfortable
with AI rewrites of their tools.** The long-term maintenance is also the
elephant in the room that needs to be addressed adequately. I haven't rewritten
any tools. Without direct experience, I am not in a position to make
suggestions. I just hope the bioinformatics community could collaboratively
come up with guidelines and common practices on tool rewriting to advance this
field in a friendly manner. After all, there are humans behind the code.
**Empathy matters more than technical benefit or legal correctness.**

Finally, **I always welcome forks and rewrites of my tools** whether I am involved
or not. If you want to rewrite my tools with AI, please feel free to do so.

[dep-hell]: https://en.wikipedia.org/wiki/Dependency_hell
[claude-code]: https://code.claude.com/docs/en/overview
[codex]: https://chatgpt.com/codex/

### Appendix: some ongoing rewrites as of 2026-04-17

* [pachterlab/edgePython][edgepython] in Python: based on [edgeR][edger] in R, explained in [this blog post][edgepy-blog]
* [seqeralabs/RustQC][RustQC] in Rust: based on multiple tools in C, R and Python, explained in [this blog post][rustqc-blog]
* [fulcrumgenomics/fgumi][fgumi] in Rust: based on part of [fgbio][fgbio] in Scala, explained in [this blog post][fgumi-blog]
* [Huang-lab/fastVEP][fastvep] in Rust: based on [VEP][vep] in Perl and [Nirvana][nirvana] in C# (unmaintained). Preprint [at bioRxiv][fastVEP-pp]
* [COMBINE-lab/sshash-rs][sshash-rs] in Rust: based on [sshash][sshash] in C++
* [huangnengCSU/rust-fastqtl][rust-fastqtl] in Rust: based on [fastqtl][fastqtl] in C++ (unmaintained)
* [Huang-lab/RastQC][rastqc] in Rust: drop-in replacement of [FastQC][fastqc] in Java
* [henriksson-lab][henriksson-lab]: multiple projects including minimap2 in C,
  BLAST in C++, HMMER in C, MUSCLE in C++, DIAMOND in C++, SKESA in C++, Kraken2 in C++, FastQC in Java, Prokka in Perl
  and more including non-bioinformatics tools and libraries.

[edgepython]: https://github.com/pachterlab/edgePython
[edger]: https://bioconductor.org/packages/release/bioc/html/edgeR.html
[fastvep]: https://github.com/Huang-lab/fastVEP
[vep]: https://github.com/Ensembl/ensembl-vep
[nirvana]: https://github.com/Illumina/Nirvana
[rastqc]: https://github.com/Huang-lab/RastQC
[fastqc]: https://github.com/s-andrews/fastqc
[rustqc]: https://github.com/seqeralabs/RustQC/
[fgumi]: https://github.com/fulcrumgenomics/fgumi
[fgbio]: https://github.com/fulcrumgenomics/fgbio
[rust-fastqtl]: https://github.com/huangnengCSU/rust-fastqtl
[fastqtl]: https://github.com/francois-a/fastqtl
[sshash-rs]: https://github.com/COMBINE-lab/sshash-rs
[sshash]: https://github.com/jermp/sshash
[fastVEP-pp]: https://www.biorxiv.org/content/10.64898/2026.04.14.718452v1
[henriksson-lab]: https://github.com/orgs/henriksson-lab/repositories
[fgumi-blog]: https://blog.fulcrumgenomics.com/p/introducing-fgumi
[edgepy-blog]: https://liorpachter.wordpress.com/2026/02/19/the-quickening/
[rustqc-blog]: https://seqera.io/blog/rustqc/
