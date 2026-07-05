---
layout: post
title: "Minibwa is the new bwa-mem"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Preprinted [in 2013][bwa-mem], [bwa-mem][bwa] is a popular mapper for 100+ bp short reads.
It is often the first choice in variant calling pipelines for human data.
However, bwa-mem has increasingly become the performance and the cost bottleneck when the upstream/downstream get optimized.
This led to multiple forks, including [bwa-mem2][bwa-mem2], [bwa-meme][bwa-meme], [Sentieon bwa][sentieon] and [Parabricks bwa][parabrick],
which produce identical or near identical output to the original bwa-mem.

These forks are limited by the outdated design of bwa-mem unfortunately.
Notably, although bwa-mem chains seeds, its chaining algorithm is inferior to the more recent minimap2 algorithm.
Bwa-mem also extends individual seeds instead of patching gaps between seeds like minimap2.
This impairs alignment through long gaps, a problem further exacerbated by the lack of a dual affine-gap penalty.
Another hidden cost behind bwa-mem forks is the increased code complexity when they try to match the bwa-mem algorithms exactly.
For example, bwa-mem2 doubles the lines of code of bwa-mem; bwa-meme further doubles bwa-mem2.
The much larger code base reduces the maintainability in the long term.
**These limitations can only be addressed by revamping the design of bwa-mem.**

[Minibwa][mb] is a fresh start without the baggage of backward compatibility.
Combining the advantages of bwa-mem and minimap2, it works for long reads and finds much more long gaps in >500bp SBX reads than bwa-mem.
With better engineering and parameter tuning, minibwa is also a few times as fast as bwa-mem.
Aligning 30X human reads in 50 minutes over 32 CPU threads, it might outpace other upstream/downstream tools if you are not careful.
In terms of mapping accuracy, minibwa competes with bwa-mem on simulated read mapping and real-world variant calling.
Andrew Carroll has kindly [confirmed][ac-eval] the performance of minibwa independently on multiple non-human datasets.

**Minibwa is the new bwa-mem.**
I am still committed to fixing critical bugs in the original bwa-mem but future development will focus on minibwa only.
If you can accept occasional differences in alignment, I encourage you to try minibwa.
It natively supports bisulfite sequencing data, is much faster and more sensitive to long gaps which will become common with upcoming SBX reads.
Regarding future development, minibwa is currently fast enough that further performance optimizations would yield diminishing returns for overall pipelines.
The next major change will be the support of alternate contigs, the only bwa-mem feature missing from minibwa.

[mb]: https://github.com/lh3/minibwa
[bwa-mem]: https://arxiv.org/abs/1303.3997
[bwa-mem2]: https://github.com/bwa-mem2/bwa-mem2
[bm2-paper]: https://ieeexplore.ieee.org/abstract/document/8820962
[bwa]: https://github.com/lh3/bwa
[aws]: https://aws.amazon.com/healthomics/pricing/
[bwa-meme]: https://github.com/kaist-ina/BWA-MEME
[meme-paper]: academic.oup.com/bioinformatics/article/38/9/2404/6543607
[sentieon]: https://support.sentieon.com/docs/usages/bwa/bwa/
[parabrick]: https://docs.nvidia.com/clara/parabricks/latest/documentation/tooldocs/man_fq2bam.html
[dragen]: https://developer.illumina.com/dragen
[bwa-long]: https://lh3.github.io/2014/12/10/bwa-mem-for-long-error-prone-reads
[mm]: https://github.com/lh3/minimap
[mm2]: https://github.com/lh3/minimap2
[blog-2018]: https://lh3.github.io/2018/04/02/minimap2-and-the-future-of-bwa
[ac-eval]: https://andrewcarroll.github.io/2026/06/30/the-best-of-both-worlds-assessing-minibwa.html
