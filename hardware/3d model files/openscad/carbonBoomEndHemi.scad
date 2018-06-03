$fn = 50;

shaftDiameter = 3.8;
shaftDepth = 10;
shaftThickness = 1.5;
hemisphereDiameter = 15;
wallThickness = 1.2;
bottomBevelHeight = 1.5;

hemisphereRadius = hemisphereDiameter/2;

// baseOuterShape();
// baseInnerShape();


union() {
	difference() {
		baseOuterShape();
		baseInnerShape();
	}


	difference() {
		intersection() {
			baseOuterShape();

			union() {
				cylinder(h=hemisphereRadius+bottomBevelHeight,r=(shaftDiameter/2)+shaftThickness);	
				translate([0,0,(hemisphereRadius+bottomBevelHeight)/2])
					cube(size=[hemisphereDiameter,wallThickness,hemisphereRadius+bottomBevelHeight],center = true);

				rotate(a=[0,0,90])	translate([0,0,(hemisphereRadius+bottomBevelHeight)/2])
					cube(size=[hemisphereDiameter,wallThickness,hemisphereRadius+bottomBevelHeight],center = true);
			}
		}	
		
		translate([0,0,-1])
		cylinder(h=shaftDepth+1,r=(shaftDiameter/2));	
	}	
}





module baseOuterShape(){
	translate([0,0,bottomBevelHeight])
	difference() {
		sphere(hemisphereRadius);
		
		translate([0,0,-(hemisphereRadius/2)-0.5])
		cube(size=[hemisphereDiameter+2,hemisphereDiameter+2,hemisphereRadius+1],center = true);
	}

	translate([0,0,bottomBevelHeight/2])
		cylinder(h=bottomBevelHeight,r1=(hemisphereRadius-bottomBevelHeight),r2=hemisphereRadius, center=true);	

}

module baseInnerShape() {
	translate([0,0,bottomBevelHeight])
	difference() {
		sphere(hemisphereRadius-wallThickness);
		translate([0,0,-(hemisphereRadius/2)-0.5])
		cube(size=[hemisphereDiameter+2,hemisphereDiameter+2,hemisphereRadius+1],center = true);
	}
	translate([0,0,bottomBevelHeight/2])
		cylinder(h=bottomBevelHeight+0.1,r1=(hemisphereRadius-bottomBevelHeight)-wallThickness,r2=hemisphereRadius-wallThickness, center=true);
}