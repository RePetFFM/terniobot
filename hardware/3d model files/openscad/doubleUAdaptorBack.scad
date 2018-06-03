$fn = 60;

screwDiameter = 3.2;
screwRadius = screwDiameter/2;

screwMountWall = 2.5;

screwMountRadius = screwRadius+screwMountWall;

mountPlateHeigh = 3;

totalHeight = 20;


// topPCB dimations
PCB_top_width = 43;
PCB_top_length = 42;

// bottomPCB dimations
PCB_bottom_width = 38;
PCB_bottom_length = 51;

// offset
offset = PCB_bottom_length-PCB_top_length;
offset = 0;
bottomOffset = 7;

bottomOffset = 10;



rotate([0,-90,0])
difference() {
	union() {
		translate([-bottomOffset-0.5,PCB_bottom_width/2,totalHeight/2])
		cube(size=[4,screwMountRadius*2,totalHeight],center=true);

		translate([-bottomOffset-0.5,-PCB_bottom_width/2,totalHeight/2])
		cube(size=[4,screwMountRadius*2,totalHeight],center=true);


		union() {
			hull() {
				translate([offset,PCB_top_width/2,totalHeight-mountPlateHeigh])
				cylinder(r=screwMountRadius,h=mountPlateHeigh);
				translate([-bottomOffset,PCB_top_width/2,mountPlateHeigh/2+(totalHeight-mountPlateHeigh)])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);	
			}

			hull() {
				translate([offset,-PCB_top_width/2,totalHeight-mountPlateHeigh])
				cylinder(r=screwMountRadius,h=mountPlateHeigh);
				translate([-bottomOffset,-PCB_top_width/2,mountPlateHeigh/2+(totalHeight-mountPlateHeigh)])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);
			}

			
				translate([-bottomOffset,PCB_top_width/2,mountPlateHeigh/2+(totalHeight-mountPlateHeigh)])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);
				translate([-bottomOffset,-PCB_top_width/2,mountPlateHeigh/2+(totalHeight-mountPlateHeigh)])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);	
			
		}


		union() {
			hull() {
				translate([-bottomOffset,PCB_bottom_width/2,mountPlateHeigh/2])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);
				translate([0,PCB_bottom_width/2,0])
				cylinder(r=screwMountRadius,h=mountPlateHeigh);
			}

			hull() {
				translate([-bottomOffset,-PCB_bottom_width/2,mountPlateHeigh/2])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);
				translate([0,-PCB_bottom_width/2,0])
				cylinder(r=screwMountRadius,h=mountPlateHeigh);
			}

			hull() {
				translate([-bottomOffset,PCB_bottom_width/2,mountPlateHeigh/2])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);
				translate([-bottomOffset,-PCB_bottom_width/2,mountPlateHeigh/2])
				cube(size=[5,screwMountRadius*2,mountPlateHeigh],center=true);
			}	
		}
	}




	union() {
		// bottom front screwholes
		translate([0,PCB_bottom_width/2,-0.5])
		cylinder(r=screwRadius,h=mountPlateHeigh+1);
		translate([0,-PCB_bottom_width/2,-0.5])
		cylinder(r=screwRadius,h=mountPlateHeigh+1);

		// top front screwholes
		translate([offset,0,totalHeight-mountPlateHeigh]) {
			translate([0,PCB_top_width/2,-0.5])
			cylinder(r=screwRadius,h=mountPlateHeigh+1);
			translate([0,-PCB_top_width/2,-0.5])
			cylinder(r=screwRadius,h=mountPlateHeigh+1);
		}
	}
}




