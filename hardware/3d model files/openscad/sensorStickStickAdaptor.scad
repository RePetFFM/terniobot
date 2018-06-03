$fn = 40;


difference() {
	// slit(10,4,20);

	union() {
		translate([0,0,-0.2])
		hull() {
			translate([0,0,6])
			cylinder(h = 0.1, r=2.9/2);
			cylinder(h = 5, r=4/2);	
		}

		translate([10,-10,-1])
		cube(size=[20,20,23]);

		translate([0,0,0.5])
		cylinder(h = 20, r=2.9/2);


		translate([0,0,4])
		rotate(a=[0,90,0])
		cylinder(h=20,r=3.2/2);

		translate([0,0,15])
		rotate(a=[0,90,0])
		cylinder(h=20,r=3.2/2);
	}
}












module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}


	