/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2007-2008 Kristian Duske
Copyright (C) 2010-2014 QuakeSpasm developers

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// r_world.c: world model rendering

#include "quakedef.h"

extern cvar_t gl_fullbrights, r_drawflat, gl_overbright, r_oldwater, r_oldskyleaf, r_showtris; //johnfitz

extern glpoly_t	*lightmap_polys[MAX_LIGHTMAPS];

byte *SV_FatPVS (vec3_t org, qmodel_t *worldmodel);

int vis_changed; //if true, force pvs to be refreshed

//==============================================================================
//
// SETUP CHAINS
//
//==============================================================================

/*
================
R_ClearTextureChains -- ericw 

clears texture chains for all textures used by the given model, and also
clears the lightmap chains
================
*/
void R_ClearTextureChains (qmodel_t *mod, texchain_t chain)
{
	int i;

	// set all chains to null
	for (i=0 ; i<mod->numtextures ; i++)
		if (mod->textures[i])
			mod->textures[i]->texturechains[chain] = NULL;
			
	// clear lightmap chains
	memset (lightmap_polys, 0, sizeof(lightmap_polys));
}

/*
================
R_ChainSurface -- ericw -- adds the given surface to its texture chain
================
*/
void R_ChainSurface (msurface_t *surf, texchain_t chain)
{
	surf->texturechain = surf->texinfo->texture->texturechains[chain];
	surf->texinfo->texture->texturechains[chain] = surf;
}

/*
===============
R_MarkSurfaces -- johnfitz -- mark surfaces based on PVS and rebuild texture chains
===============
*/
void R_MarkSurfaces (void)
{
	byte		*vis;
	mleaf_t		*leaf;
	mnode_t		*node;
	msurface_t	*surf, **mark;
	int			i, j;
	qboolean	nearwaterportal;

	// clear lightmap chains
	memset (lightmap_polys, 0, sizeof(lightmap_polys));

	// check this leaf for water portals
	// TODO: loop through all water surfs and use distance to leaf cullbox
	nearwaterportal = false;
	for (i=0, mark = r_viewleaf->firstmarksurface; i < r_viewleaf->nummarksurfaces; i++, mark++)
		if ((*mark)->flags & SURF_DRAWTURB)
			nearwaterportal = true;

	// choose vis data
	if (r_novis.value || r_viewleaf->contents == CONTENTS_SOLID || r_viewleaf->contents == CONTENTS_SKY)
		vis = Mod_NoVisPVS (cl.worldmodel);
	else if (nearwaterportal)
		vis = SV_FatPVS (r_origin, cl.worldmodel);
	else
		vis = Mod_LeafPVS (r_viewleaf, cl.worldmodel);

	// if surface chains don't need regenerating, just add static entities and return
	if (r_oldviewleaf == r_viewleaf && !vis_changed && !nearwaterportal)
	{
		leaf = &cl.worldmodel->leafs[1];
		for (i=0 ; i<cl.worldmodel->numleafs ; i++, leaf++)
			if (vis[i>>3] & (1<<(i&7)))
				if (leaf->efrags)
					R_StoreEfrags (&leaf->efrags);
		return;
	}

	vis_changed = false;
	r_visframecount++;
	r_oldviewleaf = r_viewleaf;

	// iterate through leaves, marking surfaces
	leaf = &cl.worldmodel->leafs[1];
	for (i=0 ; i<cl.worldmodel->numleafs ; i++, leaf++)
	{
		if (vis[i>>3] & (1<<(i&7)))
		{
			if (r_oldskyleaf.value || leaf->contents != CONTENTS_SKY)
				for (j=0, mark = leaf->firstmarksurface; j<leaf->nummarksurfaces; j++, mark++)
					(*mark)->visframe = r_visframecount;

			// add static models
			if (leaf->efrags)
				R_StoreEfrags (&leaf->efrags);
		}
	}

	// set all chains to null
	for (i=0 ; i<cl.worldmodel->numtextures ; i++)
		if (cl.worldmodel->textures[i])
			cl.worldmodel->textures[i]->texturechains[chain_world] = NULL;

	// rebuild chains

#if 1
	//iterate through surfaces one node at a time to rebuild chains
	//need to do it this way if we want to work with tyrann's skip removal tool
	//becuase his tool doesn't actually remove the surfaces from the bsp surfaces lump
	//nor does it remove references to them in each leaf's marksurfaces list
	for (i=0, node = cl.worldmodel->nodes ; i<cl.worldmodel->numnodes ; i++, node++)
		for (j=0, surf=&cl.worldmodel->surfaces[node->firstsurface] ; j<node->numsurfaces ; j++, surf++)
			if (surf->visframe == r_visframecount)
			{
				R_ChainSurface(surf, chain_world);
			}
#else
	//the old way
	surf = &cl.worldmodel->surfaces[cl.worldmodel->firstmodelsurface];
	for (i=0 ; i<cl.worldmodel->nummodelsurfaces ; i++, surf++)
	{
		if (surf->visframe == r_visframecount)
		{
			R_ChainSurface(surf, chain_world);
		}
	}
#endif
}

