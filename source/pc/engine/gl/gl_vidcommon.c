#include "quakedef.h"
#ifdef GLQUAKE
#include "glquake.h"
#include "gl_draw.h"
#include "shader.h"

#ifndef GL_STATIC
//standard gles2 opengl calls.
void (APIENTRY *qglBlendFunc) (GLenum sfactor, GLenum dfactor);
void (APIENTRY *qglClear) (GLbitfield mask);
void (APIENTRY *qglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (APIENTRY *qglClearStencil) (GLint s);
void (APIENTRY *qglColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void (APIENTRY *qglCopyTexImage2D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (APIENTRY *qglCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *qglCullFace) (GLenum mode);
void (APIENTRY *qglDepthFunc) (GLenum func);
void (APIENTRY *qglDepthMask) (GLboolean flag);
void (APIENTRY *qglDepthRangef) (GLclampf zNear, GLclampf zFar);
void (APIENTRY *qglDisable) (GLenum cap);
void (APIENTRY *qglEnable) (GLenum cap);
void (APIENTRY *qglFinish) (void);
void (APIENTRY *qglFlush) (void);
void (APIENTRY *qglGenTextures) (GLsizei n, GLuint *textures);
void (APIENTRY *qglGetBooleanv) (GLenum pname, GLboolean *params);
GLenum (APIENTRY *qglGetError) (void);
void (APIENTRY *qglGetFloatv) (GLenum pname, GLfloat *params);
void (APIENTRY *qglGetIntegerv) (GLenum pname, GLint *params);
const GLubyte * (APIENTRY *qglGetString) (GLenum name);
void (APIENTRY *qglHint) (GLenum target, GLenum mode);
GLboolean (APIENTRY *qglIsEnabled) (GLenum cap);
void (APIENTRY *qglPolygonOffset) (GLfloat factor, GLfloat units);
void (APIENTRY *qglReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void (APIENTRY *qglTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *qglTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *qglTexParameteri) (GLenum target, GLenum pname, GLint param);
void (APIENTRY *qglTexParameterf) (GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *qglTexParameteriv) (GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *qglTexParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *qglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *qglDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (APIENTRY *qglDrawArrays) (GLenum mode, GLint first, GLsizei count);
void (APIENTRY *qglScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *qglStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
void (APIENTRY *qglStencilFunc) (GLenum func, GLint ref, GLuint mask);
void (APIENTRY *qglDeleteTextures) (GLsizei n, const GLuint *textures);

void (APIENTRY *qglGenFramebuffersEXT)(GLsizei n, GLuint* ids);
void (APIENTRY *qglDeleteFramebuffersEXT)(GLsizei n, const GLuint* ids);
void (APIENTRY *qglBindFramebufferEXT)(GLenum target, GLuint id);
void (APIENTRY *qglDeleteRenderbuffersEXT)(GLsizei n, const GLuint* ids);
void (APIENTRY *qglFramebufferTexture2DEXT)(GLenum target, GLenum attachmentPoint, GLenum textureTarget, GLuint textureId, GLint  level);
FTEPFNGLVERTEXATTRIBPOINTER			qglVertexAttribPointer;
FTEPFNGLGETVERTEXATTRIBIV			qglGetVertexAttribiv;
FTEPFNGLENABLEVERTEXATTRIBARRAY		qglEnableVertexAttribArray;
FTEPFNGLDISABLEVERTEXATTRIBARRAY	qglDisableVertexAttribArray;
void (APIENTRY *qglStencilOpSeparateATI) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
void (APIENTRY *qglGetFramebufferAttachmentParameteriv)(GLenum  target,  GLenum  attachment,  GLenum  pname,  GLint * params);
void (APIENTRY *qglGetVertexAttribPointerv) (GLuint index, GLenum pname, GLvoid* *pointer);

//quick hack that made quake work on both 1+ext and 1.1 gl implementations.
BINDTEXFUNCPTR qglBindTexture;

/*glslang - arb_shader_objects
gl core uses different names/distinctions from the extension
*/
FTEPFNGLCREATEPROGRAMOBJECTARBPROC	qglCreateProgramObjectARB;
FTEPFNGLDELETEOBJECTARBPROC			qglDeleteProgramObject_;
FTEPFNGLDELETEOBJECTARBPROC			qglDeleteShaderObject_;
FTEPFNGLUSEPROGRAMOBJECTARBPROC		qglUseProgramObjectARB;
FTEPFNGLCREATESHADEROBJECTARBPROC	qglCreateShaderObjectARB;
FTEPFNGLSHADERSOURCEARBPROC			qglShaderSourceARB;
FTEPFNGLCOMPILESHADERARBPROC		qglCompileShaderARB;
FTEPFNGLGETOBJECTPARAMETERIVARBPROC	qglGetShaderParameteriv_;
FTEPFNGLGETOBJECTPARAMETERIVARBPROC	qglGetProgramParameteriv_;
FTEPFNGLATTACHOBJECTARBPROC			qglAttachObjectARB;
FTEPFNGLGETINFOLOGARBPROC			qglGetShaderInfoLog_;
FTEPFNGLGETINFOLOGARBPROC			qglGetProgramInfoLog_;
FTEPFNGLLINKPROGRAMARBPROC			qglLinkProgramARB;
FTEPFNGLBINDATTRIBLOCATIONARBPROC	qglBindAttribLocationARB;
FTEPFNGLGETATTRIBLOCATIONARBPROC	qglGetAttribLocationARB;
FTEPFNGLGETUNIFORMLOCATIONARBPROC	qglGetUniformLocationARB;
FTEPFNGLUNIFORMMATRIXPROC			qglUniformMatrix4fvARB;
FTEPFNGLUNIFORM4FARBPROC			qglUniform4fARB;
FTEPFNGLUNIFORM4FVARBPROC			qglUniform4fvARB;
FTEPFNGLUNIFORM3FARBPROC			qglUniform3fARB;
FTEPFNGLUNIFORM3FVARBPROC			qglUniform3fvARB;
FTEPFNGLUNIFORM4FVARBPROC			qglUniform2fvARB;
FTEPFNGLUNIFORM1IARBPROC			qglUniform1iARB;
FTEPFNGLUNIFORM1FARBPROC			qglUniform1fARB;
FTEPFNGLGETSHADERSOURCEARBPROC		qglGetShaderSource;
#endif
FTEPFNGLUNIFORMMATRIXPROC			qglUniformMatrix3x4fv;
FTEPFNGLUNIFORMMATRIXPROC			qglUniformMatrix4x3fv;


//GL_ARB_occlusion_query
void (APIENTRY *qglGenQueriesARB)(GLsizei n, GLuint *ids);
void (APIENTRY *qglDeleteQueriesARB)(GLsizei n, const GLuint *ids);
//extern GLboolean (APIENTRY *qglIsQueryARB)(GLuint id);
void (APIENTRY *qglBeginQueryARB)(GLenum target, GLuint id);
void (APIENTRY *qglEndQueryARB)(GLenum target);
//extern void (APIENTRY *qglGetQueryivARB)(GLenum target, GLenum pname, GLint *params);
//extern void (APIENTRY *qglGetQueryObjectivARB)(GLuint id, GLenum pname, GLint *params);
void (APIENTRY *qglGetQueryObjectuivARB)(GLuint id, GLenum pname, GLuint *params);


//GL_OES_get_program_binary
void (APIENTRY *qglGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
void (APIENTRY *qglProgramBinary)(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length);
#define GL_PROGRAM_BINARY_LENGTH	0x8741

//standard 1.1 opengl calls
void (APIENTRY *qglAlphaFunc) (GLenum func, GLclampf ref);
void (APIENTRY *qglBegin) (GLenum mode);
void (APIENTRY *qglCallList) (GLuint list);
void (APIENTRY *qglClearDepth) (GLclampd depth);
void (APIENTRY *qglClipPlane) (GLenum plane, const GLdouble *equation);
//void (APIENTRY *qglColor3f) (GLfloat red, GLfloat green, GLfloat blue);
//void (APIENTRY *qglColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
void (APIENTRY *qglColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRY *qglColor4fv) (const GLfloat *v);
//void (APIENTRY *qglColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
//void (APIENTRY *qglColor4ubv) (const GLubyte *v);
void (APIENTRY *qglDepthRange) (GLclampd zNear, GLclampd zFar);
void (APIENTRY *qglDrawBuffer) (GLenum mode);
void (APIENTRY *qglDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *qglEnd) (void);
void (APIENTRY *qglEndList) (void);
void (APIENTRY *qglFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLuint (APIENTRY *qglGenLists) (GLsizei range);
void (APIENTRY *qglLoadIdentity) (void);
void (APIENTRY *qglLoadMatrixf) (const GLfloat *m);
void (APIENTRY *qglNormal3f) (GLfloat nx, GLfloat ny, GLfloat nz);
void (APIENTRY *qglNormal3fv) (const GLfloat *v);
void (APIENTRY *qglMatrixMode) (GLenum mode);
void (APIENTRY *qglMultMatrixf) (const GLfloat *m);
void (APIENTRY *qglNewList) (GLuint list, GLenum mode);
//void (APIENTRY *qglOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void (APIENTRY *qglPolygonMode) (GLenum face, GLenum mode);
void (APIENTRY *qglPopMatrix) (void);
void (APIENTRY *qglPushMatrix) (void);
void (APIENTRY *qglReadBuffer) (GLenum mode);
void (APIENTRY *qglRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglScalef) (GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglShadeModel) (GLenum mode);
void (APIENTRY *qglTexCoord1f) (GLfloat s);
void (APIENTRY *qglTexCoord2f) (GLfloat s, GLfloat t);
void (APIENTRY *qglTexCoord2fv) (const GLfloat *v);
void (APIENTRY *qglTexEnvf) (GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *qglTexEnvfv) (GLenum target, GLenum pname, const GLfloat *param);
void (APIENTRY *qglTexEnvi) (GLenum target, GLenum pname, GLint param);
void (APIENTRY *qglTexGeni) (GLenum coord, GLenum pname, GLint param);
void (APIENTRY *qglTexGenfv) (GLenum coord, GLenum pname, const GLfloat *param);
void (APIENTRY *qglTexImage3D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *qglTranslatef) (GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglVertex2f) (GLfloat x, GLfloat y);
void (APIENTRY *qglVertex3f) (GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglVertex3fv) (const GLfloat *v);
void (APIENTRY *qglGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint *params);
void (APIENTRY *qglGetTexEnviv) (GLenum target, GLenum pname, GLint *params);

void (APIENTRY *qglDrawRangeElements) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);
void (APIENTRY *qglArrayElement) (GLint i);
void (APIENTRY *qglVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *qglNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *qglTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *qglColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *qglDisableClientState) (GLenum array);
void (APIENTRY *qglEnableClientState) (GLenum array);

void (APIENTRY *qglPushAttrib) (GLbitfield mask);
void (APIENTRY *qglPopAttrib) (void);

GLenum (APIENTRY *qglGetGraphicsResetStatus) (void);

void (APIENTRY *qglFogf) (GLenum pname, GLfloat param);
void (APIENTRY *qglFogi) (GLenum pname, GLint param);
void (APIENTRY *qglFogfv) (GLenum pname, const GLfloat *params);

#ifndef GL_STATIC
void (APIENTRY *qglGenBuffersARB)(GLsizei n, GLuint* ids);
void (APIENTRY *qglDeleteBuffersARB)(GLsizei n, GLuint* ids);
void (APIENTRY *qglBindBufferARB)(GLenum target, GLuint id);
void (APIENTRY *qglBufferDataARB)(GLenum target, GLsizei size, const void* data, GLenum usage);
void (APIENTRY *qglBufferSubDataARB)(GLenum target, GLint offset, GLsizei size, void* data);
void *(APIENTRY *qglMapBufferARB)(GLenum target, GLenum access);
GLboolean (APIENTRY *qglUnmapBufferARB)(GLenum target);
#endif

void (APIENTRY *qglGenVertexArrays)(GLsizei n, GLuint *arrays);
void (APIENTRY *qglBindVertexArray)(GLuint vaoarray);

const GLubyte * (APIENTRY * qglGetStringi) (GLenum name, GLuint index);
void (APIENTRY * qglGetPointerv) (GLenum pname, GLvoid **parms);

void (APIENTRY *qglDrawBuffers)(GLsizei n, GLsizei *ids);	//gl2
#ifndef GL_STATIC
void (APIENTRY *qglGenRenderbuffersEXT)(GLsizei n, GLuint *ids);
void (APIENTRY *qglBindRenderbufferEXT)(GLenum target, GLuint id);
void (APIENTRY *qglRenderbufferStorageEXT)(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height);
void (APIENTRY *qglFramebufferRenderbufferEXT)(GLenum target, GLenum attachmentPoint, GLenum textureTarget, GLuint textureId);
GLenum (APIENTRY *qglCheckFramebufferStatusEXT)(GLenum target);
#endif

void (APIENTRY *qglDepthBoundsEXT) (GLclampd zmin, GLclampd zmax);
/*
PFNGLPROGRAMSTRINGARBPROC qglProgramStringARB;
PFNGLBINDPROGRAMARBPROC qglBindProgramARB;
PFNGLGENPROGRAMSARBPROC qglGenProgramsARB;
*/
FTEPFNGLLOCKARRAYSEXTPROC qglLockArraysEXT;
FTEPFNGLUNLOCKARRAYSEXTPROC qglUnlockArraysEXT;


//extensions
//arb multitexture
#ifndef qglActiveTextureARB
qlpSelTexFUNC qglActiveTextureARB;
#endif
qlpSelTexFUNC qglClientActiveTextureARB;
qlpMTex3FUNC	qglMultiTexCoord3fARB;
qlpMTex2FUNC	qglMultiTexCoord2fARB;

//generic multitexture
lpMTexFUNC qglMTexCoord2fSGIS;
lpSelTexFUNC qglSelectTextureSGIS;
int mtexid0;

//ati_truform
FTEPFNGLPNTRIANGLESIATIPROC qglPNTrianglesiATI;
FTEPFNGLPNTRIANGLESFATIPROC qglPNTrianglesfATI;

void (APIENTRY *qglPatchParameteriARB)(GLenum pname, GLint value);	//core in gl4

//stencil shadowing
FTEPFNGLACTIVESTENCILFACEEXTPROC qglActiveStencilFaceEXT;


#define GLchar char
#ifdef NACL
#undef DEBUG
#elif defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif
#if defined(DEBUG)
//always defining this, my system headers use void instead of GLvoid which results in gcc warnings.
typedef void (APIENTRY *qGLDEBUGPROCARB)(GLenum source,
					GLenum type,
					GLuint id,
					GLenum severity,
					GLsizei length,
					const GLchar* message,
					GLvoid* userParam);
void (APIENTRY *qglDebugMessageControlARB)(GLenum source,
					GLenum type,
					GLenum severity,
					GLsizei count,
					const GLuint* ids,
					GLboolean enabled);
void (APIENTRY *qglDebugMessageInsertARB)(GLenum source,
					GLenum type,
					GLuint id,
					GLenum severity,
					GLsizei length, 
					const char* buf);
void (APIENTRY *qglDebugMessageCallbackARB)(qGLDEBUGPROCARB callback,
					void* userParam);
GLuint (APIENTRY *qglGetDebugMessageLogARB)(GLuint count,
					GLsizei bufsize,
					GLenum*	sources,
					GLenum* types,
					GLuint* ids,
					GLuint* severities,
					GLsizei* lengths,
					char* messageLog);

#ifndef GL_ARB_debug_output
#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB               0x8242
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB               0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB              0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB                  0x9145
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB       0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_ARB                0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB              0x8245
#define GL_DEBUG_SOURCE_API_ARB                       0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB             0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB           0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB               0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB               0x824A
#define GL_DEBUG_SOURCE_OTHER_ARB                     0x824B
#define GL_DEBUG_TYPE_ERROR_ARB                       0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB         0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB          0x824E
#define GL_DEBUG_TYPE_PORTABILITY_ARB                 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_ARB                 0x8250
#define GL_DEBUG_TYPE_OTHER_ARB                       0x8251
#define GL_DEBUG_SEVERITY_HIGH_ARB                    0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB                  0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB                     0x9148 
#endif


void (APIENTRY myGLDEBUGPROCAMD)(GLenum source,
					GLenum type,
					GLuint id,
					GLenum severity,
					GLsizei length,
					const GLchar* message,
					GLvoid* userParam)
{
#ifndef _WIN32
#define OutputDebugStringA(s) puts(s)
#endif
	switch(type)
	{
	case GL_DEBUG_TYPE_ERROR_ARB:
		OutputDebugStringA("Error: ");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
		OutputDebugStringA("Depricated: ");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
		OutputDebugStringA("Undefined: ");
		break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB:
		OutputDebugStringA("Portability: ");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB:
		OutputDebugStringA("Performance: ");
		break;
	default:
	case GL_DEBUG_TYPE_OTHER_ARB:
		return;
		OutputDebugStringA("Other: ");
		break;
	}
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
}
#endif


int gl_mtexarbable=0;	//max texture units
qboolean gl_mtexable = false;


extern qboolean gammaworks;	//if the gl drivers can set proper gamma.


gl_config_t gl_config;
int		gl_stencilbits;

float		gldepthmin, gldepthmax;
const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
static const char *gl_extensions;

static unsigned int gl_num_extensions;

extern cvar_t gl_workaround_ati_shadersource;


qboolean GL_CheckExtension(char *extname)
{
	int i;
	int len;
	const char *foo;
	cvar_t *v = Cvar_Get(va("gl_ext_%s", extname), "1", 0, "GL Extensions");
	if (v && !v->ival)
	{
		Con_Printf("Cvar %s is 0\n", v->name);
		return false;
	}

	if (gl_num_extensions && qglGetStringi)
	{
		for (i = 0; i < gl_num_extensions; i++)
			if (!strcmp(qglGetStringi(GL_EXTENSIONS, i), extname))
			{
				Con_DPrintf("Detected GL extension %s\n", extname);
				return true;
			}
	}

	if (!gl_extensions)
		return false;

	//the list is space delimited. we cannot just strstr lest we find leading/trailing _FOO_.
	len = strlen(extname);
	for (foo = gl_extensions; *foo; )
	{
		if (!strncmp(foo, extname, len) && (foo[len] == ' ' || !foo[len]))
			return true;
		while(*foo && *foo != ' ')
			foo++;
		if (*foo == ' ')
			foo++;
	}
	return false;
}

void APIENTRY GL_DrawRangeElementsEmul(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices)
{
	qglDrawElements(mode, count, type, indices);
}

void APIENTRY GL_Color4fv_Emul(const GLfloat *v)
{
	qglColor4f(v[0], v[1], v[2], v[3]);
}

void APIENTRY GL_BindBufferARBStub(GLenum target, GLuint id)
{
}

void APIENTRY GL_ClientStateStub(GLenum array)
{
}

void APIENTRY GL_ClientActiveTextureStub(GLenum texid)
{
}

#define getglcore getglfunction
#define getglext(name) getglfunction(name)
void GL_CheckExtensions (void *(*getglfunction) (char *name))
{
	qboolean webgl = false;
	unsigned int gl_major_version = 0;
	unsigned int gl_minor_version = 0;
	memset(&gl_config, 0, sizeof(gl_config));

	if (!strncmp(gl_version, "WebGL", 5))
	{
		gl_config.gles = true;
		webgl = true;

		if (!strcmp(gl_renderer, "Internet Explorer"))
			gl_config.webgl_ie = true;
	}
	else if (!strncmp(gl_version, "OpenGL ES", 9))
		gl_config.gles = true;
	else
		gl_config.gles = false;

	if (!gl_config.gles)
	{
		if (qglGetError())
			Con_Printf("glGetError %s:%i\n", __FILE__, __LINE__);
		qglGetIntegerv(GL_MAJOR_VERSION, &gl_major_version);
		qglGetIntegerv(GL_MINOR_VERSION, &gl_minor_version);
	}
	if (!gl_major_version || qglGetError())
	{
		/*GL_MAJOR_VERSION not supported? try and parse (es-aware)*/
		const char *s;
		for (s = gl_version; *s && (*s < '0' || *s > '9'); s++)
			;
		gl_major_version = atoi(s);
		while(*s >= '0' && *s <= '9')
			s++;
		if (*s == '.')
			s++;
		gl_minor_version = atoi(s);
	}
#ifdef _DEBUG
	{ extern cvar_t vid_gl_context_es;
	if (vid_gl_context_es.ival == 3)
	{
		gl_config.gles = true;
		gl_major_version = 1;
		gl_minor_version = 0;
	} }
#endif
	if (webgl)	//webgl version 1 equates to gles 2.
	{
		if (gl_major_version < 1)
		{	//ie reports a bollocks version. don't try using fixed function stuff.
			gl_major_version = 2;
			gl_minor_version = 0;
		}
		else if (gl_major_version == 1)
			gl_major_version += 1;
		//webgl2 is not defined yet. either 2 will be gles3 or they'll skip it or something I don't know.
		//so assume webgl2 is still equivelent to gles2, to avoid confusions.
	}
	//FIXME: verify gles3 works properly.

	//yes, I know, this can't cope with minor versions of 10+... I don't care yet.
	gl_config.glversion += gl_major_version + (gl_minor_version/10.f);

	/*gl3 adds glGetStringi instead, as core, with the old form require GL_ARB_compatibility*/
	if (gl_major_version >= 3 && qglGetStringi) /*warning: wine fails to export qglGetStringi*/
	{
		int i;
		qglGetIntegerv(GL_NUM_EXTENSIONS, &gl_num_extensions);
		if (developer.value)
		{
			Con_Printf ("GL_EXTENSIONS:\n");
			for (i = 0; i < gl_num_extensions; i++)
			{
				Con_Printf (" %s", qglGetStringi(GL_EXTENSIONS, i));
				Con_Printf("\n");
			}
			Con_Printf ("end of list\n");
		}
		else
			Con_DPrintf ("GL_EXTENSIONS: %i extensions\n", gl_num_extensions);
		gl_extensions = NULL;
	}
	else
	{
		gl_num_extensions = 0;
		gl_extensions = qglGetString (GL_EXTENSIONS);
		Con_DPrintf ("GL_EXTENSIONS: %s\n", gl_extensions);

		if (!gl_extensions)
			Sys_Error("no extensions\n");
	}

#ifdef _DEBUG
	{ extern cvar_t vid_gl_context_es;
	if (vid_gl_context_es.ival == 3)
	{
		gl_extensions = "";
	} }
#endif

	if (gl_config.gles)
		gl_config.nofixedfunc = gl_config.glversion >= 2;
	else
	{
		/*in gl3.0 things are depricated but not removed*/
		/*in gl3.1 depricated things are removed unless compatibility is present*/
		/*in gl3.2 there's a profile flag we can query*/
		if (gl_config.glversion >= 3.2)
		{
			GLint profile = 0;
#define GL_CONTEXT_PROFILE_MASK					0x9126
#define GL_CONTEXT_CORE_PROFILE_BIT				0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT	0x00000002
#define GL_CONTEXT_FLAGS						0x821E
#ifndef GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#endif
			qglGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

			if (!profile)
			{
				Con_DPrintf("Driver reports invalid profile, assuming compatibility support\n");
				gl_config.nofixedfunc = false;
			}
			else
				gl_config.nofixedfunc = !(profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT);
		}
		else if (gl_config.glversion >= 3.0)
		{
			GLint flags = 0;
			qglGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			gl_config.nofixedfunc = !!(flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT);
			if (gl_config.glversion >= 3.0999)
				gl_config.nofixedfunc = !GL_CheckExtension("GL_ARB_compatibility");
		}
		else
			gl_config.nofixedfunc = false;

#ifdef GLSLONLY
		gl_config.nofixedfunc = true;
#endif
	}

	gl_config.maxglslversion = 0;
	if (gl_config.gles && gl_config.glversion >= 2)
		gl_config.maxglslversion = 100;
	else if (gl_config.glversion >= 2)
	{
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
		const char *s = qglGetString (GL_SHADING_LANGUAGE_VERSION);

		if (s)
		{
			gl_config.maxglslversion = atoi(s) * 100;
			while(*s >= '0' && *s <= '9')
				s++;
			if (*s == '.')
				s++;
			gl_config.maxglslversion += atoi(s);
		}
		else
			gl_config.maxglslversion = 110;
	}
	else
		gl_config.maxglslversion = 110;

	//multitexture
	gl_mtexable = false;
	gl_mtexarbable = 0;
#ifndef qglActiveTextureARB
	qglActiveTextureARB = NULL;
#endif
	qglMultiTexCoord2fARB = NULL;
	qglMultiTexCoord3fARB = NULL;
	qglMTexCoord2fSGIS = NULL;
	qglSelectTextureSGIS = NULL;
	mtexid0 = 0;

#ifndef GL_STATIC
	qglGenFramebuffersEXT		= NULL;
	qglDeleteFramebuffersEXT	= NULL;
	qglBindFramebufferEXT		= NULL;
	qglGenRenderbuffersEXT		= NULL;
	qglDeleteRenderbuffersEXT	= NULL;
	qglBindRenderbufferEXT		= NULL;
	qglRenderbufferStorageEXT	= NULL;
	qglFramebufferTexture2DEXT	= NULL;

#endif

	//no GL_EXT_stencil_two_side
	qglActiveStencilFaceEXT = NULL;

	//no truform. sorry.
	qglPNTrianglesfATI = NULL;
	qglPNTrianglesiATI = NULL;

	//fragment programs
/*	gl_config.arb_fragment_program = false;
	qglProgramStringARB = NULL;
	qglGetProgramivARB = NULL;
	qglBindProgramARB = NULL;
	qglGenProgramsARB = NULL;
*/

	gl_config.ext_packed_depth_stencil = GL_CheckExtension("GL_EXT_packed_depth_stencil");

	if (GL_CheckExtension("GL_EXT_texture_filter_anisotropic"))
	{
		qglGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &gl_config.ext_texture_filter_anisotropic);

		Con_DPrintf("Anisotropic filter extension found (%dx max).\n",gl_config.ext_texture_filter_anisotropic);
	}

	if (!gl_config.gles && gl_config.glversion >= 3)
	{	//GL_ARB_texture_non_power_of_two is supposed to be mandatory in gl2+ and thus checking for it is redundant and not forwards-compatible
		//geforcefx apparently software emulates it, so only activate it unconditionally on gl3+ hardware.
		sh_config.texture_non_power_of_two = true;
		sh_config.texture_non_power_of_two_pic = true;
	}
	else if (GL_CheckExtension("GL_ARB_texture_non_power_of_two"))
	{	//gl1 devices might still support npot
		sh_config.texture_non_power_of_two = true;
		sh_config.texture_non_power_of_two_pic = true;
	}
	else if (gl_config.gles && GL_CheckExtension("GL_OES_texture_npot"))
	{	//gles devices might have full npot too, but with a different extension name. because consistancy is good...
		sh_config.texture_non_power_of_two = true;
		sh_config.texture_non_power_of_two_pic = true;
	}
	else if (gl_config.gles && gl_config.glversion >= 2)
	{	//gles2 has npot (clamp + no mips) support as base.
		sh_config.texture_non_power_of_two = false;
		sh_config.texture_non_power_of_two_pic = true;
	}
	else if (gl_config.gles && GL_CheckExtension("GL_APPLE_texture_2D_limited_npot"))
	{	//gles1 MIGHT have SOME npot support.
		sh_config.texture_non_power_of_two = false;
		sh_config.texture_non_power_of_two_pic = true;
	}
	else
	{	//really old hardware/drivers with no npot support at all.
		sh_config.texture_non_power_of_two = false;
		sh_config.texture_non_power_of_two_pic = false;
	}

//	if (GL_CheckExtension("GL_SGIS_generate_mipmap"))	//a suprising number of implementations have this broken.
//		gl_config.sgis_generate_mipmap = true;

	if (gl_config.gles)
	{
#ifndef qglActiveTextureARB
		qglActiveTextureARB = (void *) getglext("glActiveTexture");
#endif
		qglClientActiveTextureARB = (void *) getglext("glClientActiveTexture");
		qglSelectTextureSGIS = qglActiveTextureARB;
		mtexid0 = GL_TEXTURE0_ARB;
		if (!gl_config.nofixedfunc)
			qglGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &gl_mtexarbable);
		else
			gl_mtexarbable = 8;
	}
	else if (GL_CheckExtension("GL_ARB_multitexture") && !COM_CheckParm("-noamtex"))
	{	//ARB multitexture is the popular choice.
#ifndef qglActiveTextureARB
		qglActiveTextureARB = (void *) getglext("glActiveTextureARB");
#endif
		qglClientActiveTextureARB = (void *) getglext("glClientActiveTextureARB");
		qglMultiTexCoord2fARB = (void *) getglext("glMultiTexCoord2fARB");
		qglMultiTexCoord3fARB = (void *) getglext("glMultiTexCoord3fARB");

		qglGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &gl_mtexarbable);
		gl_mtexable = true;

		qglMTexCoord2fSGIS = qglMultiTexCoord2fARB;
		qglSelectTextureSGIS = qglActiveTextureARB;

		mtexid0 = GL_TEXTURE0_ARB;

#ifndef qglActiveTextureARB
		if (!qglActiveTextureARB || !qglClientActiveTextureARB || !qglMultiTexCoord2fARB)
			gl_mtexable = false;
		else if (gl_mtexarbable == 1)
		{
			Con_Printf("OpenGL Driver Bug detected: 1 texture is NOT multitexture\n");
			gl_mtexable = false;
		}
		if (!gl_mtexable)
		{
			qglActiveTextureARB = NULL;
			qglClientActiveTextureARB = NULL;
			qglMultiTexCoord2fARB = NULL;
			qglMTexCoord2fSGIS = NULL;
			qglSelectTextureSGIS = NULL;
			gl_mtexable=false;
			gl_mtexarbable = false;
		}
		else
		{
			Con_DPrintf("ARB Multitexture extensions found. Use -noamtex to disable.\n");
		}
#endif
	}
	/*
	else if (GL_CheckExtension("GL_SGIS_multitexture") && !COM_CheckParm("-nomtex"))
	{	//SGIS multitexture, limited in many ways but basic functionality is identical to ARB
		Con_SafePrintf("Multitexture extensions found.\n");
		qglMTexCoord2fSGIS = (void *) getglext("glMTexCoord2fSGIS");
		qglSelectTextureSGIS = (void *) getglext("glSelectTextureSGIS");
		gl_mtexable = true;

		mtexid0 = GL_TEXTURE0_SGIS;
	}
	*/
	if (!qglClientActiveTextureARB)
	{
		qglClientActiveTextureARB = GL_ClientActiveTextureStub;
	}

	if ((gl_config.gles && gl_config.glversion >= 2) || GL_CheckExtension("GL_EXT_stencil_wrap"))
		gl_config.ext_stencil_wrap = true;

#ifndef GL_STATIC
	qglStencilOpSeparateATI = NULL;
	if ((gl_config.gles && gl_config.glversion >= 2) || (!gl_config.gles && gl_config.glversion >= 3)) //theoretically that should be a 2 not 3.
		qglStencilOpSeparateATI = (void *) getglext("glStencilOpSeparate");
	else if (GL_CheckExtension("GL_ATI_separate_stencil"))
		qglStencilOpSeparateATI = (void *) getglext("glStencilOpSeparateATI");
#endif
	qglActiveStencilFaceEXT = NULL;
	if (GL_CheckExtension("GL_EXT_stencil_two_side"))
		qglActiveStencilFaceEXT = (void *) getglext("glActiveStencilFaceEXT");

	/*not enabled - its only useful for shadow volumes, but (on nvidia) it affects the depth values even when not clamped which results in shadow z-fighting. best rely upon infinite projection matricies instead*/
	if (GL_CheckExtension("GL_ARB_depth_clamp") || GL_CheckExtension("GL_NV_depth_clamp"))
		gl_config.arb_depth_clamp = true;

	if (GL_CheckExtension("GL_ARB_texture_compression"))
	{
		qglCompressedTexImage2DARB = (void *)getglext("glCompressedTexImage2DARB");
		qglGetCompressedTexImageARB = (void *)getglext("glGetCompressedTexImageARB");

		if (!qglCompressedTexImage2DARB || !qglGetCompressedTexImageARB)
		{
			qglCompressedTexImage2DARB = NULL;
			qglGetCompressedTexImageARB = NULL;
		}
		else
			gl_config.arb_texture_compression = true;
	}
/*
	if (GL_CheckExtension("GL_EXT_depth_bounds_test"))
		qglDepthBoundsEXT = (void *)getglext("glDepthBoundsEXT");
	else if (GL_CheckExtension("GL_NV_depth_bounds_test"))
		qglDepthBoundsEXT = (void *)getglext("glDepthBoundsNV");
	else
		qglDepthBoundsEXT = NULL;
*/
	if (GL_CheckExtension("GL_ATI_pn_triangles"))
	{
		qglPNTrianglesfATI = (void *)getglext("glPNTrianglesfATI");
		qglPNTrianglesiATI = (void *)getglext("glPNTrianglesiATI");
	}
	if (!gl_config.gles && gl_config.glversion >= 4.0)
		qglPatchParameteriARB = getglext("glPatchParameteri");
	else if (GL_CheckExtension("GL_ARB_tessellation_shader"))
		qglPatchParameteriARB = getglext("glPatchParameteriARB");
	else
		qglPatchParameteriARB = NULL;


#ifndef GL_STATIC
	if (GL_CheckExtension("GL_EXT_texture_object"))
	{
		qglBindTexture			= (void *)getglext("glBindTextureEXT");
		if (!qglBindTexture)	//grrr
			qglBindTexture			= (void *)getglext("glBindTexture");
	}
#endif

	if (GL_CheckExtension("GL_EXT_compiled_vertex_array"))
	{
		qglLockArraysEXT = (void *)getglext("glLockArraysEXT");
		qglUnlockArraysEXT = (void *)getglext("glUnlockArraysEXT");
	}

	/*various combiner features*/
	gl_config.tex_env_combine = GL_CheckExtension("GL_EXT_texture_env_combine");
	gl_config.env_add = GL_CheckExtension("GL_EXT_texture_env_add");
	gl_config.nv_tex_env_combine4 = GL_CheckExtension("GL_NV_texture_env_combine4");
	gl_config.arb_texture_env_combine = GL_CheckExtension("GL_ARB_texture_env_combine");
	gl_config.arb_texture_env_dot3 = GL_CheckExtension("GL_ARB_texture_env_dot3");

	gl_config.arb_texture_cube_map = GL_CheckExtension("GL_ARB_texture_cube_map");

#if !defined(GL_STATIC)
	/*vbos, were made core in gl1.5 or gles2.0*/
	if ((gl_config.gles && gl_config.glversion >= 2) || (!gl_config.gles && (gl_major_version > 1 || (gl_major_version == 1 && gl_minor_version >= 5))))
	{
		qglGenBuffersARB = (void *)getglext("glGenBuffers");
		qglDeleteBuffersARB = (void *)getglext("glDeleteBuffers");
		qglBindBufferARB = (void *)getglext("glBindBuffer");
		qglBufferDataARB = (void *)getglext("glBufferData");
		qglBufferSubDataARB = (void *)getglext("glBufferSubData");
		qglMapBufferARB = (void *)getglext("glMapBuffer");
		qglUnmapBufferARB = (void *)getglext("glUnmapBuffer");
	}
	else if (GL_CheckExtension("GL_ARB_vertex_buffer_object"))
	{
		qglGenBuffersARB = (void *)getglext("glGenBuffersARB");
		qglDeleteBuffersARB = (void *)getglext("glDeleteBuffersARB");
		qglBindBufferARB = (void *)getglext("glBindBufferARB");
		qglBufferDataARB = (void *)getglext("glBufferDataARB");
		qglBufferSubDataARB = (void *)getglext("glBufferSubDataARB");
		qglMapBufferARB = (void *)getglext("glMapBufferARB");
		qglUnmapBufferARB = (void *)getglext("glUnmapBufferARB");
	}
	else
	{
		qglGenBuffersARB = NULL;
		qglDeleteBuffersARB = NULL;
		qglBindBufferARB = NULL;
		qglBufferDataARB = NULL;
		qglBufferSubDataARB = NULL;
		qglMapBufferARB = NULL;
		qglUnmapBufferARB = NULL;
	}
#endif

#ifdef GL_STATIC
	gl_config.arb_shader_objects = true;
#else
	if (Cvar_Get("gl_blacklist_debug_glsl", "0", CVAR_RENDERERLATCH, "gl blacklists")->ival && !gl_config.nofixedfunc)
	{
		Con_Printf(CON_NOTICE "GLSL disabled\n");
		gl_config.arb_shader_objects = false;
		qglCreateProgramObjectARB	= NULL;
		qglDeleteProgramObject_		= NULL;
		qglDeleteShaderObject_		= NULL;
		qglUseProgramObjectARB		= NULL;
		qglCreateShaderObjectARB	= NULL;
		qglGetProgramParameteriv_	= NULL;
		qglGetShaderParameteriv_	= NULL;
		qglAttachObjectARB			= NULL;
		qglGetProgramInfoLog_		= NULL;
		qglGetShaderInfoLog_		= NULL;
		qglShaderSourceARB			= NULL;
		qglCompileShaderARB			= NULL;
		qglLinkProgramARB			= NULL;
		qglBindAttribLocationARB	= NULL;
		qglGetAttribLocationARB		= NULL;
		qglVertexAttribPointer		= NULL;
		qglGetVertexAttribiv		= NULL;
		qglGetVertexAttribPointerv	= NULL;
		qglEnableVertexAttribArray	= NULL;
		qglDisableVertexAttribArray	= NULL;
		qglGetUniformLocationARB	= NULL;
		qglUniformMatrix4fvARB		= NULL;
		qglUniformMatrix3x4fv		= NULL;
		qglUniformMatrix4x3fv		= NULL;
		qglUniform4fARB				= NULL;
		qglUniform4fvARB			= NULL;
		qglUniform3fARB				= NULL;
		qglUniform3fvARB			= NULL;
		qglUniform2fvARB			= NULL;
		qglUniform1iARB				= NULL;
		qglUniform1fARB				= NULL;
		qglGetShaderSource			= NULL;
	}
	// glslang
	//the gf2 to gf4 cards emulate vertex_shader and thus supports shader_objects.
	//but our code kinda requires both for clean workings.
	else if (strstr(gl_renderer, " Mesa ") && (gl_config.glversion < 3 || gl_config.gles) && Cvar_Get("gl_blacklist_mesa_glsl", "1", CVAR_RENDERERLATCH, "gl blacklists")->ival)
	{
//(9:12:33 PM) bigfoot: Spike, can you please blacklist your menu shader on Mesa? My machine just hard locked up again because I forgot that pressing escape in FTE is verboten
//(11:51:42 PM) bigfoot: OpenGL vendor string: Tungsten Graphics, Inc
//(11:51:50 PM) bigfoot: OpenGL version string: 2.1 Mesa 7.7.1

//blacklist all glsl, it can't handle #define macros properly either.
//if the menu shader is hardlocking, I don't know what else will do it too.
		Con_Printf(CON_NOTICE "Mesa detected, ignoring any GLSL support. Use '+set gl_blacklist_mesa_glsl 0' on the commandline to reenable it.\n");
	}
	else if (gl_config.glversion >= 2)// && (gl_config.gles || 0))
	{
		/*core names are different from extension names (more functions too)*/
		gl_config.arb_shader_objects = true;
		qglCreateProgramObjectARB	= (void *)getglext( "glCreateProgram");
		qglDeleteProgramObject_		= (void *)getglext( "glDeleteProgram");
		qglDeleteShaderObject_		= (void *)getglext( "glDeleteShader");
		qglUseProgramObjectARB		= (void *)getglext( "glUseProgram");
		qglCreateShaderObjectARB	= (void *)getglext( "glCreateShader");
		qglGetProgramParameteriv_	= (void *)getglext( "glGetProgramiv");
		qglGetShaderParameteriv_	= (void *)getglext( "glGetShaderiv");
		qglAttachObjectARB			= (void *)getglext( "glAttachShader");
		qglGetProgramInfoLog_		= (void *)getglext( "glGetProgramInfoLog");
		qglGetShaderInfoLog_		= (void *)getglext( "glGetShaderInfoLog");
		qglShaderSourceARB			= (void *)getglext("glShaderSource");
		qglCompileShaderARB			= (void *)getglext("glCompileShader");
		qglLinkProgramARB			= (void *)getglext("glLinkProgram");
		qglBindAttribLocationARB	= (void *)getglext("glBindAttribLocation");
		qglGetAttribLocationARB		= (void *)getglext("glGetAttribLocation");
		qglGetUniformLocationARB	= (void *)getglext("glGetUniformLocation");
		qglUniformMatrix4fvARB		= (void *)getglext("glUniformMatrix4fv");
		qglUniformMatrix3x4fv		= (void *)getglext("glUniformMatrix3x4fv");
		qglUniformMatrix4x3fv		= (void *)getglext("glUniformMatrix4x3fv");
		qglUniform4fARB				= (void *)getglext("glUniform4f");
		qglUniform4fvARB			= (void *)getglext("glUniform4fv");
		qglUniform3fARB				= (void *)getglext("glUniform3f");
		qglUniform3fvARB			= (void *)getglext("glUniform3fv");
		qglUniform2fvARB			= (void *)getglext("glUniform2fv");
		qglUniform1iARB				= (void *)getglext("glUniform1i");
		qglUniform1fARB				= (void *)getglext("glUniform1f");
		qglVertexAttribPointer		= (void *)getglext("glVertexAttribPointer");
		qglGetVertexAttribiv		= (void *)getglext("glGetVertexAttribiv");
		qglGetVertexAttribPointerv	= (void *)getglext("glGetVertexAttribPointerv");
		qglEnableVertexAttribArray	= (void *)getglext("glEnableVertexAttribArray");
		qglDisableVertexAttribArray	= (void *)getglext("glDisableVertexAttribArray");
		qglGetShaderSource			= (void *)getglext("glGetShaderSource");
		Con_DPrintf("GLSL available\n");
	}
	else if (GL_CheckExtension("GL_ARB_fragment_shader")
		&& GL_CheckExtension("GL_ARB_vertex_shader")
		&& GL_CheckExtension("GL_ARB_shader_objects"))
	{
		gl_config.arb_shader_objects = true;
		qglCreateProgramObjectARB	= (void *)getglext("glCreateProgramObjectARB");
		qglDeleteProgramObject_		= (void *)getglext("glDeleteObjectARB");
		qglDeleteShaderObject_		= (void *)getglext("glDeleteObjectARB");
		qglUseProgramObjectARB		= (void *)getglext("glUseProgramObjectARB");
		qglCreateShaderObjectARB	= (void *)getglext("glCreateShaderObjectARB");
		qglShaderSourceARB			= (void *)getglext("glShaderSourceARB");
		qglCompileShaderARB			= (void *)getglext("glCompileShaderARB");
		qglGetProgramParameteriv_	= (void *)getglext("glGetObjectParameterivARB");
		qglGetShaderParameteriv_	= (void *)getglext("glGetObjectParameterivARB");
		qglAttachObjectARB			= (void *)getglext("glAttachObjectARB");
		qglGetProgramInfoLog_		= (void *)getglext("glGetInfoLogARB");
		qglGetShaderInfoLog_		= (void *)getglext("glGetInfoLogARB");
		qglLinkProgramARB			= (void *)getglext("glLinkProgramARB");
		qglBindAttribLocationARB	= (void *)getglext("glBindAttribLocationARB");
		qglGetAttribLocationARB		= (void *)getglext("glGetAttribLocationARB");
		qglVertexAttribPointer		= (void *)getglext("glVertexAttribPointerARB");
		qglGetVertexAttribiv		= (void *)getglext("glGetVertexAttribivARB");
		qglGetVertexAttribPointerv	= (void *)getglext("glGetVertexAttribPointervARB");
		qglEnableVertexAttribArray	= (void *)getglext("glEnableVertexAttribArrayARB");
		qglDisableVertexAttribArray	= (void *)getglext("glDisableVertexAttribArrayARB");
		qglGetUniformLocationARB	= (void *)getglext("glGetUniformLocationARB");
		qglUniformMatrix4fvARB		= (void *)getglext("glUniformMatrix4fvARB");
		qglUniformMatrix3x4fv		= (void *)getglext("glUniformMatrix3x4fvARB");
		qglUniformMatrix4x3fv		= (void *)getglext("glUniformMatrix4x3fvARB");
		qglUniform4fARB				= (void *)getglext("glUniform4fARB");
		qglUniform4fvARB			= (void *)getglext("glUniform4fvARB");
		qglUniform3fARB				= (void *)getglext("glUniform3fARB");
		qglUniform3fvARB			= (void *)getglext("glUniform3fvARB");
		qglUniform2fvARB			= (void *)getglext("glUniform2fvARB");
		qglUniform1iARB				= (void *)getglext("glUniform1iARB");
		qglUniform1fARB				= (void *)getglext("glUniform1fARB");
		qglGetShaderSource			= (void *)getglext("glGetShaderSourceARB");

		Con_DPrintf("GLSL available\n");
	}

	if (gl_config.arb_shader_objects)
		qglGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &gl_config.maxattribs);
#endif

	qglGetProgramBinary = NULL;
	qglProgramBinary = NULL;
	if (gl_config.arb_shader_objects)
	{
		if (gl_config.glversion >= 4.1 || GL_CheckExtension("GL_ARB_get_program_binary"))
		{
			qglGetProgramBinary = (void *)getglext("glGetProgramBinary");
			qglProgramBinary = (void *)getglext("glProgramBinary");
		}
		else if (GL_CheckExtension("GL_OES_get_program_binary"))
		{
			//no PROGRAM_BINARY_RETRIEVABLE_HINT
			qglGetProgramBinary = (void *)getglext("glGetProgramBinaryOES");
			qglProgramBinary = (void *)getglext("glProgramBinaryOES");
		}
	}

	if (gl_config.glversion >= 4.5)	//the core version
		qglGetGraphicsResetStatus = (void *)getglext("glGetGraphicsResetStatus");
	else if (GL_CheckExtension("GL_ARB_robustness"))	//desktop extension
		qglGetGraphicsResetStatus = (void *)getglext("glGetGraphicsResetStatusARB");
	else if (GL_CheckExtension("GL_KHR_robustness"))	//glorified gles extension
		qglGetGraphicsResetStatus = (void *)getglext("glGetGraphicsResetStatusKHR");
	else
		qglGetGraphicsResetStatus = NULL;				//its not allowed to crash us. probably will. grr. oh well.

	//we only use vao if we don't have a choice.
	//certain drivers (*cough* mesa *cough*) update vao0 state even when a different vao is bound.
	//they also don't support client arrays, so are unusable without glsl or vertex streaming (which is *really* hard to optimise for - especially with webgl etc)
	//so only use them with gl3+ core contexts where vbo is mandatory anyway.
	if (!gl_config_nofixedfunc)
	{
		//don't bother if we've no glsl
		qglGenVertexArrays	= NULL;
		qglBindVertexArray	= NULL;
	}
	else if (gl_config.glversion >= 3 && !gl_config.gles)
	{
		/*yay core!*/
		Con_Printf("Using vao (core)\n");
		qglGenVertexArrays	= (void *)getglext("glGenVertexArrays");
		qglBindVertexArray	= (void *)getglext("glBindVertexArray");
	}
	else if (GL_CheckExtension("GL_ARB_vertex_array_object"))
	{
		Con_Printf("Using vao (extension)\n");
		qglGenVertexArrays	= (void *)getglext("glGenVertexArraysARB");
		qglBindVertexArray	= (void *)getglext("glBindVertexArrayARB");
	}
	else
	{
		qglGenVertexArrays	= NULL;
		qglBindVertexArray	= NULL;
	}

	if (gl_config.gles)
	{	//gles has different TexImage2D arguments for specifying quality.
		gl_config.arb_depth_texture = GL_CheckExtension("GL_OES_depth_texture");	//gles2
		gl_config.arb_depth_texture |= GL_CheckExtension("GL_CHROMIUM_depth_texture");	//nacl
		gl_config.arb_depth_texture |= GL_CheckExtension("GL_WEBGL_depth_texture");	//webgl. duh.
		gl_config.arb_depth_texture |= GL_CheckExtension("GL_ANGLE_depth_texture");	//gah. should just use wildcards huh (no uploads)
	}
	else
	{
		gl_config.arb_depth_texture = GL_CheckExtension("GL_ARB_depth_texture");
	}
	gl_config.arb_shadow = GL_CheckExtension("GL_ARB_shadow");
	//gl_config.arb_shadow |= GL_CheckExtension("GL_EXT_shadow_samplers");	//gles2. nvidia fucks up. depend on brute-force. :s

	if (GL_CheckExtension("GL_ARB_seamless_cube_map"))
		qglEnable(0x884F);	//TEXTURE_CUBE_MAP_SEAMLESS                   0x884F

	if (!gl_config.gles && gl_config.glversion >= 3.2)
		gl_config.geometryshaders = true;
	else
		gl_config.geometryshaders = false;

#ifdef GL_STATIC
	gl_config.ext_framebuffer_objects = true;				//exists as core in gles2
#else
	if ((gl_config.gles && gl_config.glversion >= 2) ||		//exists as core in gles2
		(!gl_config.gles && gl_config.glversion >= 3) ||	//exists as core in gl3
		GL_CheckExtension("GL_ARB_framebuffer_object"))		//exists as an extension in gl2
	{
		gl_config.ext_framebuffer_objects = true;
		qglGenFramebuffersEXT			= (void *)getglext("glGenFramebuffers");
		qglDeleteFramebuffersEXT		= (void *)getglext("glDeleteFramebuffers");
		qglBindFramebufferEXT			= (void *)getglext("glBindFramebuffer");
		qglGenRenderbuffersEXT			= (void *)getglext("glGenRenderbuffers");
		qglDeleteRenderbuffersEXT		= (void *)getglext("glDeleteRenderbuffers");
		qglBindRenderbufferEXT			= (void *)getglext("glBindRenderbuffer");
		qglRenderbufferStorageEXT		= (void *)getglext("glRenderbufferStorage");
		qglFramebufferTexture2DEXT		= (void *)getglext("glFramebufferTexture2D");
		qglFramebufferRenderbufferEXT	= (void *)getglext("glFramebufferRenderbuffer");
		qglCheckFramebufferStatusEXT	= (void *)getglext("glCheckFramebufferStatus");
		qglGetFramebufferAttachmentParameteriv	= (void *)getglext("glGetFramebufferAttachmentParameteriv");
	}
	else if (GL_CheckExtension("GL_EXT_framebuffer_object"))
	{
		gl_config.ext_framebuffer_objects = true;
		qglGenFramebuffersEXT			= (void *)getglext("glGenFramebuffersEXT");
		qglDeleteFramebuffersEXT		= (void *)getglext("glDeleteFramebuffersEXT");
		qglBindFramebufferEXT			= (void *)getglext("glBindFramebufferEXT");
		qglGenRenderbuffersEXT			= (void *)getglext("glGenRenderbuffersEXT");
		qglDeleteRenderbuffersEXT		= (void *)getglext("glDeleteRenderbuffersEXT");
		qglBindRenderbufferEXT			= (void *)getglext("glBindRenderbufferEXT");
		qglRenderbufferStorageEXT		= (void *)getglext("glRenderbufferStorageEXT");
		qglFramebufferTexture2DEXT		= (void *)getglext("glFramebufferTexture2DEXT");
		qglFramebufferRenderbufferEXT	= (void *)getglext("glFramebufferRenderbufferEXT");
		qglCheckFramebufferStatusEXT	= (void *)getglext("glCheckFramebufferStatusEXT");
		qglGetFramebufferAttachmentParameteriv	= (void *)getglext("glGetFramebufferAttachmentParameterivEXT");
	}
/*	//I don't think we care about the differences, so this code should be safe, but I have no way to test that theory right now
	else if (GL_CheckExtension("GL_OES_framebuffer_object"))
	{
		gl_config.ext_framebuffer_objects = true;
		qglGenFramebuffersEXT			= (void *)getglext("glGenFramebuffersOES");
		qglDeleteFramebuffersEXT		= (void *)getglext("glDeleteFramebuffersOES");
		qglBindFramebufferEXT			= (void *)getglext("glBindFramebufferOES");
		qglGenRenderbuffersEXT			= (void *)getglext("glGenRenderbuffersOES");
		qglDeleteRenderbuffersEXT		= (void *)getglext("glDeleteRenderbuffersOES");
		qglBindRenderbufferEXT			= (void *)getglext("glBindRenderbufferOES");
		qglRenderbufferStorageEXT		= (void *)getglext("glRenderbufferStorageOES");
		qglFramebufferTexture2DEXT		= (void *)getglext("glFramebufferTexture2DOES");
		qglFramebufferRenderbufferEXT	= (void *)getglext("glFramebufferRenderbufferOES");
		qglCheckFramebufferStatusEXT	= (void *)getglext("glCheckFramebufferStatusOES");
	}
*/
#endif
#ifdef DEBUG
	if (GL_CheckExtension("GL_ARB_debug_output"))
	{
		qglDebugMessageControlARB	= (void *)getglext("glDebugMessageControlARB");
		qglDebugMessageInsertARB	= (void *)getglext("glDebugMessageInsertARB");
		qglDebugMessageCallbackARB	= (void *)getglext("glDebugMessageCallbackARB");
		qglGetDebugMessageLogARB	= (void *)getglext("glGetDebugMessageLogARB");
	}
	else if (GL_CheckExtension("GL_KHR_debug"))
	{
		if (gl_config_gles)
		{
			qglDebugMessageControlARB	= (void *)getglext("glDebugMessageControlKHR");
			qglDebugMessageInsertARB	= (void *)getglext("glDebugMessageInsertKHR");
			qglDebugMessageCallbackARB	= (void *)getglext("glDebugMessageCallbackKHR");
			qglGetDebugMessageLogARB	= (void *)getglext("glGetDebugMessageLogKHR");
		}
		else
		{
			qglDebugMessageControlARB	= (void *)getglext("glDebugMessageControl");
			qglDebugMessageInsertARB	= (void *)getglext("glDebugMessageInsert");
			qglDebugMessageCallbackARB	= (void *)getglext("glDebugMessageCallback");
			qglGetDebugMessageLogARB	= (void *)getglext("glGetDebugMessageLog");
		}
	}
	else
	{
		qglDebugMessageControlARB = NULL;
		qglDebugMessageInsertARB = NULL;
		qglDebugMessageCallbackARB = NULL;
		qglGetDebugMessageLogARB = NULL;
	}
#endif

	if (!gl_config.nofixedfunc)
	{	//clamping breaks overbrights. so if this isn't deprecated, try to use it.
		//part of GL_ARB_color_buffer_float, made core with gl3, and then deprecated in gl3.1. *sigh*
#define GL_CLAMP_VERTEX_COLOR		0x891A
#define GL_CLAMP_FRAGMENT_COLOR		0x891B
#define GL_CLAMP_READ_COLOR			0x891C
		void (APIENTRY *qglClampColor)(GLenum target,GLenum clamp);
		qglClampColor	= (void *)getglext("glClampColor");
		if (!qglClampColor)
			qglClampColor	= (void *)getglext("glClampColorARB");
		if (qglClampColor)
		{
			qglClampColor(GL_CLAMP_VERTEX_COLOR,	GL_FALSE);
			qglClampColor(GL_CLAMP_FRAGMENT_COLOR,	GL_FALSE);
			qglClampColor(GL_CLAMP_READ_COLOR,		GL_FALSE);
		}
	}

	if ((!gl_config.gles && gl_config.glversion >= 1.5) || (gl_config.gles && gl_config.glversion >= 3.0))
	{
		qglGenQueriesARB		= (void *)getglext("glGenQueries");
		qglDeleteQueriesARB		= (void *)getglext("glDeleteQueries");
		qglBeginQueryARB		= (void *)getglext("glBeginQuery");
		qglEndQueryARB			= (void *)getglext("glEndQuery");
		qglGetQueryObjectuivARB	= (void *)getglext("glGetQueryObjectuiv");
	}
	else if (GL_CheckExtension("GL_ARB_occlusion_query"))
	{
		qglGenQueriesARB		= (void *)getglext("glGenQueriesARB");
		qglDeleteQueriesARB		= (void *)getglext("glDeleteQueriesARB");
		qglBeginQueryARB		= (void *)getglext("glBeginQueryARB");
		qglEndQueryARB			= (void *)getglext("glEndQueryARB");
		qglGetQueryObjectuivARB	= (void *)getglext("glGetQueryObjectuivARB");
	}
	else
	{
		qglGenQueriesARB		= NULL;
		qglDeleteQueriesARB		= NULL;
		qglBeginQueryARB		= NULL;
		qglEndQueryARB			= NULL;
		qglGetQueryObjectuivARB	= NULL;
	}

	if (!gl_config.gles && gl_config_nofixedfunc)
		qglDisableClientState(GL_VERTEX_ARRAY);

	if (qglGenVertexArrays)
	{
		GLuint vao;
		qglGenVertexArrays(1, &vao);
		qglBindVertexArray(vao);
		qglGenVertexArrays = NULL;
		qglBindVertexArray = NULL;
	}
}

static const char *glsl_hdrs[] =
{
	"sys/defs.h",
			"#define DEFS_DEFINED\n"
			"#ifdef VERTEX_SHADER\n"
//				"attribute vec3 v_position1;" //defined elsewhere, depending on fixed function availability
//				"attribute vec3 v_position2;"
				"attribute vec4 v_colour;"
				"attribute vec2 v_texcoord;"
				"attribute vec2 v_lmcoord;"
				"attribute vec3 v_normal;"
				"attribute vec3 v_svector;"
				"attribute vec3 v_tvector;"
				"attribute vec4 v_bone;"	//fixme: make ints
				"attribute vec4 v_weight;"
#if MAXRLIGHTMAPS > 1
				"\n#define v_lmcoord1 v_lmcoord\n"
				"attribute vec2 v_lmcoord2;"
				"attribute vec2 v_lmcoord3;"
				"attribute vec2 v_lmcoord4;"
				"\n#define v_colour1 v_colour\n"
				"attribute vec4 v_colour2;"
				"attribute vec4 v_colour3;"
				"attribute vec4 v_colour4;"
#endif
			"\n#endif\n"
			"#ifndef USE_ARB_SHADOW\n"	//fall back on regular samplers if we must
				"#define sampler2DShadow sampler2D\n"
			"#endif\n"
#ifndef NOLEGACY
			"uniform sampler2DShadow s_shadowmap;"
			"uniform samplerCube s_projectionmap;"
			"uniform sampler2D s_diffuse;"
			"uniform sampler2D s_normalmap;"
			"uniform sampler2D s_specular;"
			"uniform sampler2D s_upper;"
			"uniform sampler2D s_lower;"
			"uniform sampler2D s_fullbright;"
			"uniform sampler2D s_paletted;"
			"uniform samplerCube s_reflectcube;"
			"uniform sampler2D s_reflectmask;"
			"uniform sampler2D s_lightmap;"
			"uniform sampler2D s_deluxmap;"
			"\n#define s_lightmap0 s_lightmap\n"
			"#define s_deluxmap0 s_deluxmap\n"
#if MAXRLIGHTMAPS > 1
			"uniform sampler2D s_lightmap1;"
			"uniform sampler2D s_lightmap2;"
			"uniform sampler2D s_lightmap3;"
			"uniform sampler2D s_deluxmap1;"
			"uniform sampler2D s_deluxmap2;"
			"uniform sampler2D s_deluxmap3;\n"
#endif
#endif
			"#ifdef USEUBOS\n"
				"layout(std140) uniform u_lightinfo\n"
				"{"
					"vec3		l_lightscreen;"
					"float		l_lightradius;"
					"vec3		l_lightcolour;"
						"float		l_pad1;\n"
					"vec3		l_lightcolourscale;n"
						"float		l_pad2;"
					"vec3		l_lightposition;"
						"float		l_pad3;"
					"mat4		l_cubematrix;"
					"vec4		l_shadowmapproj;"
					"vec2		l_shadowmapscale;"
						"vec2		l_pad4;"
				"};\n"
				"layout(std140) uniform u_entityinfo\n"
				"{\n"
					"vec2		e_vblend;"
						"vec2		e_pad1;"
					"vec3		e_glowmod;"
						"float		e_pad2;"
					"vec3		e_origin;"
						"float		e_pad3;"
					"vec4		colormod;"
					"vec3		e_glowmod;"
						"float		e_pad4;"
					"vec3		e_uppercolour;"
						"float		e_pad5;"
					"vec3		e_lowercolour;"
						"float		e_pad6;"
					"vec3		w_fogcolour;"
					"float		w_fogalpha;"
					"vec3		e_light_dir;"
					"float		w_fogdensity;"
					"vec3		e_light_mul;"
					"float		w_fogdepthbias;"
					"vec3		e_light_ambient;"
					"float		e_time;"
				"};\n"
				"#ifdef SKELETAL\n"
					"layout(std140) unform u_bones\n"
					"{\n"
						"#ifdef PACKEDBONES\n"
							"vec4 m_bones[3*MAX_GPU_BONES];\n"
						"#else\n"
							"mat3x4 m_bones[MAX_GPU_BONES]\n"
						"#endif\n"
					"};\n"
				"#endif\n"
			"#else\n"
				"uniform mat4 m_model;"
				"uniform mat4 m_view;"
				"uniform mat4 m_modelview;"
				"uniform mat4 m_projection;\n"
				"#ifndef VERTEX_SHADER\n"
					"uniform mat4 m_modelviewprojection;\n"
				"#endif\n"
				"#ifdef SKELETAL\n"	//skeletal permutation tends to require glsl 120
					"#ifdef PACKEDBONES\n"
						"uniform vec4 m_bones[3*MAX_GPU_BONES];\n"
					"#else\n"
						"uniform mat3x4 m_bones[MAX_GPU_BONES];\n"
					"#endif\n"
				"#endif\n"
				"uniform mat4 m_invviewprojection;"
				"uniform mat4 m_invmodelviewprojection;"

				/*viewer properties*/
				"uniform vec3 v_eyepos;"
				"uniform vec4 w_fog[2];\n"
				"#define w_fogcolour	w_fog[0].rgb\n"
				"#define w_fogalpha		w_fog[0].a\n"
				"#define w_fogdensity	w_fog[1].x\n"
				"#define w_fogdepthbias	w_fog[1].y\n"

				/*ent properties*/
				//"uniform vec2 e_vblend;\n"
				"#ifdef LIGHTSTYLED\n"
				"uniform vec4 e_lmscale[4];\n"
				"#else\n"
				"uniform vec4 e_lmscale;\n"
				"#endif\n"
				"uniform vec3 e_origin;"
				"uniform float e_time;"
				"uniform vec3 e_eyepos;"
				"uniform vec4 e_colour;"
				"uniform vec4 e_colourident;"
				"uniform vec3 e_glowmod;"
				"uniform vec3 e_uppercolour;"
				"uniform vec3 e_lowercolour;"
				"uniform vec3 e_light_dir;"
				"uniform vec3 e_light_mul;"
				"uniform vec3 e_light_ambient;"

				/*rtlight properties, use with caution*/
				"uniform vec2	l_lightscreen;"
				"uniform float	l_lightradius;"
				"uniform vec3	l_lightcolour;"
				"uniform vec3	l_lightposition;"
				"uniform vec3	l_lightcolourscale;"
				"uniform mat4	l_cubematrix;"
				"uniform vec4	l_shadowmapproj;"
				"uniform vec2	l_shadowmapscale;"

				"uniform vec2 e_rendertexturescale;\n"
			"#endif\n"
		,
	"sys/skeletal.h",
			"#ifndef DEFS_DEFINED\n"
				"attribute vec3 v_normal;"
				"attribute vec3 v_svector;"
				"attribute vec3 v_tvector;"
			"\n#endif\n"
			"#ifdef SKELETAL\n"
				"#ifndef DEFS_DEFINED\n"
					"attribute vec4 v_bone;"
					"attribute vec4 v_weight;\n"
					"#ifdef PACKEDBONES\n"
						"uniform vec4 m_bones[3*MAX_GPU_BONES];\n"
					"#else\n"
						"uniform mat3x4 m_bones[MAX_GPU_BONES];\n"
					"#endif\n"
				"#endif\n"
				
				"#ifdef PACKEDBONES\n"
					"vec4 skeletaltransform()"
					"{"
						"mat4 wmat;"
						"wmat[0]  = m_bones[0+3*int(v_bone.x)] * v_weight.x;"
						"wmat[0] += m_bones[0+3*int(v_bone.y)] * v_weight.y;"
						"wmat[0] += m_bones[0+3*int(v_bone.z)] * v_weight.z;"
						"wmat[0] += m_bones[0+3*int(v_bone.w)] * v_weight.w;"
						"wmat[1]  = m_bones[1+3*int(v_bone.x)] * v_weight.x;"
						"wmat[1] += m_bones[1+3*int(v_bone.y)] * v_weight.y;"
						"wmat[1] += m_bones[1+3*int(v_bone.z)] * v_weight.z;"
						"wmat[1] += m_bones[1+3*int(v_bone.w)] * v_weight.w;"
						"wmat[2]  = m_bones[2+3*int(v_bone.x)] * v_weight.x;"
						"wmat[2] += m_bones[2+3*int(v_bone.y)] * v_weight.y;"
						"wmat[2] += m_bones[2+3*int(v_bone.z)] * v_weight.z;"
						"wmat[2] += m_bones[2+3*int(v_bone.w)] * v_weight.w;"
						"wmat[3] = vec4(0.0,0.0,0.0,1.0);\n"
						"return m_modelviewprojection * (vec4(v_position.xyz, 1.0) * wmat);"
					"}\n"
					"vec4 skeletaltransform_nst(out vec3 n, out vec3 t, out vec3 b)"
					"{"
						"mat4 wmat;"
						"wmat[0]  = m_bones[0+3*int(v_bone.x)] * v_weight.x;"
						"wmat[0] += m_bones[0+3*int(v_bone.y)] * v_weight.y;"
						"wmat[0] += m_bones[0+3*int(v_bone.z)] * v_weight.z;"
						"wmat[0] += m_bones[0+3*int(v_bone.w)] * v_weight.w;"
						"wmat[1]  = m_bones[1+3*int(v_bone.x)] * v_weight.x;"
						"wmat[1] += m_bones[1+3*int(v_bone.y)] * v_weight.y;"
						"wmat[1] += m_bones[1+3*int(v_bone.z)] * v_weight.z;"
						"wmat[1] += m_bones[1+3*int(v_bone.w)] * v_weight.w;"
						"wmat[2]  = m_bones[2+3*int(v_bone.x)] * v_weight.x;"
						"wmat[2] += m_bones[2+3*int(v_bone.y)] * v_weight.y;"
						"wmat[2] += m_bones[2+3*int(v_bone.z)] * v_weight.z;"
						"wmat[2] += m_bones[2+3*int(v_bone.w)] * v_weight.w;"
						"wmat[3] = vec4(0.0,0.0,0.0,1.0);"
						"n = (vec4(v_normal.xyz, 0.0) * wmat).xyz;"
						"t = (vec4(v_svector.xyz, 0.0) * wmat).xyz;"
						"b = (vec4(v_tvector.xyz, 0.0) * wmat).xyz;"
						"return m_modelviewprojection * (vec4(v_position.xyz, 1.0) * wmat);"
					"}\n"
					"vec4 skeletaltransform_wnst(out vec3 w, out vec3 n, out vec3 t, out vec3 b)"
					"{"
						"mat4 wmat;"
						"wmat[0]  = m_bones[0+3*int(v_bone.x)] * v_weight.x;"
						"wmat[0] += m_bones[0+3*int(v_bone.y)] * v_weight.y;"
						"wmat[0] += m_bones[0+3*int(v_bone.z)] * v_weight.z;"
						"wmat[0] += m_bones[0+3*int(v_bone.w)] * v_weight.w;"
						"wmat[1]  = m_bones[1+3*int(v_bone.x)] * v_weight.x;"
						"wmat[1] += m_bones[1+3*int(v_bone.y)] * v_weight.y;"
						"wmat[1] += m_bones[1+3*int(v_bone.z)] * v_weight.z;"
						"wmat[1] += m_bones[1+3*int(v_bone.w)] * v_weight.w;"
						"wmat[2]  = m_bones[2+3*int(v_bone.x)] * v_weight.x;"
						"wmat[2] += m_bones[2+3*int(v_bone.y)] * v_weight.y;"
						"wmat[2] += m_bones[2+3*int(v_bone.z)] * v_weight.z;"
						"wmat[2] += m_bones[2+3*int(v_bone.w)] * v_weight.w;"
						"wmat[3] = vec4(0.0,0.0,0.0,1.0);"
						"n = (vec4(v_normal.xyz, 0.0) * wmat).xyz;"
						"t = (vec4(v_svector.xyz, 0.0) * wmat).xyz;"
						"b = (vec4(v_tvector.xyz, 0.0) * wmat).xyz;"
						"w = (vec4(v_position.xyz, 1.0) * wmat).xyz;"
						"return m_modelviewprojection * (vec4(v_position.xyz, 1.0) * wmat);"
					"}\n"
					"vec4 skeletaltransform_n(out vec3 n)"
					"{"
						"mat4 wmat;"
						"wmat[0]  = m_bones[0+3*int(v_bone.x)] * v_weight.x;"
						"wmat[0] += m_bones[0+3*int(v_bone.y)] * v_weight.y;"
						"wmat[0] += m_bones[0+3*int(v_bone.z)] * v_weight.z;"
						"wmat[0] += m_bones[0+3*int(v_bone.w)] * v_weight.w;"
						"wmat[1]  = m_bones[1+3*int(v_bone.x)] * v_weight.x;"
						"wmat[1] += m_bones[1+3*int(v_bone.y)] * v_weight.y;"
						"wmat[1] += m_bones[1+3*int(v_bone.z)] * v_weight.z;"
						"wmat[1] += m_bones[1+3*int(v_bone.w)] * v_weight.w;"
						"wmat[2]  = m_bones[2+3*int(v_bone.x)] * v_weight.x;"
						"wmat[2] += m_bones[2+3*int(v_bone.y)] * v_weight.y;"
						"wmat[2] += m_bones[2+3*int(v_bone.z)] * v_weight.z;"
						"wmat[2] += m_bones[2+3*int(v_bone.w)] * v_weight.w;"
						"wmat[3] = vec4(0.0,0.0,0.0,1.0);"
						"n = (vec4(v_normal.xyz, 0.0) * wmat).xyz;"
						"return m_modelviewprojection * (vec4(v_position.xyz, 1.0) * wmat);"
					"}\n"
				"#else\n"
					"vec4 skeletaltransform()"
					"{"
						"mat3x4 wmat;"
						"wmat = m_bones[int(v_bone.x)] * v_weight.x;"
						"wmat += m_bones[int(v_bone.y)] * v_weight.y;"
						"wmat += m_bones[int(v_bone.z)] * v_weight.z;"
						"wmat += m_bones[int(v_bone.w)] * v_weight.w;"
						"return m_modelviewprojection * vec4(vec4(v_position.xyz, 1.0) * wmat, 1.0);"
					"}\n"
					"vec4 skeletaltransform_nst(out vec3 n, out vec3 t, out vec3 b)"
					"{"
						"mat3x4 wmat;"
						"wmat = m_bones[int(v_bone.x)] * v_weight.x;"
						"wmat += m_bones[int(v_bone.y)] * v_weight.y;"
						"wmat += m_bones[int(v_bone.z)] * v_weight.z;"
						"wmat += m_bones[int(v_bone.w)] * v_weight.w;"
						"n = vec4(v_normal.xyz, 0.0) * wmat;"
						"t = vec4(v_svector.xyz, 0.0) * wmat;"
						"b = vec4(v_tvector.xyz, 0.0) * wmat;"
						"return m_modelviewprojection * vec4(vec4(v_position.xyz, 1.0) * wmat, 1.0);"
					"}\n"
					"vec4 skeletaltransform_wnst(out vec3 w, out vec3 n, out vec3 t, out vec3 b)"
					"{"
						"mat3x4 wmat;"
						"wmat = m_bones[int(v_bone.x)] * v_weight.x;"
						"wmat += m_bones[int(v_bone.y)] * v_weight.y;"
						"wmat += m_bones[int(v_bone.z)] * v_weight.z;"
						"wmat += m_bones[int(v_bone.w)] * v_weight.w;"
						"n = vec4(v_normal.xyz, 0.0) * wmat;"
						"t = vec4(v_svector.xyz, 0.0) * wmat;"
						"b = vec4(v_tvector.xyz, 0.0) * wmat;"
						"w = vec4(v_position.xyz, 1.0) * wmat;"
						"return m_modelviewprojection * vec4(w, 1.0);"
					"}\n"
					"vec4 skeletaltransform_n(out vec3 n)"
					"{"
						"mat3x4 wmat;"
						"wmat = m_bones[int(v_bone.x)] * v_weight.x;"
						"wmat += m_bones[int(v_bone.y)] * v_weight.y;"
						"wmat += m_bones[int(v_bone.z)] * v_weight.z;"
						"wmat += m_bones[int(v_bone.w)] * v_weight.w;"
						"n = vec4(v_normal.xyz, 0.0) * wmat;"
						"return m_modelviewprojection * vec4(vec4(v_position.xyz, 1.0) * wmat, 1.0);"
					"}\n"
				"#endif\n"
			"#else\n"
				"#define skeletaltransform ftetransform\n"
				"vec4 skeletaltransform_wnst(out vec3 w, out vec3 n, out vec3 t, out vec3 b)"
				"{"
					"n = v_normal;"
					"t = v_svector;"
					"b = v_tvector;"
					"w = v_position.xyz;"
					"return ftetransform();"
				"}\n"
				"vec4 skeletaltransform_nst(out vec3 n, out vec3 t, out vec3 b)"
				"{"
					"n = v_normal;"
					"t = v_svector;"
					"b = v_tvector;"
					"return ftetransform();"
				"}\n"
				"vec4 skeletaltransform_n(out vec3 n)"
				"{"
					"n = v_normal;"
					"return ftetransform();"
				"}\n"
			"#endif\n"
		,
	"sys/fog.h",
			"#ifdef FRAGMENT_SHADER\n"
				"#ifdef FOG\n"
					"#ifndef DEFS_DEFINED\n"
					"uniform vec4 w_fog[2];\n"
					"#define w_fogcolour	w_fog[0].rgb\n"
					"#define w_fogalpha		w_fog[0].a\n"
					"#define w_fogdensity	w_fog[1].x\n"
					"#define w_fogdepthbias	w_fog[1].y\n"
					"#endif\n"

					"vec3 fog3(in vec3 regularcolour)"
					"{"
						"float z = w_fogdensity * gl_FragCoord.z / gl_FragCoord.w;\n"
						"z = max(0.0,z-w_fogdepthbias);\n"
						"#if #include \"cvar/r_fog_exp2\"\n"
						"z *= z;\n"
						"#endif\n"
						"float fac = exp2(-(z * 1.442695));\n"
						"fac = (1.0-w_fogalpha) + (clamp(fac, 0.0, 1.0)*w_fogalpha);\n"
						"return mix(w_fogcolour, regularcolour, fac);\n"
					"}\n"
					"vec3 fog3additive(in vec3 regularcolour)"
					"{"
						"float z = w_fogdensity * gl_FragCoord.z / gl_FragCoord.w;\n"
						"z = max(0.0,z-w_fogdepthbias);\n"
						"#if #include \"cvar/r_fog_exp2\"\n"
						"z *= z;\n"
						"#endif\n"
						"float fac = exp2(-(z * 1.442695));\n"
						"fac = (1.0-w_fogalpha) + (clamp(fac, 0.0, 1.0)*w_fogalpha);\n"
						"return regularcolour * fac;\n"
					"}\n"
					"vec4 fog4(in vec4 regularcolour)"
					"{"
						"return vec4(fog3(regularcolour.rgb), 1.0) * regularcolour.a;\n"
					"}\n"
					"vec4 fog4additive(in vec4 regularcolour)"
					"{"
						"float z = w_fogdensity * gl_FragCoord.z / gl_FragCoord.w;\n"
						"z = max(0.0,z-w_fogdepthbias);\n"
						"#if #include \"cvar/r_fog_exp2\"\n"
						"z *= z;\n"
						"#endif\n"
						"float fac = exp2(-(z * 1.442695));\n"
						"fac = (1.0-w_fogalpha) + (clamp(fac, 0.0, 1.0)*w_fogalpha);\n"
						"return regularcolour * vec4(fac, fac, fac, 1.0);\n"
					"}\n"
					"vec4 fog4blend(in vec4 regularcolour)"
					"{"
						"float z = w_fogdensity * gl_FragCoord.z / gl_FragCoord.w;\n"
						"z = max(0.0,z-w_fogdepthbias);\n"
						"#if #include \"cvar/r_fog_exp2\"\n"
						"z *= z;\n"
						"#endif\n"
						"float fac = exp2(-(z * 1.442695));\n"
						"fac = (1.0-w_fogalpha) + (clamp(fac, 0.0, 1.0)*w_fogalpha);\n"
						"return regularcolour * vec4(1.0, 1.0, 1.0, fac);\n"
					"}\n"
				"#else\n"
					/*don't use macros for this - mesa bugs out*/
					"vec3 fog3(in vec3 regularcolour) { return regularcolour; }\n"
					"vec3 fog3additive(in vec3 regularcolour) { return regularcolour; }\n"
					"vec4 fog4(in vec4 regularcolour) { return regularcolour; }\n"
					"vec4 fog4additive(in vec4 regularcolour) { return regularcolour; }\n"
					"vec4 fog4blend(in vec4 regularcolour) { return regularcolour; }\n"
				"#endif\n"
			"#endif\n"
		,
	"sys/offsetmapping.h",
			"uniform float cvar_r_glsl_offsetmapping_scale;\n"
			"vec2 offsetmap(sampler2D normtex, vec2 base, vec3 eyevector)\n"
			"{\n"
			"#if !defined(OFFSETMAPPING_SCALE)\n"
				"#define OFFSETMAPPING_SCALE 1.0\n"
			"#endif\n"
			"#if defined(RELIEFMAPPING) && !defined(GL_ES)\n"
				"float i, f;\n"
				"vec3 OffsetVector = vec3(normalize(eyevector.xyz).xy * cvar_r_glsl_offsetmapping_scale * OFFSETMAPPING_SCALE * vec2(-1.0, 1.0), -1.0);\n"
				"vec3 RT = vec3(vec2(base.xy"/* - OffsetVector.xy*OffsetMapping_Bias*/"), 1.0);\n"
				"OffsetVector /= 10.0;\n"
				"for(i = 1.0; i < 10.0; ++i)\n"
					"RT += OffsetVector *  step(texture2D(normtex, RT.xy).a, RT.z);\n"
				"for(i = 0.0, f = 1.0; i < 5.0; ++i, f *= 0.5)\n"
					"RT += OffsetVector * (step(texture2D(normtex, RT.xy).a, RT.z) * f - 0.5 * f);\n"
				"return RT.xy;\n"
			"#elif defined(OFFSETMAPPING)\n"
				"vec2 OffsetVector = normalize(eyevector).xy * cvar_r_glsl_offsetmapping_scale * OFFSETMAPPING_SCALE * vec2(-1.0, 1.0);\n"
				"vec2 tc = base;\n"
				"tc += OffsetVector;\n"
				"OffsetVector *= 0.333;\n"
				"tc -= OffsetVector * texture2D(normtex, tc).w;\n"
				"tc -= OffsetVector * texture2D(normtex, tc).w;\n"
				"tc -= OffsetVector * texture2D(normtex, tc).w;\n"
				"return tc;\n"
			"#else\n"
				"return base;\n"
			"#endif\n"
			"}\n"
		,
	"sys/pcf.h",
			//!!cvardf r_glsl_pcf
			"#ifndef PCF\n"
				"#define ShadowmapFilter(smap) 1.0\n"	//s_shadowmap generally. returns a scaler to say how much light should be used for this pixel.
			"#else\n"
				"#ifndef r_glsl_pcf\n"
					"#define r_glsl_pcf 9\n"
				"#endif\n"
				"#if r_glsl_pcf < 1\n"
					"#undef r_glsl_pcf\n"
					"#define r_glsl_pcf 9\n"
				"#endif\n"
				"#ifndef DEFS_DEFINED\n"
				"uniform vec4 l_shadowmapproj;\n" //light projection matrix info
				"uniform vec2 l_shadowmapscale;\n"	//xy are the texture scale, z is 1, w is the scale.
				"#endif\n"
				"vec3 ShadowmapCoord(void)\n"
				"{\n"
				"#ifdef SPOT\n"
					//bias it. don't bother figuring out which side or anything, its not needed
					//l_projmatrix contains the light's projection matrix so no other magic needed
					"return ((vtexprojcoord.xyz-vec3(0.0,0.0,0.015))/vtexprojcoord.w + vec3(1.0, 1.0, 1.0)) * vec3(0.5, 0.5, 0.5);\n"
				//"#elif defined(CUBESHADOW)\n"
				//	vec3 shadowcoord = vshadowcoord.xyz / vshadowcoord.w;
				//	#define dosamp(x,y) shadowCube(s_t4, shadowcoord + vec2(x,y)*texscale.xy).r
				"#else\n"
					//figure out which axis to use
					//texture is arranged thusly:
					//forward left  up
					//back    right down
					"vec3 dir = abs(vtexprojcoord.xyz);\n"
					//assume z is the major axis (ie: forward from the light)
					"vec3 t = vtexprojcoord.xyz;\n"
					"float ma = dir.z;\n"
					"vec3 axis = vec3(0.5/3.0, 0.5/2.0, 0.5);\n"
					"if (dir.x > ma)\n"
					"{\n"
						"ma = dir.x;\n"
						"t = vtexprojcoord.zyx;\n"
						"axis.x = 0.5;\n"
					"}\n"
					"if (dir.y > ma)\n"
					"{\n"
						"ma = dir.y;\n"
						"t = vtexprojcoord.xzy;\n"
						"axis.x = 2.5/3.0;\n"
					"}\n"
					//if the axis is negative, flip it.
					"if (t.z > 0.0)\n"
					"{\n"
						"axis.y = 1.5/2.0;\n"
						"t.z = -t.z;\n"
					"}\n"

					//we also need to pass the result through the light's projection matrix too
					//the 'matrix' we need only contains 5 actual values. and one of them is a -1. So we might as well just use a vec4.
					//note: the projection matrix also includes scalers to pinch the image inwards to avoid sampling over borders, as well as to cope with non-square source image
					//the resulting z is prescaled to result in a value between -0.5 and 0.5.
					//also make sure we're in the right quadrant type thing
					"return axis + ((l_shadowmapproj.xyz*t.xyz + vec3(0.0, 0.0, l_shadowmapproj.w)) / -t.z);\n"
				"#endif\n"
				"}\n"

				"float ShadowmapFilter(sampler2DShadow smap)\n"
				"{\n"
					"vec3 shadowcoord = ShadowmapCoord();\n"

					"#if 0\n"//def GL_ARB_texture_gather
						"vec2 ipart, fpart;\n"
						"#define dosamp(x,y) textureGatherOffset(smap, ipart.xy, vec2(x,y)))\n"
						"vec4 tl = step(shadowcoord.z, dosamp(-1.0, -1.0));\n"
						"vec4 bl = step(shadowcoord.z, dosamp(-1.0, 1.0));\n"
						"vec4 tr = step(shadowcoord.z, dosamp(1.0, -1.0));\n"
						"vec4 br = step(shadowcoord.z, dosamp(1.0, 1.0));\n"
						//we now have 4*4 results, woo
						//we can just average them for 1/16th precision, but that's still limited graduations
						//the middle four pixels are 'full strength', but we interpolate the sides to effectively give 3*3
						"vec4 col =     vec4(tl.ba, tr.ba) + vec4(bl.rg, br.rg) + " //middle two rows are full strength
								"mix(vec4(tl.rg, tr.rg), vec4(bl.ba, br.ba), fpart.y);\n" //top+bottom rows
						"return dot(mix(col.rgb, col.agb, fpart.x), vec3(1.0/9.0));\n"	//blend r+a, gb are mixed because its pretty much free and gives a nicer dot instruction instead of lots of adds.
					"#else\n"
						"#ifdef USE_ARB_SHADOW\n"
							//with arb_shadow, we can benefit from hardware acclerated pcf, for smoother shadows
							"#define dosamp(x,y) float(shadow2D(smap, shadowcoord.xyz + (vec3(x,y,0.0)*l_shadowmapscale.xyx)))\n"
						"#else\n"
							"#define dosamp(x,y) float(texture2D(smap, shadowcoord.xy + (vec2(x,y)*l_shadowmapscale.xy)).r >= shadowcoord.z)\n"
						"#endif\n"
						"float s = 0.0;\n"
						"#if r_glsl_pcf >= 1 && r_glsl_pcf < 5\n"
							"s += dosamp(0.0, 0.0);\n"
							"return s;\n"
						"#elif r_glsl_pcf >= 5 && r_glsl_pcf < 9\n"
							"s += dosamp(-1.0, 0.0);\n"
							"s += dosamp(0.0, -1.0);\n"
							"s += dosamp(0.0, 0.0);\n"
							"s += dosamp(0.0, 1.0);\n"
							"s += dosamp(1.0, 0.0);\n"
							"return s/5.0;\n"
						"#else\n"
							"s += dosamp(-1.0, -1.0);\n"
							"s += dosamp(-1.0, 0.0);\n"
							"s += dosamp(-1.0, 1.0);\n"
							"s += dosamp(0.0, -1.0);\n"
							"s += dosamp(0.0, 0.0);\n"
							"s += dosamp(0.0, 1.0);\n"
							"s += dosamp(1.0, -1.0);\n"
							"s += dosamp(1.0, 0.0);\n"
							"s += dosamp(1.0, 1.0);\n"
							"return s/9.0;\n"
						"#endif\n"
					"#endif\n"
				"}\n"
			"#endif\n"
		,
	NULL
};

qboolean GLSlang_GenerateIncludes(int maxstrings, int *strings, const GLchar *prstrings[], GLint length[], const char *shadersource)
{
	int i;
	char *incline, *inc;
	char incname[256];
	while((incline=strstr(shadersource, "#include")))
	{
		if (*strings == maxstrings)
			return false;

		/*emit up to the include*/
		if (incline - shadersource)
		{
			prstrings[*strings] = shadersource;
			length[*strings] = incline - shadersource;
			*strings += 1;
		}

		incline += 8;
		incline = COM_ParseOut (incline, incname, sizeof(incname));

		if (!strncmp(incname, "cvar/", 5))
		{
			cvar_t *var = Cvar_Get(incname+5, "0", 0, "shader cvars");
			if (var)
			{
				var->flags |= CVAR_SHADERSYSTEM;
				if (!GLSlang_GenerateIncludes(maxstrings, strings, prstrings, length, var->string))
					return false;
			}
			else
			{
				/*dump something if the cvar doesn't exist*/
				if (*strings == maxstrings)
					return false;
				prstrings[*strings] = "0";
				length[*strings] = strlen("0");
				*strings += 1;
			}
		}
		else
		{
			for (i = 0; glsl_hdrs[i]; i += 2)
			{
				if (!strcmp(incname, glsl_hdrs[i]))
				{
					if (!GLSlang_GenerateIncludes(maxstrings, strings, prstrings, length, glsl_hdrs[i+1]))
						return false;
					break;
				}
			}
			if (!glsl_hdrs[i])
			{
				if (FS_LoadFile(incname, (void**)&inc) != (qofs_t)-1)
				{
					if (!GLSlang_GenerateIncludes(maxstrings, strings, prstrings, length, inc))
					{
						FS_FreeFile(inc);
						return false;
					}
					FS_FreeFile(inc);
				}
			}
		}

		/*move the pointer past the include*/
		shadersource = incline;
	}
	if (*shadersource)
	{
		if (*strings == maxstrings)
			return false;

		/*dump the remaining shader string*/
		prstrings[*strings] = shadersource;
		length[*strings] = strlen(prstrings[*strings]);
		*strings += 1;
	}
	return true;
}

// glslang helper api function definitions
// type should be GL_FRAGMENT_SHADER_ARB or GL_VERTEX_SHADER_ARB
//doesn't check to see if it was okay. use FinishShader for that.
static GLhandleARB GLSlang_CreateShader (program_t *prog, const char *name, int ver, const char **precompilerconstants, const char *shadersource, GLenum shadertype, qboolean silent)
{
	GLhandleARB shader;
	int i;
	const GLchar *prstrings[64+16];
	GLint length[sizeof(prstrings)/sizeof(prstrings[0])];
	int strings = 0;
	char verline[64];

	if (!shadersource)
		return 0;

	if (ver)
	{
		/*required version not supported, don't even try*/
		if (ver > gl_config.maxglslversion)
			return 0;
#ifdef FTE_TARGET_WEB
		//emscripten prefixes our shader with a precision specifier, and then the browser bitches at the following (otherwise valid) #version, so don't say anything at all if its ver 100, and the browser won't complain
		if (ver != 100)
#endif
		{
			//known versions:
			//100 == gles2
			//110 == gl2.0
			//120 == gl2.1
			//130 == gl3.0
			//140 == gl3.1
			//150 [core|compatibility] == gl3.2
			//330, 400, 410, 420, 430 [core|compatibility] == gl?.??
			//300 ES == gles3
			if (gl_config_gles && ver != 100)
				Q_snprintfz(verline, sizeof(verline), "#version %u ES\n", ver);
			else if (!gl_config_gles && ver >= 150 && !gl_config_nofixedfunc)
				//favour compatibility profile, simply because we want ftransform to work properly
				//note that versions 130+140 are awkward due to deprecation stuff, both assume compatibility profiles where supported.
				//  however, 130 REMOVED ftransform in revision 2 and then re-added it as deprecated in revision 4 (of 10).
				Q_snprintfz(verline, sizeof(verline), "#version %u compatibility\n", ver);
			else
				Q_snprintfz(verline, sizeof(verline), "#version %u\n", ver);	//core assumed, where defined
			prstrings[strings] = verline;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}
	}

	while(*precompilerconstants)
	{
		prstrings[strings] = *precompilerconstants++;
		length[strings] = strlen(prstrings[strings]);
		strings++;
	}

	prstrings[strings] = "#define ENGINE_"DISTRIBUTION"\n";
	length[strings] = strlen(prstrings[strings]);
	strings++;

	switch (shadertype)
	{
	case GL_FRAGMENT_SHADER_ARB:
		prstrings[strings] = "#define FRAGMENT_SHADER\n";
		length[strings] = strlen(prstrings[strings]);
		strings++;
		if (gl_config.gles)
		{
			prstrings[strings] =
					"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
					"precision highp float;\n"
					"#else\n"
					"precision mediump float;\n"
					"#endif\n"
				;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}
		if (ver >= 130)
		{
			prstrings[strings] =
				//gl3+ deprecated the some things. these are removed in forwards-compatible / core contexts.
				//varying became either in or out, which is important if you have geometry shaders...
				"#define varying in\n"
				//now only the 'texture' function exists, with overloads for each sampler type.
				"#define texture2D texture\n"
				"#define textureCube texture\n"
				"#define shadow2D texture\n"
				//gl_FragColor and gl_FragData got deprecated too, need to make manual outputs
				"out vec4 fte_fragdata;\n"
				"#define gl_FragColor fte_fragdata\n"
			;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}

		if (prog)
		{	//for compat with vulkan, that injects samplers...
			const char *numberedsamplernames[] =
			{
				"uniform sampler2D s_t0;\n",
				"uniform sampler2D s_t1;\n",
				"uniform sampler2D s_t2;\n",
				"uniform sampler2D s_t3;\n",
				"uniform sampler2D s_t4;\n",
				"uniform sampler2D s_t5;\n",
				"uniform sampler2D s_t6;\n",
				"uniform sampler2D s_t7;\n",
			};
#ifdef NOLEGACY
			const char *defaultsamplernames[] =
			{
				"uniform sampler2DShadow s_shadowmap;\n",
				"uniform samplerCube s_projectionmap;\n",
				"uniform sampler2D s_diffuse;\n",
				"uniform sampler2D s_normalmap;\n",
				"uniform sampler2D s_specular;\n",
				"uniform sampler2D s_upper;\n",
				"uniform sampler2D s_lower;\n",
				"uniform sampler2D s_fullbright;\n",
				"uniform sampler2D s_paletted;\n",
				"uniform samplerCube s_reflectcube;\n",
				"uniform sampler2D s_reflectmask;\n",
				"uniform sampler2D s_lightmap;\n#define s_lightmap0 s_lightmap\n",
				"uniform sampler2D s_deluxmap;\n#define s_deluxmap0 s_deluxmap\n",

				"uniform sampler2D s_lightmap1;\n",
				"uniform sampler2D s_lightmap2;\n",
				"uniform sampler2D s_lightmap3;\n",
				"uniform sampler2D s_deluxmap1;\n",
				"uniform sampler2D s_deluxmap2;\n",
				"uniform sampler2D s_deluxmap3;\n",
			};
			for (i = 0; i < countof(defaultsamplernames); i++)
			{
				if (prog->defaulttextures & (1u<<i))
				{
					prstrings[strings] = defaultsamplernames[i];
					length[strings] = strlen(prstrings[strings]);
					strings++;
				}
			}
#endif
			for (i = 0; i < prog->numsamplers && i < countof(numberedsamplernames); i++)
			{
				prstrings[strings] = numberedsamplernames[i];
				length[strings] = strlen(prstrings[strings]);
				strings++;
			}
		}
		break;
	case GL_GEOMETRY_SHADER_ARB:
		prstrings[strings] = "#define GEOMETRY_SHADER\n";
		length[strings] = strlen(prstrings[strings]);
		strings++;
		break;
	case GL_TESS_CONTROL_SHADER_ARB:
		prstrings[strings] =
			"#define TESS_CONTROL_SHADER\n"
			"#if __VERSION__ < 400\n"
				"#extension GL_ARB_tessellation_shader : enable\n"
			"#endif\n";
			//varyings are arrays, so don't bother defining that here.
		length[strings] = strlen(prstrings[strings]);
		strings++;
		break;
	case GL_TESS_EVALUATION_SHADER_ARB:
		prstrings[strings] = 
			"#define TESS_EVALUATION_SHADER\n"
			"#if __VERSION__ < 400\n"
				"#extension GL_ARB_tessellation_shader : enable\n"
			"#endif\n"
			"#define varying out\n";
		length[strings] = strlen(prstrings[strings]);
		strings++;
		break;
	case GL_VERTEX_SHADER_ARB:
		prstrings[strings] = "#define VERTEX_SHADER\n";
		length[strings] = strlen(prstrings[strings]);
		strings++;
#ifdef RTLIGHTS
		if (!r_shadow_shadowmapping.ival && ver >= 120)
		{
			prstrings[strings] = "invariant gl_Position;\n";
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}
#endif
		if (gl_config.gles)
		{
			prstrings[strings] =
					"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
					"precision highp float;\n"
					"#else\n"
					"precision mediump float;\n"
					"#endif\n"
				;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}
		if (ver >= 130)
		{
			prstrings[strings] =
				"#define attribute in\n"
				"#define varying out\n"
			;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}
		if (gl_config_nofixedfunc)
		{
			prstrings[strings] =
					"attribute vec3 v_position1;\n"
					"#ifdef FRAMEBLEND\n"
					"attribute vec3 v_position2;\n"
					"uniform vec2 e_vblend;\n"
					"#define v_position ((v_position1*e_vblend.x)+(v_position2*e_vblend.y))\n"
					"#else\n"
					"#define v_position v_position1\n"
					"#endif\n"
					"uniform mat4 m_modelviewprojection;\n"
#if 1//def FTE_TARGET_WEB
					//IE is buggy
					"vec4 ftetransform() { return m_modelviewprojection * vec4(v_position, 1.0); }\n"
#else
					"#define ftetransform() (m_modelviewprojection * vec4(v_position, 1.0))\n"
#endif
				;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}
		else
		{
			prstrings[strings] =
					"#ifdef FRAMEBLEND\n"
					"attribute vec3 v_position2;\n"
					"uniform vec2 e_vblend;\n"
					"#define v_position (gl_Vertex.xyz*e_vblend.x+v_position2*e_vblend.y)\n"
					"uniform mat4 m_modelviewprojection;\n"
					"#define ftetransform() (m_modelviewprojection * vec4(v_position, 1.0))\n"
					"#else\n"
					"#define v_position gl_Vertex.xyz\n"
					"uniform mat4 m_modelviewprojection;\n"
					"#define ftetransform ftransform\n"
					"#endif\n"
				;
			length[strings] = strlen(prstrings[strings]);
			strings++;
		}

		break;
	default:
		prstrings[strings] = "#define UNKNOWN_SHADER\n";
		length[strings] = strlen(prstrings[strings]);
		strings++;
		break;
	}

	GLSlang_GenerateIncludes(sizeof(prstrings)/sizeof(prstrings[0]), &strings, prstrings, length, shadersource);

	shader = qglCreateShaderObjectARB(shadertype);

	if (gl_workaround_ati_shadersource.ival)
	{
		/*ATI Driver Bug: ATI drivers ignore the 'length' array.
		this code does what the drivers fail to do.
		this patch makes the submission more mainstream
		if ati can feck it up so much on a system with no real system memory issues, I wouldn't be surprised if embedded systems also mess it up.
		*/
		GLcharARB *combined;
		int totallen = 1;
		for (i = 0; i < strings; i++)
			totallen += length[i];
		combined = malloc(totallen);
		totallen = 0;
		combined[totallen] = 0;
		for (i = 0; i < strings; i++)
		{
			memcpy(combined + totallen, prstrings[i], length[i]);
			totallen += length[i];
			combined[totallen] = 0;
		}
		qglShaderSourceARB(shader, 1, (const GLcharARB**)&combined, NULL);
		free(combined);
	}
	else
		qglShaderSourceARB(shader, strings, prstrings, length);
	qglCompileShaderARB(shader);

	return shader;
}

//called after CreateShader. Checks for success.
//Splitting creation allows for both vertex+fragment shaders to be processed simultaneously if the driver threads glCompileShaderARB.
static GLhandleARB GLSlang_FinishShader(GLhandleARB shader, const char *name, GLenum shadertype, qboolean *silent)
{
	GLint	compiled;
	int loglen;

	if (!shader)	//if there's no shader, then there was nothing to finish...
		return shader;

	qglGetShaderParameteriv_(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if(!compiled)
	{
		char	*typedesc;
		char	str[65536];

		*str = 0;
		qglGetShaderInfoLog_(shader, sizeof(str), NULL, str);
		if (!*silent)
		{
			*silent = true;
			switch (shadertype)
			{
			case GL_FRAGMENT_SHADER_ARB:
				typedesc = "Fragment";
				break;
			case GL_VERTEX_SHADER_ARB:
				typedesc = "Vertex";
				break;
			case GL_TESS_CONTROL_SHADER_ARB:
				typedesc = "Tesselation Control";
				break;
			case GL_TESS_EVALUATION_SHADER_ARB:
				typedesc = "Tesselation Evaluation";
				break;
			default:
				typedesc = "???";
				break;
			}
			Con_Printf("%s shader (%s) compilation error:\n----------\n%s----------\n", typedesc, name, str);

			//if there's no fixed function then failure to compile the default2d shader should be considered fatal. this should help avoid black screens on android.
			if (gl_config_nofixedfunc && !strcmp(name, "default2d"))
				Sys_Error("%s shader (%s) compilation error:\n----------\n%s----------\n", typedesc, name, str);

			if (developer.ival)
			{
				unsigned int line;
				char *eol, *start;
				qglGetShaderSource(shader, sizeof(str), NULL, str);
				Con_Printf("Shader \"%s\" source:\n", name);
				for(start = str, line = 1; ;line++)
				{
					eol = strchr(start, '\n');
					if (eol)
						*eol=0;
					Con_Printf("%3u: %s\n", line, start);
					if (!eol)
						break;
					start = eol+1;
				}
			}
		}
		qglDeleteShaderObject_(shader);
		return 0;
	}

	if (developer.ival)
	{
		qglGetShaderParameteriv_(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &loglen);
		if (loglen)
		{
			char	str[8192];

			qglGetShaderInfoLog_(shader, sizeof(str), NULL, str);
			if (strstr(str, "WARNING"))
			{
				Con_Printf("Shader \"%s\" log:\n%s", name, str);
				qglGetShaderSource(shader, sizeof(str), NULL, str);
				Con_Printf("Shader \"%s\" source:\n%s", name, str);
			}
		}
	}

	return shader;
}

GLhandleARB GLSlang_CreateProgramObject (const char *name, GLhandleARB vert, GLhandleARB cont, GLhandleARB eval, GLhandleARB geom, GLhandleARB frag)
{
	GLhandleARB	program;

	program = qglCreateProgramObjectARB();
	if (vert) qglAttachObjectARB(program, vert);
	if (geom) qglAttachObjectARB(program, geom);
	if (cont) qglAttachObjectARB(program, cont);
	if (eval) qglAttachObjectARB(program, eval);
	if (frag) qglAttachObjectARB(program, frag);

	qglBindAttribLocationARB(program, VATTR_VERTEX1, "v_position1");
	qglBindAttribLocationARB(program, VATTR_COLOUR, "v_colour");
	qglBindAttribLocationARB(program, VATTR_TEXCOORD, "v_texcoord");
	qglBindAttribLocationARB(program, VATTR_LMCOORD, "v_lmcoord");
	qglBindAttribLocationARB(program, VATTR_NORMALS, "v_normal");
	qglBindAttribLocationARB(program, VATTR_SNORMALS, "v_svector");
	qglBindAttribLocationARB(program, VATTR_TNORMALS, "v_tvector");
	qglBindAttribLocationARB(program, VATTR_VERTEX2, "v_position2");

	//the following MAY not be valid in gles2.
	if (gl_config.maxattribs > VATTR_BONENUMS)
		qglBindAttribLocationARB(program, VATTR_BONENUMS, "v_bone");
	if (gl_config.maxattribs > VATTR_BONEWEIGHTS)
		qglBindAttribLocationARB(program, VATTR_BONEWEIGHTS, "v_weight");
#if MAXRLIGHTMAPS > 1
	if (gl_config.maxattribs > VATTR_COLOUR2)
		qglBindAttribLocationARB(program, VATTR_COLOUR2, "v_colour2");
	if (gl_config.maxattribs > VATTR_COLOUR3)
		qglBindAttribLocationARB(program, VATTR_COLOUR3, "v_colour3");
	if (gl_config.maxattribs > VATTR_COLOUR4)
		qglBindAttribLocationARB(program, VATTR_COLOUR4, "v_colour4");
#endif
#if MAXRLIGHTMAPS > 1
	if (gl_config.maxattribs > VATTR_LMCOORD2)
		qglBindAttribLocationARB(program, VATTR_LMCOORD2, "v_lmcoord2");
	if (gl_config.maxattribs > VATTR_LMCOORD3)
		qglBindAttribLocationARB(program, VATTR_LMCOORD3, "v_lmcoord3");
	if (gl_config.maxattribs > VATTR_LMCOORD4)
		qglBindAttribLocationARB(program, VATTR_LMCOORD4, "v_lmcoord4");
#endif

	qglLinkProgramARB(program);
	return program;
}

qboolean GLSlang_ValidateProgram(union programhandle_u *h, const char *name, qboolean silent, vfsfile_t *blobfile)
{
	char		str[2048];
	GLint linked;

	if (!h->glsl.handle)
		return false;
	qglGetProgramParameteriv_(h->glsl.handle, GL_OBJECT_LINK_STATUS_ARB, &linked);

	if(!linked)
	{
		if (!silent)
		{
			qglGetProgramInfoLog_(h->glsl.handle, sizeof(str), NULL, str);
			Con_Printf("Program link error on glsl program %s:\n%s\n", name, str);
		}

		qglDeleteProgramObject_(h->glsl.handle);
		h->glsl.handle = 0;

		return (GLhandleARB)0;
	}

	if (h->glsl.handle && blobfile && qglGetProgramBinary)
	{
		GLuint ui;
		GLenum e;
		unsigned int len, fmt;
		void *blobdata;

		qglGetProgramParameteriv_(h->glsl.handle, GL_PROGRAM_BINARY_LENGTH, &ui);
		len = ui;

		blobdata = BZ_Malloc(len);
		qglGetProgramBinary(h->glsl.handle, len, NULL, &e, blobdata);
		fmt = e;

		VFS_WRITE(blobfile, &fmt, sizeof(fmt));
		VFS_WRITE(blobfile, &len, sizeof(len));
		VFS_WRITE(blobfile, blobdata, len);
		VFS_WRITE(blobfile, &h->glsl.usetesselation, sizeof(h->glsl.usetesselation));
		BZ_Free(blobdata);
	}

	return true;
}

union programhandle_u GLSlang_CreateProgram(program_t *prog, const char *name, int ver, const char **precompilerconstants, const char *vert, const char *cont, const char *eval, const char *geom, const char *frag, qboolean silent, vfsfile_t *blobfile)
{
	union programhandle_u ret;
	GLhandleARB vs;
	GLhandleARB gs;
	GLhandleARB fs;
	GLhandleARB cs;
	GLhandleARB es;
	const char *nullconstants = NULL;

	memset(&ret, 0, sizeof(ret));

	if (!gl_config.arb_shader_objects)
		return ret;
	if ((cont || eval) && !qglPatchParameteriARB)
	{
		Con_Printf("GLSlang_CreateProgram: %s requires tesselation support, but your gl drivers do not appear to support this (gl4.0 feature)\n", name);
		return ret;
	}
	if (geom && !gl_config.geometryshaders)
	{
		Con_Printf("GLSlang_CreateProgram: %s requires geometry shader support, but your gl drivers do not appear to support this (gl3.2 feature)\n", name);
		return ret;
	}

	if (!precompilerconstants)
		precompilerconstants = &nullconstants;

	fs = GLSlang_CreateShader(prog, name, ver, precompilerconstants, frag, GL_FRAGMENT_SHADER_ARB, silent);
	gs = GLSlang_CreateShader(prog, name, ver, precompilerconstants, geom, GL_GEOMETRY_SHADER_ARB, silent);
	vs = GLSlang_CreateShader(prog, name, ver, precompilerconstants, vert, GL_VERTEX_SHADER_ARB, silent);
	cs = GLSlang_CreateShader(prog, name, ver, precompilerconstants, cont, GL_TESS_CONTROL_SHADER_ARB, silent);
	es = GLSlang_CreateShader(prog, name, ver, precompilerconstants, eval, GL_TESS_EVALUATION_SHADER_ARB, silent);

	fs = GLSlang_FinishShader(fs, name, GL_FRAGMENT_SHADER_ARB, &silent);
	gs = GLSlang_FinishShader(gs, name, GL_GEOMETRY_SHADER_ARB, &silent);
	vs = GLSlang_FinishShader(vs, name, GL_VERTEX_SHADER_ARB, &silent);
	cs = GLSlang_FinishShader(cs, name, GL_TESS_CONTROL_SHADER_ARB, &silent);
	es = GLSlang_FinishShader(es, name, GL_TESS_EVALUATION_SHADER_ARB, &silent);

	if (!vs || !fs)
		ret.glsl.handle = 0;
	else
		ret.glsl.handle = GLSlang_CreateProgramObject(name, vs, cs, es, gs, fs);
	//delete ignores 0s.
	if (vs) qglDeleteShaderObject_(vs);
	if (gs) qglDeleteShaderObject_(gs);
	if (fs) qglDeleteShaderObject_(fs);
	if (cs) qglDeleteShaderObject_(cs);
	if (es) qglDeleteShaderObject_(es);

	checkglerror();

	ret.glsl.usetesselation = (cont || eval);
	if (ret.glsl.handle && blobfile && qglGetProgramBinary)
	{
		GLuint ui;
		GLenum e;
		unsigned int len, fmt;
		void *blobdata;

		qglGetProgramParameteriv_(ret.glsl.handle, GL_PROGRAM_BINARY_LENGTH, &ui);
		len = ui;

		blobdata = BZ_Malloc(len);
		qglGetProgramBinary(ret.glsl.handle, len, NULL, &e, blobdata);
		fmt = e;

		VFS_WRITE(blobfile, &fmt, sizeof(fmt));
		VFS_WRITE(blobfile, &len, sizeof(len));
		VFS_WRITE(blobfile, blobdata, len);
		VFS_WRITE(blobfile, &ret.glsl.usetesselation, sizeof(ret.glsl.usetesselation));
		BZ_Free(blobdata);
	}

	return ret;
}

qboolean GLSlang_ValidateProgramPermu(program_t *prog, const char *name, unsigned int permu, qboolean noerrors, vfsfile_t *blobfile)
{
	return GLSlang_ValidateProgram(&prog->permu[permu].h, name, noerrors, blobfile);
}
qboolean GLSlang_CreateProgramPermu(program_t *prog, const char *name, unsigned int permu, int ver, const char **precompilerconstants, const char *vert, const char *tcs, const char *tes, const char *geom, const char *frag, qboolean noerrors, vfsfile_t *blobfile)
{
	if (!ver)
	{
		if (gl_config.gles)
			ver = 100;
		else
		{
			ver = 110;
			if (sh_config.maxver>=120 && (permu & PERMUTATION_SKELETAL))
				ver = 120;
		}
	}
	if ((permu & PERMUTATION_SKELETAL) && gl_config.maxattribs < 10)
		return false;	//can happen in gles2
#if MAXRLIGHTMAPS > 1
	if ((permu & PERMUTATION_LIGHTSTYLES) && gl_config.maxattribs < 16)
		return false;	//can happen in gles2
#endif

	prog->permu[permu].h = GLSlang_CreateProgram(prog, name, ver, precompilerconstants, vert, tcs, tes, geom, frag, noerrors, blobfile);
	if (prog->permu[permu].h.glsl.handle)
		return true;
	return false;
}

GLint GLSlang_GetUniformLocation (int prog, char *name)
{
	int i = qglGetUniformLocationARB(prog, name);
	if (i == -1)
	{
		Con_Printf("Failed to get location of uniform '%s'\n", name);
	}
	return i;
}

static qboolean GLSlang_LoadBlob(program_t *prog, const char *name, unsigned int permu, vfsfile_t *blobfile)
{
	unsigned int fmt;
	unsigned int length;
	void *binary;
	GLint success;
	if (!qglProgramBinary)
		return false;
	VFS_READ(blobfile, &fmt, sizeof(fmt));
	VFS_READ(blobfile, &length, sizeof(length));
	binary = BZ_Malloc(length);
	VFS_READ(blobfile, binary, length);
	VFS_READ(blobfile, &prog->permu[permu].h.glsl.usetesselation, sizeof(prog->permu[permu].h.glsl.usetesselation));

	prog->permu[permu].h.glsl.handle = qglCreateProgramObjectARB();
	qglProgramBinary(prog->permu[permu].h.glsl.handle, fmt, binary, length);
	BZ_Free(binary);
	qglGetProgramParameteriv_(prog->permu[permu].h.glsl.handle, GL_OBJECT_LINK_STATUS_ARB, &success);

	if (!success)
	{
		qglDeleteProgramObject_(prog->permu[permu].h.glsl.handle);
		memset(&prog->permu[permu].h, 0, sizeof(prog->permu[permu].h));
	}
	return !!success;
}

static void GLSlang_DeleteProg(program_t *prog)
{
	unsigned int permu;
	for (permu = 0; permu < countof(prog->permu); permu++)
	{
		if (prog->permu[permu].h.loaded)
		{
			qglDeleteProgramObject_(prog->permu[permu].h.glsl.handle);
			prog->permu[permu].h.glsl.handle = 0;

			BZ_Free(prog->permu[permu].parm);
			prog->permu[permu].parm = NULL;
			prog->permu[permu].numparms = 0;
		}
	}
}

static void GLSlang_ProgAutoFields(program_t *prog, const char *progname, cvar_t **cvars, char **cvarnames, int *cvartypes)
{
#define ALTLIGHTMAPSAMP 13
#define ALTDELUXMAPSAMP 16

	unsigned int i, p;
	int uniformloc;
	char tmpname[128];
	struct programpermu_s *pp;

	//figure out visible attributes
	for (p = 0; p < PERMUTATIONS; p++)
	{
		if (!prog->permu[p].h.loaded)
			continue;
		GLSlang_UseProgram(prog->permu[p].h.glsl.handle);
		for (i = 0; shader_attr_names[i].name; i++)
		{
			uniformloc = qglGetAttribLocationARB(prog->permu[p].h.glsl.handle, shader_attr_names[i].name);
			if (uniformloc != -1)
			{
				if (shader_attr_names[i].ptype != uniformloc)
					Con_Printf("Bad attribute: %s\n", shader_attr_names[i].name);
				else
					prog->permu[p].attrmask |= 1u<<uniformloc;
			}
		}
	}

	prog->numsamplers = 0;
	prog->defaulttextures = 0;
	for (p = 0; p < PERMUTATIONS; p++)
	{
		int maxparms = 0;
		pp = &prog->permu[p];
		if (!pp->h.loaded)
			continue;
		pp->numparms = 0;
		pp->parm = NULL;

		GLSlang_UseProgram(prog->permu[p].h.glsl.handle);	//we'll probably be setting samplers anyway.
		for (i = 0; shader_unif_names[i].name; i++)
		{
			uniformloc = qglGetUniformLocationARB(pp->h.glsl.handle, shader_unif_names[i].name);
			if (uniformloc >= 0)
			{
				if (pp->numparms >= maxparms)
				{
					maxparms = pp->numparms?pp->numparms * 2:8;
					pp->parm = BZ_Realloc(pp->parm, sizeof(*pp->parm) * maxparms);
				}
				pp->parm[pp->numparms].type = shader_unif_names[i].ptype;
				pp->parm[pp->numparms].handle = uniformloc;
				pp->parm[pp->numparms].pval = NULL;
				pp->numparms++;
			}
		}

		/*set cvar uniforms*/
		/*FIXME: enumerate cvars automatically instead*/
		for (i = 0; cvarnames[i]; i++)
		{			
			if (!cvars[i])
				continue;

			uniformloc = qglGetUniformLocationARB(pp->h.glsl.handle, va("cvar_%s", cvarnames[i]));
			if (uniformloc >= 0)
			{
				if (pp->numparms >= maxparms)
				{
					maxparms = pp->numparms?pp->numparms * 2:8;
					pp->parm = BZ_Realloc(pp->parm, sizeof(*pp->parm) * maxparms);
				}
				pp->parm[pp->numparms].type = cvartypes[i];
				pp->parm[pp->numparms].pval = cvars[i];
				pp->parm[pp->numparms].handle = uniformloc;
				pp->numparms++;
			}
		}

		//now scan/set texture uniforms
		if (!(pp->attrmask & (1u<<VATTR_VERTEX1)))	//a shader kinda has to use one of these...
			pp->attrmask |= (1u<<VATTR_LEG_VERTEX);
		for (i = 0; i < 8; i++)
		{
			Q_snprintfz(tmpname, sizeof(tmpname), "s_t%i", i);
			uniformloc = qglGetUniformLocationARB(pp->h.glsl.handle, tmpname);
			if (uniformloc != -1)
			{
				qglUniform1iARB(uniformloc, i);
				if (prog->numsamplers < i+1)
					prog->numsamplers = i+1;
			}
		}
		for (i = 0; sh_defaultsamplers[i].name; i++)
		{
			//figure out which ones are needed.
			if (prog->defaulttextures & sh_defaultsamplers[i].defaulttexbits)
				continue;	//don't spam
			uniformloc = qglGetUniformLocationARB(pp->h.glsl.handle, sh_defaultsamplers[i].name);
			if (uniformloc != -1)
				prog->defaulttextures |= sh_defaultsamplers[i].defaulttexbits;
		}
	}

	//multiple lightmaps is kinda hacky. if any are set, all must be. 
	if (prog->defaulttextures & ((1u<<(ALTLIGHTMAPSAMP+0)) | (1u<<(ALTLIGHTMAPSAMP+1)) | (1u<<(ALTLIGHTMAPSAMP+2))))
		prog->defaulttextures |=((1u<<(ALTLIGHTMAPSAMP+0)) | (1u<<(ALTLIGHTMAPSAMP+1)) | (1u<<(ALTLIGHTMAPSAMP+2)));
	if (prog->defaulttextures & ((1u<<(ALTDELUXMAPSAMP+0)) | (1u<<(ALTDELUXMAPSAMP+1)) | (1u<<(ALTDELUXMAPSAMP+2))))
		prog->defaulttextures |=((1u<<(ALTDELUXMAPSAMP+0)) | (1u<<(ALTDELUXMAPSAMP+1)) | (1u<<(ALTDELUXMAPSAMP+2)));

	if (prog->defaulttextures)
	{
		unsigned int sampnum;
		/*set default texture uniforms now that we know the right sampler ids*/
		for (p = 0; p < PERMUTATIONS; p++)
		{
			if (!prog->permu[p].h.glsl.handle)
				continue;
			sampnum = prog->numsamplers;
			GLSlang_UseProgram(prog->permu[p].h.glsl.handle);
			for (i = 0; sh_defaultsamplers[i].name; i++)
			{
				if (prog->defaulttextures & sh_defaultsamplers[i].defaulttexbits)
				{
					uniformloc = qglGetUniformLocationARB(prog->permu[p].h.glsl.handle, sh_defaultsamplers[i].name);
					if (uniformloc != -1)
						qglUniform1iARB(uniformloc, sampnum);
					sampnum++;
				}
			}
		}
	}
}

//the vid routines have initialised a window, and now they are giving us a reference to some of of GetProcAddress to get pointers to the funcs.
void GL_Init(void *(*getglfunction) (char *name))
{
#ifndef GL_STATIC
	qglBindTexture			= (void *)getglcore("glBindTexture");	//for compleateness. core in 1.1. needed by fte.
	qglBlendFunc		= (void *)getglcore("glBlendFunc");
	qglClear			= (void *)getglcore("glClear");
	qglClearColor		= (void *)getglcore("glClearColor");
	qglClearStencil		= (void *)getglcore("glClearStencil");
	qglColorMask		= (void *)getglcore("glColorMask");
	qglCopyTexImage2D	= (void *)getglcore("glCopyTexImage2D");
	qglCopyTexSubImage2D= (void *)getglcore("glCopyTexSubImage2D");
	qglCullFace			= (void *)getglcore("glCullFace");
	qglDepthFunc		= (void *)getglcore("glDepthFunc");
	qglDepthMask		= (void *)getglcore("glDepthMask");
	qglDepthRangef		= (void *)getglcore("glDepthRangef");
	qglDisable			= (void *)getglcore("glDisable");
	qglEnable			= (void *)getglcore("glEnable");
	qglFinish			= (void *)getglcore("glFinish");
	qglFlush			= (void *)getglcore("glFlush");
	qglGenTextures		= (void *)getglcore("glGenTextures");
	qglGetFloatv		= (void *)getglcore("glGetFloatv");
	qglGetIntegerv		= (void *)getglcore("glGetIntegerv");
	qglGetString		= (void *)getglcore("glGetString");
	qglHint				= (void *)getglcore("glHint");
	qglIsEnabled		= (void *)getglext("glIsEnabled");
	qglReadPixels		= (void *)getglcore("glReadPixels");
	qglTexImage2D		= (void *)getglcore("glTexImage2D");
	qglTexSubImage2D	= (void *)getglcore("glTexSubImage2D");
	qglTexParameteri	= (void *)getglcore("glTexParameteri");
	qglTexParameterf	= (void *)getglcore("glTexParameterf");
	qglTexParameteriv	= (void *)getglcore("glTexParameteriv");
	qglTexParameterfv	= (void *)getglcore("glTexParameterfv");
	qglViewport			= (void *)getglcore("glViewport");
	qglGetBooleanv		= (void *)getglcore("glGetBooleanv");
	qglGetError			= (void *)getglcore("glGetError");
	qglDeleteTextures	= (void *)getglcore("glDeleteTextures");
	qglDrawElements			= (void *)getglcore("glDrawElements");
	qglDrawArrays			= (void *)getglcore("glDrawArrays");
	qglStencilOp		= (void *)getglcore("glStencilOp");
	qglStencilFunc		= (void *)getglcore("glStencilFunc");
	qglScissor			= (void *)getglcore("glScissor");
	qglPolygonOffset	= (void *)getglext("glPolygonOffset");
#endif
#ifndef FTE_TARGET_WEB
	qglAlphaFunc		= (void *)getglcore("glAlphaFunc");
	qglBegin			= (void *)getglcore("glBegin");
	qglClearDepth		= (void *)getglcore("glClearDepth");
	qglClipPlane 		= (void *)getglcore("glClipPlane");
//	qglColor3f			= (void *)getglcore("glColor3f");
//	qglColor3ub			= (void *)getglcore("glColor3ub");
	qglColor4f			= (void *)getglcore("glColor4f");
	qglColor4fv			= (void *)getglext("glColor4fv");
	if (!qglColor4fv)
		qglColor4fv = GL_Color4fv_Emul;	//can be missing in gles1
//	qglColor4ub			= (void *)getglcore("glColor4ub");
//	qglColor4ubv		= (void *)getglcore("glColor4ubv");
	qglDepthRange		= (void *)getglcore("glDepthRange");
	qglDrawBuffer		= (void *)getglcore("glDrawBuffer");
	qglDrawPixels		= (void *)getglcore("glDrawPixels");
	qglEnd				= (void *)getglcore("glEnd");
	qglFrustum			= (void *)getglcore("glFrustum");
	qglGetTexLevelParameteriv	= (void *)getglcore("glGetTexLevelParameteriv");
	qglLoadIdentity		= (void *)getglcore("glLoadIdentity");
	qglLoadMatrixf		= (void *)getglcore("glLoadMatrixf");
	qglNormal3f			= (void *)getglcore("glNormal3f");
	qglNormal3fv		= (void *)getglcore("glNormal3fv");
	qglMatrixMode		= (void *)getglcore("glMatrixMode");
	qglMultMatrixf		= (void *)getglcore("glMultMatrixf");
//	qglOrtho			= (void *)getglcore("glOrtho");
	qglPolygonMode		= (void *)getglcore("glPolygonMode");
	qglPopMatrix		= (void *)getglcore("glPopMatrix");
	qglPushMatrix		= (void *)getglcore("glPushMatrix");
	qglReadBuffer		= (void *)getglcore("glReadBuffer");
	qglRotatef			= (void *)getglcore("glRotatef");
	qglScalef			= (void *)getglcore("glScalef");
	qglShadeModel		= (void *)getglcore("glShadeModel");
	qglTexCoord1f		= (void *)getglcore("glTexCoord1f");
	qglTexCoord2f		= (void *)getglcore("glTexCoord2f");
	qglTexCoord2fv		= (void *)getglcore("glTexCoord2fv");
	qglTexEnvf			= (void *)getglcore("glTexEnvf");
	qglTexEnvfv			= (void *)getglcore("glTexEnvfv");
	qglTexEnvi			= (void *)getglcore("glTexEnvi");
	qglTexGeni			= (void *)getglcore("glTexGeni");
	qglTexGenfv			= (void *)getglcore("glTexGenfv");
	qglTexImage3D		= (void *)getglext("glTexImage3D");
	qglTranslatef		= (void *)getglcore("glTranslatef");
	qglVertex2f			= (void *)getglcore("glVertex2f");
	qglVertex3f			= (void *)getglcore("glVertex3f");
	qglVertex3fv		= (void *)getglcore("glVertex3fv");
#endif

	//various vertex array stuff.
	qglArrayElement			= (void *)getglcore("glArrayElement");
	qglVertexPointer		= (void *)getglcore("glVertexPointer");
	qglNormalPointer		= (void *)getglcore("glNormalPointer");
	qglTexCoordPointer		= (void *)getglcore("glTexCoordPointer");
	qglColorPointer			= (void *)getglcore("glColorPointer");
	qglEnableClientState	= (void *)getglcore("glEnableClientState");
	qglDisableClientState	= (void *)getglcore("glDisableClientState");

	qglDrawRangeElements	= (void *)getglext("glDrawRangeElements");
	if (qglDrawRangeElements == 0)
		qglDrawRangeElements = GL_DrawRangeElementsEmul;

	//fixme: definatly make non-core
	qglPushAttrib		= (void *)getglcore("glPushAttrib");
	qglPopAttrib		= (void *)getglcore("glPopAttrib");

	//does this need to be non-core as well?
	qglFogi				= (void *)getglcore("glFogi");
	qglFogf				= (void *)getglcore("glFogf");
	qglFogfv			= (void *)getglcore("glFogfv");


	qglGetTexEnviv		= (void *)getglext("glGetTexEnviv");
	qglGetPointerv		= (void *)getglext("glGetPointerv");

	qglGetStringi		= (void *)getglext("glGetStringi");

	//used by heightmaps
	qglGenLists		= (void*)getglcore("glGenLists");
	qglNewList		= (void*)getglcore("glNewList");
	qglEndList		= (void*)getglcore("glEndList");
	qglCallList		= (void*)getglcore("glCallList");

#ifndef GL_STATIC
	qglBindBufferARB		= (void *)getglext("glBindBufferARB");
	if (!qglBindBufferARB)
		qglBindBufferARB	= (void *)getglext("glBindBuffer");
	if (!qglBindBufferARB)
		qglBindBufferARB	= GL_BindBufferARBStub;
#endif

	if (!qglGetString)
		Sys_Error("qglGetString not set. Serious gl library initialisation error\n");

	gl_vendor = qglGetString (GL_VENDOR);
	Con_SafePrintf ("GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = qglGetString (GL_RENDERER);
	Con_SafePrintf ("GL_RENDERER: %s\n", gl_renderer);

	gl_version = qglGetString (GL_VERSION);
	Con_SafePrintf ("GL_VERSION: %s\n", gl_version);

	memset(&sh_config, 0, sizeof(sh_config));

	GL_CheckExtensions (getglfunction);

	if ((gl_config.gles && gl_config.glversion >= 3) || (!gl_config.gles && gl_config.glversion >= 2))
		qglDrawBuffers = (void *)getglext("glDrawBuffers");
	else
		qglDrawBuffers = NULL;

	if (gl_config.gles && gl_config.glversion >= 2)
	{
		/*these functions do not exist in gles2, they only exist on some platforms because they were provided for gl1*/
		qglLoadMatrixf = NULL;
		qglPolygonMode = NULL;
		qglShadeModel = NULL;
		qglDepthRange = NULL;
		qglDrawBuffer = NULL;

		qglEnableClientState = GL_ClientStateStub;
		qglDisableClientState = GL_ClientStateStub;

		qglDrawRangeElements = GL_DrawRangeElementsEmul;
	}
	else if (gl_config_nofixedfunc)
	{
		qglLoadMatrixf = NULL;
		qglPolygonMode = NULL;
		qglShadeModel = NULL;
		qglDepthRange = NULL;
		qglDrawBuffer = NULL;

		qglEnableClientState = GL_ClientStateStub;
		qglDisableClientState = GL_ClientStateStub;
	}

	qglClearColor (0,0,0,1);	//clear to black so that it looks a little nicer on start.
	qglClear(GL_COLOR_BUFFER_BIT);

	if (qglPolygonMode)
		qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	if (qglShadeModel)
		qglShadeModel (GL_FLAT);

#ifdef DEBUG
	if (qglDebugMessageControlARB)
		qglDebugMessageControlARB(0, 0, 0, 0, NULL, true);
	if (qglDebugMessageCallbackARB)
	{
		qglDebugMessageCallbackARB(myGLDEBUGPROCAMD, NULL);
		qglEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
	qglGetError();	/*suck up the invalid operation error for non-debug contexts*/
#endif

	sh_config.texture_maxsize = 256;	//early minidrivers might not implement this, but anything else should.
	qglGetIntegerv(GL_MAX_TEXTURE_SIZE, &sh_config.texture_maxsize);

	//always supported
	sh_config.texfmt[PTI_RGBA8] = true;
	if (GL_CheckExtension("GL_EXT_texture_compression_s3tc"))
	{
		sh_config.texfmt[PTI_S3RGB1] = true;
		sh_config.texfmt[PTI_S3RGBA1] = true;
		sh_config.texfmt[PTI_S3RGBA3] = true;
		sh_config.texfmt[PTI_S3RGBA5] = true;
	}
	else if (gl_config.gles)
	{
		sh_config.texfmt[PTI_S3RGB1] =
		sh_config.texfmt[PTI_S3RGBA1] = GL_CheckExtension("GL_EXT_texture_compression_dxt1");
		sh_config.texfmt[PTI_S3RGBA3] = GL_CheckExtension("GL_ANGLE_texture_compression_dxt3");
		sh_config.texfmt[PTI_S3RGBA5] = GL_CheckExtension("GL_ANGLE_texture_compression_dxt5");
	}

	if (gl_config.gles)
	{
		sh_config.texfmt[PTI_RGBX8] = sh_config.texfmt[PTI_RGBA8];	//FIXME: this is faked with PTI_RGBA8

		sh_config.texfmt[PTI_RGB565] = !gl_config.webgl_ie;	//ie sucks and doesn't support things that webgl requires it to support.
		sh_config.texfmt[PTI_RGBA4444] = !gl_config.webgl_ie;
		sh_config.texfmt[PTI_RGBA5551] = !gl_config.webgl_ie;
		sh_config.texfmt[PTI_BGRA8] = false;
		sh_config.texfmt[PTI_BGRX8] = sh_config.texfmt[PTI_BGRA8];

		sh_config.minver = 100;
		sh_config.maxver = 100;
		sh_config.blobpath = "gles/%s.blob";
		sh_config.progpath = "glsl/%s.glsl";
		sh_config.shadernamefmt = "%s_gles";

		sh_config.can_mipcap = gl_config.glversion >= 3.0;
	}
	else
	{
		sh_config.can_mipcap = gl_config.glversion >= 1.2;

		sh_config.texfmt[PTI_RGBX8] = true;	//proper support

		//these require stuff like GL_UNSIGNED_SHORT_5_5_5_1 etc, which needs gl 1.2+
		if (gl_config.glversion >= 1.2)
		{
			//16bit formats
			sh_config.texfmt[PTI_RGB565] = true;
			sh_config.texfmt[PTI_RGBA4444] = true;
			sh_config.texfmt[PTI_RGBA5551] = true;
			//bgr formats
			if (GL_CheckExtension("GL_EXT_bgra"))
			{
				//32bit formats
				sh_config.texfmt[PTI_BGRX8] = true;
				sh_config.texfmt[PTI_BGRA8] = true;
				//16bit formats
				sh_config.texfmt[PTI_ARGB4444] = true;
				sh_config.texfmt[PTI_ARGB1555] = true;
			}
		}
		if (!gl_config.gles && (gl_config.glversion >= 1.4 || GL_CheckExtension("GL_ARB_depth_texture")))
		{	//depth formats
			sh_config.texfmt[PTI_DEPTH16] = true;
			sh_config.texfmt[PTI_DEPTH24] = true;
			sh_config.texfmt[PTI_DEPTH32] = true;
		}
		else if (gl_config.gles && GL_CheckExtension("GL_OES_depth_texture"))
		{	//16+32, not 24.
			sh_config.texfmt[PTI_DEPTH16] = true;
			sh_config.texfmt[PTI_DEPTH32] = true;
		}
		if (GL_CheckExtension("GL_EXT_packed_depth_stencil"))
			sh_config.texfmt[PTI_DEPTH24_8] = true;

		sh_config.minver = gl_config.arb_shader_objects?110:0;
		sh_config.maxver = gl_config.arb_shader_objects?gl_config.maxglslversion:0;
		sh_config.blobpath = "glsl/%s.blob";
		sh_config.progpath = "glsl/%s.glsl";
		sh_config.shadernamefmt = "%s_glsl";
	}

	sh_config.progs_supported	= gl_config.arb_shader_objects;
	sh_config.progs_required	= gl_config_nofixedfunc;

	if (gl_config.arb_shader_objects)
	{
		sh_config.pDeleteProg		= GLSlang_DeleteProg;
		sh_config.pLoadBlob			= qglProgramBinary?GLSlang_LoadBlob:NULL;
		sh_config.pCreateProgram	= GLSlang_CreateProgramPermu;
		sh_config.pValidateProgram	= GLSlang_ValidateProgramPermu;
		sh_config.pProgAutoFields	= GLSlang_ProgAutoFields;
	}

	if (gl_config_nofixedfunc)
	{
		sh_config.tex_env_combine		= 1;
		sh_config.nv_tex_env_combine4	= 1;
		sh_config.env_add				= 1;
	}
	else
	{
		sh_config.tex_env_combine		= gl_config.tex_env_combine;
		sh_config.nv_tex_env_combine4	= gl_config.nv_tex_env_combine4;
		sh_config.env_add				= gl_config.env_add;
	}
}


#ifdef DEBUG
#define GL_VERTEX_ARRAY_BINDING					0x85B5
#define GL_ARRAY_BUFFER							0x8892
#define GL_ELEMENT_ARRAY_BUFFER					0x8893
#define GL_ARRAY_BUFFER_BINDING					0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING			0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING			0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING			0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING			0x8898
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING   0x889A
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED			0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE				0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE			0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE				0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED		0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER			0x8645
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING	0x889F
#define GL_CURRENT_PROGRAM						0x8B8D

static char *DecodeGLEnum(GLenum num)
{
	switch(num)
	{
	case GL_CW:						return "GL_CW";
	case GL_CCW:					return "GL_CCW";
	case GL_NEVER:					return "GL_NEVER";
	case GL_LESS:					return "GL_LESS";
	case GL_EQUAL:					return "GL_EQUAL";
	case GL_LEQUAL:					return "GL_LEQUAL";
	case GL_GREATER:				return "GL_GREATER";
	case GL_NOTEQUAL:				return "GL_NOTEQUAL";
	case GL_GEQUAL:					return "GL_GEQUAL";
	case GL_ALWAYS:					return "GL_ALWAYS";
	case GL_FRONT:					return "GL_FRONT";
	case GL_BACK:					return "GL_BACK";
	case GL_FRONT_AND_BACK:			return "GL_FRONT_AND_BACK";
	case GL_COMBINE_ARB:			return "GL_COMBINE";
	case GL_MODULATE:				return "GL_MODULATE";
	case GL_REPLACE:				return "GL_REPLACE";
	case GL_ZERO:					return "GL_ZERO";
	case GL_ONE:					return "GL_ONE";
	case GL_SRC_COLOR:				return "GL_SRC_COLOR";
	case GL_ONE_MINUS_SRC_COLOR:	return "GL_ONE_MINUS_SRC_COLOR";
	case GL_SRC_ALPHA:				return "GL_SRC_ALPHA";
	case GL_ONE_MINUS_SRC_ALPHA:	return "GL_ONE_MINUS_SRC_ALPHA";
	case GL_DST_ALPHA:				return "GL_DST_ALPHA";
	case GL_ONE_MINUS_DST_ALPHA:	return "GL_ONE_MINUS_DST_ALPHA";
	case GL_DST_COLOR:				return "GL_DST_COLOR";
	case GL_ONE_MINUS_DST_COLOR:	return "GL_ONE_MINUS_DST_COLOR";
	case GL_SRC_ALPHA_SATURATE:		return "GL_SRC_ALPHA_SATURATE";
	default:						return va("0x%x", num);
	}
}
void DumpGLState(void)
{
	int rval;
	void *ptr;
	int i;
	GLint glint;
	GLint glint4[4];

//	if (qglGetVertexAttribiv)
	{
		if (qglBindVertexArray)
		{
			qglGetIntegerv(GL_VERTEX_ARRAY_BINDING, &rval);
			Sys_Printf("VERTEX_ARRAY_BINDING: %i\n", rval);
		}
		if (qglBindBufferARB)
		{
			qglGetIntegerv(GL_ARRAY_BUFFER_BINDING, &rval);
			Sys_Printf("GL_ARRAY_BUFFER_BINDING: %i\n", rval);
		}
		if (qglIsEnabled(GL_COLOR_ARRAY))
		{
			if (qglBindBufferARB)
				qglGetIntegerv(GL_COLOR_ARRAY_BUFFER_BINDING, &rval);
			else
				rval = 0;
			qglGetPointerv(GL_COLOR_ARRAY_POINTER, &ptr);
			Sys_Printf("GL_COLOR_ARRAY: %s %i:%p\n", qglIsEnabled(GL_COLOR_ARRAY)?"en":"dis", rval, ptr);
		}
//		if (qglIsEnabled(GL_FOG_COORDINATE_ARRAY_EXT))
//		{
//			qglGetPointerv(GL_FOG_COORD_ARRAY_POINTER, &ptr);
//			Sys_Printf("GL_FOG_COORDINATE_ARRAY_EXT: %i (%lx)\n", (int) qglIsEnabled(GL_FOG_COORDINATE_ARRAY_EXT), (int) ptr);
//		}
		{
			if (qglBindBufferARB)
				qglGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &rval);
			else
				rval = 0;
			Sys_Printf("GL_ELEMENT_ARRAY_BUFFER_BINDING: %i:%p\n", rval, (void*)0);
		}
		if (qglIsEnabled(GL_NORMAL_ARRAY))
		{
			if (qglBindBufferARB)
				qglGetIntegerv(GL_NORMAL_ARRAY_BUFFER_BINDING, &rval);
			else
				rval = 0;
			qglGetPointerv(GL_NORMAL_ARRAY_POINTER, &ptr);
			Sys_Printf("GL_NORMAL_ARRAY: %s %i:%p\n", qglIsEnabled(GL_NORMAL_ARRAY)?"en":"dis", rval, ptr);
		}
	//	qglGetPointerv(GL_SECONDARY_COLOR_ARRAY_POINTER, &ptr);
	//	Sys_Printf("GL_SECONDARY_COLOR_ARRAY: %i (%lx)\n", (int) qglIsEnabled(GL_SECONDARY_COLOR_ARRAY), (int) ptr);
		for (i = 0; i < 4; i++)
		{
			qglClientActiveTextureARB(mtexid0 + i);
			if (qglIsEnabled(GL_TEXTURE_COORD_ARRAY))
			{
				if (qglBindBufferARB)
					qglGetIntegerv(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING, &rval);
				else
					rval = 0;
				qglGetPointerv(GL_TEXTURE_COORD_ARRAY_POINTER, &ptr);
				Sys_Printf("GL_TEXTURE_COORD_ARRAY %i: %s %i:%p\n", i, qglIsEnabled(GL_TEXTURE_COORD_ARRAY)?"en":"dis", rval, ptr);
			}
		}
		if (qglIsEnabled(GL_VERTEX_ARRAY))
		{
			if (qglBindBufferARB)
				qglGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &rval);
			else
				rval = 0;
			qglGetPointerv(GL_VERTEX_ARRAY_POINTER, &ptr);
			Sys_Printf("GL_VERTEX_ARRAY: %s %i:%p\n", qglIsEnabled(GL_VERTEX_ARRAY)?"en":"dis", rval, ptr);
		}

		if (qglGetVertexAttribiv)
		for (i = 0; i < 16; i++)
		{
			int en, bo, as, st, ty, no;

			qglGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &en);
			if (!en)
				continue;
			if (qglBindBufferARB)
				qglGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bo);
			else
				bo = 0;
			qglGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &as);
			qglGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &st);
			qglGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &ty);
			qglGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &no);
			qglGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &ptr);

			Sys_Printf("attrib%i: %s sz:%i st:%i ty:%0x %s%i:%p\n", i, en?"en":"dis", as, st,ty,no?"norm ":"", bo, ptr);
		}

		if (qglUseProgramObjectARB)
		{
			qglGetIntegerv(GL_CURRENT_PROGRAM, &glint);
			Sys_Printf("GL_CURRENT_PROGRAM: %i\n", glint);
		}

		qglGetIntegerv(GL_BLEND, &glint);
		Sys_Printf("GL_BLEND: %i\n", glint);
		qglGetIntegerv(GL_BLEND_SRC, &glint);
		Sys_Printf("GL_BLEND_SRC: %s\n", DecodeGLEnum(glint));
		qglGetIntegerv(GL_BLEND_DST, &glint);
		Sys_Printf("GL_BLEND_DST: %s\n", DecodeGLEnum(glint));

		qglGetIntegerv(GL_DEPTH_WRITEMASK, &glint);
		Sys_Printf("GL_DEPTH_WRITEMASK: %i\n", glint);
		qglGetIntegerv(GL_DEPTH_TEST, &glint);
		Sys_Printf("GL_DEPTH_TEST: %i\n", glint);
		qglGetIntegerv(GL_DEPTH_FUNC, &glint);
		Sys_Printf("GL_DEPTH_FUNC: %s\n", DecodeGLEnum(glint));
		qglGetIntegerv(GL_CULL_FACE, &glint);
		Sys_Printf("GL_CULL_FACE: %i\n", glint);
		qglGetIntegerv(GL_CULL_FACE_MODE, &glint);
		Sys_Printf("GL_CULL_FACE_MODE: %s\n", DecodeGLEnum(glint));
		qglGetIntegerv(GL_FRONT_FACE, &glint);
		Sys_Printf("GL_FRONT_FACE: %s\n", DecodeGLEnum(glint));
		qglGetIntegerv(GL_SCISSOR_TEST, &glint);
		Sys_Printf("GL_SCISSOR_TEST: %i\n", glint);
		qglGetIntegerv(GL_STENCIL_TEST, &glint);
		Sys_Printf("GL_STENCIL_TEST: %i\n", glint);
		qglGetIntegerv(GL_COLOR_WRITEMASK, glint4);
		Sys_Printf("GL_COLOR_WRITEMASK: %i %i %i %i\n", glint4[0], glint4[1], glint4[2], glint4[3]);

		GL_SelectTexture(0);
		qglGetIntegerv(GL_TEXTURE_2D, &glint);
		Sys_Printf("0: GL_TEXTURE_2D: %i\n", glint);
		qglGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &glint);
		Sys_Printf("0: GL_TEXTURE_ENV_MODE: %s\n", DecodeGLEnum(glint));
		GL_SelectTexture(1);
		qglGetIntegerv(GL_TEXTURE_2D, &glint);
		Sys_Printf("1: GL_TEXTURE_2D: %i\n", glint);
		qglGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &glint);
		Sys_Printf("1: GL_TEXTURE_ENV_MODE: %s\n", DecodeGLEnum(glint));
		GL_SelectTexture(2);
		qglGetIntegerv(GL_TEXTURE_2D, &glint);
		Sys_Printf("2: GL_TEXTURE_2D: %i\n", glint);
		qglGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &glint);
		Sys_Printf("2: GL_TEXTURE_ENV_MODE: %s\n", DecodeGLEnum(glint));
	}
}
#endif



