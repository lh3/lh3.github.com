---
layout: post
title: "Why is bwa-aln used for ancient DNA reads?"
description: ""
category: 
tags: []
---
{% include JB/setup %}

[BWA][bwa] implements three algorithms: bwa-aln, bwa-sw and bwa-mem.
Although bwa-aln is the earliest, it remains a popular choice for ancient DNA (aDNA) data.
Is it because researchers working on aDNA are too slow to change? Not quite.
The following plot from [Dolenz et al (2024)][Dolenz] shows the mismatch rate as a function of read length for reads
mapped with mapping quality (mapQ) 25 or higher.

<img src="https://i.ibb.co/Jv4BQYD/Screenshot-2024-09-28-at-20-26-09.png" width="100%"/>

This plot suggests for reads shorter than 60bp,
bwa-aln is more sensitive to mutations or sequencing/deamination errors than bwa-mem and bowtie2.
aDNA reads are short.
The higher sensitivity at the short end will help to alleviate reference biases and improve variant calling.
This is why bwa-aln is still used for aDNA data.

**The observation has a direct algorithmic explanation**.
Suppose the true alignment of a 32bp read has one mismatch at position 12 and another mismatch at position 22 on the read.
With the default setting, bwa-mem will miss this alignment as it requires at least a 19-mer exact match.
In theory, we can find the true alignment if we reduce the minimum seed length to 11.
However, each 11-mer occurs 1430 times ($=2\times3\times10^9/4^11$) on average in the human genome.
Bwa-mem will become impractically slow if it extends each occurrence to find the best hit.
Bwa-aln on the other hand does not use exact seeds and can guarantee to find the true alignment.
It is better for short reads.

When reads get longer, it will be rare to see a true alignment without 19-mer exact matches.
Bwa-mem catches up for 75-100bp reads.
It exceeds bwa-aln for longer reads &mdash; this is how bwa-mem overtakes bwa-aln for modern 150bp reads.
**In practice, it is probably better to align shorter reads with bwa-aln and longer reads with bwa-mem**.

To explain the bowtie2 curve, we first need to understand why we use mapQ25 as a cutoff.
Suppose a read has a single best hit with $n$ mismatches and bwa-aln is tuned to find hits with up to $m$ mismatches.
Bwa-aln outputs
1) 37 if $`n<m`$ and there are no hits with $n+1$ mismatches,
2) $`\max\{0,23-10\log_{10}N\}`$ if $`n<m`$ and there are $N$ hits with $n+1$ mismatches, or
3) 25 if $`n=m`$.
This is where mapQ25 comes from.
It **overfits the bwa-aln heuristic**.

Notably, when $`n<m`$, the bwa-aln mapQ is independent of $n$.
A single real SNP on the read does not cause reference bias if $`n<m`$.
Bowtie2 is different.
As is explained in [this blog post][bowtie2-mapq], the bowtie2 mapQ estimator strongly depends on $n$.
A read mapped with more mismatches has lower mapQ.
Furthermore, bowtie2 has internal thresholds of 23 and 24, narrowly below mapQ25.
**The bowtie2 curve will probably look better with a lower mapQ threshold**.
Nonetheless, because bowtie2 uses 20-mer seeds in the very sensitive mode,
it has the same problem as bwa-mem and won't work well for 30bp reads anyway.

Bwa-aln is one of the oldest short-read aligners.
**Is it still implementing the best algorithm for aDNA data?**
I don't think so.
There have been several capable aligners that can find all hits like bwa-aln and in theory can be tuned to the bwa-aln behavior.
Although they tend to be optimized for 100bp reads,
some of them might perform better than bwa-aln for short aDNA reads.
In addition, a few papers have shown integrating known variants into the reference genome reduces reference biases,
but the implementations are rarely used.
Well, aDNA read alignment is not a solved problem.

[bwa]: https://github.com/lh3/bwa
[Dolenz]: https://academic.oup.com/bioinformatics/article/40/7/btae436/7705522
[bowtie2-mapq]: http://biofinysics.blogspot.com/2014/05/how-does-bowtie2-assign-mapq-scores.html