/*
================
R_BackFaceCull -- johnfitz -- returns true if the surface is facing away from vieworg
================
*/
qboolean R_BackFaceCull (msurface_t *surf)
{
	double dot;

	switch (surf->plane->type)
	{
	case PLANE_X:
		dot = r_refdef.vieworg[0] - surf->plane->dist;
		break;
	case PLANE_Y:
		dot = r_refdef.vieworg[1] - surf->plane->dist;
		break;
	case PLANE_Z:
		dot = r_refdef.vieworg[2] - surf->plane->dist;
		break;
	default:
		dot = DotProduct (r_refdef.vieworg, surf->plane->normal) - surf->plane->dist;
		break;
	}

	if ((dot < 0) ^ !!(surf->flags & SURF_PLANEBACK))
		return true;

	return false;
}

/*
================
R_CullSurfaces -- johnfitz
================
*/
void R_CullSurfaces (void)
{
	msurface_t *s;
	int i;
	texture_t *t;

	if (!r_drawworld_cheatsafe)
		return;

// ericw -- instead of testing (s->visframe == r_visframecount) on all world
// surfaces, use the chained surfaces, which is exactly the same set of sufaces
	for (i=0 ; i<cl.worldmodel->numtextures ; i++)
	{
		t = cl.worldmodel->textures[i];

		if (!t || !t->texturechains[chain_world])
			continue;

		for (s = t->texturechains[chain_world]; s; s = s->texturechain)
		{
			if (R_CullBox(s->mins, s->maxs) || R_BackFaceCull (s))
				s->culled = true;
			else
			{
				s->culled = false;
				rs_brushpolys++; //count wpolys here
				if (s->texinfo->texture->warpimage)
					s->texinfo->texture->update_warp = true;
			}
		}
	}
}

/*
================
R_BuildLightmapChains -- johnfitz -- used for r_lightmap 1

ericw -- now always used at the start of R_DrawTextureChains for the 
mh dynamic lighting speedup
================
*/
void R_BuildLightmapChains (qmodel_t *model, texchain_t chain)
{
	texture_t *t;
	msurface_t *s;
	int i;

	// clear lightmap chains (already done in r_marksurfaces, but clearing them here to be safe becuase of r_stereo)
	memset (lightmap_polys, 0, sizeof(lightmap_polys));

	// now rebuild them
	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain])
			continue;

		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
				R_RenderDynamicLightmaps (s);
	}
}

//==============================================================================
//
// DRAW CHAINS
//
//==============================================================================

/*
=============
R_BeginTransparentDrawing -- ericw
=============
*/
static void R_BeginTransparentDrawing (float entalpha)
{
	if (entalpha < 1.0f)
	{
		glDepthMask (GL_FALSE);
		glEnable (GL_BLEND);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor4f (1,1,1,entalpha);
	}
}

/*
=============
R_EndTransparentDrawing -- ericw
=============
*/
static void R_EndTransparentDrawing (float entalpha)
{
	if (entalpha < 1.0f)
	{
		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glColor3f (1, 1, 1);
	}
}

/*
================
R_DrawTextureChains_ShowTris -- johnfitz
================
*/
void R_DrawTextureChains_ShowTris (qmodel_t *model, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	glpoly_t	*p;

	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];
		if (!t)
			continue;

		if (r_oldwater.value && t->texturechains[chain] && (t->texturechains[chain]->flags & SURF_DRAWTURB))
		{
			for (s = t->texturechains[chain]; s; s = s->texturechain)
				if (!s->culled)
					for (p = s->polys->next; p; p = p->next)
					{
						DrawGLTriangleFan (p);
					}
		}
		else
		{
			for (s = t->texturechains[chain]; s; s = s->texturechain)
				if (!s->culled)
				{
					DrawGLTriangleFan (s->polys);
				}
		}
	}
}

/*
================
R_DrawTextureChains_Drawflat -- johnfitz
================
*/
void R_DrawTextureChains_Drawflat (qmodel_t *model, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	glpoly_t	*p;

	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];
		if (!t)
			continue;

		if (r_oldwater.value && t->texturechains[chain] && (t->texturechains[chain]->flags & SURF_DRAWTURB))
		{
			for (s = t->texturechains[chain]; s; s = s->texturechain)
				if (!s->culled)
					for (p = s->polys->next; p; p = p->next)
					{
						srand((unsigned int) (uintptr_t) p);
						glColor3f (rand()%256/255.0, rand()%256/255.0, rand()%256/255.0);
						DrawGLPoly (p);
						rs_brushpasses++;
					}
		}
		else
		{
			for (s = t->texturechains[chain]; s; s = s->texturechain)
				if (!s->culled)
				{
					srand((unsigned int) (uintptr_t) s->polys);
					glColor3f (rand()%256/255.0, rand()%256/255.0, rand()%256/255.0);
					DrawGLPoly (s->polys);
					rs_brushpasses++;
				}
		}
	}
	glColor3f (1,1,1);
	srand ((int) (cl.time * 1000));
}

