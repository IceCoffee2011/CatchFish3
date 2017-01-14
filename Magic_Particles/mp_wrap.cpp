//-----------------------------------------------------------------------------
// File: mp_wrap.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include "mp_wrap.h"

#ifdef COCOS_2DX
	using namespace cocos2d;
#else
	#ifdef _WINDOWS
	#include <windows.h>
	#pragma comment(lib, "opengl32.lib")
	#endif
#endif


#ifdef OPENIL
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#else
#include "stb_image.cc"
#endif

#pragma warning ( disable : 4996)

#ifdef OPENGL_SHADER
// attribute index
enum ATTRIBUTES_INDEX {
	ATTRIB_VERTEX,
	ATTRIB_COLOR,
	ATTRIB_TEXTUREPOSITION,
	NUM_ATTRIBUTES
};
#endif

#define offset_of(st, m) ((size_t)(&((st *)0)->m))

// ru: Перемножение матриц 3х3
// en: Matrixes 3x3 multiplication
void MagicMatrixMul3(MAGIC_MATRIX* c, const MAGIC_MATRIX* a, const MAGIC_MATRIX* b)
{
	c->_11=a->_11*b->_11+a->_12*b->_21+a->_13*b->_31;
	c->_21=a->_21*b->_11+a->_22*b->_21+a->_23*b->_31;
	c->_31=a->_31*b->_11+a->_32*b->_21+a->_33*b->_31;

	c->_12=a->_11*b->_12+a->_12*b->_22+a->_13*b->_32;
	c->_22=a->_21*b->_12+a->_22*b->_22+a->_23*b->_32;
	c->_32=a->_31*b->_12+a->_32*b->_22+a->_33*b->_32;

	c->_13=a->_11*b->_13+a->_12*b->_23+a->_13*b->_33;
	c->_23=a->_21*b->_13+a->_22*b->_23+a->_23*b->_33;
	c->_33=a->_31*b->_13+a->_32*b->_23+a->_33*b->_33;
}

void MagicMatrixMul(MAGIC_MATRIX* c, const MAGIC_MATRIX* a, const MAGIC_MATRIX* b)
{
	c->_11=a->_11*b->_11+a->_12*b->_21+a->_13*b->_31+a->_14*b->_41;
	c->_21=a->_21*b->_11+a->_22*b->_21+a->_23*b->_31+a->_24*b->_41;
	c->_31=a->_31*b->_11+a->_32*b->_21+a->_33*b->_31+a->_34*b->_41;
	c->_41=a->_41*b->_11+a->_42*b->_21+a->_43*b->_31+a->_44*b->_41;

	c->_12=a->_11*b->_12+a->_12*b->_22+a->_13*b->_32+a->_14*b->_42;
	c->_22=a->_21*b->_12+a->_22*b->_22+a->_23*b->_32+a->_24*b->_42;
	c->_32=a->_31*b->_12+a->_32*b->_22+a->_33*b->_32+a->_34*b->_42;
	c->_42=a->_41*b->_12+a->_42*b->_22+a->_43*b->_32+a->_44*b->_42;

	c->_13=a->_11*b->_13+a->_12*b->_23+a->_13*b->_33+a->_14*b->_43;
	c->_23=a->_21*b->_13+a->_22*b->_23+a->_23*b->_33+a->_24*b->_43;
	c->_33=a->_31*b->_13+a->_32*b->_23+a->_33*b->_33+a->_34*b->_43;
	c->_43=a->_41*b->_13+a->_42*b->_23+a->_43*b->_33+a->_44*b->_43;

	c->_14=a->_11*b->_14+a->_12*b->_24+a->_13*b->_34+a->_14*b->_44;
	c->_24=a->_21*b->_14+a->_22*b->_24+a->_23*b->_34+a->_24*b->_44;
	c->_34=a->_31*b->_14+a->_32*b->_24+a->_33*b->_34+a->_34*b->_44;
	c->_44=a->_41*b->_14+a->_42*b->_24+a->_43*b->_34+a->_44*b->_44;
}

// Транспонирование матрицы (смена местами строк и столбцов)
void MagicMatrixTransposing(MAGIC_MATRIX* m)
{
	MAGIC_MATRIX v=*m;

	m->_12=v._21;
	m->_13=v._31;
	m->_14=v._41;

	m->_21=v._12;
	m->_23=v._32;
	m->_24=v._42;

	m->_31=v._13;
	m->_32=v._23;
	m->_34=v._43;

	m->_41=v._14;
	m->_42=v._24;
	m->_43=v._34;
}


