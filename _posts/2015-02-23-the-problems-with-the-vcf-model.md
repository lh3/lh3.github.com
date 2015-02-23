---
layout: post
title: "The problems with the VCF model"
description: ""
category: 
tags: []
---
{% include JB/setup %}

*This and the next posts were mostly written on the plane when I felt tired and
did not have Internet connections. The logical flow is not very clear. In
addition, I have to admit that I have not thought through the topic when I was
writting it up. Now I have a clearer picture after I finish the posts. I will
still put them online for a historical record.*

### Edit-based representation

VCF represents a variant by substituting the reference allele sequence with the
variant allele. Substitution is a type of edit. In fact, HGVS, GVF and most
other variant formats or representations are edit-based. However, edit-based
representations have an intrinsic problem: edits are determined by the alignment
between the variant allele and the reference allele, and alignments are affected
by the scoring system. As a result, one allele sequence could be represented in
multiple ways. Here is an example:

    Ref:    AAGCTA--CTAG----CT                 AAGCTA------CTAGCT
    Allele: AAGCTAGACTAGGAAGCT       or        AAGCTAGACTAGGAAGCT
	    (2 gap opens, 0 mismatch)          (1 gap open, 2 mismatches)

In this example, the allele sequence is the same, but in the VCF format, it
could be represented in two different ways. This is a serious problem. Some
blame VCF but this is not exactly fair. We have had the problem for over a
decade. We were not aware of it only because we haven't dealt with so many sites
and samples.

A possible solution to the one-variant-multiple-representation problem is to use
a context-based representation. I will not go into details in this post except
pointing out that adopting a context-based representation requires a huge shift
in the modeling of genetic variants and will take time. Those who are interested
in this topic should read the preprint by Benedict et al (2014).

### Multiple alleles per record

In VCF, we frequently **have to** squeeze multiple alleles in one VCF line;
otherwise we will not be able to represent a diploid genotype with two ALT
alleles sometimes. Then what is the rule to combine multiple alleles? The VCF
spec doesn't specify. **Conventionally**, we merge all overlapping alleles into
one VCF line. Why couldn't we promote this convention into the spec? It is
because the convention leads to various issues.

Firstly, merging overlapping alleles doesn't practically work with long
deletions. Then when should we merge and when shouldn't? There are no consistent
solutions.

Secondly, merging is sensitive to rare variants. Suppose we have two common SNPs
at position 1002 and 1005 among 100,000 samples. The two SNPs will be put on
separate VCF lines as they have no overlaps. However, if the 100,001st sample
has a deletion from position 1001 to 1006, we will have to squash the two SNPs
and the one deletion into one VCF line, but this complicates the annotation and
the analysis of the two SNPs. Such a scenario may happen often given many
samples.

Thirdly, merging is not always possible when data are unphased. Still consider
the example above. If the two SNPs at 1002 and 1005 are unphased, we will not
know how to join them for each sample. (TODO: how bcftools merge works?)

Fourthly, merging multi-sample VCFs will add many "./." or "0/0" genotypes. The
resultant VCF is usually much larger than the sum of input. Merging is not
scalable. Strictly speaking, the space inefficiency is caused by the dense
representation of VCF, which I will come back shortly.

As a consequence of the points above, VCF merging as is required by the
multi-allele-per-record representation is a complex, expensive, inconsistent and
indeterministic operation. It effectively creates a boundary between VCFs
produced from different projects and hampers data integration. In addition,
the multi-allele-per-record representation also makes annotation harder
because we annotate individual alleles, not a VCF line. Furthermore, not every
VCF follows the non-overlapping convention. The inconsistencies between VCFs
are frequently troublesome.

The VCF representation also raises a serious theoretical concern: what does each
VCF line stand for? It is in fact this conceptual ambiguity that leads to all
the problems in this section.

### Dense representation

VCF encodes a matrix of genotypes (site-by-sample) with a dense representation
whereby it explicitly gives the genotype of every cell. For many samples, this
matrix is sparse in the sense that the vast majority of cells are "0/0" (if the
VCF is produced by multi-sample calling, including gVCF merging) or "./." (if
produced by merging). The more samples, the more sites, the more sparse the
matrix, and the more space and computation VCF costs. VCF is not scalable.

### Conclusion

I have listed three major problems with the VCF model: edit-based
representation, multiple allele per record and dense representation. While I am
not sure how to solve the first problem without disruptively trashing our
established practices on variants, a few believe it should be possible to solve
the other two problems. This will be explained in my second post of this sequel.