/*
================
R_DrawTextureChains_Glow -- johnfitz
================
*/
void R_DrawTextureChains_Glow (qmodel_t *model, entity_t *ent, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	gltexture_t	*glt;
	qboolean	bound;

	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain] || !(glt = R_TextureAnimation(t, ent != NULL ? ent->frame : 0)->fullbright))
			continue;

		bound = false;

		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
			{
				if (!bound) //only bind once we are sure we need this texture
				{
					GL_Bind (glt);
					bound = true;
				}
				DrawGLPoly (s->polys);
				rs_brushpasses++;
			}
	}
}

//==============================================================================
//
// VBO SUPPORT
//
//==============================================================================

static unsigned int R_NumTriangleIndicesForSurf (msurface_t *s)
{
	return 3 * (s->numedges - 2);
}

/*
================
R_TriangleIndicesForSurf

Writes out the triangle indices needed to draw s as a triangle list.
The number of indices it will write is given by R_NumTriangleIndicesForSurf.
================
*/
static void R_TriangleIndicesForSurf (msurface_t *s, unsigned int *dest)
{
	int i;
	for (i=2; i<s->numedges; i++)
	{
		*dest++ = s->vbo_firstvert;
		*dest++ = s->vbo_firstvert + i - 1;
		*dest++ = s->vbo_firstvert + i;
	}
}

#define MAX_BATCH_SIZE 4096

static unsigned int vbo_indices[MAX_BATCH_SIZE];
static unsigned int num_vbo_indices;

/*
================
R_ClearBatch
================
*/
static void R_ClearBatch ()
{
	num_vbo_indices = 0;
}

/*
================
R_FlushBatch

Draw the current batch if non-empty and clears it, ready for more R_BatchSurface calls.
================
*/
static void R_FlushBatch ()
{
	if (num_vbo_indices > 0)
	{
		glDrawElements (GL_TRIANGLES, num_vbo_indices, GL_UNSIGNED_INT, vbo_indices);
		num_vbo_indices = 0;
	}
}

/*
================
R_BatchSurface

Add the surface to the current batch, or just draw it immediately if we're not
using VBOs.
================
*/
static void R_BatchSurface (msurface_t *s)
{
	int num_surf_indices;

	num_surf_indices = R_NumTriangleIndicesForSurf (s);
	
	if (num_vbo_indices + num_surf_indices > MAX_BATCH_SIZE)
		R_FlushBatch();
	
	R_TriangleIndicesForSurf (s, &vbo_indices[num_vbo_indices]);
	num_vbo_indices += num_surf_indices;
}

/*
================
R_DrawTextureChains_Multitexture -- johnfitz
================
*/
void R_DrawTextureChains_Multitexture (qmodel_t *model, entity_t *ent, texchain_t chain)
{
	int			i, j;
	msurface_t	*s;
	texture_t	*t;
	float		*v;
	qboolean	bound;

	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain] || t->texturechains[chain]->flags & (SURF_DRAWTILED | SURF_NOTEXTURE))
			continue;

		bound = false;
		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
			{
				if (!bound) //only bind once we are sure we need this texture
				{
					GL_Bind ((R_TextureAnimation(t, ent != NULL ? ent->frame : 0))->gltexture);
					
					if (t->texturechains[chain]->flags & SURF_DRAWFENCE)
						glEnable(GL_ALPHA_TEST); // Flip alpha test back on
					
					GL_EnableMultitexture(); // selects TEXTURE1
					bound = true;
				}
				GL_Bind (lightmap_textures[s->lightmaptexturenum]);
#ifdef VITA
				glBegin(GL_TRIANGLE_FAN);
#else
				glBegin(GL_POLYGON);
#endif
				v = s->polys->verts[0];
				for (j=0 ; j<s->polys->numverts ; j++, v+= VERTEXSIZE)
				{
					GL_MTexCoord2fFunc (GL_TEXTURE0_ARB, v[3], v[4]);
					GL_MTexCoord2fFunc (GL_TEXTURE1_ARB, v[5], v[6]);
					glVertex3fv (v);
				}
				glEnd ();
				rs_brushpasses++;
			}
		GL_DisableMultitexture(); // selects TEXTURE0

		if (bound && t->texturechains[chain]->flags & SURF_DRAWFENCE)
			glDisable (GL_ALPHA_TEST); // Flip alpha test back off
	}
}

