---
layout: post
title: "Short RNA seq read alignment with minimap2"
description: ""
category: 
tags: []
---
{% include JB/setup %}

***TL;DR:*** *the new preset `splice:sr` in minimap2 can align short RNA-seq reads.
It is similar to STAR in resource usage,
approaches STAR in junction accuracy,
and is overall better at SNP calling.*

[Minimap2][mm2] can align short or long genomic reads and long RNA-seq reads.
Short RNA-seq read alignment is a notable omission.
This is changed with the latest [v2.29][v2.29] release
&#x2014; minimap2 now has a new `splice:sr` preset for aligning short RNA-seq reads.
Learning from [STAR][star], minimap2 can optionally take known gene annotation
in the [12-column BED format][bed12] to align junctions close to the ends of reads.
It also supports 2-pass alignment.
Here are a few example command lines:
```
# 1-pass alignment
minimap2 -ax splice:sr -t8 ref.fa r1.fq r2.fq | samtools sort -@4 -m4g -o srt.bam -

# 1-pass with known annotation
paftools.js gff2bed gencode.gtf.gz > gencode.bed  # if not done already
minimap2 -ax splice:sr -j gencode.bed ref.fa r1.fq.gz r2.fq.gz

# 2-pass
minimap2 -ax splice:sr -j gencode.bed --write-junc ref.fa r1.fq r2.fq > junc.bed
minimap2 -ax splice:sr -j gencode.bed --pass1=junc.bed ref.fa r1.fq r2.fq > out.sam
```

To understand the accuracy of minimap2, I downloaded 107 million 2&#215;150bp
HG002/GM24385 reads [from GIAB][giab-read], aligned them with STAR and minimap2
and called SNPs with [minipileup][miniplp].
I chose this sample because GIAB provides highly [accurate SNP calls][q100].
Although we don't know the true mapping of each read, **we can use SNP calls as a proxy
to evaluate the mapping accuracy of aligners**.

The following table shows the main results.
GenCode v47 comprehensive chromosomal annotation (gc47) was used during alignment.
The minimap2 command lines are the same as above except input and output.
For STAR v2.7.11b, I applied basic command lines as well as a more complex setting
[from the SMaHT consortium][smaht-aln] (credit to Michele Berselli).
I used [sambamba][sambamba] to mark potential duplicates for SNP calling.
A-to-G or T-to-C SNP calls are ignored as many of them are caused by RNA editing.

|Aligner |Setting               |#junc|#good|%good|SNP TP|  FP|%FDR |Time(s)|
|:-------|:---------------------|----:|----:|----:|----:|----:|----:|------:|
|STAR    |1pass                 |19.8m|19.4m|97.9%|41855|16707|28.5%|  ~1112|
|        |1pass+gc47            |23.9m|23.6m|98.4%|41864|5179 |11.0%|  ~1136|
|        |2pass+gc47            |24.1m|23.5m|97.6%|41850|5133 |10.9%|  ~2304|
|        |[SMaHT][smaht-aln]    |25.3m|24.6m|97.3%|43362|13706|24.0%|   3832|
|        |[GIAB (old)][giab-aln]|25.4m|24.6m|96.7%|42524|2802 | 6.2%|    N/A|
|mm2     |1pass                 |20.9m|20.3m|97.1%|41176|778  | 1.9%|   1555|
|        |1pass+gc47            |25.3m|24.7m|97.6%|41184|709  | 1.7%|   1554|
|        |2pass+gc47            |25.5m|24.7m|97.0%|41187|709  | 1.7%|   2983|

In the table, `#junc` is the number of junctions in reads after deduplication
and `#good` is the number of junctions found in GenCode.
Minimap2 and STAR reported similar number of junctions.
STAR found a higher percentage of junctions in GenCode.
In terms of run time, STAR is faster in the basic mode
but slower with the SMaHT setting.
STAR uses 49GB at the peak, while minimap2 tops at 20GB.
**Minimap2 and STAR are broadly comparable in resource usage and junction accuracy.**

On SNP calling where the ground truth is known,
minimap2 achieves much higher precision (i.e. 1-FDR) at a minor loss in sensitivity.
Most STAR SNPs missed by minimap2 are supported by <10 reads.
They are often visible in the minimap2 alignment but
some of their supporting reads have mapping quality <20 and are filtered out during minipileup SNP calling.
With RNA-seq, the majority of transcriptomic SNPs are expected to be missed due to uneven gene expression
or ignored due to RNA editing.
A small loss in sensitivity is a minor issue, in my opinion.

I inspected ~20 false positive SNPs from the STAR alignment.
In more than half of the cases, reads are mismapped to wrong locations
but are still given a mapping quality 255 by STAR.
Mismapped reads tend to have more mismatches.
They are often not mapped in the basic setting
but are mapped in the SMaHT setting that is more permissive.
This is why the basic setting achieves higher precision.
While minimap2 may also mismap some of these reads,
it is aware of the mapping difficulty and usually gives them low mapping quality.
Besides mismapping, a large fraction of STAR false positives seem to be caused
by misalignment towards the ends of reads.
Minimap2 clips trailing mismatches and is less affected.
In addition, STAR places a gap at the leftmost position with respect to the read instead of the reference.
As a result, if the read is mapped to the reverse strand, the gap is placed at the rightmost position on the reference.
This would confuse pileup-based SNP callers without a realignment or local assembly step.
In the table, the "GIAB (old)" alignment was generated with STAR v2.6.0a using different gene annotation.
I don't know why the SNP calling result is better on this alignment.
Overall, the SNP analysis suggests **minimap2 is more careful about read mapping**.

**Minimap2 doesn't aim to match all STAR features.**
It doesn't output counts or transcriptome alignment and doesn't have the allele-specific improvement.
These need to be done with downstream tools.
I have no plan to implement builtin support of scRNA-seq as this seems too application specific and may change over time.
Minimap2 by default outputs chimeric alignment and improperly paired reads
which can be used for calling structural variants or gene fusions,
but it is not optimized for back splicing in circular RNA.

STAR is the de facto standard for short RNA-seq read alignment against a reference genome.
Realistically, I don't expect STAR users to switch to minimap2.
Nonetheless, when STAR doesn't meet your need sometimes, minimap2 could be a possible alternative.
Let me know on GitHub if you have issues or questions.

[mm2]: https://github.com/lh3/minimap2
[v2.29]: https://github.com/lh3/minimap2/releases/tag/v2.29
[star]: https://github.com/alexdobin/STAR
[bed12]: https://genome.ucsc.edu/FAQ/FAQformat.html#format1
[miniplp]: https://github.com/lh3/minipileup
[giab-read]: https://ftp-trace.ncbi.nlm.nih.gov/giab/ftp/data_RNAseq/AshkenazimTrio/HG002_NA24385_son/Google_Illumina/mRNA/reads/
[smaht-aln]: https://smaht-dac.github.io/pipelines-docs/DOCS/ANALYSIS/Short-Read_RNA-seq_Paired-End/1_Alignment.html
[giab-aln]: https://ftp-trace.ncbi.nlm.nih.gov/giab/ftp/data_RNAseq/AshkenazimTrio/HG002_NA24385_son/Google_Illumina/mRNA/alignments/
[q100]: https://ftp-trace.ncbi.nlm.nih.gov/giab/ftp/data/AshkenazimTrio/analysis/NIST_HG002_DraftBenchmark_defrabbV0.019-20241113/
[sambamba]: https://github.com/biod/sambamba
