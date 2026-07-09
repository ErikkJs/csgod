#!/usr/bin/perl 

# CMPS 3500
# Lab 08 - Perl Scripting, Parsing C Programs
# 

#Erik Soto

# 2. Parsing C programs (cparse.pl)
# Create a perl script that counts tokens in a C program.
#
# Should output:
#   1. Number of words (variable and reserved) on the line.
#   2. Number of numeric literals without decimal points on the line
#   3. Number of numeric literals with decimal points on the line
#   4. Number of braces, brackets, and parentheses on the line.
#
# How to Run: perl cparse.pl -f mainProg.c

#!/user/bin/perl

my $filename = '';
my $var = 0;
my @array;

while (@ARGV) {                         # parse command line arguments 
  if ($ARGV[0] eq '-f') {
    shift;                              # move array elements up 
    $filename = $ARGV[0];
    shift;
    next;                               # jump to start of while loop 
  }
  elsif ( $ARGV[0] eq '-h') {           # display usage 
     print "\nUsage: calcSales.perl [-h] [-f <filename>]\n\n";
     shift;
     if (!@ARGV) {
        exit;
     }   
     else {
       next; }
  }   
  else {   
     shift;                             # unknown argument so skip it
     next;
  }
}

open(FILE, '<',$filename) or die "Could not open file: $!";

my ($words,$ints,$floats,$braces) = (0,0,0,0);
my ($twords,$tints,$tfloats,$tbraces) = (0,0,0,0);
my %dataMap;

while (<FILE>) {

    $words = scalar(split(/\s+/, $_));
    $twords += scalar(split(/\s+/, $_));

    $braces += () = $_ =~ /\{/gi;
    $braces += () = $_ =~ /\}/gi;
    $braces += () = $_ =~ /\(/gi;
    $braces += () = $_ =~ /\)/gi;
    $tbraces += $braces;

    $ints = () = $_ =~ /\d+/gi;
    $tints += $ints;

    $floats = () = $_ =~ /\d+\.\d+/gi;
    $tfloats += $floats;

    print("words=$words, ints= $ints, floats= $floats, braces=$braces\n");
    $braces = 0;
}

print("======================================\n");
print("Total: \n");
print("words= $twords, ints= $tints, floats= $tfloats, braces= $tbraces \n");