/*
================
R_DrawTextureChains_NoTexture -- johnfitz

draws surfs whose textures were missing from the BSP
================
*/
void R_DrawTextureChains_NoTexture (qmodel_t *model, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	qboolean	bound;

	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain] || !(t->texturechains[chain]->flags & SURF_NOTEXTURE))
			continue;

		bound = false;

		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
			{
				if (!bound) //only bind once we are sure we need this texture
				{
					GL_Bind (t->gltexture);
					bound = true;
				}
				DrawGLPoly (s->polys);
				rs_brushpasses++;
			}
	}
}

/*
================
R_DrawTextureChains_TextureOnly -- johnfitz
================
*/
void R_DrawTextureChains_TextureOnly (qmodel_t *model, entity_t *ent, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	qboolean	bound;

	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain] || t->texturechains[chain]->flags & (SURF_DRAWTURB | SURF_DRAWSKY))
			continue;

		bound = false;

		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
			{
				if (!bound) //only bind once we are sure we need this texture
				{
					GL_Bind ((R_TextureAnimation(t, ent != NULL ? ent->frame : 0))->gltexture);
					
					if (t->texturechains[chain]->flags & SURF_DRAWFENCE)
						glEnable(GL_ALPHA_TEST); // Flip alpha test back on
					
					bound = true;
				}
				DrawGLPoly (s->polys);
				rs_brushpasses++;
			}
			
		if (bound && t->texturechains[chain]->flags & SURF_DRAWFENCE)
			glDisable (GL_ALPHA_TEST); // Flip alpha test back off
	}
}

/*
================
GL_WaterAlphaForEntitySurface -- ericw
 
Returns the water alpha to use for the entity and surface combination.
================
*/
float GL_WaterAlphaForEntitySurface (entity_t *ent, msurface_t *s)
{
	float entalpha;
	if (ent == NULL || ent->alpha == ENTALPHA_DEFAULT)
		entalpha = GL_WaterAlphaForSurface(s);
	else
		entalpha = ENTALPHA_DECODE(ent->alpha);
	return entalpha;
}

/*
================
R_DrawTextureChains_Water -- johnfitz
================
*/
void R_DrawTextureChains_Water (qmodel_t *model, entity_t *ent, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	glpoly_t	*p;
	qboolean	bound;
	float entalpha;

	if (r_drawflat_cheatsafe || r_lightmap_cheatsafe) // ericw -- !r_drawworld_cheatsafe check moved to R_DrawWorld_Water ()
		return;

	if (r_oldwater.value)
	{
		for (i=0 ; i<model->numtextures ; i++)
		{
			t = model->textures[i];
			if (!t || !t->texturechains[chain] || !(t->texturechains[chain]->flags & SURF_DRAWTURB))
				continue;
			bound = false;
			entalpha = 1.0f;
			for (s = t->texturechains[chain]; s; s = s->texturechain)
				if (!s->culled)
				{
					if (!bound) //only bind once we are sure we need this texture
					{
						entalpha = GL_WaterAlphaForEntitySurface (ent, s);
						R_BeginTransparentDrawing (entalpha);
						GL_Bind (t->gltexture);
						bound = true;
					}
					for (p = s->polys->next; p; p = p->next)
					{
						DrawWaterPoly (p);
						rs_brushpasses++;
					}
				}
			R_EndTransparentDrawing (entalpha);
		}
	}
	else
	{
		for (i=0 ; i<model->numtextures ; i++)
		{
			t = model->textures[i];
			if (!t || !t->texturechains[chain] || !(t->texturechains[chain]->flags & SURF_DRAWTURB))
				continue;
			bound = false;
			entalpha = 1.0f;
			for (s = t->texturechains[chain]; s; s = s->texturechain)
				if (!s->culled)
				{
					if (!bound) //only bind once we are sure we need this texture
					{
						entalpha = GL_WaterAlphaForEntitySurface (ent, s);
						R_BeginTransparentDrawing (entalpha);
						GL_Bind (t->warpimage);

						if (model != cl.worldmodel)
						{
							// ericw -- this is copied from R_DrawSequentialPoly.
							// If the poly is not part of the world we have to
							// set this flag
							t->update_warp = true; // FIXME: one frame too late!
						}

						bound = true;
					}
					DrawGLPoly (s->polys);
					rs_brushpasses++;
				}
			R_EndTransparentDrawing (entalpha);
		}
	}
}

/*
================
R_DrawTextureChains_White -- johnfitz -- draw sky and water as white polys when r_lightmap is 1
================
*/
void R_DrawTextureChains_White (qmodel_t *model, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;

	glDisable (GL_TEXTURE_2D);
	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain] || !(t->texturechains[chain]->flags & SURF_DRAWTILED))
			continue;

		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
			{
				DrawGLPoly (s->polys);
				rs_brushpasses++;
			}
	}
	glEnable (GL_TEXTURE_2D);
}