rendererinfo_t openglrendererinfo = {
	//customise the text printed depending on the actual type of opengl that we're locking ourselves to
#ifdef FTE_TARGET_WEB
	"WebGL",
#elif defined(GLESONLY)
	#ifdef GLSLONLY
		"OpenGLES2+",
	#else
		"OpenGLES",
	#endif
#else
	"OpenGL",
#endif
	{
		//reorder these too, if only so that 'setrenderer' lists gles-only builds as using gles instead of gl
#if defined(GLESONLY)
		"gles",
		"opengles",
		"gl",
		"opengl",
#else
		"gl",
		"opengl",
		"gles",
		"opengles",
#endif
	},
	QR_OPENGL,


	GLDraw_Init,
	GLDraw_DeInit,

	GL_UpdateFiltering,
	GL_LoadTextureMips,
	GL_DestroyTexture,

	GLR_Init,
	GLR_DeInit,
	GLR_RenderView,

	GLVID_Init,
	GLVID_DeInit,
	GLVID_SwapBuffers,
	GLVID_ApplyGammaRamps,
	NULL,
	NULL,
	NULL,
	GLVID_SetCaption,	//setcaption
	GLVID_GetRGBInfo,


	GLSCR_UpdateScreen,

	GLBE_SelectMode,
	GLBE_DrawMesh_List,
	GLBE_DrawMesh_Single,
	GLBE_SubmitBatch,
	GLBE_GetTempBatch,
	GLBE_DrawWorld,
	GLBE_Init,
	GLBE_GenBrushModelVBO,
	GLBE_ClearVBO,
	GLBE_UploadAllLightmaps,
	GLBE_SelectEntity,
	GLBE_SelectDLight,
	GLBE_Scissor,
	GLBE_LightCullModel,

	GLBE_VBO_Begin,
	GLBE_VBO_Data,
	GLBE_VBO_Finish,
	GLBE_VBO_Destroy,

	GLBE_RenderToTextureUpdate2d,

	""
};

#endif

