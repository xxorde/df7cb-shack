$fn = 200;
r = 71.9/2 - 2;
ri = 60.5/2+.5;

difference() {
union() {
translate([0, 0, 42])
cylinder(h=3, r1=r-.4, r2=r-1);
translate([0, 0, 30])
cylinder(h=12, r1=r-.2, r2=r-.4);
translate([0, 0, 15])
cylinder(h=15, r1=r, r2=r-.2);
cylinder(h=15, r=r);
}
cylinder(h = 38, r=ri);
}