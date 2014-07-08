---
layout: post
title: "Abreak: evaluating de novo assemblies"
description: ""
category: 
tags: []
---
{% include JB/setup %}

"Abreak" is a subcommand of [htscmd][htslib] on [the lite branch][lite]. It
takes an assembly-to-reference alignment as input and counts the number of
alignment break points. An earlier version was used in [my fermi paper][fermi]
to measure the missassembly rate of human de novo assemblies. A typical output
looks like:
```
Number of unmapped contigs: 239
Total length of unmapped contigs: 54588
Number of alignments dropped due to excessive overlaps: 0
Mapped contig bases: 2933399461
Mapped N50: 6241
Number of break points: 102146
Number of Q10 break points longer than (0,100,200,500)bp: (28719,7206,4644,3222)
Number of break points after patching gaps short than 500bp: 94298
Number of Q10 break points longer than (0,100,200,500)bp after gap patching: (23326,5320,3369,2194)
```
Here it gives the mapped contig bases, mapped N50 and number of break points
with flanking sequences longer than 0, 100, 200 and 500bp.

Although [GAGE-B][gageb] and [QUAST][quast] are more powerful, the use of
MUMmer limits them to small genomes only. In contrast, "abreak" works with any
aligners supporting chimeric alignment. When BWA-SW or BWA-MEM is used to map
contigs, "abreak" can easily and efficiently work with mammal-sized assemblies.

[htslib]: https://github.com/samtools/htslib
[lite]: https://github.com/samtools/htslib/tree/lite
[fermi]: http://bioinformatics.oxfordjournals.org/content/28/14/1838
[quast]: http://bioinf.spbau.ru/en/quast
[gageb]: http://ccb.jhu.edu/gage_b/

