# CMPS 3500
# Lab 08 - Perl Scripting, Parsing C Programs
# Erik Soto

#!/usr/bin/perl 

# 1. Calculating Sales (calcSales.pl)
# Create a perl script that reads an input file specifying sales figures
# and outputs a tabulation of these figures.
#
# How to Run: perl calcSales.pl -f salesFigures.txt


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

open($inf, '<', $filename) or die "Can't read input file: $!";

my $data;
my %dataMap;

print "Name     SALES\n";
print OUTFILE "Lines filtered by ID with 2nd Digit >= 7 \n";
while(<$inf>)           # read through line by line
{
    chomp;                  # remove [CR/LF]
    next if /^$/;           # by pass empty lines
    $data = $_;             # assign $_ in array
    $data =~ s/\s//g;       # remove spaces
    $data =~ tr/A-Z/a-z/;   # convert to lowercase

    ($words1,$words2) = split /\+/, $data;
    if ($words2 eq "")
    {
        $dataMap{$words1} = $dataMap{$words1} + 0
    }
    else
    {
        $dataMap{$words1} = $dataMap{$words1} + $words2;
    }
}

foreach $_ (sort keys %dataMap)
{
    printf("%-10s %-10s \n", $_, $dataMap{$_});
}

close($inf);

