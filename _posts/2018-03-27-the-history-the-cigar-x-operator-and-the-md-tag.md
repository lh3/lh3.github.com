---
layout: post
title: "The history the MD tag and the CIGAR X operator"
description: ""
category: 
tags: []
---
{% include JB/setup %}

In the SAM format, the "X" and "=" CIGAR operators were not part of the
original spec. Nonetheless, they were among the first several features added
after the initial release of the spec. I was resistant to this feature for
several reasons. First, CIGAR describes alignment, but sequence matches and
mismatches are not indispensable properties of alignment.  Second, for this
reason, most older alignment formats did not distinguish sequence matches and
mismatches, either. When we convert from other formats to SAM, it is
non-trivial to generate "X" and "=". Third, an "X" does not tell us the
mismatching base. Its application is limited in practice. In the end, I still
added "X" and "=" to the spec in response to the request of several important
users. However, I have to say I regret the decision.

There was also a fourth reason: before X/=, there was the "MD" tag, which
encodes mismatching bases in addition to positions. "MD" was in the original
spec. The motivation was to reconstruct the reference subsequence in the
alignment. I learned the idea from a variant of the Eland format used
internally at Illumina. However, because at that time Eland didn't do gapped
alignment, the Illumina version of MD was unable to encode gaps, so I
introduced "^", representing deleted sequences from the reference.

Something unexpected happened down the road, though. Without "^", we could
represent adjacent mismatches simply with two letters like "AC". With "^",
there was an ambiguity like "^AC" - is it 2bp deletion, or 1bp deletion
followed by a mismatch? To resolve this issue, we changed MD to require a zero
before each mismatch like "^A0C". It was an oversight.

There is a bigger problem with "MD": it is too complicated to use. We have to
keep track of MD, CIGAR and query string at the same time to generate the
reference string. I thought to use it a few times, but was stopped by the
complexity. I have never used this tag until very recently, with a lot of
efforts.

This is why in minimap2, I came up with a new custom tag "[cs][cs]". It encodes
CIGAR and both query and target sequence differences, such that we can parse
all information from one string. It greatly simplifies code. "cs" is also
critical to the PAF format that doesn't store query sequences. I firmly
believe "cs" is "MD" done right.

In reality, though, something better is not necessarily more popular. "cs" came
too late. I even don't know if it will become a standard tag in SAM. Minimap2
will keep using "cs" anyway as PAF is an important part of minimap2.

[cs]: https://github.com/lh3/minimap2#cs
