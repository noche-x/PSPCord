/*
 * Helper for use with the PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed as 'free to use and modify as long as credited appropriately'
 *
 * valloc.c - Standard C library VRAM allocation routines.
 *
 * Copyright (c) 2006 Alexander Berl <a.berl@gmx.de>
 *
 */
#ifndef _VALLOC_H
#define _VALLOC_H

#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

	/*
		The following are a few helperfunctions to help manage vram in gu-examples.
		Do not use for your own code, it's better you manage it in your own way.
	*/

	/* make a static allocation of vram memory and return pointer relative to vram start */
	void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
	/* make a static allocation of vram memory and return absolute pointer */
	void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm);

	// the following is not yet implemented
	/*
	void beginDynamicVramFrame();
	void endDynamicVramFrame();
	void* getDynamicVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
	void* getDynamicVramTexture(unsigned int width, unsigned int height, unsigned int psm);
	*/

#define vrelptr	vGuPointer
#define vabsptr vCPUPointer

size_t vgetMemorySize(unsigned int width, unsigned int height, unsigned int psm);

// Return a pointer relative to VRAM Base address useable by GU
void* vGuPointer( void* ptr );
// Return an absolute pointer useable by CPU
void* vCPUPointer( void* ptr );

// Returns an absolute pointer useable by CPU
void* valloc( size_t size );
void  vfree( void* ptr );

size_t vmemavail();
size_t vlargestblock();


#ifdef __cplusplus
}
#endif

#endif	// _VALLOC_H
