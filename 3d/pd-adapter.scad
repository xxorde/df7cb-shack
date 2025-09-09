length = 27.8;
width = 10.8;
height = 3.2;
wall = 1.2;

difference() {
    cube([length, width+2*wall, wall+2*height]);
    translate([0, wall, wall])
        cube([length, width, height]);
    translate([0, wall+0.2, wall+height])
        cube([length, width-0.4, height]);
}