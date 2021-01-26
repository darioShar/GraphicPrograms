#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
//in vec2 fragmentUV;

out vec4 color;
uniform float time;
//uniform sampler2D mySampler;     // The texture

void main() {
	/*if (fragmentPosition.x + 1 != 0) {    // Si je suis à droite
		if (fragmentPosition.y+1 != 0) {    // Si je suis en haut
			color = vec4(fragmentColor.r * (sin(time) +1.0)*0.5 ,
					fragmentColor.g * (sin(time) +1.0)*0.5,
					fragmentColor.b, fragmentColor.a * time);
		}
		else {
			color = vec4(fragmentColor.r * (sin(time) +1.0)*0.5,
					fragmentColor.g * (sin(time) +1.0)*0.5,
					fragmentColor.b, fragmentColor.a * time);
		}
		
	}
	else if (fragmentPosition.y + 1 != 0){   // La je suis en haut à gauche
		color = vec4(fragmentColor.r,
				fragmentColor.g ,
				fragmentColor.b, fragmentColor.a * time);
	}
	else {
		color = vec4(fragmentColor.r,
				fragmentColor.g ,
				fragmentColor.b, fragmentColor.a * time);
	}*/
	/*color = vec4(fragmentColor.r,
				fragmentColor.g ,
				fragmentColor.b, fragmentColor.a * time); */
				
				// HERE IT IS FOR THE TEXTURE // vec4 textureColor = texture(mySampler, fragmentUV);
				// color = textureColor * fragmentColor;
				color = fragmentColor;
				
				/*
				color = vec4(fragmentColor.r * (cos(fragmentPosition.x + time) +1.0)*0.5,
				fragmentColor.g * (cos(fragmentPosition.y + time) +1.0)*0.5,
				fragmentColor.b * (cos(fragmentPosition.x + time) +1.0)*0.5, fragmentColor.a)
				*/
}