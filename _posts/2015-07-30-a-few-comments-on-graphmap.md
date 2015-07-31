---
layout: post
title: "A few comments on GraphMap"
description: ""
category: 
tags: []
---
{% include JB/setup %}

[GraphMap][gm] is a new long-read mapper initially tuned for error-prone ONT
reads. There are quite a few interesting points methodologically. The following
two comments are mostly about techical and practical aspects. Before you read
the comments, please bear in mind that I am the developer of BWA-MEM. I could
be biased.

### Comment 1: consensus quality

The most striking point I found in the [preprint][pp] (and earlier on their
website) is that GraphMap significantly outperforms [LAST][last] on consensus
calling for lambda phage, a tiny genome for which mapping (finding the
approximate position of a read) should hardly be a problem. I do not believe
GraphMap is better on mapping. Then there are two remaining differences:
GraphMap does semi-global alignment while LAST does local alignment; 2)
GraphMap uses an edit-distance based scoring system match=1, mismatch=-1,
gapOpen=0 and gapExt=-1, while LAST uses gapOpen=-1. Factor 1) may matter more
if the authors are keeping all the fragmented local hits. If the authors are
only looking at the best hit, then 2) should be the more important factor. In
the latter case, we should be able to improve LAST consensus by using the same
scoring system. It would be good if the authors try to understand why LAST
is not doing as well.

Note that BWA-MEM has a bug in alignment. It is unable to perform the simple
edit-distance based alignment. I cannot evaluate the effect of scoring systems
with my own tool. I need to fix that at some point.

### Comment 2: speed and memory

I have tried GraphMap on 250 human PacBio reads. It took ~3000 CPU seconds to
map them on the prebuilt index (i.e. time on indexing is excluded). The peak
RAM is ~100GB. BWA-MEM took 26 CPU seconds with most of time spent on loading
the BWA index. It seems that GraphMap is not quite ready for whole human genome
mapping yet.

### Comment 3: issues with end-to-end mapping

GraphMap forces end-to-end alignment. This is something I am trying to avoid in
BWA-MEM and BWA-SW. The 1000g SV group used to show the BLASR alignment of a
read containing an inversion. BLASR aligned through the inversion. Because the
true alignment is not linear, it leaves many mismatches and gaps in the
inverted region. Similarly, if there is a translocation, forcing end-to-end
alignment will produce wrong signals. On the other hand, the end-to-end
behavior may help accuracy when the tail of a read is error rich and is thus
more likely to be mismapped. Forcing end-to-end certainly helps to improve
speed as the mapper can more quickly filter out bad hits.

I have manually gone through the alignment of 250 reads (results are on
`ftp://hengli-data:lh3data@ftp.broadinstitute.org/pacbio-250`). The general
problem with BWA-MEM is that it may break a linear alignment into multiple
parts if there is a long INDEL or a region enriched with errors. This is
usually not a big problem because the aligned bases are still correct.
I found 5 such cases out of these 250 reads. There are a few cases where
GraphMap is likely to be wrong. For example, read `/183/10678_20910` probably
has an inversion in the middle.  The percent divergence of the three parts
is 19%, 22% and 18%. GraphMap aligns it through, resulting an alignment with
27% divergence. The similar seems to happen to `/310/7790_13785`. GraphMap
produces an alignment of 35% divergence.

There are also several ambiguous cases. For example, BWA-MEM gives
two hits on different chromsomes for `/401/0_23424` with 9% and 11% divergence,
respectively. GraphMap maps it to one place with 19% divergence. I don't know
which is correct. Nonetheless, I do know the answer in one case. BWA-MEM again
gives two hits for `/618/343_4960`, one on chr11 and the other on chr12.
GraphMap puts it on chr11. We can't tell just from this, but fortunately the
read happens to have another subread `/618/5003_6006`, which is mapped to chr12
in one piece, very close to the BWA-MEM hit for `/618/343_4960`. BWA-MEM is
likely to be right.

Generally speaking, forcing end-to-end produces better alignment when the true
alignment is linear. In practice, though, telling whether the alignment is
linear is non-trivial. That's why BWA-MEM produces local hits. As a side note,
the authors claim that BWA-MEM can't find kb-long INDELs. That is true. BWA-MEM
can't put such INDELs in one CIGAR. It usually produce two alignments flanking
the long INDELs, which can be identified later with post processing (e.g. by
`htsbox abreak`). This is an intentional design choice.

### Concluding remarks

All that being said, GraphMap clearly represents an advance in sequence
alignment. Some observations in the manuscript (e.g. comment 1) are very
interesting and worth further investigation. I am sure the speed and memory can
be improved (comment 2) if the authors have such needs. As to the final
comment, when and how to break linear alignment is an unresolved issue.
I wouldn't mind if the authors are not providing a satifactory answer.
Overall, if I were a reviewer, I would accept this manuscript for publication.

[gm]: https://github.com/isovic/graphmap
[pp]: http://biorxiv.org/content/early/2015/06/10/020719
[last]: http://last.cbrc.jp
