$fn = 50;


/*
difference(){
	cylinder(h = 15, r=19.5);	

	translate([0,0,-1])
	cylinder(h = 17, r=17.5);
}
*/


translate([0,19,0])
cylinder(h = 3, r=1.55);
translate([-10,19,0])
cylinder(h = 3, r=1.55);

translate([0,0,0])
union() {
	difference() {
		union() {
			hull() {
				translate([0,19,0])
				cylinder(h = 3, r=5);
				translate([-10,19,0])
				cylinder(h = 3, r=5);
			}
				

			hull() {
				translate([0,-19,0])
				cylinder(h = 3, r=5);
				translate([-10,-19,0])
				cylinder(h = 3, r=5);	
			}
		}
		translate([0,19,-1])
		cylinder(h = 6, r=1.55);
		translate([0,-19,-1])
		cylinder(h = 6, r=1.55);
	}



	difference() {
		translate([-10,0,5]) cube(size=[12,52,10],center=true);	

		translate([-10,0,7]) cube(size=[20,46,8],center=true);

		translate([-7,30,7])
		rotate(a=[90,0,0])
		cylinder(h = 60, r=1.55);

		translate([-12.5,30,7])
		rotate(a=[90,0,0])
		cylinder(h = 60, r=1.55);

	}
}




