var render, canvas, gl;

var points =[];
var start = vec4(0,-1, 0, 1);
var end = vec4(0,-0.7, 0, 1);

var NumTimesToSubdivide = 8;
var shrink = 0.9;
var theta = Math.PI/20;

//Push first two vertices 
function draw_first_line(){
    points.push(start);
    points.push(end);
}

//Call this function to rotate direction by theta 
//Return a new normalized direction, which length is 1
function rotateVector(direction, theta){
    var rotateM = mat4(Math.cos(theta), -Math.sin(theta), 0, 0, 
        Math.sin(theta), Math.cos(theta), 0, 0,
         0, 0,1, 0,
          0,0,0,1);
    return normalize(mult(rotateM, direction))
}



//recursively call itself to push all vertices 
function addpoints(start, end, depth){
    if(depth>0){

        // get direction from start to end
        let dir = subtract(end,start);

        // get rotational directions
        let left = rotateVector(dir,theta);
        let right = rotateVector(dir, -theta);

        // get shrink
        let shr = shrink * length(dir);

        // calculate points c and d
        let d = add(end,scale(shr,left));
        let c = add(end,scale(shr,right));

        //Push (end, c) and (end, d) to the points array
        points.push(end,c,0,1);
        points.push(end,d,0,1);


        //Recursively grow the tree from two vertices end and c
        //Recursively grow the tree from two vertices end and d
        addpoints(end,c,depth-1);
        addpoints(end,d,depth-1);

    }else{
        draw_first_line();
    }  
}

window.onload = function init()  {

    canvas = document.getElementById( "gl-canvas" );

    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport( 0, 0, canvas.width, canvas.height );

    // black background
    gl.clearColor( 0.0, 0.0, 0.0, 1.0 );
    // white background
    // gl.clearColor( 1.0, 1.0, 1.0, 1.0 );

    addpoints(start, end, NumTimesToSubdivide);

    var program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    var vBufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, vBufferId );

    gl.bufferData( gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW );

    var vPosition = gl.getAttribLocation( program, "vPosition" );
    gl.vertexAttribPointer( vPosition, 4, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vPosition );

    document.getElementById("shrink" ).onclick = function () {
        shrink = document.getElementById('input1').value;
        points = [];
        init();
    };
    document.getElementById( "theta" ).onclick = function () {
        theta = document.getElementById('input2').value;
        theta = radians(theta);
        points = [];
        init();
    };

    document.getElementById("NumTimesToSubdivide" ).onclick = function () {
        NumTimesToSubdivide = document.getElementById('input3').value;
        points = [];
        init();
    };

    render();
}

render = function(){
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.drawArrays( gl.LINES, 0, points.length );
}
