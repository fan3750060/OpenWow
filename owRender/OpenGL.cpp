#include "stdafx.h"

// General
#include "OpenGL.h"

#include <cstdlib>
#include <cstring>

#pragma region MyRegion

// GL 1.1
#if (GL_VERSION_NUM >= 11)
PFNGLGETTEXIMAGEPROC glGetTexImage = nullptr;
PFNGLPOLYGONMODEPROC glPolygonMode = nullptr;
PFNGLCLEARDEPTH glClearDepth = nullptr;
PFNGLDRAWBUFFERPROC glDrawBuffer = nullptr;
#endif

// GL 1.2
#if (GL_VERSION_NUM >= 12)
PFNGLBLENDCOLORPROC glBlendColor = nullptr;
PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = nullptr;
PFNGLTEXIMAGE3DPROC glTexImage3D = nullptr;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = nullptr;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = nullptr;
#endif

// GL 1.3
#if (GL_VERSION_NUM >= 13)
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = nullptr;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = nullptr;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = nullptr;
#endif

// GL 1.4
#if (GL_VERSION_NUM >= 14)
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = nullptr;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = nullptr;
PFNGLPOINTPARAMETERFPROC glPointParameterf = nullptr;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = nullptr;
PFNGLPOINTPARAMETERIPROC glPointParameteri = nullptr;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = nullptr;
#endif

// GL 1.5
#if (GL_VERSION_NUM >= 15)
PFNGLGENQUERIESPROC glGenQueries = nullptr;
PFNGLDELETEQUERIESPROC glDeleteQueries = nullptr;
PFNGLISQUERYPROC glIsQuery = nullptr;
PFNGLBEGINQUERYPROC glBeginQuery = nullptr;
PFNGLENDQUERYPROC glEndQuery = nullptr;
PFNGLGETQUERYIVPROC glGetQueryiv = nullptr;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = nullptr;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLISBUFFERPROC glIsBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = nullptr;
PFNGLMAPBUFFERPROC glMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = nullptr;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv = nullptr;
#endif

// GL 2.0
#if (GL_VERSION_NUM >= 20)
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = nullptr;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMFVPROC glGetUniformfv = nullptr;
PFNGLGETUNIFORMIVPROC glGetUniformiv = nullptr;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = nullptr;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = nullptr;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = nullptr;
PFNGLISPROGRAMPROC glIsProgram = nullptr;
PFNGLISSHADERPROC glIsShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FPROC glUniform2f = nullptr;
PFNGLUNIFORM3FPROC glUniform3f = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM2IPROC glUniform2i = nullptr;
PFNGLUNIFORM3IPROC glUniform3i = nullptr;
PFNGLUNIFORM4IPROC glUniform4i = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC glUniform4iv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
#endif

// GL 2.1
#if (GL_VERSION_NUM >= 21)
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv = nullptr;
#endif

// GL 3.0
#if (GL_VERSION_NUM >= 30)
PFNGLGETSTRINGIPROC glGetStringi = nullptr;
PFNGLCOLORMASKIPROC glColorMaski = 0;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v = 0;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v = 0;
PFNGLENABLEIPROC glEnablei = 0;
PFNGLDISABLEIPROC glDisablei = 0;
PFNGLISENABLEDIPROC glIsEnabledi = 0;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback = 0;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback = 0;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange = 0;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = 0;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings = 0;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying = 0;
PFNGLCLAMPCOLORPROC glClampColor = 0;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender = 0;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender = 0;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = 0;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv = 0;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv = 0;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i = 0;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i = 0;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i = 0;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i = 0;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui = 0;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui = 0;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui = 0;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui = 0;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv = 0;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv = 0;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv = 0;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv = 0;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv = 0;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv = 0;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv = 0;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv = 0;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv = 0;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv = 0;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv = 0;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv = 0;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv = 0;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = 0;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = 0;
PFNGLUNIFORM1UIPROC glUniform1ui = 0;
PFNGLUNIFORM2UIPROC glUniform2ui = 0;
PFNGLUNIFORM3UIPROC glUniform3ui = 0;
PFNGLUNIFORM4UIPROC glUniform4ui = 0;
PFNGLUNIFORM1UIVPROC glUniform1uiv = 0;
PFNGLUNIFORM2UIVPROC glUniform2uiv = 0;
PFNGLUNIFORM3UIVPROC glUniform3uiv = 0;
PFNGLUNIFORM4UIVPROC glUniform4uiv = 0;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv = 0;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv = 0;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv = 0;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv = 0;
PFNGLCLEARBUFFERIVPROC glClearBufferiv = 0;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv = 0;
PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLCLEARBUFFERFIPROC glClearBufferfi = 0;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = 0;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = 0;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = 0;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = 0;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = 0;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = 0;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = 0;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = 0;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = 0;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = 0;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = 0;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = 0;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = 0;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = 0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = 0;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = 0;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = 0;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = 0;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLISVERTEXARRAYPROC glIsVertexArray = 0;
#endif

// GL 3.1
#if (GL_VERSION_NUM >= 31)
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = 0;
PFNGLTEXBUFFERPROC glTexBuffer = 0;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = 0;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = 0;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = 0;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName = 0;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = 0;
#endif

// GL 3.2
#if (GL_VERSION_NUM >= 32)
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = 0;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex = 0;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex = 0;
PFNGLPROVOKINGVERTEXPROC glProvokingVertex = 0;
PFNGLFENCESYNCPROC glFenceSync = 0;
PFNGLISSYNCPROC glIsSync = 0;
PFNGLDELETESYNCPROC glDeleteSync = 0;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync = 0;
PFNGLWAITSYNCPROC glWaitSync = 0;
PFNGLGETINTEGER64VPROC glGetInteger64v = 0;
PFNGLGETSYNCIVPROC glGetSynciv = 0;
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v = 0;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v = 0;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = 0;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = 0;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample = 0;
PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv = 0;
PFNGLSAMPLEMASKIPROC glSampleMaski = 0;
#endif

// GL 3.3
#if (GL_VERSION_NUM >= 33)
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed = 0;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex = 0;
PFNGLGENSAMPLERSPROC glGenSamplers = 0;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = 0;
PFNGLISSAMPLERPROC glIsSampler = 0;
PFNGLBINDSAMPLERPROC glBindSampler = 0;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = 0;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv = 0;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = 0;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = 0;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv = 0;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv = 0;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv = 0;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv = 0;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv = 0;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv = 0;
PFNGLQUERYCOUNTERPROC glQueryCounter = 0;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v = 0;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v = 0;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = 0;
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui = 0;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv = 0;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui = 0;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv = 0;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui = 0;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv = 0;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui = 0;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv = 0;
#endif

// GL 4.0
#if (GL_VERSION_NUM >= 40)
PFNGLMINSAMPLESHADINGPROC glMinSampleShading = 0;
PFNGLBLENDEQUATIONIPROC glBlendEquationi = 0;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei = 0;
PFNGLBLENDFUNCIPROC glBlendFunci = 0;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei = 0;
PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect = 0;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect = 0;
PFNGLUNIFORM1DPROC glUniform1d = 0;
PFNGLUNIFORM2DPROC glUniform2d = 0;
PFNGLUNIFORM3DPROC glUniform3d = 0;
PFNGLUNIFORM4DPROC glUniform4d = 0;
PFNGLUNIFORM1DVPROC glUniform1dv = 0;
PFNGLUNIFORM2DVPROC glUniform2dv = 0;
PFNGLUNIFORM3DVPROC glUniform3dv = 0;
PFNGLUNIFORM4DVPROC glUniform4dv = 0;
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv = 0;
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv = 0;
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv = 0;
PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv = 0;
PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv = 0;
PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv = 0;
PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv = 0;
PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv = 0;
PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv = 0;
PFNGLGETUNIFORMDVPROC glGetUniformdv = 0;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation = 0;
PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex = 0;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv = 0;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName = 0;
PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName = 0;
PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv = 0;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv = 0;
PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv = 0;
PFNGLPATCHPARAMETERIPROC glPatchParameteri = 0;
PFNGLPATCHPARAMETERFVPROC glPatchParameterfv = 0;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback = 0;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks = 0;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks = 0;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback = 0;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback = 0;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback = 0;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback = 0;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream = 0;
PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed = 0;
PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed = 0;
PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv = 0;
#endif

