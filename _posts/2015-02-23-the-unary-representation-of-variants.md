---
layout: post
title: "The unary representation of variants"
description: ""
category: 
tags: []
---
{% include JB/setup %}

As is discussed in my previous post, a major but potentially fixable problem
with the VCF model is that we allow to and sometimes have to squeeze multiple
alleles in one VCF line. This post gives the solution, the unary representation.
The representation was first conceived by Richard Durbin a couple of years ago,
but in my view had a few practical issues initially and thus never openly
presented.

In the following, I will present a *toy* format named as variant allele format
(VAF), but I should note that this format is mainly to demonstrate the logical
relationships between objects in the unary representation. I am ***NOT***
proposing a new polished format in this post.

### The *toy* VAF format

Here is an example:

    #LS @1000g  NA06879    NA12345
    #LS @SGDP   HGDP12345  HGDP23456
    AL  chr1_501C>T  rs12345  AC=3
    GT  @1000g:GT     1|0            .|.
    GT  @SGDP:GT:GL   0|0:0,30,100   1|1:100,30,0
    //
    UC  chr1_601_700uncalled
    GT  @1000g
    GT  HGDP12345
    //
    AL  chr1_1002A>T       .  .
    GT  NA06879:GT    1/1
    GT  NA12345:GT    1/chr1_1002_1005del
    GT  @SGDP:GT      1/0   1/chr1_1002_1005del
    //
    AL  chr1_1002_1005del  .  .
    GT  NA12345:GT    1/chr1_1002A>T
    GT  @1000g:GT     *     1/chr1_1002A>T
    //

where a `#LS` header line defines an ordered list of samples. An `AL` line
encodes an allele sequence in an unambiguous HGVS-like format (which will be
discussed later) and the following `GT` lines give the genotypes of each sample,
where "1" indicates the presence of the ALT allele, "0" the presence of the
reference allele and "\*" suggests the sample does not contain the allele (NB:
this is different from missing data "."; see also discussion point 5 below). If
at a locus, a sample possesses two overlapping ALT alleles, one of the alleles
needs to be encoded in the HGVS-like format on the GT line.

If we use one `#LS` line and append the `GT` lines to the `AL` lines, the format
above will look like VCF. However, conceptually, VAF has two important
differences: 1) in VAF, we can choose to exclude homozygous reference sites (in
VCF, we need to explicitly write 0/0 at these sites), and 2) in VAF, the primary
unit is allele (in VCF, each data line doesn't have a clear meaning). With these
two properties, merging two VAFs is simply done via line copying: if an allele
in the second VAF is absent in the first, we copy the entire `AL` record to the
first file; if present, we only copy `GT` lines to the first VAF. Simple merging
makes it trivial to integrate variants from various projects, which is very
complicated and costly with VCF. VAF is also more consistent and simplifies
some optional fields like AC and AF and fields related to variant annotation.

### Further comments on VAF

1. Atomic vs compound alleles. An atomic allele is the "smallest" allele. A
compound allele consists of multiple atomic alleles. In VAF, each allele should
be atomic.

2. Unambiguous representation of an atomic allele. In HGVS, a C to G SNP could
also be described by a 1bp inversion. This is problematic. We could impose extra
rules such that the same atomic allele is always represented by the same string.

3. At the AL lines, it is probably better to convert `chr1_501C>T` to `chr1 500
501 C T`, but then we need a way to describe large alleles (e.g. translocations)
and a consistent procedure to convert TAB-delimited allele representation to a
string.

4. My major concern with the initial unary proposal is that retrieving genotypes
requires to combine multiple allele lines, which greatly complicates
programming. VAF solves this problem by listing non-reference alleles that are
different from the current allele record. This is only necessary at sites having
two different ALT alleles.

5. An alternative solution to point 4 is to add `RA` lines to list alleles
related to the current allele record. We could then use integers in the GT
lines. However, doing this will complicate merging as we need to recompute
allele number. Simple line copying won't work.

6. I think having the special "\*" genotype is convenient, but it is not quite
necessary. If we feel "\*" is confusing, we could replace "\*" with
"0/chr1_1002A>T" in the example above.

7. For a diploid sample, the same genotype may be presented twice (or four times
for a tetraploid sample). This is a disadvantage.

8. In comparison to VCF, it is harder to get overlapping genotypes containing
different alleles. Another disadvantage. However, I am not sure we need this
operation.

### VAF in Avro

    record Call {
        string callSetId;   // sample ID
        array<union{int,Path}> genotype; // array size == ploidy
        array<double> genotypeLikelihood = [];
        map<array<string>> info = {};  // other VCF FORMAT fields
    }
    record Allele {
        Path path;   // allele position and sequence
        array<Call> calls = [];
        map<array<string>> info = {};  // other VCF INFO fields
    }

A slightly different schema, where we put other related overlapping alleles in
`Allele` (see also point 5 in the discussions above).

    record Call {
        string callSetId;   // sample ID
        array<int> genotype; // array size == ploidy
        array<double> genotypeLikelihood = [];
        map<array<string>> info = {};  // other VCF FORMAT fields
    }
    record Allele {
        Path path;   // allele position and sequence
        // other paths overlapping `path` and used in `calls`
        array<Path> relatedPaths = [];
        array<Call> calls = [];
        map<array<string>> info = {};  // other VCF INFO fields
    }
