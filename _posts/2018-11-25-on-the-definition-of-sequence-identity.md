---
layout: post
title: "On the definition of sequence identity"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Sequence identity measures the similarity between two sequences. For sequencing
data, it is often thought as the opposite of sequencing error rate. When we say
"the sequence divergence between two species is ABC" or "the sequencing error
rate is XYZ", we assume everyone knows how to compute identity. In fact, there
are more than one ways to compute identity. This blog post discusses a few
definitions and how [minimap2][mm2] implements it.

I will start with the following example: what is the identity between the two
sequences?
```
CCAGTGTGGCCGATACCCCAGGTTGGCACGCATCGTTGCCTTGGTAAGC
CCAGTGTGGCCGATGCCCGTGCTACGCATCGTTGCCTTGGTAAGC
```
The classical way to find identity is to perform alignment first. If match=1,
mismatch=-2, gapOpen=-2 and gapExt=-1, we get the following alignment:
```
Ref+:  1 CCAGTGTGGCCGATaCCCcagGTtgGCTACGCATC-TTGCCTTGGTAAGC 49
         |||||||||||||| |||   ||  |||||||||| ||||||||||||||
Qry+:  1 CCAGTGTGGCCGATgCCC---GT--GCTACGCATCgTTGCCTTGGTAAGC 45
```
Here we have 43 matching bases, 1 mismatch, 5 deletions and 1 insertion to the
first/Ref sequence. The CIGAR is `18M3D2M2D2M1I22M`.

### Gap-excluded identity

With this definition, we exclude all gapped columns from the alignment. The
identity equals "#matches / (#matches + #mismatches)". In the example able, the
gap-excluded identity is 43/(43+1)=97.7%.

An obvious problem with this definition is that it doesn't count gaps. However,
it is an often used definition. We may hear that the chimpanzee and human
genome differ by a couple of percent. Here we are referring to such
gap-excluded identity. The exact sentence in the [first chimpanzee genome
paper][chimp-paper] is "Single-nucleotide substitutions occur at a mean rate of
1.23% between copies of the human and chimpanzee genome".

### BLAST identity

BLAST identity is defined as the number of matching bases over the number of
alignment columns. In this example, there are 50 columns, so the identity is
43/50=86%. In a SAM file, the number of columns can be calculated by summing
over the lengths of M/I/D CIGAR operators. The number of matching bases equals
the column length minus the NM tag. Here is a Perl one-liner to calculate
BLAST identity:
```
perl -ane 'if(/NM:i:(\d+)/){$n=$1;$l=0;$l+=$1 while/(\d+)[MID]/g;print(($l-$n)/$l,"\n")}'
```
where variable `$n` is the sum of mismatches and gaps and `$l` is the alignment
length. In the [PAF][paf] format, column 10 divived by column 11 gives the
BLAST identity.

BLAST identity is perhaps the most common definition, but it should be used
with caution when we filter alignments by identity. Suppose we are aligning
1000bp query sequence that has a ~300bp ALU insertion in the middle. The
alignment will have a BLAST identity around 70% and is likely to get filtered
out. In evolution, an ALT insertion is created by one event. It should not be
counted as 300 independent differences.

### Gap-compressed identity

At least for filtering, a better definition of sequence identity is the
following: we count consecutive gaps as one difference. By compressing gaps in
the example above, we are effectively dealing with this alignment:
```
Ref+:  CCAGTGTGGCCGATaCCCcGTtGCTACGCATC-TTGCCTTGGTAAGC
       |||||||||||||| ||| || |||||||||| ||||||||||||||
Qry+:  CCAGTGTGGCCGATgCCC-GT-GCTACGCATCgTTGCCTTGGTAAGC
```
The identity is 43/(50-2-1)=91.5%. I have been using this definition for
various tasks. The latest minimap2 at github outputs such identity at a new
`ed:f` tag. There is a Perl one-liner for this as well:
```
perl -ane 'if(/NM:i:(\d+)/){$n=$1;$m=$g=$o=0;$m+=$1 while/(\d+)M/g;$g+=$1,++$o while/(\d+)[ID]/g;print(1-($n-$g+$o)/($m+$o),"\n")}'
```
where `$m` is the sum of `M` operations, `$g` the sum of `I` and `D` operations
and `$o` the number of gap opens.

### Effect of scoring

Scoring affects alignment and thus sequence identity. For the same pair of
sequences, if we change gapOpen=-4, we end up with a different alignment
```
Ref+:  1 CCAGTGTGGCCGATaCCCcaggtTGgcACGCATCGTTGCCTTGGTAAGC 49
         |||||||||||||| |||     ||  ||||||||||||||||||||||
Qry+:  1 CCAGTGTGGCCGATgCCC----gTGctACGCATCGTTGCCTTGGTAAGC 45
```
The BLAST identity is 83.7% and the gap-compressed identity is 89.1%. **Even if
we stick with one definition, the identity can be different if we change the
scoring.**

## Concluding remarks

The estimate of sequence identity varies with definitions and alignment
scoring. When you see someone talking about "sequencing error rate" next time, 
ask about the definition and scoring in use to make sure that is the error rate
you intend to compare. If you want to estimate error rate or identity on your
own, try the following command line
```
minimap2 -c ref.fa query.fa \
  | perl -ane 'if(/tp:A:P/&&/NM:i:(\d+)/){$n+=$1;$m+=$1 while/(\d+)M/g;$g+=$1,++$o while/(\d+)[ID]/g}END{print(($n-$g+$o)/($m+$o),"\n")}'
```
Remember that for fair comparisons, use the same scoring.

[mm2]: https://github.com/lh3/minimap2
[chimp-paper]: https://www.nature.com/articles/nature04072
[paf]: https://github.com/lh3/miniasm/blob/master/PAF.md
