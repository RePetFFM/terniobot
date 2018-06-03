$fn = 40;





union() {

	difference() {
		hull() {
			translate([100,0,0]) cylinder(h = 4, r=2);
			translate([0,0,0]) cylinder(h = 4, r=2);
		}

		translate([0,0,1.95]) slit(15,2.1);		

		union() { // screw hols
			// translate([13,0,-0.5]) cylinder(h = 6, r=2/2);
			translate([1,0,-0.5]) cylinder(h = 6, r=2/2);	
		}

		translate([100,0.2,0])
		rotate(a=[0,0,180])
		union() {
			translate([2,0.5,0]) 
				hull() {
					translate([0,-5,-1]) cylinder(h = 6, r=1);
					translate([0,0,-1]) cylinder(h = 6, r=1);
				}

			translate([2,0.5,0]) {
				difference() {
					hull() {
						translate([0,0,-1]) cylinder(h = 6, r=1);
						translate([10,0,-1]) cylinder(h = 6, r=1);
					}

					translate([2,1,-1]) cylinder(h = 6, r=1);
				}
			}			
		}

		translate([2,0,0])
		union() {
			translate([20,0,-1])  {
				slit(15,1);
			}

			translate([41,0,-1])  {
				slit(15,1);
			}	

			translate([62,0,-1])  {
				slit(15,1);
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


