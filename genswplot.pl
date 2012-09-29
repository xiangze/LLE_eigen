#!/usr/bin/perl

# generate sampled data
#  tt = (3*pi/2)*(1+2*rand(1,N));
#  height = 21*rand(1,N);
#  X = [tt.*cos(tt); height; tt.*sin(tt)];
#  scatter3(X(1,:),X(2,:),X(3,:),12,tt,'+');

my $N=$ARGV[0];

my $Pi=3.1415;
my $h=4;

my ($x,$y,$z);
my @ts;

for my $n(1 .. $N) {
    push @ts ,2*$Pi*(rand()+3);
}
#push @ts ,2*$Pi*(rand()+3);

for my $t(@ts) {
    $x=$t*cos($t);
    $z=$t*sin($t);
    $y=rand($h);
    print "$x,$z,$y,$t\n";
}