#ifdef OPENGL_SHADER
void GetVertexBufferAttributes(GLuint index, BUFFER_ATTRIBUTES* bc)
{
		glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &(bc->pointer));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &(bc->enabled));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &(bc->size));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &(bc->type));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &(bc->normalized));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &(bc->stride));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &(bc->buffer_binding));
}
#endif

void BindTexture(GLuint textureId)
{
	#ifdef COCOS_2DX
	ccGLBindTexture2D(textureId);
	#else
	glBindTexture(GL_TEXTURE_2D, textureId);
	#endif
}

// eng: Class storing the textural atlas
// rus: Класс, который хранит текстурный атлас
MP_Atlas_WRAP::MP_Atlas_WRAP(int width, int height, const char* file) : MP_Atlas(width, height, file)
{
	int prev_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

	glGenTextures(1, &texture);
	BindTexture(texture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    #ifndef OPENGL_SHADER
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    #endif

	if (file_name.empty())
	{
		int size=width*height*4;
		unsigned char* null_data=new unsigned char[size];

		for (int i=0;i<size;i++)
			null_data[i]=0;

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, null_data);

		delete []null_data;
		null_data=NULL;
	}
	else
	{
		#ifdef OPENIL
		ILuint il_texture;
		ilGenImages(1,&il_texture);
		ilBindImage(il_texture);

		device->LoadTextureFromFile(file_name.c_str());

		ILubyte* pixels=ilGetData();
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		ilDeleteImages(1,&il_texture);
		il_texture=0;
		#else
		int wi, he;
		unsigned char* pixels=device->LoadTextureFromFile(file_name.c_str(), &wi, &he);
		if (pixels)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			stbi_image_free(pixels);
		}
		#endif
	}
	BindTexture(prev_texture);
}

// eng: Destroy atlas texture
// rus: Уничтожить текстуру атласа
void MP_Atlas_WRAP::Destroy()
{
	if (texture)
	{
		glDeleteTextures(1, &texture);
		texture=0;
	}
}

// eng: Loading of frame texture
// rus: Загрузка текстурного кадра
void MP_Atlas_WRAP::LoadTexture(const MAGIC_CHANGE_ATLAS* c)
{
	int prev_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

	#ifdef OPENIL
	ILuint frame;
	ilGenImages(1,&frame);
	ilBindImage(frame);

	if (c->data)
	{
		// eng: loading from memory
		// rus: загрузка из памяти
		ilLoadL(IL_TYPE_UNKNOWN,c->data,c->length);
	}
	else
	{
		// eng: loading from file
		// rus: загрузка из файла
		device->LoadTextureFromFile(c->file);
	}

	ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE);
 
	int depth=ilGetInteger(IL_IMAGE_DEPTH);
	int wi_frame=ilGetInteger(IL_IMAGE_WIDTH);
	int he_frame=ilGetInteger(IL_IMAGE_HEIGHT);
	if (wi_frame!=c->width || he_frame!=c->height)
		iluScale(c->width, c->height, depth);

	ILuint* pixels=(ILuint*)ilGetData();

	BindTexture(texture);
	GLint unpack_alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D,0,c->x,c->y,c->width,c->height,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

	ilDeleteImages(1,&frame);
	#else
	
	unsigned char* pixels=NULL;
	int wi_frame, he_frame;
	if (c->data)
	{
		// eng: loading from memory
		// rus: загрузка из памяти
		int n;
		pixels=stbi_load_from_memory((const unsigned char*)c->data, c->length, &wi_frame, &he_frame, &n, 4);
		if (!pixels)
		{
            #ifdef OPENGL_WIN
			const char* error=stbi_failure_reason();
			MessageBox(NULL, error, "Magic Particles", MB_OK | MB_ICONERROR);
			#endif
		}
	}
	else
	{
		// eng: loading from file
		// rus: загрузка из файла
		pixels=device->LoadTextureFromFile(c->file, &wi_frame, &he_frame);
	}

	if (wi_frame!=c->width || he_frame!=c->height)
	{
		// масштабируем текстуру
		int size=c->width*c->height*4;
		unsigned int* to=(unsigned int*)malloc(size);
		unsigned int* from=(unsigned int*)pixels;

		float scale_x=((float)c->width)/wi_frame;
		float scale_y=((float)c->height)/he_frame;

		int pitch_to=c->width;
		int pitch_from=wi_frame;
		
		for (int i=0;i<c->width;i++)
		{
			for (int j=0;j<c->height;j++)
			{
				int i2=(int)(((float)i)/scale_x);
				int j2=(int)(((float)j)/scale_y);

				unsigned int color=from[j2*pitch_from+i2];
				to[j*pitch_to+i]=color;
			}
		}

		stbi_image_free(pixels);
		pixels=(unsigned char*)to;
	}

	BindTexture(texture);
	GLint unpack_alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D,0,c->x,c->y,c->width,c->height,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

	stbi_image_free(pixels);
	#endif

	BindTexture(prev_texture);
}

