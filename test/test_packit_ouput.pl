#!/usr/bin/perl
#
# The traditional first program.

# Strict and warnings are recommended.
use strict;
use warnings;

# Print a message.
print `echo -e "\\xa\\xb\\xc\\x0\\x0\\x80\\x3f\\xa\\x82\\xdf\\x19\\xdf" | ../src/packit -i - -p ../test/fixed_basic.gp -n1`;

#exit(1);