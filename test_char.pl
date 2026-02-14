#!/usr/bin/env perl
# test_char.pl

use strict;
use warnings;
use Getopt::Std;
use File::Basename;
use Carp;

my $i;
for ($i = 1; $i <= 255; $i++) {
  my $txt = "a" . chr($i);

  print "$i=";
  if ($txt =~ /a./) {
    print ".";
  }
  if ($txt =~ /a\s/) {
    print "s";
  }
  if ($txt =~ /a\S/) {
    print "S";
  }
  if ($txt =~ /a\w/) {
    print "w";
  }
  if ($txt =~ /a\W/) {
    print "W";
  }
  if ($txt =~ /a\d/) {
    print "d";
  }
  if ($txt =~ /a\D/) {
    print "D";
  }
  if ($txt =~ /a$/) {
    print "\$";
  }
  if ($txt =~ /a[\s]/) {
    print "[s]";
  }
  if ($txt =~ /a[\S]/) {
    print "[S]";
  }
  if ($txt =~ /a[\w]/) {
    print "[w]";
  }
  if ($txt =~ /a[\W]/) {
    print "[W]";
  }
  if ($txt =~ /a[\d]/) {
    print "[d]";
  }
  if ($txt =~ /a[\D]/) {
    print "[D]";
  }
  if ($txt =~ /a[a-z]/) {
    print "[a-z]";
  }
  if ($txt =~ /a[A-Z]/) {
    print "[A-Z]";
  }
  if ($txt =~ /a[0-9]/) {
    print "[0-9]";
  }
  print "\n";
}

# All done.
exit(0);