// eng: Cleaning up of rectangle
// rus: Очистка прямоугольника
void MP_Atlas_WRAP::CleanRectangle(const MAGIC_CHANGE_ATLAS* c)
{
	int prev_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

	BindTexture(texture);

	int size=c->width*c->height*4;
	unsigned char* null_data=new unsigned char[size];
	for (int i=0;i<size;i++)
		null_data[i]=0;

	GLint unpack_alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D,0,c->x,c->y,c->width,c->height,GL_RGBA,GL_UNSIGNED_BYTE,null_data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

	delete []null_data;
	null_data=NULL;

	BindTexture(prev_texture);
}

// eng: Particle drawing
// rus: Отрисовка частицы
void MP_Atlas_WRAP::Draw(MAGIC_PARTICLE_VERTEXES* vertexes)
{
    OpenGLBuffer& buffer=device->GetBuffer();
 
	OPENGL_VERTEX* vertex=buffer.NextParticles();
	GLuint color=vertexes->color;
	// eng: exchange Red and Blue
	// rus: меняем местами красную и синюю цветовые компоненты
	GLubyte* to=(GLubyte*)&color;
	GLubyte* from=(GLubyte*)&(vertexes->color);
	to[0]=from[2];
	to[2]=from[0];

	#ifdef MAGIC_3D
	vertex->x=vertexes->vertex1.x;
	vertex->y=vertexes->vertex1.y;
	vertex->u=vertexes->u1;
	vertex->v=vertexes->v1;
	vertex->color=color;
	vertex->z=vertexes->vertex1.z;
	vertex++;

	vertex->x=vertexes->vertex2.x;
	vertex->y=vertexes->vertex2.y;
	vertex->u=vertexes->u2;
	vertex->v=vertexes->v2;
	vertex->color=color;
	vertex->z=vertexes->vertex2.z;
	vertex++;

	vertex->x=vertexes->vertex3.x;
	vertex->y=vertexes->vertex3.y;
	vertex->u=vertexes->u3;
	vertex->v=vertexes->v3;
	vertex->color=color;
	vertex->z=vertexes->vertex3.z;
	vertex++;

	vertex->x=vertexes->vertex4.x;
	vertex->y=vertexes->vertex4.y;
	vertex->u=vertexes->u4;
	vertex->v=vertexes->v4;
	vertex->color=color;
	vertex->z=vertexes->vertex4.z;
	vertex++;

	#else
	vertex->x=vertexes->vertex1.x;
	vertex->y=vertexes->vertex1.y;
	vertex->u=vertexes->u1;
	vertex->v=vertexes->v1;
	vertex->color=color;
	vertex++;

	vertex->x=vertexes->vertex2.x;
	vertex->y=vertexes->vertex2.y;
	vertex->u=vertexes->u2;
	vertex->v=vertexes->v2;
	vertex->color=color;
	vertex++;

	vertex->x=vertexes->vertex3.x;
	vertex->y=vertexes->vertex3.y;
	vertex->u=vertexes->u3;
	vertex->v=vertexes->v3;
	vertex->color=color;
	vertex++;

	vertex->x=vertexes->vertex4.x;
	vertex->y=vertexes->vertex4.y;
	vertex->u=vertexes->u4;
	vertex->v=vertexes->v4;
	vertex->color=color;
	vertex++;
	#endif
};

