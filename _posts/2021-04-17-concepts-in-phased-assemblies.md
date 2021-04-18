---
layout: post
title: "Concepts in phased assemblies"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Concepts in phased assembly:

* [Contig][contig]: a contiguous sequence in an assembly. A contig does not
  contain long stretches of unknown sequences (aka *assembly gaps*).

* Scaffold: a sequence consists of one or multiple contigs connected by
  assembly gaps of typically inexact sizes. A scaffold is also called a
  [supercontig][scontig], though this terminology is rarely used nowadays.

* [Haplotig][haplotig]: a contig that comes from the same haplotype. In an
  unphased assembly, a contig may join alleles from different parental
  haplotypes in a diploid or polyploid genome.

* Switch error: a change from one parental allele to another parental allele on
  a contig (see the figure below). This terminology has been used for measuring
  reference-based phasing accuracy [for two decades][switch]. A haplotig is
  supposed to have no switch errors.

* Yak hamming error: an allele not on the most supported haplotype of a
  contig (see the figure below). Its main purpose is to test how close a contig is
  to a haplotig. This definition is tricky. The terminology was perhaps first
  used by [Porubsky et al (2017)][hamming] in the context of reference-based
  phasing. However, adapting it for contigs is not straightforward. The
  [yak][yak] definition is not widely accepted. The hamming error rate is
  arguably less important in practice (Richard Durbin, personal communication).

Types of phased assemblies. 

* Collapsed assembly: a complete assembly with parental alleles randomly
  switching in a contig. Here, an assembly is *complete* if it fully represents
  a haploid genome. Most conventional assemblers produce collapsed assemblies.
  A collapsed assembly is also called a squashed assembly or a conensus
  assembly.

* Primary assembly: a complete assembly with long stretches of phased blocks.
  The concept has been [used by GRC][grc-def]. Falcon-unzip is perhaps the
  first to produce such assemblies.

* Alternate assembly: an incomplete assembly consisting of haplotigs in
  heterozygous regions. An alternate assembly always accompanies a primary
  assembly. It is not useful by itself as it is fragmented and incomplete.

* Partially phased assembly: sets of complete assemblies with long stretches
  of phased blocks, representing an entire diploid/polyploid genome. Peregrine
  is perhaps the first to produce such assemblies.  This concept is coined by
  me as I could not find a proper one in the existing literature. I don't like
  the terminology. If someone has a better naming, let me know.

* Haplotype-resolved assembly: sets of complete assemblies consisting of
  haplotigs, representing an entire diploid/polyploid genome. This concept has
  been inconsistently used in publications without a clear definition. The
  above is my take in our recent papers.

Furthermore, we may have chromosome-scale haplotype-resolved assembly where
haplotigs from the same chromosome are fully phased. For germline genomes,
the highest standard is [telomere-to-telomere][t2t] assembly where each
chromosome is fully phased and assembled without gaps.

<style> .extable td,th { padding: 4px; } </style>
<table border="1" class="extable">
<tr><th>Asm type </th><th>Complete?</th><th>Haplotig?</th><th>N50  </th><th>SwitchErr</th><th>HammingErr</th></tr>
<tr><td>Collapsed</td><td>Yes      </td><td>No       </td><td>Long </td><td>Many     </td><td>Many      </td></tr>
<tr><td>Primary  </td><td>Yes      </td><td>No       </td><td>Long </td><td>Some     </td><td>Many      </td></tr>
<tr><td>Alternate</td><td>No       </td><td>Yes      </td><td>Short</td><td>Few      </td><td>Few       </td></tr>
<tr><td>Partial  </td><td>Yes      </td><td>No       </td><td>Long </td><td>Some     </td><td>Many      </td></tr>
<tr><td>Resolved </td><td>Yes      </td><td>Yes      </td><td>Long </td><td>Few      </td><td>Few       </td></tr>
</table>

<img src="http://www.liheng.org/images/asmconcepts/phased-asm-flow.png" alt="drawing" width="500"/>

[contig]: https://www.genome.gov/genetics-glossary/Contig
[haplotig]: https://www.ncbi.nlm.nih.gov/books/NBK44482/
[switch]: https://pubmed.ncbi.nlm.nih.gov/12386835/
[hamming]: https://pubmed.ncbi.nlm.nih.gov/29101320/
[yak]: https://github.com/lh3/yak
[grc-def]: https://www.ncbi.nlm.nih.gov/grc/help/definitions/
[t2]: https://github.com/nanopore-wgs-consortium/CHM13
[scontig]: https://en.wiktionary.org/wiki/supercontig
