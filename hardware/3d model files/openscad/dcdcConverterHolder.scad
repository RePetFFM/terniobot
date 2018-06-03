$fn = 50;




		
		



difference() {
	hull() {
		translate([0,0,0])
		cylinder(h=2,r=5/2);

		translate([0,30.5,0])
		cylinder(h=2,r=5/2);
		
		translate([27+3,30.5,0])
		cylinder(h=2,r=5/2);

		translate([27+3,0,0])
		cylinder(h=2,r=5/2);
	}

	translate([0,0,-1])
	union() {
		union() {
			// boti shaft bohrung
			translate([0,0,0]) cylinder(h=4,r=3.2/2);
			translate([0,25,0]) cylinder(h=4,r=3.2/2);

			// dcdc converter bohrung
			translate([27+3,30.5,0]) cylinder(h=4,r=3.2/2);
			translate([27+3-16,0,0]) cylinder(h=4,r=3.2/2);
		}

		hull() {
			translate([8,6,0])
			cylinder(h=4,r=5/2);

			translate([8,30.5-5,0])
			cylinder(h=4,r=5/2);
			
			translate([27+3-5,30.5-5,0])
			cylinder(h=4,r=5/2);

			translate([27+3-5,6,0])
			cylinder(h=4,r=5/2);
		}
	}
}
