$fn = 40;




union() {
	mirror([0,1,0])
	serverHolder();

	translate([0,29.7,0])
	serverHolder();

	difference() {
		hull() {
			translate([-18,-8,0]) cylinder(h = 4.5, r=2);
			translate([-18,27.7,0]) cylinder(h = 4.5, r=2);
			translate([-5,-10,0])
			cube(size=[5,20+29.7,4.5]);
		}

		translate([-7,0,-1])
		rotate(a=[0,0,90])
		slit(8,3,10);

		translate([-7,20,-1])
		rotate(a=[0,0,90])
		slit(8,3,10);
	}
	


	translate([-16,2,4])
	rotate(a=[90,45,0])
	cube(size=[5,5,3],center=true);

	translate([-16,14,4])
	rotate(a=[90,45,0])
	cube(size=[5,5,3],center=true);
	

	translate([-16,8,7])
	rotate(a=[0,-90,0])
	difference() {
		hull() {
			translate([30,0,0]) cylinder(h = 4, r=3.5);
			translate([0,0,0]) cylinder(h = 4, r=3.5);

			translate([-3.5,-7.5,0])
			#cube(size=[20,15,4]);
		}
		translate([30/2,0,0]) {
			union() {
				translate([-29/2,0,-1]) cylinder(h = 6, r=(2.5/2));
				translate([0,0,3]) cylinder(h = 2, r=(12/2));
				translate([0,0,-2]) cylinder(h = 10, r=(4/2));
				translate([29/2,0,-1]) cylinder(h = 6, r=(2.5/2));
			}
		}	
	}
}


module serverHolder() {
	difference() {
		#cube(size=[13,10,2.5]);

		#translate([13/2,-8+3.5,0])
		rotate(a=[0,0,90])
		slit(8,3.1/2,3);

		#translate([13.5,10.5,1])
		rotate(a=[0,0,45])
		cube(size=[5,5,5],center=true);	
	}
}



module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}
