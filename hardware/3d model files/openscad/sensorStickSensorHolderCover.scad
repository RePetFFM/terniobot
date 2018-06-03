$fn = 40;



difference() {
	union() {
		union() {
			slit(10,3.5,2.5);
			rotate(a=[0,0,90]) slit(9+7,3.5,2.5);
		}

		hull() {
			translate([-8,-8,0])
			slit(16,2,1.5);
			translate([-8,12,0])
			slit(16,2,1.5);
		}

		hull() {
			translate([-6,-6,1])
			slit(12,2,1.5);
			translate([-6,8,1])
			slit(12,2,1.5);
		}
	}


	union() {
		translate([10,0,-1]) cylinder(h = 10, r=3.2/2);
		translate([0,16,-1]) cylinder(h = 10, r=3.2/2);	
	}

	translate([-6.5,-6.25,-0.5])
	cube(size=[13.5,12.5,5]);
}





module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}


	