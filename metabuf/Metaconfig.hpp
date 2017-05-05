#	pragma once

#	ifndef METABUF_MALLOC
#	include <malloc.h>
#	define METABUF_MALLOC(S) malloc(S)
#	endif

#	ifndef METABUF_FREE
#	include <malloc.h>
#	define METABUF_FREE(B, S) free(B)
#	endif

#	ifndef METABUF_MEMCPY
#	include <algorithm>
#	define METABUF_MEMCPY(Dest, Src, Size) std::copy((const unsigned char *)Src, (const unsigned char *)Src + Size, (unsigned char *)Dest)
#	endif