// eng: Setting of intense
// rus: Установить интенсивность
void MP_Atlas_WRAP::SetIntense(bool intense)
{
	if (intense)
	{
		// eng: turn on intense
		// rus: включить интенсивность
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
	}
	else
	{
		// eng: turn off intense
		// rus: отключить интенсивность
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

// eng: Beginning of emitter drawing
// rus: Начало отрисовки эмиттера
void MP_Atlas_WRAP::BeginDrawEmitter(MP_Emitter* emitter)
{
	BindTexture(texture);
}

// eng: Beginning of particles drawing
// rus: Начало отрисовки частиц
void MP_Atlas_WRAP::BeginDrawParticles(int particles_count)
{
    OpenGLBuffer& buffer=device->GetBuffer();
	buffer.Create(particles_count);
}

// eng: End of particles drawing
// rus: Конец отрисовки частиц
void MP_Atlas_WRAP::EndDrawParticles()
{
	MP_Atlas::EndDrawParticles();
    OpenGLBuffer& buffer=device->GetBuffer();
	int k_particles=buffer.GetCount();
	if (k_particles)
	{
		#ifdef OPENGL_SHADER
        
        buffer.CopyBuffersToGPU();
		GLsizei step=sizeof(OPENGL_VERTEX);
        
        int offset_pos=offset_of(OPENGL_VERTEX, x);
        int offset_color=offset_of(OPENGL_VERTEX, color);
        int offset_uv=offset_of(OPENGL_VERTEX, u);

        #ifdef MAGIC_3D
		glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, step, (GLvoid*) offset_pos);
		#else
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, step, (GLvoid*) offset_pos);
		#endif
		glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, step, (GLvoid*) offset_color); //enable the normalized flag
		glVertexAttribPointer(ATTRIB_TEXTUREPOSITION, 2, GL_FLOAT, 0, step,  (GLvoid*) offset_uv);

		glDrawElements(GL_TRIANGLES, k_particles * 6, GL_UNSIGNED_SHORT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        
		#else
        
		OPENGL_VERTEX* ar=buffer.GetArray();

		GLsizei step=sizeof(OPENGL_VERTEX);

		float* first_x=&(ar->x);

		#ifdef MAGIC_3D
		glVertexPointer(3,GL_FLOAT,step,first_x);
		#else
		glVertexPointer(2,GL_FLOAT,step,first_x);
		#endif

		float* first_u=&(ar->u);
		glTexCoordPointer(2,GL_FLOAT,step,first_u);

		GLuint* first_color=&(ar->color);
		glColorPointer(4,GL_UNSIGNED_BYTE,step,first_color);

		glDrawElements(GL_TRIANGLES, k_particles*6, GL_UNSIGNED_SHORT, buffer.GetIndexArray());
		#endif
	}
}

// --------------------------------------------------------------------------------

// eng: Class controlling drawing
// rus: Класс, который управляет рисованием
#ifndef OPENGL_WIN
MP_Device_WRAP::MP_Device_WRAP(int width, int height) : MP_Device(width, height)
#else
MP_Device_WRAP::MP_Device_WRAP(int width, int height, HWND hwnd, int bits) : MP_Device(width, height)
#endif
{
	#ifdef OPENIL
	ilInit();
	#endif

    #ifdef OPENGL_WIN
	this->hwnd=hwnd;
	hDC=NULL;
	hRC=NULL;
	this->bits=bits;
    #endif


	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (i==j)
				view.m[i][j]=1;
			else
				view.m[i][j]=0;
		}
	}

	projection=view;


	#ifdef OPENGL_SHADER
	vertex_shader=fragment_shader=0;
	program=0;
    
    uniform_view_matrix=uniform_projection_matrix=0xffffffff;
	#endif

	#ifdef OPENGL_SAVE_ATTRIBUTES
	prev_texture=-1;
	#endif
}