// GL 4.1
#if (GL_VERSION_NUM >= 41)
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler = 0;
PFNGLSHADERBINARYPROC glShaderBinary = 0;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat = 0;
PFNGLDEPTHRANGEFPROC glDepthRangef = 0;
PFNGLCLEARDEPTHFPROC glClearDepthf = 0;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary = 0;
PFNGLPROGRAMBINARYPROC glProgramBinary = 0;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = 0;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = 0;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = 0;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv = 0;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = 0;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = 0;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = 0;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline = 0;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv = 0;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i = 0;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv = 0;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f = 0;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv = 0;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d = 0;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv = 0;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui = 0;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv = 0;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i = 0;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv = 0;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f = 0;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv = 0;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d = 0;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv = 0;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui = 0;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv = 0;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i = 0;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv = 0;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f = 0;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv = 0;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d = 0;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv = 0;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui = 0;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv = 0;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i = 0;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv = 0;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f = 0;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv = 0;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d = 0;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv = 0;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui = 0;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv = 0;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv = 0;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline = 0;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog = 0;
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d = 0;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d = 0;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d = 0;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d = 0;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv = 0;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv = 0;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv = 0;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv = 0;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer = 0;
PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv = 0;
PFNGLVIEWPORTARRAYVPROC glViewportArrayv = 0;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf = 0;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv = 0;
PFNGLSCISSORARRAYVPROC glScissorArrayv = 0;
PFNGLSCISSORINDEXEDPROC glScissorIndexed = 0;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv = 0;
PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv = 0;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed = 0;
PFNGLGETFLOATI_VPROC glGetFloati_v = 0;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v = 0;
#endif

// GL 4.2
#if (GL_VERSION_NUM >= 42)
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance = 0;
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ = 0;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv = 0;
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture = 0;
PFNGLMEMORYBARRIERPROC glMemoryBarrier = 0;
PFNGLTEXSTORAGE1DPROC glTexStorage1D = 0;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = 0;
PFNGLTEXSTORAGE3DPROC glTexStorage3D = 0;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced = 0;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced = 0;
#endif

// GL 4.3
#if (GL_VERSION_NUM >= 43)
PFNGLCLEARBUFFERDATAPROC glClearBufferData = 0;
PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData = 0;
PFNGLDISPATCHCOMPUTEPROC glDispatchCompute = 0;
PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect = 0;
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData = 0;
PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri = 0;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv = 0;
PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v = 0;
PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage = 0;
PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage = 0;
PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData = 0;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData = 0;
PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer = 0;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer = 0;
PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect = 0;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect = 0;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv = 0;
PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex = 0;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName = 0;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv = 0;
PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation = 0;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex = 0;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding = 0;
PFNGLTEXBUFFERRANGEPROC glTexBufferRange = 0;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample = 0;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample = 0;
PFNGLTEXTUREVIEWPROC glTextureView = 0;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer = 0;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat = 0;
PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat = 0;
PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat = 0;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = 0;
PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor = 0;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl = 0;
PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert = 0;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = 0;
PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog = 0;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup = 0;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;
PFNGLOBJECTLABELPROC glObjectLabel = 0;
PFNGLGETOBJECTLABELPROC glGetObjectLabel = 0;
PFNGLOBJECTPTRLABELPROC glObjectPtrLabel = 0;
PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel = 0;
#endif

// GL 4.4
#if (GL_VERSION_NUM >= 44)
PFNGLBUFFERSTORAGEPROC glBufferStorage = 0;
PFNGLCLEARTEXIMAGEPROC glClearTexImage = 0;
PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage = 0;
PFNGLBINDBUFFERSBASEPROC glBindBuffersBase = 0;
PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange = 0;
PFNGLBINDTEXTURESPROC glBindTextures = 0;
PFNGLBINDSAMPLERSPROC glBindSamplers = 0;
PFNGLBINDIMAGETEXTURESPROC glBindImageTextures = 0;
PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers = 0;
#endif

// Extensions

// GL_ARB_timer_query
PFNGLQUERYCOUNTERPROC glQueryCounterARB = nullptr;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64vARB = nullptr;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64vARB = nullptr;

// GL_OES_EGL_image
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = nullptr;

#pragma endregion

