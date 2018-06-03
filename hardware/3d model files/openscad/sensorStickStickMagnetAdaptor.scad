$fn = 40;

stickDepth = 12;
stickDiameter = 2.5;

magnetDepth = 2;
magnetDiameter = 5.3;


sensorDepth  = 8;
sensorDiameter = 4.5;

screwHoleDepth = 11;
screwHoleDiameter = 3.2;

wallWidth = magnetDiameter+4;

sumHeightSensor = sensorDepth+magnetDepth+2;
sumHeightStick = stickDepth+magnetDepth+2;


stick();
// translate([0,20,0]) sensor();
// sensor();

// stick block
module stick() {
	difference() {
		slit(screwHoleDepth,wallWidth/2,sumHeightStick);

		union() {

			translate([screwHoleDepth,-wallWidth,-1])
			cube(size=[20,wallWidth*2,sumHeightStick+2]);

			translate([0,0,-0.5])
			cylinder(h = stickDepth+0.5, r=stickDiameter/2);

			// magnets
			translate([0,0,sumHeightStick-magnetDepth])
			cylinder(h = magnetDepth+0.5, r=magnetDiameter/2);

			translate([magnetDiameter+1.5,0,sumHeightStick-magnetDepth])
			cylinder(h = magnetDepth+0.5, r=magnetDiameter/2);


			// screw hole
			translate([0,0,(screwHoleDiameter/2)+4])
			rotate(a=[0,90,0])
			cylinder(h=screwHoleDepth+0.1,r=screwHoleDiameter/2);
		}
	}
}
	


// sensort block
module sensor() {
	difference() {
		slit(screwHoleDepth,wallWidth/2,sumHeightSensor);

		union() {

			translate([screwHoleDepth,-wallWidth,-1])
			cube(size=[20,wallWidth*2,sumHeightSensor+2]);

			translate([0,0,-0.5])
			cylinder(h = sensorDepth+0.5, r=sensorDiameter/2);

			// magnets
			translate([0,0,sumHeightSensor-magnetDepth])
			cylinder(h = magnetDepth+0.5, r=magnetDiameter/2);

			translate([magnetDiameter+1.5,0,sumHeightSensor-magnetDepth])
			cylinder(h = magnetDepth+0.5, r=magnetDiameter/2);


			// screw hole
			translate([0,0,(screwHoleDiameter/2)+2])
			rotate(a=[0,90,0])
			cylinder(h=screwHoleDepth+0.1,r=screwHoleDiameter/2);
		}
	}	
}






module slit(length,radius,height) {
	hull() {
		translate([length,0,0]) cylinder(h = height, r=radius);
		translate([0,0,0]) cylinder(h = height, r=radius);
	}
}


	