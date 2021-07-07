---
layout: post
title: "Remapping an aligned BAM"
description: ""
category: 
tags: []
---
{% include JB/setup %}

This is a short post on how to remap short reads in an aligned BAM using
bwa-mem. My recommendation is (requiring bash)
```
samtools collate -Oun128 in.bam | samtools fastq -OT RG,BC - \
  | bwa mem -pt8 -CH <(samtools view -H in.bam|grep ^@RG) ref.fa - \
  | samtools sort -@4 -m4g -o out.bam -
```
Here, `samtools collate` groups the two reads in a read pair and outputs an
uncompressed BAM stream. `samtools fastq` consumes this stream and generates an
interleaved FASTQ. Option `-T RG,BC` copies RG and BC tags in the input BAM to
the output FASTQ comment lines. `bwa mem -C` then copies these tags to the
output SAM. Option `-H <(...)` inserts header `@RG` lines. Option `-p`
processes an interleaved FASTQ stream. Finally, `samtools sort` generates
sorted BAM. I often use `-@4 -m4g` for faster sorting. If you have unaligned
BAM (aka uBAM), you can skip the first `collate` step.

I wrote about design command-line interface a couple of days ago. This posts
exemplifies the power of a proper design: you can chain multiple tools together
to achieve high performance without writing any high-performance code.