bool MP_Device_WRAP::Create()
{
	Destroy();

	#ifdef OPENGL_WIN
	GLuint		PixelFormat;

	static	PIXELFORMATDESCRIPTOR pfd=				// eng: pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// eng: Size Of This Pixel Format Descriptor
		1,											// eng: Version Number
		PFD_DRAW_TO_WINDOW |						// eng: Format Must Support Window
		PFD_SUPPORT_OPENGL |						// eng: Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// eng: Must Support Double Buffering
		PFD_TYPE_RGBA,								// eng: Request An RGBA Format
		bits,										// eng: Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// eng: Color Bits Ignored
		0,											// eng: No Alpha Buffer
		0,											// eng: Shift Bit Ignored
		0,											// eng: No Accumulation Buffer
		0, 0, 0, 0,									// eng: Accumulation Bits Ignored
		0,											// eng: No Z-Buffer (Depth Buffer)  
		0,											// eng: No Stencil Buffer
		0,											// eng: No Auxiliary Buffer
		PFD_MAIN_PLANE,								// eng: Main Drawing Layer
		0,											// eng: Reserved
		0, 0, 0										// eng: Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hwnd)))							// eng: Did We Get A Device Context?
	{
		Destroy();
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// eng: Did Windows Find A Matching Pixel Format?
	{
		Destroy();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: eturn FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// eng: Are We Able To Set The Pixel Format?
	{
		Destroy();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// eng: Are We Able To Get A Rendering Context?
	{
		Destroy();
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// eng: Try To Activate The Rendering Context
	{
		Destroy();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	RECT client_rect;
	GetClientRect(hwnd,&client_rect);
	window_width=client_rect.right;
	window_height=client_rect.bottom;
	#endif


	#ifdef OPENGL_SHADER
	#ifndef COCOS_2DX
	// vertex shader
	vertex_shader=glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_code;
	vertex_shader_code+="attribute vec4 position;";
	vertex_shader_code+="attribute vec4 color;";
	vertex_shader_code+="attribute vec2 inputTextureCoordinate;";
	vertex_shader_code+="uniform mat4 modelViewMatrix;";
	vertex_shader_code+="uniform mat4 projectionMatrix;";
	vertex_shader_code+="varying vec4 colorVarying;";
	vertex_shader_code+="varying vec2 textureCoordinate;";
	vertex_shader_code+="void main()";
	vertex_shader_code+="{";
	//#ifdef MAGIC_3D
	vertex_shader_code+="gl_Position = projectionMatrix * modelViewMatrix * position;";
	//#else
	//vertex_shader_code+="gl_Position = position;";
	//#endif
	vertex_shader_code+="colorVarying = color;";
	vertex_shader_code+="textureCoordinate = inputTextureCoordinate.xy;";
	vertex_shader_code+="}";
    
    const char* code=vertex_shader_code.c_str();
	const GLchar** code_ptr=&code;

	glShaderSource(vertex_shader, 1, code_ptr, 0);
	glCompileShader(vertex_shader);
	char log[1000];
	int len=0;
	glGetShaderInfoLog(vertex_shader, 1000, &len, log);

	// fragment shader
	fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_code;
	fragment_shader_code+="#ifdef GL_ES\r\n";
	fragment_shader_code+="// define default precision for float, vec, mat.\r\n";
	fragment_shader_code+="precision lowp float;\r\n";
	fragment_shader_code+="#endif\r\n";
	fragment_shader_code+="varying vec4 colorVarying;\r\n";
	fragment_shader_code+="#ifdef GL_ES\r\n";
	fragment_shader_code+="varying lowp vec2 textureCoordinate;\r\n";
	fragment_shader_code+="#else\r\n";
	fragment_shader_code+="varying vec2 textureCoordinate;\r\n";
	fragment_shader_code+="#endif\r\n";
	fragment_shader_code+="uniform sampler2D texture;";
	fragment_shader_code+="void main()";
	fragment_shader_code+="{";
	fragment_shader_code+="vec4 a = texture2D(texture, textureCoordinate);";
	fragment_shader_code+="gl_FragColor = a * colorVarying;";
	fragment_shader_code+="}";
    
    code=fragment_shader_code.c_str();
	code_ptr=&code;

	glShaderSource(fragment_shader, 1, code_ptr, 0);
	glCompileShader(fragment_shader);

	glGetShaderInfoLog(fragment_shader, 1000, &len, log);

	// program
	program=glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glBindAttribLocation(program, ATTRIB_VERTEX, "position");
	glBindAttribLocation(program, ATTRIB_COLOR, "color");
	glBindAttribLocation(program, ATTRIB_TEXTUREPOSITION, "inputTextureCoordinate");

    glLinkProgram(program);
    
	uniform_view_matrix=glGetUniformLocation(program, "modelViewMatrix");
	uniform_projection_matrix=glGetUniformLocation(program, "projectionMatrix");

	#endif
	#endif

	return true;
}

// eng: Destroying
// rus: Уничтожение
void MP_Device_WRAP::Destroy()
{
	MP_Device::Destroy();

	#ifdef OPENGL_WIN
	if (hRC)											// eng: Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// eng: Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// eng: Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// eng: Set RC To NULL
	}

	if (hDC && !ReleaseDC(hwnd,hDC))					// eng: Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// eng: Set DC To NULL
	}
	#endif

	#ifdef OPENGL_SHADER
	#ifndef COCOS_2DX
	if (vertex_shader)
	{
		glDeleteShader(vertex_shader);
		vertex_shader=0;
	}

	if (fragment_shader)
	{
		glDeleteShader(fragment_shader);
		fragment_shader=0;
	}

	if (program)
	{
		glDeleteProgram(program);
		program=0;
	}
	#endif
	#endif
}

// eng: Setting of scene
// rus: Настройка сцены
void MP_Device_WRAP::SetScene3d()
{
    #ifdef OPENGL_WIN
	// eng: viewport
	// rus: вьюпорт
	MP_VIEWPORT view_port;
	view_port.x=0;
	view_port.y=0;
	view_port.width=(unsigned int)window_width;
	view_port.height=(unsigned int)window_height;
	view_port.min_z=0.f;
	view_port.max_z=1.f;
	SetViewport(&view_port);
    #endif

	// eng: projection matrix
	// rus: проектная матрица
	MAGIC_MATRIX proj;
	#ifdef MAGIC_3D
	MatrixPerspectiveFovLH(&proj, (float)(MAGIC_PI * 0.15f), ((float)window_width)/((float)window_height), 0.0f, 1000.0f);
	#else
	MatrixOrthoLH(&proj, (float)window_width, (float)window_height, -10000.f, 10000.f);
	#endif
	SetProjectionMatrix(&proj);

	// eng: view matrix
	// rus: матрица вида
	MAGIC_MATRIX view;
    #ifdef MAGIC_3D
    float dist=DIST_CAMERA_3D;
    #else
    float dist=-840.f;
    #endif
	MP_POSITION eye(0.f,0.f,dist);
	MP_POSITION at(0.f,0.f,0.f);
	MP_POSITION up(0.f,1.f,0.f);

	MatrixLookAtLH(&view,&eye,&at,&up);
	SetViewMatrix(&view);

	#ifdef MAGIC_3D
	MAGIC_CAMERA camera;
	camera.pos.x=eye.x;
	camera.pos.y=eye.y;
	camera.pos.z=eye.z;
	camera.dir.x=at.x-camera.pos.x;
	camera.dir.y=at.y-camera.pos.y;
	camera.dir.z=at.z-camera.pos.z;
	Magic_SetCamera(&camera);
	#endif
}

// eng: Beginning of scene drawing
// rus: Начало отрисовки сцены
void MP_Device_WRAP::BeginScene()
{
	MP_Device::BeginScene();

	#ifdef OPENGL_SAVE_ATTRIBUTES
	SaveAttributes();
	#endif

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	MAGIC_MATRIX world;
	MatrixIdentity(&world);

	#ifdef OPENGL_SHADER
	// нужно подключить шейдер

	glUseProgram(program);

	glEnableVertexAttribArray(ATTRIB_VERTEX);

	glEnableVertexAttribArray(ATTRIB_COLOR);

	glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITION);

    #ifndef MAGIC_3D
    world._41=(float)(-window_width/2);
    world._42=(float)(window_height/2);
	if (Magic_GetAxis()==MAGIC_pXnY)
		world._42=-world._42;
    #endif

	MAGIC_MATRIX view_world;
	MagicMatrixMul(&view_world, &view, &world);

	glUniformMatrix4fv(uniform_view_matrix, 1, false, (float*)&view_world);
	glUniformMatrix4fv(uniform_projection_matrix, 1, false, (float*)&projection);

	#else
	SetWorldMatrix(&world);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//glEnableClientState(GL_INDEX_ARRAY);
	#endif

	#ifdef OPENGL_WIN
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// eng: Black Background
	glClear(GL_COLOR_BUFFER_BIT);			// eng: Clear Screen
	#endif
}

// eng: End of scene drawing
// rus: Конец отрисовки сцены
void MP_Device_WRAP::EndScene()
{
	#ifdef OPENGL_SAVE_ATTRIBUTES
	RestoreAttributes();
	#endif

	#ifdef OPENGL_WIN
	SwapBuffers(hDC);						// eng: Swap Buffers (Double Buffering)
	#endif
}

// eng: Indicates that device is lost
// rus: Проверка на потерю устройства рисования
bool MP_Device_WRAP::IsLost()
{
	return false;
}

void MP_Device_WRAP::SetWorldMatrix(MAGIC_MATRIX* m)
{
    #ifndef OPENGL_SHADER
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((const float*)m);
	#ifndef MAGIC_3D
	glTranslatef(-window_width/2.f,-window_height/2.f,0);
	#endif
    #endif
}

void MP_Device_WRAP::SetViewMatrix(MAGIC_MATRIX* m)
{
	view=*m;

    #ifndef OPENGL_SHADER
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const float*)&projection);
	glMultMatrixf((const float*)&view);
    #endif
}

