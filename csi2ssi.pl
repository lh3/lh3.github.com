#!/usr/bin/perl -w

use strict;
use warnings;

die("Usage: csi2ssi.pl <html_file> [...]\n") unless (@ARGV);

foreach my $file (@ARGV) {
  my ($in, $out) = ($file, "$file.shtml");
  $out = "$1.shtml" if ($file =~ /^(.+)\.html$/);
  my ($fhin, $fhout);
  open($fhin, $in) || die("Fail to open file $in.\n");
  open($fhout, ">$out");
  my $header;
  my @inc = ();
  while (<$fhin>) {
	s/((href|url)\s*=\s*(['"]?)[A-Za-z][^:\s]+\.)html(#?[A-Za-z0-9_-]*(['"]?))/$1shtml$4/ig;
	if (/script.*jsInclude\.js/i) {
	} elsif (/body\s+onLoad.*jsInclude\s*\((.*)\)/i) {
	  my $line = $1;
	  @inc = split(/[',\s]+/, $line);
	  shift(@inc);
	  $header = shift(@inc);
	  print $fhout "<body>\n";
	} elsif (@inc && /span\s+id\s*=.*$header.*span/i) {
	  foreach my $p (@inc) {
		$p =~ s/\.html$/\.shtml/;
		print $fhout qq(<!--#include virtual="$p" -->\n);
	  }
	} else {
	  print $fhout $_;
	}
  }
  close($fhout); close($fhin);
}
