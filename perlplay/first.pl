#! /usr/bin/perl

my @stuff = ( { one, two }, three, four ) ;

foreach(@ARGV) {
	print "hello: ", $_, "\n" ;
}

foreach(@stuff) {
	print "my hello: ", "$_", "\n" ;
}

# end of file
