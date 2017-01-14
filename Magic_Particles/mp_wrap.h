//-----------------------------------------------------------------------------
// File: mp_wrap.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES_OPENGL
#define MAGIC_PARTICLES_OPENGL

#include <stdlib.h>
#include "mp.h"

#ifdef COCOS_2DX
	#include "cocos2d.h"
	#include "platform/CCPlatformConfig.h"
	#include <CCGL.h>

	#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
		#define internalformat GL_RGBA
	#else
		#define internalformat GL_RGBA8
	#endif

	#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
		#include <android/asset_manager.h>
	#endif
#else
	#ifdef _WINDOWS
		#include <windows.h>
		#include <gl/gl.h>
		#define internalformat GL_RGBA8
		#define DIST_CAMERA_3D  -840
	#endif

	#ifdef __APPLE__
		#include "TargetConditionals.h"
		#if (TARGET_OS_IPHONE==1) || (TARGET_IPHONE_SIMULATOR==1)
			#include <stdlib.h>
			#ifdef OPENGL_SHADER
				#import <OpenGLES/ES2/gl.h>
				#import <OpenGLES/ES2/glext.h>
			#else
				#import <OpenGLES/ES1/gl.h>
				#import <OpenGLES/ES1/glext.h>
			#endif
			#define internalformat GL_RGBA
			#define DIST_CAMERA_3D  -2520
		#else
			#if (TARGET_OS_MAC==1)
				#include <GLUT/glut.h>
				#define internalformat GL_RGBA8
				#define DIST_CAMERA_3D  -840
			#endif
		#endif
	#endif

	#if defined(__ANDROID__)
		#include <jni.h>
		#include <android/asset_manager.h>
		#include <android/log.h>

		#define  LOG_TAG    "libwpapper_android"
		#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
		#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>

		#define internalformat GL_RGBA
		#define DIST_CAMERA_3D  -2520
	#endif
#endif

#ifdef OPENIL
#include "IL/ilu.h"
#endif


struct OPENGL_VERTEX
{
	GLfloat x,y;
	#ifdef MAGIC_3D
	GLfloat z;
    #endif
	GLfloat u,v;
	GLuint color;
};

// eng: Class of vertexes buffer
// rus: Класс буфера вершин
class OpenGLBuffer
{
protected:
    #ifdef OPENGL_SHADER
    // VBO buffers
    GLuint buffers[2];
    #endif
        
    int k_particles;
    int max_particles;
        
    OPENGL_VERTEX* m_vertex;
        
    GLushort* m_index;
        
public:
    OpenGLBuffer()
    {
        k_particles=0;
        max_particles=0;
            
        m_vertex=NULL;
        m_index=NULL;
            
        #ifdef OPENGL_SHADER
        glGenBuffers(2, buffers);
        #endif
    }
        
    virtual ~OpenGLBuffer()
    {
        if (m_vertex)
        {
            free(m_vertex);
            m_vertex=NULL;
        }
            
        if (m_index)
        {
            free(m_index);
            m_index=NULL;
        }
            
        k_particles=0;
        max_particles=0;
            
        #ifdef OPENGL_SHADER
        glDeleteBuffers(2, buffers);
        #endif
    }
        
    // rus: Создает буфер указанного размера
    void Create(int count)
    {
        if (count>max_particles)
        {
            m_vertex=(OPENGL_VERTEX*)realloc(m_vertex,sizeof(OPENGL_VERTEX)*count*4);
            m_index=(GLushort*)realloc(m_index,sizeof(GLushort)*count*6);
                
            for (int i=max_particles;i<count;i++)
            {
                int vertex=i*4;
                GLushort* ind=&(m_index[i*6]);
                *ind=vertex;
                ind++;
                *ind=vertex+1;
                ind++;
                *ind=vertex+2;
                ind++;
                *ind=vertex+3;
                ind++;
                *ind=vertex;
                ind++;
                *ind=vertex+2;
                ind++;
            }

            max_particles=count;

            #ifdef OPENGL_SHADER
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * count * 6, m_index, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            #endif
        }
        k_particles=0;
    };
        
    // rus: Возвращает количество частиц в буфере
    int GetCount(){return k_particles;}
        
    // rus: Возвращает вершинный буфер
    OPENGL_VERTEX* GetArray(){return m_vertex;}
        
    // rus: Возвращает индексный буфер
    GLushort* GetIndexArray(){return m_index;}
        
    // rus: Возвращает указатель на очередную вершину
    OPENGL_VERTEX* NextParticles()
    {
        OPENGL_VERTEX* vert=&(m_vertex[k_particles*4]);
        k_particles++;
        return vert;
    }
        
    #ifdef OPENGL_SHADER
    // put all data in GPU
    void CopyBuffersToGPU()
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(OPENGL_VERTEX) * k_particles * 4, m_vertex, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
        
    }
    #endif
};


struct BUFFER_ATTRIBUTES
{
	void* pointer;
	GLint enabled;
	GLint size;
	GLint type;
	GLint normalized;
	GLint stride;
	GLint buffer_binding;
};