/*
================
R_DrawLightmapChains -- johnfitz -- R_BlendLightmaps stripped down to almost nothing
================
*/
void R_DrawLightmapChains (void)
{
	int			i, j;
	glpoly_t	*p;
	float		*v;

	for (i=0 ; i<MAX_LIGHTMAPS ; i++)
	{
		if (!lightmap_polys[i])
			continue;

		GL_Bind (lightmap_textures[i]);
		for (p = lightmap_polys[i]; p; p=p->chain)
		{
#ifdef VITA
			glBegin(GL_TRIANGLE_FAN);
#else
			glBegin (GL_POLYGON);
#endif
			v = p->verts[0];
			for (j=0 ; j<p->numverts ; j++, v+= VERTEXSIZE)
			{
				glTexCoord2f (v[5], v[6]);
				glVertex3fv (v);
			}
			glEnd ();
			rs_brushpasses++;
		}
	}
}

static GLuint r_world_program;

// uniforms used in vert shader

// uniforms used in frag shader
static GLuint texLoc;
static GLuint LMTexLoc;
static GLuint fullbrightTexLoc;
static GLuint useFullbrightTexLoc;
static GLuint useOverbrightLoc;
static GLuint useAlphaTestLoc;
static GLuint alphaLoc;
static GLuint grayscale_enableLoc;
#ifdef VITA
static GLuint fogDensityLoc;
#endif

#define vertAttrIndex 0
#define texCoordsAttrIndex 1
#define LMCoordsAttrIndex 2

/*
=============
GLWorld_CreateShaders
=============
*/
void GLWorld_CreateShaders (void)
{
	const glsl_attrib_binding_t bindings[] = {
		{ "Vert", vertAttrIndex },
		{ "TexCoords", texCoordsAttrIndex },
		{ "LMCoords", LMCoordsAttrIndex }
	};
#ifdef VITA
	const GLchar *vertSource = \
		"uniform float4x4 gl_ModelViewProjectionMatrix;\n"
		"\n"
		"void main(\n"
		"	float2 TexCoords,\n"
		"	float3 Vert,\n"
		"	float2 LMCoords,\n"
		"	float2 out gl_TexCoord : TEXCOORD0,\n"
		"	float2 out gl_TexCoord1 : TEXCOORD1,\n"
		"	float4 out gl_Position : POSITION\n"
		") {\n"
		"	gl_TexCoord = TexCoords;\n"
		"	gl_TexCoord1 = LMCoords;\n"
		"	gl_Position = mul(gl_ModelViewProjectionMatrix, float4(Vert, 1.0));\n"
		"}\n";

	const GLchar *fragSource = \
		"uniform sampler2D Tex;\n"
		"uniform sampler2D LMTex;\n"
		"uniform sampler2D FullbrightTex;\n"
		"uniform int UseFullbrightTex;\n"
		"uniform int UseOverbright;\n"
		"uniform int UseAlphaTest;\n"
		"uniform float Alpha;\n"
		"uniform int gs_mod;\n"
		"uniform float fog_density;\n"
		"\n"
		"float4 main(\n"
		"	float4 coords : WPOS,\n"
		"	float2 gl_TexCoord : TEXCOORD0,\n"
		"	float2 gl_TexCoord1 : TEXCOORD1\n"
		") {\n"
		"	float4 result = tex2D(Tex, gl_TexCoord);\n"
		"	if (UseAlphaTest && (result.a < 0.666))\n"
		"		discard;\n"
		"	result *= tex2D(LMTex, gl_TexCoord1);\n"
		"	if (UseOverbright)\n"
		"		result.rgb *= 2.0;\n"
		"	if (UseFullbrightTex)\n"
		"		result += tex2D(FullbrightTex, gl_TexCoord);\n"
		"	result = clamp(result, 0.0, 1.0);\n"
		"	float FogFragCoord = coords.z / coords.w;\n"
		"	float fog = exp(-fog_density * fog_density * FogFragCoord * FogFragCoord);\n"
		"	fog = clamp(fog, 0.0, 1.0);\n"
		"	result = lerp(float4(0.3, 0.3, 0.3, 1.0), result, fog);\n"
		"	result.a = Alpha;\n" // FIXME: This will make almost transparent things cut holes though heavy fog
		"   if (gs_mod) {\n"
		"       float value = clamp((result.r * 0.33) + (result.g * 0.55) + (result.b * 0.11), 0.0, 1.0);\n"
		"       result.r = value;\n"
		"       result.g = value;\n"
		"       result.b = value;\n"
		"   }"
		"	return result;\n"
		"}\n";
#else
	const GLchar *vertSource = \
		"#version 110\n"
		"\n"
		"attribute vec3 Vert;\n"
		"attribute vec2 TexCoords;\n"
		"attribute vec2 LMCoords;\n"
		"\n"
		"varying float FogFragCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_TexCoord[0] = vec4(TexCoords, 0.0, 0.0);\n"
		"	gl_TexCoord[1] = vec4(LMCoords, 0.0, 0.0);\n"
		"	gl_Position = gl_ModelViewProjectionMatrix * vec4(Vert, 1.0);\n"
		"	FogFragCoord = gl_Position.w;\n"
		"}\n";
	
	const GLchar *fragSource = \
		"#version 110\n"
		"\n"
		"uniform sampler2D Tex;\n"
		"uniform sampler2D LMTex;\n"
		"uniform sampler2D FullbrightTex;\n"
		"uniform bool UseFullbrightTex;\n"
		"uniform bool UseOverbright;\n"
		"uniform bool UseAlphaTest;\n"
		"uniform float Alpha;\n"
		"uniform bool gs_mod;\n"

		"\n"
		"varying float FogFragCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	vec4 result = texture2D(Tex, gl_TexCoord[0].xy);\n"
		"	if (UseAlphaTest && (result.a < 0.666))\n"
		"		discard;\n"
		"	result *= texture2D(LMTex, gl_TexCoord[1].xy);\n"
		"	if (UseOverbright)\n"
		"		result.rgb *= 2.0;\n"
		"	if (UseFullbrightTex)\n"
		"		result += texture2D(FullbrightTex, gl_TexCoord[0].xy);\n"
		"	result = clamp(result, 0.0, 1.0);\n"
		"	float fog = exp(-gl_Fog.density * gl_Fog.density * FogFragCoord * FogFragCoord);\n"
		"	fog = clamp(fog, 0.0, 1.0);\n"
		"	result = mix(gl_Fog.color, result, fog);\n"
		"	result.a = Alpha;\n" // FIXME: This will make almost transparent things cut holes though heavy fog
		"   if (gs_mod) {\n"
		"       float value = clamp((result.r * 0.33) + (result.g * 0.55) + (result.b * 0.11), 0.0, 1.0);\n"
		"       result.r = value;\n"
		"       result.g = value;\n"
		"       result.b = value;\n"
		"   }"
		"	gl_FragColor = result;\n"
		"}\n";
#endif
	if (!gl_glsl_alias_able)
		return;
	
	r_world_program = GL_CreateProgram (vertSource, fragSource, sizeof(bindings)/sizeof(bindings[0]), bindings);
	
	if (r_world_program != 0)
	{
		// get uniform locations
#ifndef VITA
		texLoc = GL_GetUniformLocation (&r_world_program, "Tex");
		LMTexLoc = GL_GetUniformLocation (&r_world_program, "LMTex");
		fullbrightTexLoc = GL_GetUniformLocation (&r_world_program, "FullbrightTex");
#endif
		useFullbrightTexLoc = GL_GetUniformLocation (&r_world_program, "UseFullbrightTex");
		useOverbrightLoc = GL_GetUniformLocation (&r_world_program, "UseOverbright");
		useAlphaTestLoc = GL_GetUniformLocation (&r_world_program, "UseAlphaTest");
		alphaLoc = GL_GetUniformLocation (&r_world_program, "Alpha");
		grayscale_enableLoc = GL_GetUniformLocation (&r_world_program, "gs_mod");
#ifdef VITA
		fogDensityLoc = GL_GetUniformLocation(&r_world_program, "fog_density");
#endif
	}
}

