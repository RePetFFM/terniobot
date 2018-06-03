$fn = 40;

translate([10,0,0]) {
	difference() {
		#slit(7,10/2,7);
		translate([0,0,-1])
		#slit(7,7/2,6);

		translate([0,0,5.25])
		#slit(7,3.1/2,7);
	}
}


translate([-10,0,0]) {
	mirror([1,0,0])
	difference() {
		#slit(7,10/2,7);
		translate([0,0,-1])
		#slit(7,7/2,6);

		translate([0,0,5.25])
		#slit(7,3.1/2,7);
	}
}





translate([-25/2,-35,0])
difference() {
	#hull() {
		translate([0,0,0]) cylinder(h = 4, r=5/2);
		translate([25,0,0]) cylinder(h = 4, r=5/2);

		translate([0,70,0]) cylinder(h = 4, r=5/2);
		translate([25,70,0]) cylinder(h = 4, r=5/2);
	}


	translate([5,5,2])
	#hull() {
		translate([0,0,0]) cylinder(h = 5, r=3/2);
		translate([15,0,0]) cylinder(h = 5, r=3/2);

		translate([0,60,0]) cylinder(h = 5, r=3/2);
		translate([15,60,0]) cylinder(h = 5, r=3/2);
	}	

	translate([5,-5,3])
	#hull() {
		translate([0,0,0]) cylinder(h = 5, r=3/2);
		translate([15,0,0]) cylinder(h = 5, r=3/2);

		translate([0,80,0]) cylinder(h = 5, r=3/2);
		translate([15,80,0]) cylinder(h = 5, r=3/2);
	}	


	translate([-16,5,-0.5])
	#hull() {
		translate([0,0,0]) cylinder(h = 5, r=3/2);
		translate([15,0,0]) cylinder(h = 5, r=3/2);

		translate([0,60,0]) cylinder(h = 5, r=3/2);
		translate([15,60,0]) cylinder(h = 5, r=3/2);
	}	

	translate([23+3,5,-0.5])
	#hull() {
		translate([0,0,0]) cylinder(h = 5, r=3/2);
		translate([15,0,0]) cylinder(h = 5, r=3/2);

		translate([0,60,0]) cylinder(h = 5, r=3/2);
		translate([15,60,0]) cylinder(h = 5, r=3/2);
	}	

	translate([25/2,35,0])
	union() {
		translate([10,0,0]) {
			translate([0,0,-1])
			#slit(7,7/2,6);
		}
		translate([-10,0,0]) {
			mirror([1,0,0])
			translate([0,0,-1])
			#slit(7,7/2,6);
		}	
	}

}


/*
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
*/




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







module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}


	