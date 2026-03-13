var VSHADER_SOURCE =
	"attribute vec4 a_Position;\n" +
	"attribute float a_PointSize;\n" +
	"void main() {\n" +
	"  gl_Position = a_Position;\n" +
	"  gl_PointSize = a_PointSize;\n" +
	"}\n";

var FSHADER_SOURCE =
	"void main() {\n" + "  gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n" + "}\n";

// Replacement for the textbook's getWebGLContext(canvas)
function getWebGLContext(canvas) {
	return (
		canvas.getContext("webgl") || canvas.getContext("experimental-webgl")
	);
}

// Replacement for the textbook's initShaders(gl, vSource, fSource)
function initShaders(gl, vSource, fSource) {
	var vShader = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vShader, vSource);
	gl.compileShader(vShader);
	if (!gl.getShaderParameter(vShader, gl.COMPILE_STATUS)) {
		console.log("Vertex shader error: " + gl.getShaderInfoLog(vShader));
		return false;
	}

	var fShader = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fShader, fSource);
	gl.compileShader(fShader);
	if (!gl.getShaderParameter(fShader, gl.COMPILE_STATUS)) {
		console.log("Fragment shader error: " + gl.getShaderInfoLog(fShader));
		return false;
	}

	var program = gl.createProgram();
	gl.attachShader(program, vShader);
	gl.attachShader(program, fShader);
	gl.linkProgram(program);
	if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
		console.log("Program link error: " + gl.getProgramInfoLog(program));
		return false;
	}

	gl.program = program; // mirrors what the textbook library does
	gl.useProgram(program);
	return true;
}

var g_points = [];
function click(ev, gl, canvas, a_Position) {
	var x = ev.clientX;
	var y = ev.clientY;

	var rect = ev.target.getBoundingClientRect();

	x = (x - rect.left - canvas.width / 2) / (canvas.width / 2);
	y = (canvas.height / 2 - (y - rect.top)) / (canvas.height / 2);

	g_points.push(x);
	g_points.push(y);

	// Redraw all points
	gl.clear(gl.COLOR_BUFFER_BIT);
	for (var i = 0; i < g_points.length; i += 2) {
		gl.vertexAttrib3f(a_Position, g_points[i], g_points[i + 1], 0.0);
		gl.drawArrays(gl.POINTS, 0, 1);
	}
}

function main() {
	// Retrieve <canvas> element
	var canvas = document.getElementById("webgl");

	// Get the rendering context for WebGL
	var gl = getWebGLContext(canvas);
	if (!gl) {
		console.log("Failed to get the rendering context for WebGL");
		return;
	}

	// Initialize shaders
	if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
		console.log("Failed to initialize shaders.");
		return;
	}

	// Set the color for clearing <canvas>
	gl.clearColor(0.0, 0.0, 0.0, 1.0);

	canvas.addEventListener("click", (ev) => {
		console.log("clicked!", ev);
		click(ev, gl, canvas, a_Position);
	});

	// Clear <canvas>
	gl.clear(gl.COLOR_BUFFER_BIT);

	var a_Position = gl.getAttribLocation(gl.program, "a_Position");
	var size = gl.getAttribLocation(gl.program, "a_PointSize");

	gl.vertexAttrib1f(size, 10.0);

	console.log("Rockn' Roll,Morning Light falls on You".slice(7, 17));
}

main();