extern GLuint gl_bmodel_vbo;

/*
================
R_DrawTextureChains_GLSL -- ericw

Draw lightmapped surfaces with fulbrights in one pass, using VBO.
Requires 3 TMUs, OpenGL 2.0
================
*/
void R_DrawTextureChains_GLSL (qmodel_t *model, entity_t *ent, texchain_t chain)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;
	qboolean	bound;
	int		lastlightmap;
	gltexture_t	*fullbright = NULL;
	float		entalpha;
	
	entalpha = (ent != NULL) ? ENTALPHA_DECODE(ent->alpha) : 1.0f;

// enable blending / disable depth writes
	if (entalpha < 1)
	{
		glDepthMask (GL_FALSE);
		glEnable (GL_BLEND);
	}
	
	GL_UseProgramFunc (r_world_program);
	
// Bind the buffers
	GL_BindBuffer (GL_ARRAY_BUFFER, gl_bmodel_vbo);
	GL_BindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0); // indices come from client memory!

	GL_EnableVertexAttribArrayFunc (vertAttrIndex);
	GL_EnableVertexAttribArrayFunc (texCoordsAttrIndex);
	GL_EnableVertexAttribArrayFunc (LMCoordsAttrIndex);
	
	GL_VertexAttribPointerFunc (vertAttrIndex,      3, GL_FLOAT, GL_FALSE, VERTEXSIZE * sizeof(float), ((float *)0));
	GL_VertexAttribPointerFunc (texCoordsAttrIndex, 2, GL_FLOAT, GL_FALSE, VERTEXSIZE * sizeof(float), ((float *)0) + 3);
	GL_VertexAttribPointerFunc (LMCoordsAttrIndex,  2, GL_FLOAT, GL_FALSE, VERTEXSIZE * sizeof(float), ((float *)0) + 5);
	
