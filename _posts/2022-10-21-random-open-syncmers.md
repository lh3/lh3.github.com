---
layout: post
mathjax: true
title: "Random open syncmers"
description: ""
category: 
tags: []
---
{% include JB/setup %}

A $k$-long sequence $P$ is a ($k$,$s$)-open-[syncmer][syncmer], $s\le k$, if
$P[1,s]$ is the smallest among all $s$-mers in $P$. Suppose function $\phi$ is
a bijective hash function of $k$-long sequences. $P$ is a random
($k$,$s$)-syncmer if $\phi(P)$ is an open syncmer. Because we often map
$k$-mers to integers, $\phi$ can take the form of an [invertible integer hash
function][invhash]. In practice, $\phi$ does not have to be a bijection. It can
also map a sequence to an integer of a different length or even operate in the
bit space (see the [miniprot preprint][mp-pre]).

As overlapping $k$-mers have dependency, the definition of the original open
syncmer often involves one more parameter to improve its quality. Original open
syncmers also do not work well with protein sequences with varying amino acid
frequency. Using a good hash function, random open syncmers do not have these
problems.

I implemented random open syncmers [in minimap2][mm2-syncmer]. In comparison to
random minimizers of the same density, syncmers lead to better chaining scores
but are more repetitive. This is partly because ($k$,$w$)-minimizers are
generating $k$-mers from a $k+w-1$ window and to some extent, using slightly
longer $k$-mers in effect. Due to the repetitiveness, syncmers slow down
minimap2 chaining a lot, similar to the observation made by [Shaw and Yu
(2022)][sy22]. I tried a few different syncmer configurations and found
minimzers and syncmers are comparable overall. In practical implementation, it
probably does not matter what strategy to use. Nonetheless, in theoretical
analysis, random open syncmers are the better choice as they are largely
independent of each other under a good hash function.

[sy22]: https://academic.oup.com/bioinformatics/article/38/20/4659/6432031
[invhash]: https://gist.github.com/lh3/974ced188be2f90422cc
[syncmer]: https://peerj.com/articles/10805/
[mp-pre]: https://arxiv.org/abs/2210.08052
[mm2-syncmer]: https://github.com/lh3/minimap2/blob/c2f07ff2ac8bdc5c6768e63191e614ea9012bd5d/sketch.c#L145-L192
