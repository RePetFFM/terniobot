$fn = 50;


/*
difference(){
	cylinder(h = 15, r=19.5);	

	translate([0,0,-1])
	cylinder(h = 17, r=17.5);
}
*/


rotate(a=[0,-90,0])
union() {
	difference() {
		union() {

			difference() {
				cylinder(h = 2.5, r=19.5);	

				translate([0,0,-1])
				cylinder(h = 4, r=13);

				union() {
							

					translate([0,-20,-1])
					cube(size=[40,40,10]);

					translate([-26,-20,-1])
					cube(size=[10,40,10]);	


				}
			}

			translate([0,16.25,0])
			#cylinder(h = 2.5, r=3.25);

			translate([0,-16.25,0])
			#cylinder(h = 2.5, r=3.25);
		}
		

		translate([0,0,-1])
		union() {
			rotate(a=[0,0,120])
			headMountScrewhole();

			rotate(a=[0,0,-120])
			headMountScrewhole();	
		}
	}



	difference() {
		union(){
			cylinder(h = 15, r=21);
			translate([-10,0,10+2.5]) cube(size=[12,45,5],center=true);	
		}

	translate([-7,25,12.5])
	rotate(a=[90,0,0])
	cylinder(h = 50, r=1.55);

	translate([-12.5,25,12.5])
	rotate(a=[90,0,0])
	cylinder(h = 50, r=1.55);
		
		cylinder(h = 17, r=19.5);

		translate([-26,-20,-1])
		#cube(size=[10,40,30]);	

		translate([-4,-25,-1])
		#cube(size=[40,50,30]);	
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


