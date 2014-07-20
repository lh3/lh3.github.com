---
layout: post
title: "Alternatives to PSMC"
description: ""
category: 
tags: []
---
{% include JB/setup %}

[PSMC][psmc] is my program to infer the historical effective population size
from a diploid genome. It was [published in Nature][nature] three years ago and
has been cited over 100 times so far. Whenever I see a PSMC plot in a paper, I
feel a moment of joy both as a scientist and as a programmer.

PSMC is okay, but now there are better models and implementations at least in
theory. [MSMC][msmc], which has recently [published in Nature Genetics][msmc2],
not only extends PSMC to multiple haplotypes, but also improves PSMC
for a diploid genome. It precalculates transition matrices over long runs of
homozygosity and becomes fast enough to perform whole-genome inference without
binning the input like PSMC. More importantly, for a diploid genome, MSMC
implements the PSMC' model. It is a better approximation to the
coalescent-with-recombination model by allowing non-effective recombinations.
It is able to give a much better estimate of the recombination rate. I was lazy
when I was working on PSMC. I knew PSMC' is better, but I skipped that because
its derivation is more complicated and because PSMC worked well to infer other
parameters.

Another important tool is [dical][dical] by Kelly Harris et al. It also uses
better model and has a [time complexity linear in the number of
states][dical2]. This is a significant advantage over the PSMC implementation
whose time complexity is quadratic in the number of states. Dical runs much
faster.

You can still use PSMC if you prefer. It comes with a few useful scripts and is
fast enough for most applications. Just beware that there may be better ones
when PSMC is not for you.

[psmc]: https://github.com/lh3/psmc
[nature]: http://www.nature.com/nature/journal/v475/n7357/full/nature10231.html
[dical]: http://sourceforge.net/projects/dical/
[dical2]: http://arxiv.org/abs/1403.0858
[msmc]: https://github.com/stschiff/msmc
[msmc2]: http://www.nature.com/ng/journal/vaop/ncurrent/full/ng.3015.html
