//fragment.frag
#version 120

uniform sampler2D img;
varying vec2 texcoord;

varying vec3 position;
varying vec3 normal;
 
uniform vec3 lightPos;
 
uniform vec3 mambient;  //gl_FrontMaterial
uniform vec3 mdiffuse;
uniform vec3 mspecular;
uniform float shininess;
 
uniform vec3 lambient;  //gl_LightSource[0]
uniform vec3 ldiffuse;
uniform vec3 lspecular;

varying vec2 v_texCoord;
varying vec2 v_blurTexCoords[14];
 
void main()
{
	vec3 texcolor=vec3(texture2D(img,texcoord));

	float dist=length(position-lightPos);   //distance from light-source to surface
	float att=1.0/(1.0+0.1*dist+0.1*dist*dist);    //attenuation (constant,linear,quadric)
	vec3 ambient=texcolor*lambient; //the ambient light
	   
	vec3 surf2light=normalize(lightPos-position);
	vec3 norm=normalize(normal);
	float dcont=max(0.0,dot(norm,surf2light));
	vec3 diffuse=dcont*(texcolor*ldiffuse);
	   
	vec3 surf2view=normalize(-position);
	vec3 reflection=reflect(-surf2light,norm);
	   
	float scont=pow(max(0.0,dot(surf2view,reflection)),shininess);
	vec3 specular=scont*lspecular*mspecular;
	   
	gl_FragColor=vec4((ambient+diffuse+specular)*att,1.0);  //<- don't forget the paranthesis (ambient+diffuse+specular)

	gl_FragColor = vec4(0.0);
    gl_FragColor += texture2D(img, v_blurTexCoords[ 0])*0.0044299121055113265;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 1])*0.00895781211794;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 2])*0.0215963866053;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 3])*0.0443683338718;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 4])*0.0776744219933;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 5])*0.115876621105;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 6])*0.147308056121;
    gl_FragColor += texture2D(img, v_texCoord         )*0.159576912161;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 7])*0.147308056121;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 8])*0.115876621105;
    gl_FragColor += texture2D(img, v_blurTexCoords[ 9])*0.0776744219933;
    gl_FragColor += texture2D(img, v_blurTexCoords[10])*0.0443683338718;
    gl_FragColor += texture2D(img, v_blurTexCoords[11])*0.0215963866053;
    gl_FragColor += texture2D(img, v_blurTexCoords[12])*0.00895781211794;
    gl_FragColor += texture2D(img, v_blurTexCoords[13])*0.0044299121055113265;

	//gl_FragColor = texcolor;
}