void MP_Device_WRAP::SetProjectionMatrix(MAGIC_MATRIX* m)
{
	projection=*m;

    #ifdef OPENGL_WIN
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const float*)&projection);
	glMultMatrixf((const float*)&view);
    #endif
}

void MP_Device_WRAP::SetViewport(MP_VIEWPORT* viewport)
{
	glViewport(viewport->x,viewport->y,viewport->width,viewport->height);
}

#ifdef OPENIL
// eng: Loading texture from file
// rus: Загрузка текстуры из файла
void MP_Device_WRAP::LoadTextureFromFile(const char* file_name)
{
	std::string texture_file=MP->GetPathToTexture();
	texture_file+=file_name;

	std::string mode="rb";
	const wchar_t* pmode=MP->utf8_to_wchar(mode.c_str());
	wchar_t wmode[3];
	wmode[0]=pmode[0];
	wmode[1]=pmode[1];
	wmode[2]=0;

	const wchar_t* wfile=MP->utf8_to_wchar(texture_file.c_str());
	
	FILE* file=_wfopen(wfile, wmode);
	fseek(file,0,SEEK_END);
	long len=ftell(file);
	fseek(file,0,SEEK_SET);

	char* data=new char[len];

	fread(data,1,len,file);
	fclose(file);

	ilLoadL(IL_TYPE_UNKNOWN,data,len);

	delete []data;
}
#else
unsigned char* MP_Device_WRAP::LoadTextureFromFile(const char* file_name, int* width, int* height)
{
	std::string texture_file=MP->GetPathToTexture();
	texture_file+=file_name;

	#if defined(__ANDROID__) && !defined(COCOS_2DX)
		extern AAssetManager* asset_mgr;
		AAsset* file=AAssetManager_open(asset_mgr, texture_file.c_str(), AASSET_MODE_UNKNOWN);
		long len=AAsset_getLength(file);
		unsigned char* data=new unsigned char[len];
		AAsset_read(file,data,len);
		AAsset_close(file);

	#else

		#ifdef _WINDOWS

			std::string mode="rb";
			const wchar_t* pmode=MP->utf8_to_wchar(mode.c_str());
			wchar_t wmode[3];
			wmode[0]=pmode[0];
			wmode[1]=pmode[1];
			wmode[2]=0;

			const wchar_t* wfile=MP->utf8_to_wchar(texture_file.c_str());

			FILE* file=_wfopen(wfile, wmode);

		#else

			FILE* file=fopen(texture_file.c_str(), "rb");

		#endif

		fseek(file,0,SEEK_END);
		long len=ftell(file);
		fseek(file,0,SEEK_SET);

		unsigned char* data=new unsigned char[len];

		fread(data,1,len,file);
		fclose(file);

	#endif

	int n;
	unsigned char* pixels=stbi_load_from_memory(data, len, width, height, &n, 4);
	if (!pixels)
	{
	    #ifdef OPENGL_WIN
		const char* error=stbi_failure_reason();
		MessageBox(NULL, error, "Magic Particles", MB_OK | MB_ICONERROR);
		#endif
	}

	delete []data;
	return pixels;
}

