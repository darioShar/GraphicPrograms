#define PI 3.1415926535897932384626433832795

//max bin
uniform float maxHeight;
uniform float strongestFrequency;

// Actual bin
uniform float height;
uniform float frequency; // frequency mapped to [0,1]

// previous bin
uniform float previousHeight;

// Next bin
uniform float nextHeight;

void main()
{
	if (height > 1.0) {
		gl_FragColor = vec4(1.0, 0.3, 0.0, 1.0);
	}
	else {


		float r = 1.0 * sqrt(1.0-frequency) * (1.0 - strongestFrequency);
		float g = (pow((3.0*height + previousHeight + nextHeight) / 5.0, 0.39))*(maxHeight)*(1.0 - frequency) + maxHeight;
		while (g >= 1.0) g -= maxHeight / 3.0;
		float b = ((sin(height*PI/2.0 + PI/4.0)) * pow(frequency, 2.0)) + strongestFrequency;

		gl_FragColor = vec4(r, g, b, 1.0);

		/*float r = 1.0 * sqrt(1.0-frequency);
		float g = (1.0 - pow((3*height + previousHeight + nextHeight) / 5.0, 0.39))*(1.0 - maxHeight)*(1.0 - frequency);
		float b = (((sin(height*PI/2 + PI/4)) * pow(frequency, 2.0) ) + maxHeight); */

		/*float r = 1.0 * sqrt(1.0-frequency);
		float g = 1.0 - pow((3*height + previousHeight + nextHeight) / 5.0, 0.39);//pow((height + previousHeight + nextHeight) / 3.0, 0.39);
		float b = (sin(height*PI/2 + PI/4)) * pow(frequency, 2.0);
		gl_FragColor = vec4(r, g*(1.0 - maxHeight)*(1.0 - frequency), (b + maxHeight), 1.0);*/

		/*gl_FragColor = vec4(1.0 * sqrt(frequency),
		 (1.0 -  sin(height*PI/2)) * frequency * frequency * frequency,
		  (sin(height*PI/2 + PI/4)) * frequency * frequency * frequency,
		   1.0);*/
	}
}