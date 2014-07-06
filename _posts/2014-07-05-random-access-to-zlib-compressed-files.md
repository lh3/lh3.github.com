---
layout: post
title: "Random access to zlib compressed files"
description: ""
category: 
tags: []
---
{% include JB/setup %}

[Zlib][zlib]/[gzip][gzip] is probably the most popular library/tool for general
data compression. In zlib, there is an API `gzseek()` which places the file
position indicator at a specified offset in the uncompressed file. However,
whenever it gets called, it starts from the beginning of the file and reads
through all the data up to the specified offset. For huge files, this is very
slow.

It is actually possible to achieve faster random access in a generic gzip file.
The [zran.c][zran] in the zlib source code package gives an example
implementation. It works by keeping 32kB uncompressed data right before an
access point. With the 32kB data, we can decompress data after the access
point - we do not need to decompress from the beginning. My friend Jue Ruan
found this example and and implemented [zrio][zrio], a small library that
keeps the 32kB data in an index file to achieve random access to generic gzip
files. This library is used in [maqview][maqview].

However, keeping 32kB data per access point is quite heavy. To drop this 32kB
dependency, Jue sought a better solution: calling
`deflate(stream,Z_FULL_FLUSH)` every 64kB. After `Z_FULL_FLUSH`, we can decompress
the following data independent of the previous data -- keeping 32kB is not
necessary any more. The resultant compressed stream is still fully compatible
with zlib. Jue implemented this idea in [RAZF][razf]. In addition to this
stream reset, RAZF also writes an index table at the end of the file. Given
an uncompressed offset, we can look up the table to find the nearest access
point ahead of the offset to achieve random access. The index is much smaller
and the speed is much faster.

The first prototype of [BAM][BAM] was using RAZF. At that time, a major concern
was that RAZF is using low-level zlib APIs which were not available in other
programming languages. This would limit the adoption of BAM. The size of the
index might also become a concern given >100GB files. In the discussion,
[Gerton Lunter][gerton] directed us to [dictzip][dictzip], another tool for
random access in gzip-compatible files. Dictzip would not work well for a huge
BAM due to the constraint of the gzip header. However, its key idea --
concatenating small gzip blocks -- led Bob Handsaker to design something
better: [BGZF][bgzf] (section 4.1).

The key observation Bob made in BGZF is that when we seek the middle of a
compressed file, all we need is a virtual position which is not necessarily the
real position in the uncompressed file. In BGZF, the virtual position is a
tuple `(block_file_position,in_block_offset)`, where `block_file_position` is
the file postion, in the compressed file, of the start of a gzip block and
`in_block_offset` is the offset within the uncompressed gzip block. With the
tuple, we can unambiguously pinpoint a byte in the uncompressed file. When we
keep the tuple in an index file, we can jump to the position without looking up
another index. BGZF is smaller than RAZF and easier to implement. It has been
implemented in C, Java, Javascript and Go. Recently, Petr Danecek has [extended
BGZF][bgzf2] with an extra index file to achieve random access with offset in
uncompressed file.

In the analysis of high-throughput sequencing data, BGZF plays a crucial role
in reducing the storage cost while maintaining the easy accessibility to the
data. It is a proven technology scaled to TB of data.

[zran]: http://www.opensource.apple.com/source/zlib/zlib-22/zlib/examples/zran.c
[gzip]: http://www.gzip.org
[zlib]: http://www.zlib.net
[zrio]: https://sourceforge.net/p/maq/code/HEAD/tree/trunk/maqview/zrio.h
[maqview]: http://maq.sourceforge.net/maqview.shtml
[razf]: https://github.com/lh3/samtools-legacy/blob/master/razf.h
[BAM]: http://samtools.sourceforge.net
[dictzip]: http://linuxcommand.org/man_pages/dictzip1.html
[gerton]: http://www.well.ox.ac.uk/dr-gerton-lunter
[bgzf]: http://samtools.github.io/hts-specs/SAMv1.pdf
[bgzf2]: https://github.com/samtools/htslib/blob/develop/htslib/bgzf.h
[tabix]: https://github.com/samtools/tabix
