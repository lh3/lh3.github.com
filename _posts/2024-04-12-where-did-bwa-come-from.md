---
layout: post
title: "Where did BWA come from?"
description: ""
category: 
tags: []
---
{% include JB/setup %}

My blog posts are often motivated by casual conversations with other
scientists. This blog post is an example. Someone asked me how I thought of
[BWA][bwa] in the first place. The short answer is: I didn't; I learned from
[Tak-Wah Lam][tklam].

Here is the longer story. On a 1000G conference call in April 2008, as I
remember, Tak-Wah gave a short talk on a prototype of [SOAP2][soap2] and showed
that it was faster than [MAQ][maq] and SOAP by over an order of magnitude.
Honestly, I thought it was too good to be real (sorry, Tak-Wah!). Tak-Wah
mentioned BWT but did not describe the algorithm. I was curious about how that
works and found his [BWT-SW paper][bwt-sw] published earlier that year.
I learned the basic of BWT/FM-index from the BWT-SW paper. FM-index only gives
exact matches. I came up with the backtracking idea independently to allow
mismatches and gaps.

I started to implement BWA in late May 2008. Realizing efficient BWT
construction is challenging, I adapted the BWT-SW implementation which remains
in BWA. It took me another 2-3 months to get a decent prototype working for
single-end reads. That version was over ten times faster than MAQ &mdash; Tak-Wah
was correct of course. After I [released samtools][sam], I came back to BWA. I
added the support of paired-end reads, made BWA the first aligner to output
SAM and published it in mid 2009. SOAP2 was actually published one month later
than BWA, but without SOAP2, there would be no BWA.

[bwa]: https://pubmed.ncbi.nlm.nih.gov/19451168/
[tklam]: https://www.cs.hku.hk/index.php/people/academic-staff/twlam
[soap2]: https://pubmed.ncbi.nlm.nih.gov/19497933/
[maq]: https://genome.cshlp.org/content/18/11/1851.full.html
[bwt-sw]: https://pubmed.ncbi.nlm.nih.gov/18227115/
[sam]: http://lh3.github.io/2015/01/27/the-early-history-of-the-sambam-format
[bowtie]: https://pubmed.ncbi.nlm.nih.gov/19261174/
