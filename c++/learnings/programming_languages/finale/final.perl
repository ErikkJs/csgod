#!/usr/bin/perl

use strict;
use warnings;

my $usage="Usage: $0 [-h] [--csv|--tab] -f <personal_data> -g <general_info> -o <outfile>\n";

die "$usage" if $#ARGV < 5;

my $infile1;
my $infile2;
my $outfile;
my $csv = 0;
my $tab = 0;

while(@ARGV) {
  if($ARGV[0] eq '-h') {
    print $usage;
    exit;
  }
  elsif($ARGV[0] eq '--csv') {
    $csv = 1;
    shift;
  }
  elsif($ARGV[0] eq '--tab') {
    $tab = 1;
    shift;
  }
  elsif($ARGV[0] eq '-f') {
    shift;
    if($ARGV[0] eq '') {
      $ARGV[0] = '-h';
      next;
    }
    else {
      $infile1 = $ARGV[0];
      shift;
    }
  }
  elsif($ARGV[0] eq '-g') {
    shift;
    if($ARGV[0] eq '') {
      $ARGV[0] = '-h';
      next;
    }
    else {
      $infile2 = $ARGV[0];
      shift;
    }
  }
  elsif($ARGV[0] eq '-o') {
    shift;
    if($ARGV[0] eq '') {
      $ARGV[0] = '-h';
      next;
    }
    else {
      $outfile = $ARGV[0];
      shift;
    }
  }
  else {
    print "Invalid argument given.\n";
    print $usage;
    exit;
  }
}

die "YOU CAINT DO THAT SON.\n$usage" if($csv == 1 && $tab == 1);

open(DATA, $infile1) or die "Cannot open $infile1 for reading: $!.\n";
open(INFO, $infile2) or die "Cannot open $infile2 for reading: $!.\n";
open(OUT, ">$outfile") or die "Cannot open $outfile for writing: $!.\n";

my $delim = ($tab == 1) ? "\t" : ",";

my %stocks;
my %prices;
my %names;
my %values;

my $line;
my @fields;

while(<DATA>) {
  chomp;
  $line = $_;
  @fields = split /$delim/, $line;
  $stocks{$fields[0]} = $fields[1];
}

while(<INFO>) {
  chomp;
  $line = $_;
  @fields = split /$delim/, $line;
  $prices{$fields[0]} = $fields[1];
  $names{$fields[0]} = $fields[2];
}

my $key;

foreach (keys %stocks) {
  $key = $_;
  $values{$key} = $stocks{$key} * $prices{$key};
  print OUT "$names{$key}$delim$key$delim$stocks{$key}$delim$prices{$key}";
  print OUT "$delim$values{$key}\n";
}

close DATA;
close INFO;
close OUT;

my $symbol;
print "Enter a symbol to liquidate: ";
$symbol = <stdin>;
chomp($symbol);
$symbol =~ tr/a-z/A-Z/;

if($values{$symbol}) {
  print "$symbol liquidated for \$$values{$symbol}.\n";
}
else {
  print "$symbol not in personal stock data.\n";
}