// eng: Class storing the textural atlas
// rus: Класс, который хранит текстурный атлас
class MP_Atlas_WRAP : public MP_Atlas
{
protected:
	GLuint texture;

public:
	MP_Atlas_WRAP(int width, int height, const char* file);

	// eng: Destroy atlas texture
	// rus: Уничтожить текстуру атласа
	virtual void Destroy();

	// eng: Loading of frame texture
	// rus: Загрузка текстурного кадра
	virtual void LoadTexture(const MAGIC_CHANGE_ATLAS* c);

	// eng: Cleaning up of rectangle
	// rus: Очистка прямоугольника
	virtual void CleanRectangle(const MAGIC_CHANGE_ATLAS* c);

	// eng: Particle drawing
	// rus: Отрисовка частицы
	virtual void Draw(MAGIC_PARTICLE_VERTEXES* vertex_rectangle);

	// eng: Beginning of emitter drawing
	// rus: Начало отрисовки эмиттера
	virtual void BeginDrawEmitter(MP_Emitter* emitter);

	// eng: Beginning of particles drawing
	// rus: Начало отрисовки частиц
	virtual void BeginDrawParticles(int particles_count);

	// eng: End of particles drawing
	// rus: Конец отрисовки частиц
	virtual void EndDrawParticles();

	// eng: Setting of intense
	// rus: Установить интенсивность
	virtual void SetIntense(bool intense);
};

// eng: Class controlling drawing
// rus: Класс, который управляет рисованием
class MP_Device_WRAP : public MP_Device
{
protected:
	#ifdef OPENGL_WIN
	HWND hwnd;
	HDC	hDC;		// eng: Private GDI Device Context
	HGLRC hRC;		// eng: Permanent Rendering Context
	int bits;
	#endif

	MAGIC_MATRIX view, projection;

	#ifdef OPENGL_SHADER
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;
	#endif

	OpenGLBuffer buffer;

	#ifdef OPENGL_SAVE_ATTRIBUTES
	// attributes
	int prev_texture;
	GLboolean attr_GL_BLEND;
	GLboolean attr_GL_DEPTH_WRITEMASK;
	GLboolean attr_GL_DEPTH_TEST;

	#ifdef OPENGL_SHADER
	GLint prev_program;

	GLint attr_GL_BLEND_SRC_ALPHA;
	GLint attr_GL_BLEND_DST_ALPHA;
	GLint attr_GL_ARRAY_BUFFER_BINDING;
	GLint attr_GL_ELEMENT_ARRAY_BUFFER_BINDING;

	GLint attr_ATTRIB_VERTEX;
	GLint attr_ATTRIB_COLOR;
	GLint attr_ATTRIB_TEXTUREPOSITION;

	BUFFER_ATTRIBUTES attr_vertex_buffer;
	BUFFER_ATTRIBUTES attr_color_buffer;
	BUFFER_ATTRIBUTES attr_textureposition_buffer;
	#else
	GLboolean attr_GL_TEXTURE_2D;
	GLboolean attr_GL_COLOR_MATERIAL;
	GLboolean attr_GL_LIGHTING;
	GLboolean attr_GL_VERTEX_ARRAY;
	GLboolean attr_GL_TEXTURE_COORD_ARRAY;
	GLboolean attr_GL_COLOR_ARRAY;
	#endif
	#endif

public:

    #ifndef OPENGL_WIN
	MP_Device_WRAP(int width, int height);
    #else
    MP_Device_WRAP(int width, int height, HWND hwnd, int bits);
    #endif
    
	// eng: Creating
	// rus: Создание
	virtual bool Create();

	// eng: Destroying
	// rus: Уничтожение
	virtual void Destroy();

	// eng: Setting of scene
	// rus: Настройка сцены
	virtual void SetScene3d();

	// eng: Beginning of scene drawing
	// rus: Начало отрисовки сцены
	virtual void BeginScene();

	// eng: End of scene drawing
	// rus: Конец отрисовки сцены
	virtual void EndScene();

	// eng: Indicates that device is lost
	// rus: Проверка на потерю устройства рисования
	virtual bool IsLost();

	virtual void SetWorldMatrix(MAGIC_MATRIX* m);
	virtual void SetViewMatrix(MAGIC_MATRIX* m);
	virtual void SetProjectionMatrix(MAGIC_MATRIX* m);

	virtual void SetViewport(MP_VIEWPORT* viewport);

	#ifdef OPENIL
	// eng: Loading texture from file
	// rus: Загрузка текстуры из файла
	virtual void LoadTextureFromFile(const char* file_name);
	#else
	unsigned char* LoadTextureFromFile(const char* file_name, int* width, int* height);
	#endif
    
    OpenGLBuffer& GetBuffer(){return buffer;}

	#ifdef OPENGL_SAVE_ATTRIBUTES
	void SaveAttributes();
	void RestoreAttributes();
	#endif
};


#endif