bool initOpenGLExtensions(IOpenGLAdapter* _adapter)
{
	CGroupOpenGL& openglSettings = GetSettingsGroup<CGroupOpenGL>();

    bool r = true;

    // GL 1.1
    r &= (glPolygonMode = (PFNGLPOLYGONMODEPROC)_adapter->getProcAddress("glPolygonMode")) != nullptr;
    r &= (glGetTexImage = (PFNGLGETTEXIMAGEPROC)_adapter->getProcAddress("glGetTexImage")) != nullptr;
    r &= (glClearDepth = (PFNGLCLEARDEPTH)_adapter->getProcAddress("glClearDepth")) != nullptr;
    r &= (glDrawBuffer = (PFNGLDRAWBUFFERPROC)_adapter->getProcAddress("glDrawBuffer")) != nullptr;

    // GL 1.2
    r &= (glBlendColor = (PFNGLBLENDCOLORPROC)_adapter->getProcAddress("glBlendColor")) != nullptr;
    r &= (glBlendEquation = (PFNGLBLENDEQUATIONPROC)_adapter->getProcAddress("glBlendEquation")) != nullptr;
    r &= (glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)_adapter->getProcAddress("glDrawRangeElements")) != nullptr;
    r &= (glTexImage3D = (PFNGLTEXIMAGE3DPROC)_adapter->getProcAddress("glTexImage3D")) != nullptr;
    r &= (glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)_adapter->getProcAddress("glTexSubImage3D")) != nullptr;
    r &= (glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)_adapter->getProcAddress("glCopyTexSubImage3D")) != nullptr;

    // GL 1.3
    r &= (glActiveTexture = (PFNGLACTIVETEXTUREPROC)_adapter->getProcAddress("glActiveTexture")) != nullptr;
    r &= (glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)_adapter->getProcAddress("glSampleCoverage")) != nullptr;
    r &= (glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)_adapter->getProcAddress("glCompressedTexImage3D")) != nullptr;
    r &= (glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)_adapter->getProcAddress("glCompressedTexImage2D")) != nullptr;
    r &= (glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)_adapter->getProcAddress("glCompressedTexImage1D")) != nullptr;
    r &= (glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)_adapter->getProcAddress("glCompressedTexSubImage3D")) != nullptr;
    r &= (glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)_adapter->getProcAddress("glCompressedTexSubImage2D")) != nullptr;
    r &= (glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)_adapter->getProcAddress("glCompressedTexSubImage1D")) != nullptr;
    r &= (glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)_adapter->getProcAddress("glGetCompressedTexImage")) != nullptr;

    // GL 1.4
    r &= (glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)_adapter->getProcAddress("glBlendFuncSeparate")) != nullptr;
    r &= (glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)_adapter->getProcAddress("glMultiDrawArrays")) != nullptr;
    r &= (glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)_adapter->getProcAddress("glMultiDrawElements")) != nullptr;
    r &= (glPointParameterf = (PFNGLPOINTPARAMETERFPROC)_adapter->getProcAddress("glPointParameterf")) != nullptr;
    r &= (glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)_adapter->getProcAddress("glPointParameterfv")) != nullptr;
    r &= (glPointParameteri = (PFNGLPOINTPARAMETERIPROC)_adapter->getProcAddress("glPointParameteri")) != nullptr;
    r &= (glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)_adapter->getProcAddress("glPointParameteriv")) != nullptr;

    // GL 1.5
    r &= (glGenQueries = (PFNGLGENQUERIESPROC)_adapter->getProcAddress("glGenQueries")) != nullptr;
    r &= (glDeleteQueries = (PFNGLDELETEQUERIESPROC)_adapter->getProcAddress("glDeleteQueries")) != nullptr;
    r &= (glIsQuery = (PFNGLISQUERYPROC)_adapter->getProcAddress("glIsQuery")) != nullptr;
    r &= (glBeginQuery = (PFNGLBEGINQUERYPROC)_adapter->getProcAddress("glBeginQuery")) != nullptr;
    r &= (glEndQuery = (PFNGLENDQUERYPROC)_adapter->getProcAddress("glEndQuery")) != nullptr;
    r &= (glGetQueryiv = (PFNGLGETQUERYIVPROC)_adapter->getProcAddress("glGetQueryiv")) != nullptr;
    r &= (glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)_adapter->getProcAddress("glGetQueryObjectiv")) != nullptr;
    r &= (glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)_adapter->getProcAddress("glGetQueryObjectuiv")) != nullptr;
    r &= (glBindBuffer = (PFNGLBINDBUFFERPROC)_adapter->getProcAddress("glBindBuffer")) != nullptr;
    r &= (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)_adapter->getProcAddress("glDeleteBuffers")) != nullptr;
    r &= (glGenBuffers = (PFNGLGENBUFFERSPROC)_adapter->getProcAddress("glGenBuffers")) != nullptr;
    r &= (glIsBuffer = (PFNGLISBUFFERPROC)_adapter->getProcAddress("glIsBuffer")) != nullptr;
    r &= (glBufferData = (PFNGLBUFFERDATAPROC)_adapter->getProcAddress("glBufferData")) != nullptr;
    r &= (glBufferSubData = (PFNGLBUFFERSUBDATAPROC)_adapter->getProcAddress("glBufferSubData")) != nullptr;
    r &= (glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)_adapter->getProcAddress("glGetBufferSubData")) != nullptr;
    r &= (glMapBuffer = (PFNGLMAPBUFFERPROC)_adapter->getProcAddress("glMapBuffer")) != nullptr;
    r &= (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)_adapter->getProcAddress("glUnmapBuffer")) != nullptr;
    r &= (glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)_adapter->getProcAddress("glGetBufferParameteriv")) != nullptr;
    r &= (glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)_adapter->getProcAddress("glGetBufferPointerv")) != nullptr;

    // GL 2.0
    r &= (glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)_adapter->getProcAddress("glBlendEquationSeparate")) != nullptr;
    r &= (glDrawBuffers = (PFNGLDRAWBUFFERSPROC)_adapter->getProcAddress("glDrawBuffers")) != nullptr;
    r &= (glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)_adapter->getProcAddress("glStencilOpSeparate")) != nullptr;
    r &= (glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)_adapter->getProcAddress("glStencilFuncSeparate")) != nullptr;
    r &= (glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)_adapter->getProcAddress("glStencilMaskSeparate")) != nullptr;
    r &= (glAttachShader = (PFNGLATTACHSHADERPROC)_adapter->getProcAddress("glAttachShader")) != nullptr;
    r &= (glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)_adapter->getProcAddress("glBindAttribLocation")) != nullptr;
    r &= (glCompileShader = (PFNGLCOMPILESHADERPROC)_adapter->getProcAddress("glCompileShader")) != nullptr;
    r &= (glCreateProgram = (PFNGLCREATEPROGRAMPROC)_adapter->getProcAddress("glCreateProgram")) != nullptr;
    r &= (glCreateShader = (PFNGLCREATESHADERPROC)_adapter->getProcAddress("glCreateShader")) != nullptr;
    r &= (glDeleteProgram = (PFNGLDELETEPROGRAMPROC)_adapter->getProcAddress("glDeleteProgram")) != nullptr;
    r &= (glDeleteShader = (PFNGLDELETESHADERPROC)_adapter->getProcAddress("glDeleteShader")) != nullptr;
    r &= (glDetachShader = (PFNGLDETACHSHADERPROC)_adapter->getProcAddress("glDetachShader")) != nullptr;
    r &= (glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)_adapter->getProcAddress("glDisableVertexAttribArray")) != nullptr;
    r &= (glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)_adapter->getProcAddress("glEnableVertexAttribArray")) != nullptr;
    r &= (glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)_adapter->getProcAddress("glGetActiveAttrib")) != nullptr;
    r &= (glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)_adapter->getProcAddress("glGetActiveUniform")) != nullptr;
    r &= (glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)_adapter->getProcAddress("glGetAttachedShaders")) != nullptr;
    r &= (glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)_adapter->getProcAddress("glGetAttribLocation")) != nullptr;
    r &= (glGetProgramiv = (PFNGLGETPROGRAMIVPROC)_adapter->getProcAddress("glGetProgramiv")) != nullptr;
    r &= (glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)_adapter->getProcAddress("glGetProgramInfoLog")) != nullptr;
    r &= (glGetShaderiv = (PFNGLGETSHADERIVPROC)_adapter->getProcAddress("glGetShaderiv")) != nullptr;
    r &= (glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)_adapter->getProcAddress("glGetShaderInfoLog")) != nullptr;
    r &= (glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)_adapter->getProcAddress("glGetShaderSource")) != nullptr;
    r &= (glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)_adapter->getProcAddress("glGetUniformLocation")) != nullptr;
    r &= (glGetUniformfv = (PFNGLGETUNIFORMFVPROC)_adapter->getProcAddress("glGetUniformfv")) != nullptr;
    r &= (glGetUniformiv = (PFNGLGETUNIFORMIVPROC)_adapter->getProcAddress("glGetUniformiv")) != nullptr;
    r &= (glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)_adapter->getProcAddress("glGetVertexAttribdv")) != nullptr;
    r &= (glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)_adapter->getProcAddress("glGetVertexAttribfv")) != nullptr;
    r &= (glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)_adapter->getProcAddress("glGetVertexAttribiv")) != nullptr;
    r &= (glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)_adapter->getProcAddress("glGetVertexAttribPointerv")) != nullptr;
    r &= (glIsProgram = (PFNGLISPROGRAMPROC)_adapter->getProcAddress("glIsProgram")) != nullptr;
    r &= (glIsShader = (PFNGLISSHADERPROC)_adapter->getProcAddress("glIsShader")) != nullptr;
    r &= (glLinkProgram = (PFNGLLINKPROGRAMPROC)_adapter->getProcAddress("glLinkProgram")) != nullptr;
    r &= (glShaderSource = (PFNGLSHADERSOURCEPROC)_adapter->getProcAddress("glShaderSource")) != nullptr;
    r &= (glUseProgram = (PFNGLUSEPROGRAMPROC)_adapter->getProcAddress("glUseProgram")) != nullptr;
    r &= (glUniform1f = (PFNGLUNIFORM1FPROC)_adapter->getProcAddress("glUniform1f")) != nullptr;
    r &= (glUniform2f = (PFNGLUNIFORM2FPROC)_adapter->getProcAddress("glUniform2f")) != nullptr;
    r &= (glUniform3f = (PFNGLUNIFORM3FPROC)_adapter->getProcAddress("glUniform3f")) != nullptr;
    r &= (glUniform4f = (PFNGLUNIFORM4FPROC)_adapter->getProcAddress("glUniform4f")) != nullptr;
    r &= (glUniform1i = (PFNGLUNIFORM1IPROC)_adapter->getProcAddress("glUniform1i")) != nullptr;
    r &= (glUniform2i = (PFNGLUNIFORM2IPROC)_adapter->getProcAddress("glUniform2i")) != nullptr;
    r &= (glUniform3i = (PFNGLUNIFORM3IPROC)_adapter->getProcAddress("glUniform3i")) != nullptr;
    r &= (glUniform4i = (PFNGLUNIFORM4IPROC)_adapter->getProcAddress("glUniform4i")) != nullptr;
    r &= (glUniform1fv = (PFNGLUNIFORM1FVPROC)_adapter->getProcAddress("glUniform1fv")) != nullptr;
    r &= (glUniform2fv = (PFNGLUNIFORM2FVPROC)_adapter->getProcAddress("glUniform2fv")) != nullptr;
    r &= (glUniform3fv = (PFNGLUNIFORM3FVPROC)_adapter->getProcAddress("glUniform3fv")) != nullptr;
    r &= (glUniform4fv = (PFNGLUNIFORM4FVPROC)_adapter->getProcAddress("glUniform4fv")) != nullptr;
    r &= (glUniform1iv = (PFNGLUNIFORM1IVPROC)_adapter->getProcAddress("glUniform1iv")) != nullptr;
    r &= (glUniform2iv = (PFNGLUNIFORM2IVPROC)_adapter->getProcAddress("glUniform2iv")) != nullptr;
    r &= (glUniform3iv = (PFNGLUNIFORM3IVPROC)_adapter->getProcAddress("glUniform3iv")) != nullptr;
    r &= (glUniform4iv = (PFNGLUNIFORM4IVPROC)_adapter->getProcAddress("glUniform4iv")) != nullptr;
    r &= (glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)_adapter->getProcAddress("glUniformMatrix2fv")) != nullptr;
    r &= (glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)_adapter->getProcAddress("glUniformMatrix3fv")) != nullptr;
    r &= (glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)_adapter->getProcAddress("glUniformMatrix4fv")) != nullptr;
    r &= (glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)_adapter->getProcAddress("glValidateProgram")) != nullptr;
    r &= (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)_adapter->getProcAddress("glVertexAttribPointer")) != nullptr;

    // GL 2.1
    r &= (glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)_adapter->getProcAddress("glUniformMatrix2x3fv")) != nullptr;
    r &= (glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)_adapter->getProcAddress("glUniformMatrix3x2fv")) != nullptr;
    r &= (glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)_adapter->getProcAddress("glUniformMatrix2x4fv")) != nullptr;
    r &= (glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)_adapter->getProcAddress("glUniformMatrix4x2fv")) != nullptr;
    r &= (glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)_adapter->getProcAddress("glUniformMatrix3x4fv")) != nullptr;
    r &= (glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)_adapter->getProcAddress("glUniformMatrix4x3fv")) != nullptr;

    // GL 3.0 - GL 3.3 
    {
        // GL 3.0
        r &= (glGetStringi = (PFNGLGETSTRINGIPROC)_adapter->getProcAddress("glGetStringi")) != nullptr;
        r &= (glColorMaski = (PFNGLCOLORMASKIPROC)_adapter->getProcAddress("glColorMaski")) != nullptr;
        r &= (glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)_adapter->getProcAddress("glGetBooleani_v")) != nullptr;
        r &= (glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)_adapter->getProcAddress("glGetIntegeri_v")) != nullptr;
        r &= (glEnablei = (PFNGLENABLEIPROC)_adapter->getProcAddress("glEnablei")) != nullptr;
        r &= (glDisablei = (PFNGLDISABLEIPROC)_adapter->getProcAddress("glDisablei")) != nullptr;
        r &= (glIsEnabledi = (PFNGLISENABLEDIPROC)_adapter->getProcAddress("glIsEnabledi")) != nullptr;
        r &= (glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glBeginTransformFeedback")) != nullptr;
        r &= (glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glEndTransformFeedback")) != nullptr;
        r &= (glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)_adapter->getProcAddress("glBindBufferRange")) != nullptr;
        r &= (glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)_adapter->getProcAddress("glBindBufferBase")) != nullptr;
        r &= (glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)_adapter->getProcAddress("glTransformFeedbackVaryings")) != nullptr;
        r &= (glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)_adapter->getProcAddress("glGetTransformFeedbackVarying")) != nullptr;
        r &= (glClampColor = (PFNGLCLAMPCOLORPROC)_adapter->getProcAddress("glClampColor")) != nullptr;
        r &= (glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)_adapter->getProcAddress("glBeginConditionalRender")) != nullptr;
        r &= (glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)_adapter->getProcAddress("glEndConditionalRender")) != nullptr;
        r &= (glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)_adapter->getProcAddress("glVertexAttribIPointer")) != nullptr;
        r &= (glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)_adapter->getProcAddress("glGetVertexAttribIiv")) != nullptr;
        r &= (glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)_adapter->getProcAddress("glGetVertexAttribIuiv")) != nullptr;
        r &= (glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)_adapter->getProcAddress("glVertexAttribI1i")) != nullptr;
        r &= (glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)_adapter->getProcAddress("glVertexAttribI2i")) != nullptr;
        r &= (glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)_adapter->getProcAddress("glVertexAttribI3i")) != nullptr;
        r &= (glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)_adapter->getProcAddress("glVertexAttribI4i")) != nullptr;
        r &= (glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)_adapter->getProcAddress("glVertexAttribI1ui")) != nullptr;
        r &= (glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)_adapter->getProcAddress("glVertexAttribI2ui")) != nullptr;
        r &= (glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)_adapter->getProcAddress("glVertexAttribI3ui")) != nullptr;
        r &= (glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)_adapter->getProcAddress("glVertexAttribI4ui")) != nullptr;
        r &= (glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)_adapter->getProcAddress("glVertexAttribI1iv")) != nullptr;
        r &= (glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)_adapter->getProcAddress("glVertexAttribI2iv")) != nullptr;
        r &= (glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)_adapter->getProcAddress("glVertexAttribI3iv")) != nullptr;
        r &= (glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)_adapter->getProcAddress("glVertexAttribI4iv")) != nullptr;
        r &= (glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)_adapter->getProcAddress("glVertexAttribI1uiv")) != nullptr;
        r &= (glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)_adapter->getProcAddress("glVertexAttribI2uiv")) != nullptr;
        r &= (glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)_adapter->getProcAddress("glVertexAttribI3uiv")) != nullptr;
        r &= (glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)_adapter->getProcAddress("glVertexAttribI4uiv")) != nullptr;
        r &= (glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)_adapter->getProcAddress("glVertexAttribI4bv")) != nullptr;
        r &= (glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)_adapter->getProcAddress("glVertexAttribI4sv")) != nullptr;
        r &= (glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)_adapter->getProcAddress("glVertexAttribI4ubv")) != nullptr;
        r &= (glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)_adapter->getProcAddress("glVertexAttribI4usv")) != nullptr;
        r &= (glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)_adapter->getProcAddress("glGetUniformuiv")) != nullptr;
        r &= (glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)_adapter->getProcAddress("glBindFragDataLocation")) != nullptr;
        r &= (glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)_adapter->getProcAddress("glGetFragDataLocation")) != nullptr;
        r &= (glUniform1ui = (PFNGLUNIFORM1UIPROC)_adapter->getProcAddress("glUniform1ui")) != nullptr;
        r &= (glUniform2ui = (PFNGLUNIFORM2UIPROC)_adapter->getProcAddress("glUniform2ui")) != nullptr;
        r &= (glUniform3ui = (PFNGLUNIFORM3UIPROC)_adapter->getProcAddress("glUniform3ui")) != nullptr;
        r &= (glUniform4ui = (PFNGLUNIFORM4UIPROC)_adapter->getProcAddress("glUniform4ui")) != nullptr;
        r &= (glUniform1uiv = (PFNGLUNIFORM1UIVPROC)_adapter->getProcAddress("glUniform1uiv")) != nullptr;
        r &= (glUniform2uiv = (PFNGLUNIFORM2UIVPROC)_adapter->getProcAddress("glUniform2uiv")) != nullptr;
        r &= (glUniform3uiv = (PFNGLUNIFORM3UIVPROC)_adapter->getProcAddress("glUniform3uiv")) != nullptr;
        r &= (glUniform4uiv = (PFNGLUNIFORM4UIVPROC)_adapter->getProcAddress("glUniform4uiv")) != nullptr;
        r &= (glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)_adapter->getProcAddress("glTexParameterIiv")) != nullptr;
        r &= (glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)_adapter->getProcAddress("glTexParameterIuiv")) != nullptr;
        r &= (glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)_adapter->getProcAddress("glGetTexParameterIiv")) != nullptr;
        r &= (glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)_adapter->getProcAddress("glGetTexParameterIuiv")) != nullptr;
        r &= (glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)_adapter->getProcAddress("glClearBufferiv")) != nullptr;
        r &= (glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)_adapter->getProcAddress("glClearBufferuiv")) != nullptr;
        r &= (glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)_adapter->getProcAddress("glClearBufferfv")) != nullptr;
        r &= (glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)_adapter->getProcAddress("glClearBufferfi")) != nullptr;
        r &= (glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)_adapter->getProcAddress("glIsRenderbuffer")) != nullptr;
        r &= (glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)_adapter->getProcAddress("glBindRenderbuffer")) != nullptr;
        r &= (glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)_adapter->getProcAddress("glDeleteRenderbuffers")) != nullptr;
        r &= (glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)_adapter->getProcAddress("glGenRenderbuffers")) != nullptr;
        r &= (glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)_adapter->getProcAddress("glRenderbufferStorage")) != nullptr;
        r &= (glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)_adapter->getProcAddress("glGetRenderbufferParameteriv")) != nullptr;
        r &= (glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)_adapter->getProcAddress("glIsFramebuffer")) != nullptr;
        r &= (glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)_adapter->getProcAddress("glBindFramebuffer")) != nullptr;
        r &= (glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)_adapter->getProcAddress("glDeleteFramebuffers")) != nullptr;
        r &= (glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)_adapter->getProcAddress("glGenFramebuffers")) != nullptr;
        r &= (glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)_adapter->getProcAddress("glCheckFramebufferStatus")) != nullptr;
        r &= (glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)_adapter->getProcAddress("glFramebufferTexture1D")) != nullptr;
        r &= (glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)_adapter->getProcAddress("glFramebufferTexture2D")) != nullptr;
        r &= (glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)_adapter->getProcAddress("glFramebufferTexture3D")) != nullptr;
        r &= (glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)_adapter->getProcAddress("glFramebufferRenderbuffer")) != nullptr;
        r &= (glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)_adapter->getProcAddress("glGetFramebufferAttachmentParameteriv")) != nullptr;
        r &= (glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)_adapter->getProcAddress("glGenerateMipmap")) != nullptr;
        r &= (glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)_adapter->getProcAddress("glBlitFramebuffer")) != nullptr;
        r &= (glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)_adapter->getProcAddress("glRenderbufferStorageMultisample")) != nullptr;
        r &= (glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)_adapter->getProcAddress("glFramebufferTextureLayer")) != nullptr;
        r &= (glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)_adapter->getProcAddress("glMapBufferRange")) != nullptr;
        r &= (glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)_adapter->getProcAddress("glFlushMappedBufferRange")) != nullptr;
        r &= (glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)_adapter->getProcAddress("glBindVertexArray")) != nullptr;
        r &= (glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)_adapter->getProcAddress("glDeleteVertexArrays")) != nullptr;
        r &= (glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)_adapter->getProcAddress("glGenVertexArrays")) != nullptr;
        r &= (glIsVertexArray = (PFNGLISVERTEXARRAYPROC)_adapter->getProcAddress("glIsVertexArray")) != nullptr;

        // GL 3.1
        if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 31)
        {
            r &= (glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)_adapter->getProcAddress("glDrawArraysInstanced")) != nullptr;
            r &= (glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)_adapter->getProcAddress("glDrawElementsInstanced")) != nullptr;
            r &= (glTexBuffer = (PFNGLTEXBUFFERPROC)_adapter->getProcAddress("glTexBuffer")) != nullptr;
            r &= (glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)_adapter->getProcAddress("glPrimitiveRestartIndex")) != nullptr;
            r &= (glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)_adapter->getProcAddress("glCopyBufferSubData")) != nullptr;
            r &= (glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)_adapter->getProcAddress("glGetUniformIndices")) != nullptr;
            r &= (glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)_adapter->getProcAddress("glGetActiveUniformsiv")) != nullptr;
            r &= (glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)_adapter->getProcAddress("glGetActiveUniformName")) != nullptr;
            r &= (glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)_adapter->getProcAddress("glGetUniformBlockIndex")) != nullptr;
            r &= (glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)_adapter->getProcAddress("glGetActiveUniformBlockiv")) != nullptr;
            r &= (glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)_adapter->getProcAddress("glGetActiveUniformBlockName")) != nullptr;
            r &= (glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)_adapter->getProcAddress("glUniformBlockBinding")) != nullptr;
        }

        // GL 3.2
        if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 32)
        {
            r &= (glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)_adapter->getProcAddress("glDrawElementsBaseVertex")) != nullptr;
            r &= (glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)_adapter->getProcAddress("glDrawRangeElementsBaseVertex")) != nullptr;
            r &= (glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)_adapter->getProcAddress("glDrawElementsInstancedBaseVertex")) != nullptr;
            r &= (glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)_adapter->getProcAddress("glMultiDrawElementsBaseVertex")) != nullptr;
            r &= (glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)_adapter->getProcAddress("glProvokingVertex")) != nullptr;
            r &= (glFenceSync = (PFNGLFENCESYNCPROC)_adapter->getProcAddress("glFenceSync")) != nullptr;
            r &= (glIsSync = (PFNGLISSYNCPROC)_adapter->getProcAddress("glIsSync")) != nullptr;
            r &= (glDeleteSync = (PFNGLDELETESYNCPROC)_adapter->getProcAddress("glDeleteSync")) != nullptr;
            r &= (glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)_adapter->getProcAddress("glClientWaitSync")) != nullptr;
            r &= (glWaitSync = (PFNGLWAITSYNCPROC)_adapter->getProcAddress("glWaitSync")) != nullptr;
            r &= (glGetInteger64v = (PFNGLGETINTEGER64VPROC)_adapter->getProcAddress("glGetInteger64v")) != nullptr;
            r &= (glGetSynciv = (PFNGLGETSYNCIVPROC)_adapter->getProcAddress("glGetSynciv")) != nullptr;
            r &= (glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)_adapter->getProcAddress("glGetInteger64i_v")) != nullptr;
            r &= (glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)_adapter->getProcAddress("glGetBufferParameteri64v")) != nullptr;
            r &= (glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)_adapter->getProcAddress("glFramebufferTexture")) != nullptr;
            r &= (glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)_adapter->getProcAddress("glTexImage2DMultisample")) != nullptr;
            r &= (glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)_adapter->getProcAddress("glTexImage3DMultisample")) != nullptr;
            r &= (glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)_adapter->getProcAddress("glGetMultisamplefv")) != nullptr;
            r &= (glSampleMaski = (PFNGLSAMPLEMASKIPROC)_adapter->getProcAddress("glSampleMaski")) != nullptr;
        }

        // GL 3.3
        if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 33)
        {
            r &= (glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)_adapter->getProcAddress("glBindFragDataLocationIndexed")) != nullptr;
            r &= (glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)_adapter->getProcAddress("glGetFragDataIndex")) != nullptr;
            r &= (glGenSamplers = (PFNGLGENSAMPLERSPROC)_adapter->getProcAddress("glGenSamplers")) != nullptr;
            r &= (glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)_adapter->getProcAddress("glDeleteSamplers")) != nullptr;
            r &= (glIsSampler = (PFNGLISSAMPLERPROC)_adapter->getProcAddress("glIsSampler")) != nullptr;
            r &= (glBindSampler = (PFNGLBINDSAMPLERPROC)_adapter->getProcAddress("glBindSampler")) != nullptr;
            r &= (glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)_adapter->getProcAddress("glSamplerParameteri")) != nullptr;
            r &= (glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)_adapter->getProcAddress("glSamplerParameteriv")) != nullptr;
            r &= (glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)_adapter->getProcAddress("glSamplerParameterf")) != nullptr;
            r &= (glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)_adapter->getProcAddress("glSamplerParameterfv")) != nullptr;
            r &= (glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)_adapter->getProcAddress("glSamplerParameterIiv")) != nullptr;
            r &= (glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)_adapter->getProcAddress("glSamplerParameterIuiv")) != nullptr;
            r &= (glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)_adapter->getProcAddress("glGetSamplerParameteriv")) != nullptr;
            r &= (glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)_adapter->getProcAddress("glGetSamplerParameterIiv")) != nullptr;
            r &= (glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)_adapter->getProcAddress("glGetSamplerParameterfv")) != nullptr;
            r &= (glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)_adapter->getProcAddress("glGetSamplerParameterIuiv")) != nullptr;
            r &= (glQueryCounter = (PFNGLQUERYCOUNTERPROC)_adapter->getProcAddress("glQueryCounter")) != nullptr;
            r &= (glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)_adapter->getProcAddress("glGetQueryObjecti64v")) != nullptr;
            r &= (glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)_adapter->getProcAddress("glGetQueryObjectui64v")) != nullptr;
            r &= (glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)_adapter->getProcAddress("glVertexAttribDivisor")) != nullptr;
            r &= (glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)_adapter->getProcAddress("glVertexAttribP1ui")) != nullptr;
            r &= (glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)_adapter->getProcAddress("glVertexAttribP1uiv")) != nullptr;
            r &= (glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)_adapter->getProcAddress("glVertexAttribP2ui")) != nullptr;
            r &= (glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)_adapter->getProcAddress("glVertexAttribP2uiv")) != nullptr;
            r &= (glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)_adapter->getProcAddress("glVertexAttribP3ui")) != nullptr;
            r &= (glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)_adapter->getProcAddress("glVertexAttribP3uiv")) != nullptr;
            r &= (glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)_adapter->getProcAddress("glVertexAttribP4ui")) != nullptr;
            r &= (glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)_adapter->getProcAddress("glVertexAttribP4uiv")) != nullptr;
        }
    }

    // GL 4.0 - GL 4.4
