$fn = 60;

motorDiameter = 28;
standWidth = 7;
standClearence = 15;

shaftDiameter = 6.2;

// motorHolder();
centerPice();

module motorHolder() {
	difference() {
		minkowski()
		{
			cube(size=[motorDiameter,(motorDiameter/2)+standClearence,standWidth-1]);
			cylinder(r=1,h=1);
		}
		

		translate([motorDiameter/2,0,-1])
		cylinder(r=motorDiameter/2,h=standWidth+2);

		translate([-3,-3,-1])
		cube(size=[motorDiameter+6,motorDiameter/4,standWidth+2]);

		translate([motorDiameter/2,(motorDiameter/2)+standClearence+1-shaftDiameter,-1])
		cylinder(r=shaftDiameter/2,h=standWidth+2);
	}
}


module centerPice() {
	difference() {
		hull() {
			translate([-shaftDiameter*1.5,20,0]) cube(size=[shaftDiameter*3,3,(shaftDiameter*2)]);

			rotate([0,0,120]) translate([-shaftDiameter*1.5,20,0]) cube(size=[shaftDiameter*3,3,(shaftDiameter*2)]);

			rotate([0,0,-120]) translate([-shaftDiameter*1.5,20,0]) cube(size=[shaftDiameter*3,3,(shaftDiameter*2)]);
		}

		hull() {
			translate([-shaftDiameter*1.5,17,-1]) cube(size=[shaftDiameter*3,3,(shaftDiameter*2)+2]);

			rotate([0,0,120]) translate([-shaftDiameter*1.5,17,-1]) cube(size=[shaftDiameter*3,3,(shaftDiameter*2)+2]);

			rotate([0,0,-120]) translate([-shaftDiameter*1.5,17,-1]) cube(size=[shaftDiameter*3,3,(shaftDiameter*2)+2]);
		}


		translate([0,0,shaftDiameter]) rotate([-90,0,0]) cylinder(r=shaftDiameter/2,h=23+2);
		translate([0,0,shaftDiameter]) rotate([-90,0,120]) cylinder(r=shaftDiameter/2,h=23+2);
		translate([0,0,shaftDiameter]) rotate([-90,0,-120]) cylinder(r=shaftDiameter/2,h=23+2);
	}
	
}