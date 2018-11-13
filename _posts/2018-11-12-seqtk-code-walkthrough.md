---
layout: post
title: "Seqtk: code walkthrough"
description: ""
category: 
tags: []
---
{% include JB/setup %}

On Twitter, [Zach charlop-powers][zachcp] asked me to give a code walkthrough
for [seqtk][seqtk]. This post does that, to a certain extent.

Seqtk is a fairly simple project. It uses two single-header libraries for hash
table and FASTQ parsing, respectively. Its single `.c` file consists of mostly
independent components, one for each seqtk command. I will start with the two
single-header libraries.

### Buffered stream reader

In standard C, the [read()][read-sys] system call is the most basic function to
read from file. It is usually slow to read data byte by byte. That is why the C
library provides the [fread()][fread] series. fread() reads large chunks of
data with read() into an internal buffer and returns smaller data blocks on
request. It may dramatically reduce the expensive read() system calls and is
mostly the preferred choice.

fread() is efficient. However, it only works with data streams coming from
[file descriptors][fd], not from a zlib file handler for example. More recent programming languages provide generic
buffered readers. Take Go's [Bufio][go-bufio] as an example. It demands a
read() like function from the user code, and provides a buffered single-byte
reader and an efficient line reader in return. The buffered functionalities are
harder to implement on your own.

The buffered reader in [kseq.h][kseq] predates Go, but the basic idea is similar.
In this file, [ks\_getuntil()][getuntil] reads up to a delimitor such as a line
break. It moves data with memcpy() and uses a single loop to test delimitor. 10
years ago when "kseq.h" was first implemented, [zlib][zlib] didn't support
buffered I/O. Line reading with zlib was very slow. "kseq.h" is critical to
the performance of FASTA/Q parsing.

### FASTA/Q parser

The [parser][kseqread] parses FASTA and FASTQ at the same time. It [looks
for][L183] '@' or '&gt;' when necessary, and then [reads][L188] name and
comment. To read sequence, the parser first [reads the first character][L194]
on a line. If the character is '+' or indicates a FASTA/Q header, the parser
stops; if not, it [reads the rest of line][L197] into the sequence buffer.
If the parser stops at a FASTA/Q header, it returns the sequence as a FASTA
record and [indicates][L199] the header character has been read, such that it
need not look for it for the next record. If the parser stops at '+', it
[skips][L212] the rest of line and starts to [read quality strings][L214] line
by line until the quality string is no shorter than the sequence. The parser
returns an error if it reaches the end of file before reading enough quality,
or the quality string turns out to be longer than sequence. Given a
malformatted FASTA/Q file, the parser won't lead to memory violation except
when there is not enough memory.

A basic tip on fast file parsing: read by line or by chunk, not by byte. Even
with a buffered reader, using fgetc() etc to read every byte is slow. In fact,
it is possible to make the FASTA/Q parser faster by reading chunks of fixed
size, but the current pasrser is fast enough for typical FASTA/Q.

### Hash table

File [khash.h][khash] implements an [open-addressing hash table][open-addr]
with power-of-2 capacity and quadratic probing. It uses a 2-bit-per-bucket
[meta table][H165] to indicate whether a bucket is used or deleted. The query
and insertion operations are fairly standard. There are no tricks. Rehashing in
khash is different from other libraries, but that is not an important aspect.

Both "khash.h" and "kseq.h" heavily depend on C macros. They look ugly.
Unfortunately, in C, that is the only way to achieve the performance of
type-specific code.

### Seqtk

The only performance-related trick I can think of in [seqtk.c][seqtkc] is the
[tables][trans-tbl] to map nucleotides to integers. It is commonly used
elsewhere. Another convenience-related trick is [isatty()][isatty]. This
function can [test][use-isatty] if there is an incoming stream from the
standard input. Gzip probably uses this function, too.

Seqtk.c also implements a simple 3-column [BED reader][bed-read] and comes with
a [Mersenne Twister][mt19937] pseudorandom number generator (PRNG). That PRNG
is a [mistake][mt-problem], though seqtk doesn't need a good PRNG anyway.

The rest of seqtk consists of mostly indepedent functions, each implementing a
seqtk command. I will briefly explain a couple of them. "trimfq" uses a modified
Mott algorithm (please search text "Mott algorithm" in [phred.doc][phred-doc]).
I think this is a much cleaner and more theoretically sound algorithm than most
ad hoc methods in various read trimmers. The "sample" command takes the
advantage of [reservoir sampling][rsample]. The core implementation only takes
[two lines][C1073]. You can in fact sample a text file with an awk one liner:
```
cat file.txt | awk -v k=10 '{y=x++<k?x-1:int(rand()*x);if(y<k)a[y]=$0}END{for(z in a)print a[z]}'
```

### Concluding remarks

This is the first time I present a code walkthrough in a blog post. Not sure if
it is helpful or even qualified as a walkthrough...

[C1073]: https://github.com/lh3/seqtk/blob/v1.3/seqtk.c#L1073-L1074
[rsample]: https://en.wikipedia.org/wiki/Reservoir_sampling
[phred-doc]: https://www.codoncode.com/support/phred.doc.html
[bed-read]: https://github.com/lh3/seqtk/blob/v1.3/seqtk.c#L52
[use-isatty]: https://github.com/lh3/seqtk/blob/v1.3/seqtk.c#L375
[isatty]: http://man7.org/linux/man-pages/man3/isatty.3.html
[mt-problem]: http://www.pcg-random.org/other-rngs.html
[mt19937]: https://en.wikipedia.org/wiki/Mersenne_Twister
[trans-tbl]: https://github.com/lh3/seqtk/blob/v1.3/seqtk.c#L117-L169
[seqtkc]: https://github.com/lh3/seqtk/blob/v1.3/seqtk.c
[open-addr]: https://en.wikipedia.org/wiki/Open_addressing
[kseq]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h
[khash]: https://github.com/lh3/seqtk/blob/v1.3/khash.h
[zachcp]: http://zachcp.org/
[seqtk]: https://github.com/lh3/seqtk
[read-sys]: http://man7.org/linux/man-pages/man2/read.2.html
[fread]: http://man7.org/linux/man-pages/man3/fread.3.html
[fd]: https://en.wikipedia.org/wiki/File_descriptor
[java-reader]: https://docs.oracle.com/javase/8/docs/api/java/io/BufferedReader.html
[go-bufio]: https://golang.org/pkg/bufio/
[zlib]: https://zlib.net/
[getuntil]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L94-L144
[kseqread]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L178-L219
[L183]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L183
[L188]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L188
[L194]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L194
[L197]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L197
[L199]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L199
[L212]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L212
[L214]: https://github.com/lh3/seqtk/blob/v1.3/kseq.h#L214
[H165]: https://github.com/lh3/seqtk/blob/v1.3/khash.h#L165
