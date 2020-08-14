var gl, canvas;
var program;

var points = []; //Store the points for the fern
var number_points = 100000; // Number of points

var pattern_type = 0;
/*[a, b, e, c, d, f]*/
var pattern = [
	[
		[0.0, 0.0, 0.0, 0.0, 0.16, 0.0],
		[0.2, -0.26, 0.0, 0.23, 0.22, 1.6],
		[-0.15, 0.28, 0.0, 0.26, 0.24, 0.44],
		[0.75, 0.04, 0.0, -0.04, 0.85, 1.6]
	],
	[
		[0.0, 0.0, 0.0, 0.0, 0.16, 0.0],
		[0.2, -0.26, 0.0, 0.23, 0.22, 1.6],
		[-0.15, 0.28, 0.0, 0.26, 0.24, 0.44],
		[0.85, 0.04, 0.0, -0.04, 0.85, 1.6]
	]
];

var color_index = 0,
	colorLoc;

var projectionMatrix, projectionMatrix;
var left = -5;
var right = 5;
var bottom = 0;
var ytop = 10;
var near = -1;
var far = 1;

window.onload = function init() {
	// Retrieve <canvas> element
	canvas = document.getElementById("gl-canvas");

	//Get the rendering context for WebGL
	gl = WebGLUtils.setupWebGL(canvas);
	if (!gl) {
		console.log('Fill to get the rendering context for WebGL');
		return;
	};

	//Generate points
	addpoints(pattern_type);

	//Configure WebGL
	gl.clearColor(1.0, 1.0, 1.0, 1.0);

	//Initialize shaders
	program = initShaders(gl, "vertex-shader", "fragment-shader");
	gl.useProgram(program);

	// Load the data into the GPU
	var vBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW);

	var vPosition = gl.getAttribLocation(program, "vPosition");
	gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(vPosition);

	//Get the storage
	colorLoc = gl.getUniformLocation(program, "color_index");
	projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");

	var menu1 = document.getElementById("menu1");
	menu1.addEventListener("change", function () {
		switch (menu1.selectedIndex) {
			case 0:
				points = [];
				pattern_type = 0; //Pattern 1
				init();
				break;
			case 1:
				points = [];
				pattern_type = 1; //Pattern 2
				init();
				break;
		}
	});

	var menu2 = document.getElementById("menu2");
	menu2.addEventListener("change", function () {
		switch (menu2.selectedIndex) {
			case 0:
				color_index = 0; //green
				break;
			case 1:
				color_index = 1; //blue
				break;
			case 2:
				color_index = 2; //black
				break;
			case 3:
				color_index = 3; //red
				break;
			case 4:
				color_index = 4; //magenta
				break;
			case 5:
				color_index = 5; //orange
				break;
		}
	});

	//Draw the initial fern
	render();
}

/*Generate points and push it to points array: for example points.push(vec4(x, y, 0, 1));
	x will be the x representation of the new generated point
	y will be the y representation of the new generated point
*/
//pattern type: 0 pattern 1; 
//              1 pattern 2;
//The z representation of each point generate in this method is always 0.
function addpoints(pattern_type) {
	//Generate 100000 points and push them to points array

	points.push(vec4(0.0, 0.0, 0, 1)); // start with x = 0 y = 0

	// random number and options for probabilities of different constants
	var rand, a, b, c, d, e, f, choice;
	var op1 = 0.1;
	var op2 = op1 + 0.08;
	var op3 = op2 + 0.08;
	var op4 = op3 + 0.74;

	// coordinates for new point
	var x, y;

	for (var i = 0; i < number_points - 1; i++) {

		// randomly assign constants to generate points
		rand = Math.random();
		// assignment order: [a, b, e, c, d, f]
		if (rand <= op1) {
			choice = 0;
		} else if (rand > op1 && rand <= op2) {
			choice = 1;
		} else if (rand > op2 && rand <= op3) {
			choice = 2;
		} else if (rand > op3 && rand <= op4) {
			choice = 3;
		} else {
			console.log('Error generating constants for random points');
		}

		a = pattern[pattern_type][choice][0];
		b = pattern[pattern_type][choice][1];
		e = pattern[pattern_type][choice][2];
		c = pattern[pattern_type][choice][3];
		d = pattern[pattern_type][choice][4];
		f = pattern[pattern_type][choice][5];

		// create new point
		x = (a * points[i][0]) + (b * points[i][1]) + e;
		y = (c * points[i][0]) + (d * points[i][1]) + f;
		//console.log(points[i]);
		points.push(vec4(x, y, 0, 1));
	}






}

//Draw the initial fern
function render() {
	gl.clear(gl.COLOR_BUFFER_BIT);

	projectionMatrix = ortho(left, right, bottom, ytop, near, far);
	gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));
	//console.log(color_index);
	gl.uniform1i(colorLoc, color_index);
	//Draw the fern
	gl.drawArrays(gl.POINTS, 100, points.length - 100);
	requestAnimationFrame(render);
}