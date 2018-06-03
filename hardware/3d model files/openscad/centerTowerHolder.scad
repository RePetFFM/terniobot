$fn = 40;



difference() {
	union() {
		translate([-7,0,0])
		rotate(a=[0,0,30])
		holderWing();

		translate([7,0,0])
		rotate(a=[0,0,-30])
		holderWing();	

		hull() {
			translate([7,0,0]) cylinder(h = 10, r=2.5/2);
			translate([-7,0,0]) cylinder(h = 10, r=2.5/2);
		}
	}	

	translate([0,3,4]) 
	rotate(a=[90,0,0])
	cylinder(h = 10, r=3.1/2);
	
}





module holderWing() {
	union() {
		translate([0,0,0]) cylinder(h = 10, r=2.5/2);
		translate([-2.5/2,0,0])
		difference() {
			cube(size=[2.5,20,10]);

			translate([-1,10,5]) 
			rotate(a=[90,0,90]) 
			slit(5,1.55);	
		}	
	}	
}







module slit(length,radius) {
	hull() {
		translate([length,0,0]) cylinder(h = 6, r=radius);
		translate([0,0,0]) cylinder(h = 6, r=radius);
	}
}


	