#if (GL_VERSION_NUM >= 40)
    if (openglSettings.majorVersion >= 4)
    {
        // GL 4.0
        r &= (glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)_adapter->getProcAddress("glMinSampleShading")) != nullptr;
        r &= (glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)_adapter->getProcAddress("glBlendEquationi")) != nullptr;
        r &= (glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)_adapter->getProcAddress("glBlendEquationSeparatei")) != nullptr;
        r &= (glBlendFunci = (PFNGLBLENDFUNCIPROC)_adapter->getProcAddress("glBlendFunci")) != nullptr;
        r &= (glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)_adapter->getProcAddress("glBlendFuncSeparatei")) != nullptr;
        r &= (glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)_adapter->getProcAddress("glDrawArraysIndirect")) != nullptr;
        r &= (glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)_adapter->getProcAddress("glDrawElementsIndirect")) != nullptr;
        r &= (glUniform1d = (PFNGLUNIFORM1DPROC)_adapter->getProcAddress("glUniform1d")) != nullptr;
        r &= (glUniform2d = (PFNGLUNIFORM2DPROC)_adapter->getProcAddress("glUniform2d")) != nullptr;
        r &= (glUniform3d = (PFNGLUNIFORM3DPROC)_adapter->getProcAddress("glUniform3d")) != nullptr;
        r &= (glUniform4d = (PFNGLUNIFORM4DPROC)_adapter->getProcAddress("glUniform4d")) != nullptr;
        r &= (glUniform1dv = (PFNGLUNIFORM1DVPROC)_adapter->getProcAddress("glUniform1dv")) != nullptr;
        r &= (glUniform2dv = (PFNGLUNIFORM2DVPROC)_adapter->getProcAddress("glUniform2dv")) != nullptr;
        r &= (glUniform3dv = (PFNGLUNIFORM3DVPROC)_adapter->getProcAddress("glUniform3dv")) != nullptr;
        r &= (glUniform4dv = (PFNGLUNIFORM4DVPROC)_adapter->getProcAddress("glUniform4dv")) != nullptr;
        r &= (glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)_adapter->getProcAddress("glUniformMatrix2dv")) != nullptr;
        r &= (glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)_adapter->getProcAddress("glUniformMatrix3dv")) != nullptr;
        r &= (glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)_adapter->getProcAddress("glUniformMatrix4dv")) != nullptr;
        r &= (glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)_adapter->getProcAddress("glUniformMatrix2x3dv")) != nullptr;
        r &= (glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)_adapter->getProcAddress("glUniformMatrix2x4dv")) != nullptr;
        r &= (glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)_adapter->getProcAddress("glUniformMatrix3x2dv")) != nullptr;
        r &= (glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)_adapter->getProcAddress("glUniformMatrix3x4dv")) != nullptr;
        r &= (glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)_adapter->getProcAddress("glUniformMatrix4x2dv")) != nullptr;
        r &= (glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)_adapter->getProcAddress("glUniformMatrix4x3dv")) != nullptr;
        r &= (glGetUniformdv = (PFNGLGETUNIFORMDVPROC)_adapter->getProcAddress("glGetUniformdv")) != nullptr;
        r &= (glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)_adapter->getProcAddress("glGetSubroutineUniformLocation")) != nullptr;
        r &= (glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)_adapter->getProcAddress("glGetSubroutineIndex")) != nullptr;
        r &= (glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)_adapter->getProcAddress("glGetActiveSubroutineUniformiv")) != nullptr;
        r &= (glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)_adapter->getProcAddress("glGetActiveSubroutineUniformName")) != nullptr;
        r &= (glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)_adapter->getProcAddress("glGetActiveSubroutineName")) != nullptr;
        r &= (glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)_adapter->getProcAddress("glUniformSubroutinesuiv")) != nullptr;
        r &= (glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)_adapter->getProcAddress("glGetUniformSubroutineuiv")) != nullptr;
        r &= (glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)_adapter->getProcAddress("glGetProgramStageiv")) != nullptr;
        r &= (glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)_adapter->getProcAddress("glPatchParameteri")) != nullptr;
        r &= (glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)_adapter->getProcAddress("glPatchParameterfv")) != nullptr;
        r &= (glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glBindTransformFeedback")) != nullptr;
        r &= (glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)_adapter->getProcAddress("glDeleteTransformFeedbacks")) != nullptr;
        r &= (glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)_adapter->getProcAddress("glGenTransformFeedbacks")) != nullptr;
        r &= (glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glIsTransformFeedback")) != nullptr;
        r &= (glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glPauseTransformFeedback")) != nullptr;
        r &= (glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glResumeTransformFeedback")) != nullptr;
        r &= (glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)_adapter->getProcAddress("glDrawTransformFeedback")) != nullptr;
        r &= (glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)_adapter->getProcAddress("glDrawTransformFeedbackStream")) != nullptr;
        r &= (glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)_adapter->getProcAddress("glBeginQueryIndexed")) != nullptr;
        r &= (glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)_adapter->getProcAddress("glEndQueryIndexed")) != nullptr;
        r &= (glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)_adapter->getProcAddress("glGetQueryIndexediv")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 41)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 41)
    {
        // GL 4.1
        r &= (glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)_adapter->getProcAddress("glReleaseShaderCompiler")) != nullptr;
        r &= (glShaderBinary = (PFNGLSHADERBINARYPROC)_adapter->getProcAddress("glShaderBinary")) != nullptr;
        r &= (glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)_adapter->getProcAddress("glGetShaderPrecisionFormat")) != nullptr;
        r &= (glDepthRangef = (PFNGLDEPTHRANGEFPROC)_adapter->getProcAddress("glDepthRangef")) != nullptr;
        r &= (glClearDepthf = (PFNGLCLEARDEPTHFPROC)_adapter->getProcAddress("glClearDepthf")) != nullptr;
        r &= (glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)_adapter->getProcAddress("glGetProgramBinary")) != nullptr;
        r &= (glProgramBinary = (PFNGLPROGRAMBINARYPROC)_adapter->getProcAddress("glProgramBinary")) != nullptr;
        r &= (glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)_adapter->getProcAddress("glProgramParameteri")) != nullptr;
        r &= (glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)_adapter->getProcAddress("glUseProgramStages")) != nullptr;
        r &= (glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)_adapter->getProcAddress("glActiveShaderProgram")) != nullptr;
        r &= (glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)_adapter->getProcAddress("glCreateShaderProgramv")) != nullptr;
        r &= (glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)_adapter->getProcAddress("glBindProgramPipeline")) != nullptr;
        r &= (glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)_adapter->getProcAddress("glDeleteProgramPipelines")) != nullptr;
        r &= (glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)_adapter->getProcAddress("glGenProgramPipelines")) != nullptr;
        r &= (glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)_adapter->getProcAddress("glIsProgramPipeline")) != nullptr;
        r &= (glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)_adapter->getProcAddress("glGetProgramPipelineiv")) != nullptr;
        r &= (glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)_adapter->getProcAddress("glProgramUniform1i")) != nullptr;
        r &= (glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)_adapter->getProcAddress("glProgramUniform1iv")) != nullptr;
        r &= (glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)_adapter->getProcAddress("glProgramUniform1f")) != nullptr;
        r &= (glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)_adapter->getProcAddress("glProgramUniform1fv")) != nullptr;
        r &= (glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)_adapter->getProcAddress("glProgramUniform1d")) != nullptr;
        r &= (glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)_adapter->getProcAddress("glProgramUniform1dv")) != nullptr;
        r &= (glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)_adapter->getProcAddress("glProgramUniform1ui")) != nullptr;
        r &= (glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)_adapter->getProcAddress("glProgramUniform1uiv")) != nullptr;
        r &= (glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)_adapter->getProcAddress("glProgramUniform2i")) != nullptr;
        r &= (glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)_adapter->getProcAddress("glProgramUniform2iv")) != nullptr;
        r &= (glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)_adapter->getProcAddress("glProgramUniform2f")) != nullptr;
        r &= (glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)_adapter->getProcAddress("glProgramUniform2fv")) != nullptr;
        r &= (glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)_adapter->getProcAddress("glProgramUniform2d")) != nullptr;
        r &= (glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)_adapter->getProcAddress("glProgramUniform2dv")) != nullptr;
        r &= (glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)_adapter->getProcAddress("glProgramUniform2ui")) != nullptr;
        r &= (glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)_adapter->getProcAddress("glProgramUniform2uiv")) != nullptr;
        r &= (glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)_adapter->getProcAddress("glProgramUniform3i")) != nullptr;
        r &= (glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)_adapter->getProcAddress("glProgramUniform3iv")) != nullptr;
        r &= (glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)_adapter->getProcAddress("glProgramUniform3f")) != nullptr;
        r &= (glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)_adapter->getProcAddress("glProgramUniform3fv")) != nullptr;
        r &= (glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)_adapter->getProcAddress("glProgramUniform3d")) != nullptr;
        r &= (glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)_adapter->getProcAddress("glProgramUniform3dv")) != nullptr;
        r &= (glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)_adapter->getProcAddress("glProgramUniform3ui")) != nullptr;
        r &= (glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)_adapter->getProcAddress("glProgramUniform3uiv")) != nullptr;
        r &= (glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)_adapter->getProcAddress("glProgramUniform4i")) != nullptr;
        r &= (glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)_adapter->getProcAddress("glProgramUniform4iv")) != nullptr;
        r &= (glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)_adapter->getProcAddress("glProgramUniform4f")) != nullptr;
        r &= (glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)_adapter->getProcAddress("glProgramUniform4fv")) != nullptr;
        r &= (glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)_adapter->getProcAddress("glProgramUniform4d")) != nullptr;
        r &= (glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)_adapter->getProcAddress("glProgramUniform4dv")) != nullptr;
        r &= (glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)_adapter->getProcAddress("glProgramUniform4ui")) != nullptr;
        r &= (glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)_adapter->getProcAddress("glProgramUniform4uiv")) != nullptr;
        r &= (glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)_adapter->getProcAddress("glProgramUniformMatrix2fv")) != nullptr;
        r &= (glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)_adapter->getProcAddress("glProgramUniformMatrix3fv")) != nullptr;
        r &= (glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)_adapter->getProcAddress("glProgramUniformMatrix4fv")) != nullptr;
        r &= (glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)_adapter->getProcAddress("glProgramUniformMatrix2dv")) != nullptr;
        r &= (glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)_adapter->getProcAddress("glProgramUniformMatrix3dv")) != nullptr;
        r &= (glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)_adapter->getProcAddress("glProgramUniformMatrix4dv")) != nullptr;
        r &= (glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)_adapter->getProcAddress("glProgramUniformMatrix2x3fv")) != nullptr;
        r &= (glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)_adapter->getProcAddress("glProgramUniformMatrix3x2fv")) != nullptr;
        r &= (glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)_adapter->getProcAddress("glProgramUniformMatrix2x4fv")) != nullptr;
        r &= (glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)_adapter->getProcAddress("glProgramUniformMatrix4x2fv")) != nullptr;
        r &= (glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)_adapter->getProcAddress("glProgramUniformMatrix3x4fv")) != nullptr;
        r &= (glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)_adapter->getProcAddress("glProgramUniformMatrix4x3fv")) != nullptr;
        r &= (glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)_adapter->getProcAddress("glProgramUniformMatrix2x3dv")) != nullptr;
        r &= (glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)_adapter->getProcAddress("glProgramUniformMatrix3x2dv")) != nullptr;
        r &= (glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)_adapter->getProcAddress("glProgramUniformMatrix2x4dv")) != nullptr;
        r &= (glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)_adapter->getProcAddress("glProgramUniformMatrix4x2dv")) != nullptr;
        r &= (glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)_adapter->getProcAddress("glProgramUniformMatrix3x4dv")) != nullptr;
        r &= (glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)_adapter->getProcAddress("glProgramUniformMatrix4x3dv")) != nullptr;
        r &= (glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)_adapter->getProcAddress("glValidateProgramPipeline")) != nullptr;
        r &= (glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)_adapter->getProcAddress("glGetProgramPipelineInfoLog")) != nullptr;
        r &= (glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)_adapter->getProcAddress("glVertexAttribL1d")) != nullptr;
        r &= (glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)_adapter->getProcAddress("glVertexAttribL2d")) != nullptr;
        r &= (glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)_adapter->getProcAddress("glVertexAttribL3d")) != nullptr;
        r &= (glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)_adapter->getProcAddress("glVertexAttribL4d")) != nullptr;
        r &= (glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)_adapter->getProcAddress("glVertexAttribL1dv")) != nullptr;
        r &= (glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)_adapter->getProcAddress("glVertexAttribL2dv")) != nullptr;
        r &= (glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)_adapter->getProcAddress("glVertexAttribL3dv")) != nullptr;
        r &= (glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)_adapter->getProcAddress("glVertexAttribL4dv")) != nullptr;
        r &= (glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)_adapter->getProcAddress("glVertexAttribLPointer")) != nullptr;
        r &= (glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)_adapter->getProcAddress("glGetVertexAttribLdv")) != nullptr;
        r &= (glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)_adapter->getProcAddress("glViewportArrayv")) != nullptr;
        r &= (glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)_adapter->getProcAddress("glViewportIndexedf")) != nullptr;
        r &= (glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)_adapter->getProcAddress("glViewportIndexedfv")) != nullptr;
        r &= (glScissorArrayv = (PFNGLSCISSORARRAYVPROC)_adapter->getProcAddress("glScissorArrayv")) != nullptr;
        r &= (glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)_adapter->getProcAddress("glScissorIndexed")) != nullptr;
        r &= (glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)_adapter->getProcAddress("glScissorIndexedv")) != nullptr;
        r &= (glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)_adapter->getProcAddress("glDepthRangeArrayv")) != nullptr;
        r &= (glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)_adapter->getProcAddress("glDepthRangeIndexed")) != nullptr;
        r &= (glGetFloati_v = (PFNGLGETFLOATI_VPROC)_adapter->getProcAddress("glGetFloati_v")) != nullptr;
        r &= (glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)_adapter->getProcAddress("glGetDoublei_v")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 42)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 42)
    {
        // GL 4.2
        r &= (glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)_adapter->getProcAddress("glDrawArraysInstancedBaseInstance")) != nullptr;
        r &= (glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)_adapter->getProcAddress("glDrawElementsInstancedBaseInstance")) != nullptr;
        r &= (glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)_adapter->getProcAddress("glDrawElementsInstancedBaseVertexBaseInstance")) != nullptr;
        r &= (glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)_adapter->getProcAddress("glGetInternalformativ")) != nullptr;
        r &= (glGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)_adapter->getProcAddress("glGetActiveAtomicCounterBufferiv")) != nullptr;
        r &= (glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)_adapter->getProcAddress("glBindImageTexture")) != nullptr;
        r &= (glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)_adapter->getProcAddress("glMemoryBarrier")) != nullptr;
        r &= (glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)_adapter->getProcAddress("glTexStorage1D")) != nullptr;
        r &= (glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)_adapter->getProcAddress("glTexStorage2D")) != nullptr;
        r &= (glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)_adapter->getProcAddress("glTexStorage3D")) != nullptr;
        r &= (glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)_adapter->getProcAddress("glDrawTransformFeedbackInstanced")) != nullptr;
        r &= (glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)_adapter->getProcAddress("glDrawTransformFeedbackStreamInstanced")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 43)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 43)
    {
        // GL 4.3
        r &= (glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)_adapter->getProcAddress("glClearBufferData")) != nullptr;
        r &= (glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)_adapter->getProcAddress("glClearBufferSubData")) != nullptr;
        r &= (glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)_adapter->getProcAddress("glDispatchCompute")) != nullptr;
        r &= (glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)_adapter->getProcAddress("glDispatchComputeIndirect")) != nullptr;
        r &= (glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)_adapter->getProcAddress("glCopyImageSubData")) != nullptr;
        r &= (glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)_adapter->getProcAddress("glFramebufferParameteri")) != nullptr;
        r &= (glGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)_adapter->getProcAddress("glGetFramebufferParameteriv")) != nullptr;
        r &= (glGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC)_adapter->getProcAddress("glGetInternalformati64v")) != nullptr;
        r &= (glInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC)_adapter->getProcAddress("glInvalidateTexSubImage")) != nullptr;
        r &= (glInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC)_adapter->getProcAddress("glInvalidateTexImage")) != nullptr;
        r &= (glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)_adapter->getProcAddress("glInvalidateBufferSubData")) != nullptr;
        r &= (glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)_adapter->getProcAddress("glInvalidateBufferData")) != nullptr;
        r &= (glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)_adapter->getProcAddress("glInvalidateFramebuffer")) != nullptr;
        r &= (glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)_adapter->getProcAddress("glInvalidateSubFramebuffer")) != nullptr;
        r &= (glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)_adapter->getProcAddress("glMultiDrawArraysIndirect")) != nullptr;
        r &= (glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)_adapter->getProcAddress("glMultiDrawElementsIndirect")) != nullptr;
        r &= (glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)_adapter->getProcAddress("glGetProgramInterfaceiv")) != nullptr;
        r &= (glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)_adapter->getProcAddress("glGetProgramResourceIndex")) != nullptr;
        r &= (glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)_adapter->getProcAddress("glGetProgramResourceName")) != nullptr;
        r &= (glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)_adapter->getProcAddress("glGetProgramResourceiv")) != nullptr;
        r &= (glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)_adapter->getProcAddress("glGetProgramResourceLocation")) != nullptr;
        r &= (glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)_adapter->getProcAddress("glGetProgramResourceLocationIndex")) != nullptr;
        r &= (glShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)_adapter->getProcAddress("glShaderStorageBlockBinding")) != nullptr;
        r &= (glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC)_adapter->getProcAddress("glTexBufferRange")) != nullptr;
        r &= (glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)_adapter->getProcAddress("glTexStorage2DMultisample")) != nullptr;
        r &= (glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)_adapter->getProcAddress("glTexStorage3DMultisample")) != nullptr;
        r &= (glTextureView = (PFNGLTEXTUREVIEWPROC)_adapter->getProcAddress("glTextureView")) != nullptr;
        r &= (glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)_adapter->getProcAddress("glBindVertexBuffer")) != nullptr;
        r &= (glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)_adapter->getProcAddress("glVertexAttribFormat")) != nullptr;
        r &= (glVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)_adapter->getProcAddress("glVertexAttribIFormat")) != nullptr;
        r &= (glVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC)_adapter->getProcAddress("glVertexAttribLFormat")) != nullptr;
        r &= (glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)_adapter->getProcAddress("glVertexAttribBinding")) != nullptr;
        r &= (glVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC)_adapter->getProcAddress("glVertexBindingDivisor")) != nullptr;
        r &= (glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)_adapter->getProcAddress("glDebugMessageControl")) != nullptr;
        r &= (glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)_adapter->getProcAddress("glDebugMessageInsert")) != nullptr;
        r &= (glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)_adapter->getProcAddress("glDebugMessageCallback")) != nullptr;
        r &= (glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)_adapter->getProcAddress("glGetDebugMessageLog")) != nullptr;
        r &= (glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)_adapter->getProcAddress("glPushDebugGroup")) != nullptr;
        r &= (glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)_adapter->getProcAddress("glPopDebugGroup")) != nullptr;
        r &= (glObjectLabel = (PFNGLOBJECTLABELPROC)_adapter->getProcAddress("glObjectLabel")) != nullptr;
        r &= (glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)_adapter->getProcAddress("glGetObjectLabel")) != nullptr;
        r &= (glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)_adapter->getProcAddress("glObjectPtrLabel")) != nullptr;
        r &= (glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)_adapter->getProcAddress("glGetObjectPtrLabel")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 44)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 44)
    {
        // GL 4.4
        r &= (glBufferStorage = (PFNGLBUFFERSTORAGEPROC)_adapter->getProcAddress("glBufferStorage")) != nullptr;
        r &= (glClearTexImage = (PFNGLCLEARTEXIMAGEPROC)_adapter->getProcAddress("glClearTexImage")) != nullptr;
        r &= (glClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC)_adapter->getProcAddress("glClearTexSubImage")) != nullptr;
        r &= (glBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)_adapter->getProcAddress("glBindBuffersBase")) != nullptr;
        r &= (glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)_adapter->getProcAddress("glBindBuffersRange")) != nullptr;
        r &= (glBindTextures = (PFNGLBINDTEXTURESPROC)_adapter->getProcAddress("glBindTextures")) != nullptr;
        r &= (glBindSamplers = (PFNGLBINDSAMPLERSPROC)_adapter->getProcAddress("glBindSamplers")) != nullptr;
        r &= (glBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC)_adapter->getProcAddress("glBindImageTextures")) != nullptr;
        r &= (glBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC)_adapter->getProcAddress("glBindVertexBuffers")) != nullptr;
    }
#endif

    // OES image
    openglSettings.OES_EGL_image = _adapter->isExtensionSupported("GL_OES_EGL_image");
    if (openglSettings.OES_EGL_image)
    {
        r &= (glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)_adapter->getProcAddress("glEGLImageTargetTexture2DOES")) != nullptr;
    }

    // Default extensions, suitable for any OpenGL version
    openglSettings.EXT_texture_filter_anisotropic = _adapter->isExtensionSupported("GL_EXT_texture_filter_anisotropic");
    openglSettings.EXT_texture_compression_s3tc = _adapter->isExtensionSupported("GL_EXT_texture_compression_s3tc") || _adapter->isExtensionSupported("GL_S3_s3tc");
    openglSettings.EXT_texture_sRGB = _adapter->isExtensionSupported("GL_EXT_texture_sRGB");

    return r;
}

