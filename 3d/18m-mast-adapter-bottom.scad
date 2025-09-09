$fn = 200;
r = 68/2;
ri = 60.5/2+.5;

difference() {
union() {
cylinder(h=10, r=r+7);
cylinder(h=40, r=r);
}
cylinder(h = 60, r=ri);
cylinder(h=9, r1=ri+5, r2=ri);
}