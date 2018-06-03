$fn = 40;



difference() {
	union() {
		servoAdapter();
		thumbStickHousingBottom();	
	}
	
	translate([0,0,2.5+1.5]) thumbStick();
	translate([0,0,-1]) cylinder(h = 6, r=(10/2));	
}


module thumbStickHousingBottom() {
	hull() {
		translate([-16/2,-20/2,2])
		cube(size=[16,20,5]);

		translate([-12/2,-16/2,0])
		cube(size=[12,16,5]);	
	}

	difference() {
		union() {
			slit(10,3.5,16+2.5+1.5);
			rotate(a=[0,0,90]) slit(9+7,3.5,16+2.5+1.5);
		}

		translate([10,0,12]) cylinder(h = 10, r=3.2/2);

		translate([0,16,12]) cylinder(h = 10, r=3.2/2);
		

		translate([17,0,10])
		rotate(a=[-90,0,0])
		hull() {
			rotate(a=[0,0,45])
			cube(size=[10,10,10],center=true);

			translate([0,20,0])
			rotate(a=[0,0,45])
			cube(size=[10,10,10],center=true);	
		}	


		#translate([0,20,0])
		rotate(a=[-90,0,90])
		hull() {
			rotate(a=[0,0,45])
			cube(size=[10,10,10],center=true);

			translate([0,20,0])
			rotate(a=[0,0,45])
			cube(size=[10,10,10],center=true);	
		}	
	}
}

module thumbStick() {
	#union() {
		translate([0,0,8])
		cube(size=[12.4,16.4,16],center=true);

		translate([0,-4.2,-1.7])
		cube(size=[6.2,8.4,2]);

		translate([-6.2-5,-8.2,1])
		cube(size=[5,16.4,15]);

		translate([6.2-4.5,16.3/2,0])
		cube(size=[4.5,2,3]);

		translate([-12.5/2,16.3/2,3])
		cube(size=[12.5,4.5,13]);	

		// bottom pin
		#mirror([0,1,0])
		translate([12/2,16.4/2-3,0])
		cube(size=[10,3,16]);	

		translate([12/2,16.4/2-3,0])
		cube(size=[10,3,16]);	

		// pressbutton space
		translate([-12.4/2,-16.4/2-5,2])
		cube(size=[12.4,5,14]);
	}
}



module servoAdapter() {
	difference() {
		union() {
			hull() {
				translate([15,0,0]) cylinder(h = 2.5, r=3.5);
				translate([-15,0,0]) cylinder(h = 2.5, r=3.5);
			}
			translate([0,0,0]) cylinder(h = 2.5, r=(14/2));
		}
 		
		translate([0,0,0]) {
			union() {
				translate([29/2,0,-1]) cylinder(h = 6, r=(2.5/2));
				
				translate([-29/2,0,-1]) cylinder(h = 6, r=(2.5/2));	
			}
		}	
	}
}
	


module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}


	