#endif

#ifdef OPENGL_SAVE_ATTRIBUTES
void MP_Device_WRAP::SaveAttributes()
{
	glGetBooleanv(GL_BLEND, &attr_GL_BLEND);

	glGetIntegerv(GL_BLEND_SRC_ALPHA, &attr_GL_BLEND_SRC_ALPHA);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &attr_GL_BLEND_DST_ALPHA);

	glGetBooleanv(GL_DEPTH_WRITEMASK, &attr_GL_DEPTH_WRITEMASK);
	glGetBooleanv(GL_DEPTH_TEST, &attr_GL_DEPTH_TEST);
	
	#ifdef OPENGL_SHADER
	glGetIntegerv(GL_CURRENT_PROGRAM, &prev_program);

    glGetVertexAttribiv(ATTRIB_VERTEX, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attr_ATTRIB_VERTEX);
	glGetVertexAttribiv(ATTRIB_COLOR, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attr_ATTRIB_COLOR);
	glGetVertexAttribiv(ATTRIB_TEXTUREPOSITION, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attr_ATTRIB_TEXTUREPOSITION);
	
	//GetVertexBufferAttributes(ATTRIB_VERTEX, &attr_vertex_buffer);
	//GetVertexBufferAttributes(ATTRIB_COLOR, &attr_color_buffer);
	//GetVertexBufferAttributes(ATTRIB_TEXTUREPOSITION, &attr_textureposition_buffer);

	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &attr_GL_ARRAY_BUFFER_BINDING);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &attr_GL_ELEMENT_ARRAY_BUFFER_BINDING);
		
	#else
	glGetBooleanv(GL_TEXTURE_2D, &attr_GL_TEXTURE_2D);
	glGetBooleanv(GL_COLOR_MATERIAL, &attr_GL_COLOR_MATERIAL);
	glGetBooleanv(GL_LIGHTING, &attr_GL_LIGHTING);
	glGetBooleanv(GL_VERTEX_ARRAY, &attr_GL_VERTEX_ARRAY);
	glGetBooleanv(GL_TEXTURE_COORD_ARRAY, &attr_GL_TEXTURE_COORD_ARRAY);
	glGetBooleanv(GL_COLOR_ARRAY, &attr_GL_COLOR_ARRAY);
	#endif

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);
}

