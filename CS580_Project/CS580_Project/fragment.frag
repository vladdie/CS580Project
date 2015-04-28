//fragment.frag
#version 120

uniform sampler2D img;
varying vec2 texcoord;

varying vec3 position;
varying vec3 normal;

const float blurclamp = .02;
const float bias = 0.05;
const float focus = 1;
 
void main()
{
	float aspectratio = 1200.0/700.0;
	vec2 aspcorr = vec2(1.0, aspectratio);

	float highlim = focus + 1.0;
	float lowlim = focus - 1.0;

	float factor = (position.z - focus);

	if (position.z < highlim && position.z > lowlim) {
		factor = 0.0;
	} else {
		factor = (position.z - focus);
	}

	vec2 dofblur = vec2 (clamp( factor * bias, -blurclamp, blurclamp ));

	vec4 col = vec4(0.0);
       
    col += texture2D(img, texcoord);
    col += texture2D(img, texcoord + (vec2( 0.0,0.4 ) * aspcorr) * dofblur);
    col += texture2D(img, texcoord + (vec2( 0.15,0.37 ) * aspcorr) * dofblur);
    col += texture2D(img, texcoord + (vec2( 0.29,0.29 ) * aspcorr) * dofblur);
    col += texture2D(img, texcoord + (vec2( -0.37,0.15 ) * aspcorr) * dofblur);       
    col += texture2D(img, texcoord + (vec2( 0.4,0.0 ) * aspcorr) * dofblur);   
    col += texture2D(img, texcoord + (vec2( 0.37,-0.15 ) * aspcorr) * dofblur);       
    col += texture2D(img, texcoord + (vec2( 0.29,-0.29 ) * aspcorr) * dofblur);       
    col += texture2D(img, texcoord + (vec2( -0.15,-0.37 ) * aspcorr) * dofblur);
    col += texture2D(img, texcoord + (vec2( 0.0,-0.4 ) * aspcorr) * dofblur); 
    col += texture2D(img, texcoord + (vec2( -0.15,0.37 ) * aspcorr) * dofblur);
    col += texture2D(img, texcoord + (vec2( -0.29,0.29 ) * aspcorr) * dofblur);
    col += texture2D(img, texcoord + (vec2( 0.37,0.15 ) * aspcorr) * dofblur); 
    col += texture2D(img, texcoord + (vec2( -0.4,0.0 ) * aspcorr) * dofblur); 
    col += texture2D(img, texcoord + (vec2( -0.37,-0.15 ) * aspcorr) * dofblur);       
    col += texture2D(img, texcoord + (vec2( -0.29,-0.29 ) * aspcorr) * dofblur);       
    col += texture2D(img, texcoord + (vec2( 0.15,-0.37 ) * aspcorr) * dofblur);
       
    col += texture2D(img, texcoord + (vec2( 0.15,0.37 ) * aspcorr) * dofblur*0.9);
    col += texture2D(img, texcoord + (vec2( -0.37,0.15 ) * aspcorr) * dofblur*0.9);           
    col += texture2D(img, texcoord + (vec2( 0.37,-0.15 ) * aspcorr) * dofblur*0.9);           
    col += texture2D(img, texcoord + (vec2( -0.15,-0.37 ) * aspcorr) * dofblur*0.9);
    col += texture2D(img, texcoord + (vec2( -0.15,0.37 ) * aspcorr) * dofblur*0.9);
    col += texture2D(img, texcoord + (vec2( 0.37,0.15 ) * aspcorr) * dofblur*0.9);            
    col += texture2D(img, texcoord + (vec2( -0.37,-0.15 ) * aspcorr) * dofblur*0.9);   
    col += texture2D(img, texcoord + (vec2( 0.15,-0.37 ) * aspcorr) * dofblur*0.9);   
       
    col += texture2D(img, texcoord + (vec2( 0.29,0.29 ) * aspcorr) * dofblur*0.7);
    col += texture2D(img, texcoord + (vec2( 0.4,0.0 ) * aspcorr) * dofblur*0.7);       
    col += texture2D(img, texcoord + (vec2( 0.29,-0.29 ) * aspcorr) * dofblur*0.7);   
    col += texture2D(img, texcoord + (vec2( 0.0,-0.4 ) * aspcorr) * dofblur*0.7);     
    col += texture2D(img, texcoord + (vec2( -0.29,0.29 ) * aspcorr) * dofblur*0.7);
    col += texture2D(img, texcoord + (vec2( -0.4,0.0 ) * aspcorr) * dofblur*0.7);     
    col += texture2D(img, texcoord + (vec2( -0.29,-0.29 ) * aspcorr) * dofblur*0.7);   
    col += texture2D(img, texcoord + (vec2( 0.0,0.4 ) * aspcorr) * dofblur*0.7);
                         
    col += texture2D(img, texcoord + (vec2( 0.29,0.29 ) * aspcorr) * dofblur*0.4);
    col += texture2D(img, texcoord + (vec2( 0.4,0.0 ) * aspcorr) * dofblur*0.4);       
    col += texture2D(img, texcoord + (vec2( 0.29,-0.29 ) * aspcorr) * dofblur*0.4);   
    col += texture2D(img, texcoord + (vec2( 0.0,-0.4 ) * aspcorr) * dofblur*0.4);     
    col += texture2D(img, texcoord + (vec2( -0.29,0.29 ) * aspcorr) * dofblur*0.4);
    col += texture2D(img, texcoord + (vec2( -0.4,0.0 ) * aspcorr) * dofblur*0.4);     
    col += texture2D(img, texcoord + (vec2( -0.29,-0.29 ) * aspcorr) * dofblur*0.4);   
    col += texture2D(img, texcoord + (vec2( 0.0,0.4 ) * aspcorr) * dofblur*0.4);       
                       
    gl_FragColor = col/41.0;
    gl_FragColor.a = 1.0;

	// The below code removes the depth of field effect - Do not uncomment it
	//vec3 texcolor=vec3(texture2D(img,texcoord));
	//gl_FragColor = texcolor;
}