// set uniforms
#ifndef VITA
	GL_Uniform1iFunc (texLoc, 0);
	GL_Uniform1iFunc (LMTexLoc, 1);
	GL_Uniform1iFunc (fullbrightTexLoc, 2);
#endif
	GL_Uniform1iFunc (useFullbrightTexLoc, 0);
	GL_Uniform1iFunc (useOverbrightLoc, (int)gl_overbright.value);
	GL_Uniform1iFunc (useAlphaTestLoc, 0);
	GL_Uniform1fFunc (alphaLoc, entalpha);
#ifdef VITA
	GL_Uniform1fFunc (fogDensityLoc, Fog_GetDensity() / 64.0f);
#endif

	// naievil -- experimental grayscale shader
	GL_Uniform1fFunc (grayscale_enableLoc, sv_player->v.renderGrayscale);

	
	for (i=0 ; i<model->numtextures ; i++)
	{
		t = model->textures[i];

		if (!t || !t->texturechains[chain] || t->texturechains[chain]->flags & (SURF_DRAWTILED | SURF_NOTEXTURE))
			continue;

	// Enable/disable TMU 2 (fullbrights)
	// FIXME: Move below to where we bind GL_TEXTURE0
		if (gl_fullbrights.value && (fullbright = R_TextureAnimation(t, ent != NULL ? ent->frame : 0)->fullbright))
		{
			GL_SelectTexture (GL_TEXTURE2);
			GL_Bind (fullbright);
			GL_Uniform1iFunc (useFullbrightTexLoc, 1);
		}
		else
			GL_Uniform1iFunc (useFullbrightTexLoc, 0);

		R_ClearBatch ();

		bound = false;
		lastlightmap = 0; // avoid compiler warning
		for (s = t->texturechains[chain]; s; s = s->texturechain)
			if (!s->culled)
			{
				if (!bound) //only bind once we are sure we need this texture
				{
					GL_SelectTexture (GL_TEXTURE0);
					GL_Bind ((R_TextureAnimation(t, ent != NULL ? ent->frame : 0))->gltexture);
					
					if (t->texturechains[chain]->flags & SURF_DRAWFENCE)
						GL_Uniform1iFunc (useAlphaTestLoc, 1); // Flip alpha test back on
										
					bound = true;
					lastlightmap = s->lightmaptexturenum;
				}
				
				if (s->lightmaptexturenum != lastlightmap)
					R_FlushBatch ();

				GL_SelectTexture (GL_TEXTURE1);
				GL_Bind (lightmap_textures[s->lightmaptexturenum]);
				lastlightmap = s->lightmaptexturenum;
				R_BatchSurface (s);

				rs_brushpasses++;
			}

		R_FlushBatch ();

		if (bound && t->texturechains[chain]->flags & SURF_DRAWFENCE)
			GL_Uniform1iFunc (useAlphaTestLoc, 0); // Flip alpha test back off
	}
	
	// clean up
	GL_DisableVertexAttribArrayFunc (vertAttrIndex);
	GL_DisableVertexAttribArrayFunc (texCoordsAttrIndex);
	GL_DisableVertexAttribArrayFunc (LMCoordsAttrIndex);
	
	GL_UseProgramFunc (0);
	GL_SelectTexture (GL_TEXTURE0);
	
	if (entalpha < 1)
	{
		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);
	}
}