void MP_Device_WRAP::RestoreAttributes()
{
	if (attr_GL_BLEND)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendFunc(attr_GL_BLEND_SRC_ALPHA, attr_GL_BLEND_DST_ALPHA);
	
	if (attr_GL_DEPTH_WRITEMASK)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	if (attr_GL_DEPTH_TEST)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	#ifdef OPENGL_SHADER
	glUseProgram(prev_program);
    
	if (attr_ATTRIB_VERTEX)
		glEnableVertexAttribArray(ATTRIB_VERTEX);
	else
		glDisableVertexAttribArray(ATTRIB_VERTEX);

	if (attr_ATTRIB_COLOR)
		glEnableVertexAttribArray(ATTRIB_COLOR);
	else
		glDisableVertexAttribArray(ATTRIB_COLOR);

	if (attr_ATTRIB_TEXTUREPOSITION)
		glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITION);
	else
		glDisableVertexAttribArray(ATTRIB_TEXTUREPOSITION);

	//BUFFER_ATTRIBUTES* s=&attr_vertex_buffer;
	//glVertexAttribPointer(ATTRIB_VERTEX, s->size, s->type, s->normalized, s->stride,  s->pointer);
	//s=&attr_color_buffer;
	//glVertexAttribPointer(ATTRIB_COLOR, s->size, s->type, s->normalized, s->stride,  s->pointer);
	//s=&attr_textureposition_buffer;
	//glVertexAttribPointer(ATTRIB_TEXTUREPOSITION, s->size, s->type, s->normalized, s->stride,  s->pointer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attr_GL_ELEMENT_ARRAY_BUFFER_BINDING);
	glBindBuffer(GL_ARRAY_BUFFER, attr_GL_ARRAY_BUFFER_BINDING);

	#else
	if (attr_GL_TEXTURE_2D)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	if (attr_GL_COLOR_MATERIAL)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);

	if (attr_GL_LIGHTING)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if (attr_GL_VERTEX_ARRAY)
		glEnableClientState(GL_VERTEX_ARRAY);
	else
		glDisableClientState(GL_VERTEX_ARRAY);

	if (attr_GL_TEXTURE_COORD_ARRAY)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (attr_GL_COLOR_ARRAY)
		glEnableClientState(GL_COLOR_ARRAY);
	else
		glDisableClientState(GL_COLOR_ARRAY);

	//glEnableClientState(GL_INDEX_ARRAY);
	#endif

	BindTexture(prev_texture);
}
#endif

