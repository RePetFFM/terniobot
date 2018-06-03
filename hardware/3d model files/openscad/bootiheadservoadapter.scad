$fn = 40;





difference() {
	union() {
		cylinder(h = 2.5, r=20);
		hull() {
			translate([0,10,0]) cylinder(h = 2.5, r=2);
			translate([0,-10,0]) cylinder(h = 2.5, r=2);
			translate([63,10,0]) cylinder(h = 2.5, r=2);
			translate([63,-10,0]) cylinder(h = 2.5, r=2);
		}

		translate([5,10,1]) slit(45,1,3);
		translate([5,-10,1]) slit(45,1,3);
	}
	
	translate([29,0,0])
	serverHole();

	translate([0,0,-0.2])
	union() {
		rotate(a=[0,0,0])
		headMountScrewhole();

		rotate(a=[0,0,120])
		headMountScrewhole();

		rotate(a=[0,0,-120])
		headMountScrewhole();

		cylinder(h = 6, r=9);	

		rotate(a=[0,0,60]) headMountScrewholeSpace();
	}	
}




module headMountScrewhole() {
	translate([15,6,0]) cylinder(h = 6, r=1.6);
	translate([15,-6,0]) cylinder(h = 6, r=1.6);
}

module headMountScrewholeSpace() {
	rotate(a=[0,0,0]) translate([25,0,0]) cylinder(h = 6, r=6);	
	rotate(a=[0,0,120]) translate([25,0,0]) cylinder(h = 6, r=6);	
	rotate(a=[0,0,-120]) translate([25,0,0]) cylinder(h = 6, r=6);	
}

module serverHole() {
	union() {
		translate([0,-13/2,-0.2])
		cube(size=[29.5,13,5]);

		translate([0-3.5,0,-0.2])
		slit(5,3.1/2,6);

		translate([29.5+3.5,0,-0.2])
		rotate(a=[0,0,180])
		slit(5,3.1/2,6);	
	}
}


module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}