/*
=============
R_DrawWorld -- johnfitz -- rewritten
=============
*/
void R_DrawTextureChains (qmodel_t *model, entity_t *ent, texchain_t chain)
{
	float entalpha;
	
	if (ent != NULL)
		entalpha = ENTALPHA_DECODE(ent->alpha);
	else
		entalpha = 1;

// ericw -- the mh dynamic lightmap speedup: make a first pass through all
// surfaces we are going to draw, and rebuild any lightmaps that need it.
// this also chains surfaces by lightmap which is used by r_lightmap 1.
// the previous implementation of the speedup uploaded lightmaps one frame
// late which was visible under some conditions, this method avoids that.
	R_BuildLightmapChains (model, chain);
	R_UploadLightmaps ();

	if (r_drawflat_cheatsafe)
	{
		glDisable (GL_TEXTURE_2D);
		R_DrawTextureChains_Drawflat (model, chain);
		glEnable (GL_TEXTURE_2D);
		return;
	}

	if (r_fullbright_cheatsafe)
	{
		R_BeginTransparentDrawing (entalpha);
		R_DrawTextureChains_TextureOnly (model, ent, chain);
		R_EndTransparentDrawing (entalpha);
		goto fullbrights;
	}

	if (r_lightmap_cheatsafe)
	{
		if (!gl_overbright.value)
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glColor3f(0.5, 0.5, 0.5);
		}
		R_DrawLightmapChains ();
		if (!gl_overbright.value)
		{
			glColor3f(1,1,1);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		R_DrawTextureChains_White (model, chain);
		return;
	}

	R_BeginTransparentDrawing (entalpha);

	R_DrawTextureChains_NoTexture (model, chain);

	// OpenGL 2 fast path
	if (r_world_program != 0)
	{
		R_EndTransparentDrawing (entalpha);
		
		R_DrawTextureChains_GLSL (model, ent, chain);
		return;
	}

	if (gl_overbright.value)
	{
		if (gl_texture_env_combine && gl_mtexable) //case 1: texture and lightmap in one pass, overbright using texture combiners
		{
			GL_EnableMultitexture ();
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);
			GL_DisableMultitexture ();
			R_DrawTextureChains_Multitexture (model, ent, chain);
			GL_EnableMultitexture ();
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.0f);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			GL_DisableMultitexture ();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		else if (entalpha < 1) //case 2: can't do multipass if entity has alpha, so just draw the texture
		{
			R_DrawTextureChains_TextureOnly (model, ent, chain);
		}
		else //case 3: texture in one pass, lightmap in second pass using 2x modulation blend func, fog in third pass
		{
			//to make fog work with multipass lightmapping, need to do one pass
			//with no fog, one modulate pass with black fog, and one additive
			//pass with black geometry and normal fog
			Fog_DisableGFog ();
			R_DrawTextureChains_TextureOnly (model, ent, chain);
			Fog_EnableGFog ();
			glDepthMask (GL_FALSE);
			glEnable (GL_BLEND);
			glBlendFunc (GL_DST_COLOR, GL_SRC_COLOR); //2x modulate
			Fog_StartAdditive ();
			R_DrawLightmapChains ();
			Fog_StopAdditive ();
			if (Fog_GetDensity() > 0)
			{
				glBlendFunc(GL_ONE, GL_ONE); //add
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glColor3f(0,0,0);
				R_DrawTextureChains_TextureOnly (model, ent, chain);
				glColor3f(1,1,1);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable (GL_BLEND);
			glDepthMask (GL_TRUE);
		}
	}
	else
	{
		if (gl_mtexable) //case 4: texture and lightmap in one pass, regular modulation
		{
			GL_EnableMultitexture ();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			GL_DisableMultitexture ();
			R_DrawTextureChains_Multitexture (model, ent, chain);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		else if (entalpha < 1) //case 5: can't do multipass if entity has alpha, so just draw the texture
		{
			R_DrawTextureChains_TextureOnly (model, ent, chain);
		}
		else //case 6: texture in one pass, lightmap in a second pass, fog in third pass
		{
			//to make fog work with multipass lightmapping, need to do one pass
			//with no fog, one modulate pass with black fog, and one additive
			//pass with black geometry and normal fog
			Fog_DisableGFog ();
			R_DrawTextureChains_TextureOnly (model, ent, chain);
			Fog_EnableGFog ();
			glDepthMask (GL_FALSE);
			glEnable (GL_BLEND);
			glBlendFunc(GL_ZERO, GL_SRC_COLOR); //modulate
			Fog_StartAdditive ();
			R_DrawLightmapChains ();
			Fog_StopAdditive ();
			if (Fog_GetDensity() > 0)
			{
				glBlendFunc(GL_ONE, GL_ONE); //add
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glColor3f(0,0,0);
				R_DrawTextureChains_TextureOnly (model, ent, chain);
				glColor3f(1,1,1);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable (GL_BLEND);
			glDepthMask (GL_TRUE);
		}
	}

	R_EndTransparentDrawing (entalpha);

fullbrights:
	if (gl_fullbrights.value)
	{
		glDepthMask (GL_FALSE);
		glEnable (GL_BLEND);
		glBlendFunc (GL_ONE, GL_ONE);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor3f (entalpha, entalpha, entalpha);
		Fog_StartAdditive ();
		R_DrawTextureChains_Glow (model, ent, chain);
		Fog_StopAdditive ();
		glColor3f (1, 1, 1);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable (GL_BLEND);
		glDepthMask (GL_TRUE);
	}
}

/*
=============
R_DrawWorld -- ericw -- moved from R_DrawTextureChains, which is no longer specific to the world.
=============
*/
void R_DrawWorld (void)
{
	if (!r_drawworld_cheatsafe)
		return;

	R_DrawTextureChains (cl.worldmodel, NULL, chain_world);
}

/*
=============
R_DrawWorld_Water -- ericw -- moved from R_DrawTextureChains_Water, which is no longer specific to the world.
=============
*/
void R_DrawWorld_Water (void)
{
	if (!r_drawworld_cheatsafe)
		return;

	R_DrawTextureChains_Water (cl.worldmodel, NULL, chain_world);
}

/*
=============
R_DrawWorld_ShowTris -- ericw -- moved from R_DrawTextureChains_ShowTris, which is no longer specific to the world.
=============
*/
void R_DrawWorld_ShowTris (void)
{
	if (!r_drawworld_cheatsafe)
		return;

	R_DrawTextureChains_ShowTris (cl.worldmodel, chain_world);
}
