$fn = 40;




union() {
	difference() {
		hull() {
			translate([110,0,0]) cylinder(h = 4, r=3);
			translate([0,0,0]) cylinder(h = 4, r=3);
		}

		translate([-2,0,1.95]) slit(17,2.1);		

		union() { // screw hols
			translate([13,0,-0.5]) cylinder(h = 6, r=2/2);
			translate([1,0,-0.5]) cylinder(h = 6, r=2/2);	
		}

		union() {
			translate([20,0,-1])  {
				slit(15,1);
			}

			translate([40,0,-1])  {
				slit(15,1);
			}

			translate([60,0,-1])  {
				slit(15,1);
			}	

			translate([80,0,-1])  {
				slit(15,1);
			}	
		}

		
	}


	difference() {
		union() {
			hull() {
				translate([150,0,0]) cylinder(h = 4, r=3.5);
				translate([110,0,0]) cylinder(h = 4, r=3.5);
			}
			translate([115+29/2,0,0]) cylinder(h = 4, r=(14/2));
		}
 		
		translate([115,0,0]) {
			union() {
				translate([0,0,-1]) cylinder(h = 6, r=(2.5/2));
				translate([29/2,0,3]) cylinder(h = 2, r=(12/2));
				translate([29/2,0,-1]) cylinder(h = 5, r=(6/2));
				translate([29,0,-1]) cylinder(h = 6, r=(2.5/2));	
			}
		}	
	}
}


module slit(length,radius) {
	hull() {
		translate([length,0,0]) cylinder(h = 6, r=radius);
		translate([0,0,0]) cylinder(h = 6, r=radius);
	}
}


	