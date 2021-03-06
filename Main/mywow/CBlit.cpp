#include "stdafx.h"
#include "CBlit.h"
#include "CImage.h"

bool CBlit::Blit( CImage* dest, const vector2di& destPos, const dimension2du& destDimension, CImage* src, const vector2di& srcPos )
{
	if ( !dest || !src ||
		dest->getDimension().Width < destPos.X + destDimension.Width ||
		dest->getDimension().Height < destPos.Y + destDimension.Height ||
		src->getDimension().Width < srcPos.X + destDimension.Width ||
		src->getDimension().Height < srcPos.Y + destDimension.Height )
	{
		return false;
	}

	SBlitJob job;

	job.width = destDimension.Width;
	job.height = destDimension.Height;

	job.srcPitch = src->getPitch();
	job.srcPixelMul = src->getBytesPerPixel();
	job.src = ( (u8*)src->lock() + job.srcPitch * srcPos.Y + job.srcPixelMul * srcPos.X );

	job.dstPitch = dest->getPitch();
	job.dstPixelMul = dest->getBytesPerPixel();
	job.dst = ( (u8*)dest->lock() + job.dstPitch * destPos.Y + job.dstPixelMul * destPos.X );

	ECOLOR_FORMAT srcFormat = src->getColorFormat();
	ECOLOR_FORMAT destFormat = dest->getColorFormat();

	if ( srcFormat == destFormat )
	{
		executeBlit_TextureCopy_x_to_x( &job );
	}
	else
		if ( srcFormat == ECF_A8R8G8B8)
		{
			if ( destFormat == ECF_A1R5G5B5 )
				executeBlit_TextureCopy_32_to_16(&job);
			else if ( destFormat == ECF_R8G8B8 )
				executeBlit_TextureCopy_32_to_24(&job);
			else
			{
				src->unlock();
				dest->unlock();
				return false;
			}
		}
		else
			if ( srcFormat == ECF_A1R5G5B5 )
			{
				if ( destFormat == ECF_R8G8B8 )
					executeBlit_TextureCopy_16_to_24(&job);
				else if ( destFormat == ECF_A8R8G8B8 )
					executeBlit_TextureCopy_16_to_32(&job);
				else
				{
					src->unlock();
					dest->unlock();
					return false;
				}
			}
			else
				if ( srcFormat == ECF_R8G8B8 )
				{
					if ( destFormat == ECF_A1R5G5B5 )
						executeBlit_TextureCopy_24_to_16(&job);
					else if ( destFormat == ECF_A8R8G8B8 )
						executeBlit_TextureCopy_24_to_32(&job);
					else
					{
						src->unlock();
						dest->unlock();
						return false;
					}
				}
				else
				{
					src->unlock();
					dest->unlock();
					return false;
				}

				src->unlock();
				dest->unlock();

				return true;
}

void CBlit::executeBlit_TextureCopy_x_to_x( const SBlitJob * job )
{
	const void *src = (void*) job->src;
	void *dst = (void*) job->dst;

	const u32 widthPitch = job->width * job->dstPixelMul;
	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		memcpy( dst, src, widthPitch );

		src = (void*) ( (u8*) (src) + job->srcPitch );
		dst = (void*) ( (u8*) (dst) + job->dstPitch );
	}
}

void CBlit::executeBlit_TextureCopy_16_to_32( const SBlitJob * job )
{
	const u16 *src = (u16*) job->src;
	u32 *dst = (u32*) job->dst;

	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		for ( s32 dx = 0; dx != job->width; ++dx )
		{
			dst[dx] = SColor::A1R5G5B5toA8R8G8B8( src[dx] );
		}

		src = (u16*) ( (u8*) (src) + job->srcPitch );
		dst = (u32*) ( (u8*) (dst) + job->dstPitch );
	}
}

void CBlit::executeBlit_TextureCopy_32_to_16( const SBlitJob * job )
{
	const u32 *src = static_cast<const u32*>(job->src);
	u16 *dst = static_cast<u16*>(job->dst);

	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		for ( s32 dx = 0; dx != job->width; ++dx )
		{
			const u32 s = src[dx];
			dst[dx] = SColor::A8R8G8B8toA1R5G5B5( s );
		}

		src = (u32*) ( (u8*) (src) + job->srcPitch );
		dst = (u16*) ( (u8*) (dst) + job->dstPitch );
	}
}

void CBlit::executeBlit_TextureCopy_16_to_24( const SBlitJob * job )
{
	const u16 *src = (u16*) job->src;
	u8 *dst = (u8*) job->dst;

	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		for ( s32 dx = 0; dx != job->width; ++dx )
		{
			u32 colour = SColor::A1R5G5B5toA8R8G8B8( src[dx] );
			u8 * writeTo = &dst[dx * 3];
			*writeTo++ = (colour >> 16)& 0xFF;
			*writeTo++ = (colour >> 8) & 0xFF;
			*writeTo++ = colour & 0xFF;
		}

		src = (u16*) ( (u8*) (src) + job->srcPitch );
		dst += job->dstPitch;
	}
}

void CBlit::executeBlit_TextureCopy_24_to_16( const SBlitJob * job )
{
	const void *src = (void*) job->src;
	u16 *dst = (u16*) job->dst;

	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		u8 * s = (u8*) src;

		for ( s32 dx = 0; dx != job->width; ++dx )
		{
			dst[dx] = SColor::RGB16(s[0], s[1], s[2]);
			s += 3;
		}

		src = (void*) ( (u8*) (src) + job->srcPitch );
		dst = (u16*) ( (u8*) (dst) + job->dstPitch );
	}
}

void CBlit::executeBlit_TextureCopy_24_to_32( const SBlitJob * job )
{
	void *src = (void*) job->src;
	u32 *dst = (u32*) job->dst;

	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		u8 * s = (u8*) src;

		for ( s32 dx = 0; dx != job->width; ++dx )
		{
			dst[dx] = 0xFF000000 | s[0] << 16 | s[1] << 8 | s[2];
			s += 3;
		}

		src = (void*) ( (u8*) (src) + job->srcPitch );
		dst = (u32*) ( (u8*) (dst) + job->dstPitch );
	}
}

void CBlit::executeBlit_TextureCopy_32_to_24( const SBlitJob * job )
{
	const u32 * src = (u32*) job->src;
	u8 * dst = (u8*) job->dst;

	for ( s32 dy = 0; dy != job->height; ++dy )
	{
		for ( s32 dx = 0; dx != job->width; ++dx )
		{
			u8 * writeTo = &dst[dx * 3];
			*writeTo++ = (src[dx] >> 16)& 0xFF;
			*writeTo++ = (src[dx] >> 8) & 0xFF;
			*writeTo++ = src[dx] & 0xFF;
		}

		src = (u32*) ( (u8*) (src) + job->srcPitch );
		dst += job->dstPitch ;
	}
}

void CBlit::convert_viaFormat( const void* sP, ECOLOR_FORMAT sF, s32 sN, void* dP, ECOLOR_FORMAT dF )
{
	switch (sF)
	{
	case ECF_A1R5G5B5:
		switch (dF)
		{
		case ECF_A1R5G5B5:
			convert_A1R5G5B5toA1R5G5B5(sP, sN, dP);
			break;
		case ECF_R5G6B5:
			convert_A1R5G5B5toR5G6B5(sP,sN,dP);
			break;
		case ECF_A8R8G8B8:
			convert_A1R5G5B5toA8R8G8B8(sP, sN, dP);
			break;
		case ECF_R8G8B8:
			convert_A1R5G5B5toR8G8B8(sP, sN, dP);
			break;
		default:
			ASSERT(false);
            break;
		}
		break;
	case ECF_R5G6B5:
		switch (dF)
		{
		case ECF_A1R5G5B5:
			convert_R5G6B5toA1R5G5B5(sP, sN, dP);
			break;
		case ECF_R5G6B5:
			convert_R5G6B5toR5G6B5(sP, sN, dP);
			break;
		case ECF_A8R8G8B8:
			convert_R5G6B5toA8R8G8B8(sP, sN, dP);
			break;
		case ECF_R8G8B8:
			convert_R5G6B5toR8G8B8(sP, sN, dP);
			break;
        default:
            ASSERT(false);
            break;
		}
		break;
	case ECF_A8R8G8B8:
		switch (dF)
		{
		case ECF_A1R5G5B5:
			convert_A8R8G8B8toA1R5G5B5(sP, sN, dP);
			break;
		case ECF_R5G6B5:
			convert_A8R8G8B8toR5G6B5(sP, sN, dP);
			break;
		case ECF_A8R8G8B8:
			convert_A8R8G8B8toA8R8G8B8(sP, sN, dP);
			break;
		case ECF_R8G8B8:
			convert_A8R8G8B8toR8G8B8(sP, sN, dP);
			break;
		default:
			ASSERT(false);
            break;
		}
		break;
	case ECF_R8G8B8:
		switch (dF)
		{
		case ECF_A1R5G5B5:
			convert_R8G8B8toA1R5G5B5(sP, sN, dP);
			break;
		case ECF_R5G6B5:
			convert_R8G8B8toR5G6B5(sP, sN, dP);
			break;
		case ECF_A8R8G8B8:
			convert_R8G8B8toA8R8G8B8(sP, sN, dP);
			break;
		case ECF_R8G8B8:
			convert_R8G8B8toR8G8B8(sP, sN, dP);
			break;
		default:
			ASSERT(false);
            break;
		}
		break;
    default:
        ASSERT(false);
        break;
	}
}

void CBlit::convert_A1R5G5B5toR8G8B8( const void* sP, s32 sN, void* dP )
{
	u16* sB = (u16*)sP;
	u8 * dB = (u8 *)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		dB[2] = (u8)((*sB & 0x7c00) >> 7);
		dB[1] = (u8)((*sB & 0x03e0) >> 2);
		dB[0] = (u8)((*sB & 0x1f) << 3);

		sB += 1;
		dB += 3;
	}
}

void CBlit::convert_A1R5G5B5toB8G8R8(const void* sP, s32 sN, void* dP)
{
	u16* sB = (u16*)sP;
	u8 * dB = (u8 *)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		dB[0] = (u8)((*sB & 0x7c00) >> 7);
		dB[1] = (u8)((*sB & 0x03e0) >> 2);
		dB[2] = (u8)((*sB & 0x1f) << 3);

		sB += 1;
		dB += 3;
	}
}

void CBlit::convert_A1R5G5B5toA8R8G8B8( const void* sP, s32 sN, void* dP )
{
	u16* sB = (u16*)sP;
	u32* dB = (u32*)dP;

	for (s32 x = 0; x < sN; ++x)
		*dB++ = SColor::A1R5G5B5toA8R8G8B8(*sB++);
}

void CBlit::convert_A1R5G5B5toA1R5G5B5( const void* sP, s32 sN, void* dP )
{
	memcpy(dP, sP, sN * 2);
}

void CBlit::convert_A1R5G5B5toR5G6B5(const void* sP, s32 sN, void* dP)
{
	u16* sB = (u16*)sP;
	u16* dB = (u16*)dP;

	for (s32 x = 0; x < sN; ++x)
		*dB++ = SColor::A1R5G5B5toR5G6B5(*sB++);
}

void CBlit::convert_A8R8G8B8toA8B8G8R8( const void* sP, s32 sN, void* dP )
{
	u8* sB = (u8*)sP;
	u8* dB = (u8*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		// sB[3] is alpha
		dB[0] = sB[2];
		dB[1] = sB[1];
		dB[2] = sB[0];

		sB += 4;
		dB += 4;
	}
}

void CBlit::convert_A8R8G8B8toR8G8B8( const void* sP, s32 sN, void* dP )
{
	u8* sB = (u8*)sP;
	u8* dB = (u8*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		// sB[3] is alpha
		dB[0] = sB[2];
		dB[1] = sB[1];
		dB[2] = sB[0];

		sB += 4;
		dB += 3;
	}
}

void CBlit::convert_A8R8G8B8toB8G8R8(const void* sP, s32 sN, void* dP)
{
	u8* sB = (u8*)sP;
	u8* dB = (u8*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		// sB[3] is alpha
		dB[0] = sB[0];
		dB[1] = sB[1];
		dB[2] = sB[2];

		sB += 4;
		dB += 3;
	}
}

void CBlit::convert_A8R8G8B8toA8R8G8B8( const void* sP, s32 sN, void* dP )
{
	memcpy(dP, sP, sN * 4);
}

void CBlit::convert_A8R8G8B8toA1R5G5B5( const void* sP, s32 sN, void* dP )
{
	u32* sB = (u32*)sP;
	u16* dB = (u16*)dP;

	for (s32 x = 0; x < sN; ++x)
		*dB++ = SColor::A8R8G8B8toA1R5G5B5(*sB++);
}

void CBlit::convert_A8R8G8B8toR5G6B5(const void* sP, s32 sN, void* dP)
{
	u8 * sB = (u8 *)sP;
	u16* dB = (u16*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		s32 r = sB[2] >> 3;
		s32 g = sB[1] >> 2;
		s32 b = sB[0] >> 3;

		dB[0] = (u16)((r << 11) | (g << 5) | (b));

		sB += 4;
		dB += 1;
	}
}

void CBlit::convert_R8G8B8toR8G8B8( const void* sP, s32 sN, void* dP )
{
	memcpy(dP, sP, sN * 3);
}

void CBlit::convert_R8G8B8toA8R8G8B8( const void* sP, s32 sN, void* dP )
{
	u8*  sB = (u8* )sP;
	u32* dB = (u32*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		*dB = 0xff000000 | (sB[0]<<16) | (sB[1]<<8) | sB[2];

		sB += 4;
		++dB;
	}
}

void CBlit::convert_R8G8B8toA1R5G5B5( const void* sP, s32 sN, void* dP )
{
	u8 * sB = (u8 *)sP;
	u16* dB = (u16*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		s32 r = sB[0] >> 3;
		s32 g = sB[1] >> 3;
		s32 b = sB[2] >> 3;

		dB[0] = (u16)((0x8000) | (r << 10) | (g << 5) | (b));

		sB += 3;
		dB += 1;
	}
}

void CBlit::convert_R8G8B8toR5G6B5(const void* sP, s32 sN, void* dP)
{
	u8 * sB = (u8 *)sP;
	u16* dB = (u16*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		s32 r = sB[0] >> 3;
		s32 g = sB[1] >> 2;
		s32 b = sB[2] >> 3;

		dB[0] = (u16)((r << 11) | (g << 5) | (b));

		sB += 3;
		dB += 1;
	}
}

void CBlit::convert_R5G6B5toR5G6B5(const void* sP, s32 sN, void* dP)
{
	memcpy(dP, sP, sN * 2);
}

void CBlit::convert_R5G6B5toR8G8B8(const void* sP, s32 sN, void* dP)
{
	u16* sB = (u16*)sP;
	u8 * dB = (u8 *)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		dB[0] = (u8)((*sB & 0xf800) << 8);
		dB[1] = (u8)((*sB & 0x07e0) << 2);
		dB[2] = (u8)((*sB & 0x001f) << 3);

		sB += 4;
		dB += 3;
	}
}

void CBlit::convert_R5G6B5toB8G8R8(const void* sP, s32 sN, void* dP)
{
	u16* sB = (u16*)sP;
	u8 * dB = (u8 *)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		dB[2] = (u8)((*sB & 0xf800) << 8);
		dB[1] = (u8)((*sB & 0x07e0) << 2);
		dB[0] = (u8)((*sB & 0x001f) << 3);

		sB += 4;
		dB += 3;
	}
}

void CBlit::convert_R5G6B5toA8R8G8B8(const void* sP, s32 sN, void* dP)
{
	u16* sB = (u16*)sP;
	u32* dB = (u32*)dP;

	for (s32 x = 0; x < sN; ++x)
		*dB++ = SColor::R5G6B5toA8R8G8B8(*sB++);
}

void CBlit::convert_R5G6B5toA1R5G5B5(const void* sP, s32 sN, void* dP)
{
	u16* sB = (u16*)sP;
	u16* dB = (u16*)dP;

	for (s32 x = 0; x < sN; ++x)
		*dB++ = SColor::R5G6B5toA1R5G5B5(*sB++);
}

void CBlit::convert_B8G8R8toA8R8G8B8(const void* sP, s32 sN, void* dP)
{
	u8*  sB = (u8* )sP;
	u32* dB = (u32*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		*dB = 0xff000000 | (sB[2]<<16) | (sB[1]<<8) | sB[0];

		sB += 3;
		++dB;
	}
}

void CBlit::convert_B8G8R8A8toA8R8G8B8(const void* sP, s32 sN, void* dP)
{
	u8* sB = (u8*)sP;
	u8* dB = (u8*)dP;

	for (s32 x = 0; x < sN; ++x)
	{
		dB[0] = sB[3];
		dB[1] = sB[2];
		dB[2] = sB[1];
		dB[3] = sB[0];

		sB += 4;
		dB += 4;
	}
}

void CBlit::convert1BitTo16Bit( const u8* in, u16* out, s32 width, s32 height, s32 linepad/*=0*/, bool flip/*=false*/ )
{
	if (!in || !out)
	{
		ASSERT(false);
		return;
	}

	if (flip)
		out += width * height;

	for (s32 y=0; y<height; ++y)
	{
		s32 shift = 7;
		if (flip)
			out -= width;

		for (s32 x=0; x<width; ++x)
		{
			out[x] = *in>>shift & 0x01 ? (u16)0xffff : (u16)0x8000;

			if ((--shift)<0) // 8 pixel done
			{
				shift=7;
				++in;
			}
		}

		if (shift != 7) // width did not fill last byte
			++in;

		if (!flip)
			out += width;
		in += linepad;
	}
}

void CBlit::convert4BitTo16Bit( const u8* in, u16* out, s32 width, s32 height, const s32* palette, s32 linepad/*=0*/, bool flip/*=false*/ )
{
	if (!in || !out || !palette)
	{
		ASSERT(false);
		return;
	}

	if (flip)
		out += width*height;

	for (s32 y=0; y<height; ++y)
	{
		s32 shift = 4;
		if (flip)
			out -= width;

		for (s32 x=0; x<width; ++x)
		{
			out[x] = SColor::X8R8G8B8toA1R5G5B5(palette[(u8)((*in >> shift) & 0xf)]);

			if (shift==0)
			{
				shift = 4;
				++in;
			}
			else
				shift = 0;
		}

		if (shift == 0) // odd width
			++in;

		if (!flip)
			out += width;
		in += linepad;
	}
}

void CBlit::convert8BitTo16Bit( const u8* in, u16* out, s32 width, s32 height, const s32* palette, s32 linepad/*=0*/, bool flip/*=false*/ )
{
	if (!in || !out || !palette)
	{
		ASSERT(false);
		return;
	}

	if (flip)
		out += width * height;

	for (s32 y=0; y<height; ++y)
	{
		if (flip)
			out -= width; // one line back
		for (s32 x=0; x<width; ++x)
		{
			out[x] = SColor::X8R8G8B8toA1R5G5B5(palette[(u8)(*in)]);
			++in;
		}
		if (!flip)
			out += width;
		in += linepad;
	}
}

void CBlit::convert16BitTo16Bit( const u16* in, u16* out, s32 width, s32 height, s32 linepad/*=0*/, bool flip/*=false*/ )
{
	if (!in || !out)
	{
		ASSERT(false);
		return;
	}

	if (flip)
		out += width * height;

	for (s32 y=0; y<height; ++y)
	{
		if (flip)
			out -= width;

		memcpy(out, in, width*sizeof(u16));

		if (!flip)
			out += width;
		in += width;
		in += linepad;
	}
}

void CBlit::convert24BitTo24Bit( const u8* in, u8* out, s32 width, s32 height, s32 linepad/*=0*/, bool flip/*=false*/, bool changeRB /*= false*/ )
{
	if (!in || !out)
	{
		ASSERT(false);
		return;
	}

	const s32 lineWidth = 3 * width;
	if (flip)
		out += lineWidth * height;

	for (s32 y=0; y<height; ++y)
	{
		if (flip)
			out -= lineWidth;

		if (changeRB)
		{
			for (s32 x=0; x<lineWidth; x+=3)
			{
				out[x+0] = in[x+2];
				out[x+1] = in[x+1];
				out[x+2] = in[x+0];
			}
		}
		else
		{
			memcpy(out,in,lineWidth);
		}

		if (!flip)
			out += lineWidth;
		in += lineWidth;
		in += linepad;
	}
}

void CBlit::convert32BitTo32Bit( const s32* in, s32* out, s32 width, s32 height, s32 linepad, bool flip/*=false*/, bool changeRB /*= false*/ )
{
	if (!in || !out)
	{
		ASSERT(false);
		return;
	}

	if (flip)
		out += width * height;

	for (s32 y=0; y<height; ++y)
	{
		if (flip)
			out -= width;

		if (changeRB)
		{
			u8* pOut = (u8*)out;
			u8* pIn = (u8*)in;
			for (s32 x=0; x<4*width; x+=4)
			{
				pOut[x] = pIn[x+2];
				pOut[x+1] = pIn[x+1];
				pOut[x+2] = pIn[x];
				pOut[x+3] = pIn[x+3];
			}
		}
		else
		{
			memcpy(out, in, width*sizeof(s32));
		}

		if (!flip)
			out += width;
		in += width;
		in += linepad;
	}
}


void CBlit::resizeBilinearA8R8G8B8( const void* src, u32 w1, u32 h1, void* target, u32 w2, u32 h2, ECOLOR_FORMAT format )
{
	u32 bpp = getBytesPerPixelFromFormat(format);
	u32* pixels = (u32*)src;
	int w = (int)w1;
	int h = (int)h1;

	u32 a, b, c, d;
	int x, y, index;
	u32 gray, red, blue, green;
	float x_ratio = ((float)(w-1))/w2 ;
	float y_ratio = ((float)(h-1))/h2 ;
	float x_diff, y_diff;
	for (u32 i=0;i<h2;i++) {
		for (u32 j=0;j<w2;j++) {
			x = (int)(x_ratio * j) ;
			y = (int)(y_ratio * i) ;
			x_diff = (x_ratio * j) - x ;
			y_diff = (y_ratio * i) - y ;
			index = y*w+x ;

			// range is 0 to 255 thus bitwise AND with 0xff
			a = pixels[index];
			b = pixels[index+1];
			c = pixels[index+w];
			d = pixels[index+w+1];
			
			gray = (u32) (((a>>24)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>24)&0xff)*(x_diff)*(1-y_diff) +
				((c>>24)&0xff)*(y_diff)*(1-x_diff)   + ((d>>24)&0xff)*(x_diff*y_diff));

			// blue element
			// Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
			blue = (u32) ((a&0xff)*(1-x_diff)*(1-y_diff) + (b&0xff)*(x_diff)*(1-y_diff) +
				(c&0xff)*(y_diff)*(1-x_diff)   + (d&0xff)*(x_diff*y_diff));

			// green element
			// Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
			green = (u32) (((a>>8)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>8)&0xff)*(x_diff)*(1-y_diff) +
				((c>>8)&0xff)*(y_diff)*(1-x_diff)   + ((d>>8)&0xff)*(x_diff*y_diff));

			// red element
			// Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
			red = (u32) (((a>>16)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>16)&0xff)*(x_diff)*(1-y_diff) +
				((c>>16)&0xff)*(y_diff)*(1-x_diff)   + ((d>>16)&0xff)*(x_diff*y_diff));

			SColor color(gray, red, green, blue);
			convert_viaFormat(&color, ECF_A8R8G8B8, 1, (u8*)target + bpp * (i * w2 + j), format);                                
		}
	}
}

void CBlit::resizeBilinearR8G8B8( const void* src, u32 w1, u32 h1, void* target, u32 w2, u32 h2, ECOLOR_FORMAT format )
{
	u32 bpp = getBytesPerPixelFromFormat(format);
	u8* pixels = (u8*)src;
	int w = (int)w1;
	int h = (int)h1;

	u8 a[3], b[3], c[3], d[3];
	int x, y, index;
	u8 red, blue, green;
	float x_ratio = ((float)(w-1))/w2 ;
	float y_ratio = ((float)(h-1))/h2 ;
	float x_diff, y_diff;
	for (u32 i=0;i<h2;i++) {
		for (u32 j=0;j<w2;j++) {
			x = (int)(x_ratio * j) ;
			y = (int)(y_ratio * i) ;
			x_diff = (x_ratio * j) - x ;
			y_diff = (y_ratio * i) - y ;
			index = y*w+x ;

			// range is 0 to 255 thus bitwise AND with 0xff
			a[0] = pixels[index * 3];
			a[1] = pixels[index * 3 + 1];
			a[2] = pixels[index * 3 + 2];
			b[0] = pixels[(index+1) * 3];
			b[1] = pixels[(index+1) * 3 + 1];
			b[2] = pixels[(index+1) * 3 + 2];
			c[0] = pixels[(index+w) * 3];
			c[1] = pixels[(index+w) * 3 + 1];
			c[2] = pixels[(index+w) * 3 + 2];
			d[0] = pixels[(index+w+1) * 3];
			d[1] = pixels[(index+w+1) * 3 + 1];
			d[2] = pixels[(index+w+1) * 3 + 2];

			// blue element
			// Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
			blue = (u8) (a[2]*(1-x_diff)*(1-y_diff) + b[2]*(x_diff)*(1-y_diff) +
				c[2]*(y_diff)*(1-x_diff)  + d[2]*(x_diff*y_diff));

			// green element
			// Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
			green = (u8) (a[1]*(1-x_diff)*(1-y_diff) + b[1]*(x_diff)*(1-y_diff) +
				c[1]*(y_diff)*(1-x_diff)  + d[1]*(x_diff*y_diff));

			// red element
			// Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
			red = (u8) (a[0]*(1-x_diff)*(1-y_diff) + b[0]*(x_diff)*(1-y_diff) +
				c[0]*(y_diff)*(1-x_diff)  + d[0]*(x_diff*y_diff));

			SColor color(red, green, blue);
			convert_viaFormat(&color, ECF_R8G8B8, 1, (u8*)target + bpp * (i * w2 + j), format);                                
		}
	}
}

void CBlit::resizeBicubicA8R8G8B8( const void* src, u32 w1, u32 h1, void* target, u32 w2, u32 h2, ECOLOR_FORMAT format )
{
	u32 bpp = getBytesPerPixelFromFormat(format);
	u32* pixels = (u32*)src;
	int w = (int)w1;
	int h = (int)h1;

	f32 xscale = (f32)w1/(f32)w2;
	f32 yscale = (f32)h1/(f32)h2;

	int index;
	f32 gray, red, blue, green;
	f32 f_x, f_y, a, b, r1, r2;
	int i_x, i_y, xx, yy;

	for (u32 y=0; y<h2; ++y)
	{
		f_y = y * yscale - 0.5f;
		i_y = (int)floor32_(f_y);
		a = f_y - (f32)floor32_(f_y);

		for (u32 x=0; x<w2; ++x)
		{
			f_x = x * xscale - 0.5f;
			i_x = (int)floor32_(f_x);
			b = f_x - (f32)floor32_(f_x);

			//clear
			gray = red = green = blue = 0;

			for (int m=-1; m<3; ++m)
			{
				r1 = KernelBSpline((f32)m - a);
				yy = i_y + m;

				if (yy < 0)
					yy = 0;
				if (yy >= (int)h1)
					yy = (int)h1 -1;
				
				for (int n=-1; n<3; ++n)
				{
					r2 = r1 * KernelBSpline(b - (f32)n);
					xx = i_x + n;
					if (xx < 0)
						xx = 0;
					if (xx >= (int)w1)
						xx = (int)w1 - 1;

					index = yy*w+xx ;

					// range is 0 to 255 thus bitwise AND with 0xff
					SColor c(pixels[index]);

					gray += c.getAlpha() * r2;
					red += c.getRed() * r2;
					green += c.getGreen() * r2;
					blue += c.getBlue() * r2;
				}
			}

			// set to dest
			SColor color((u32)(u8)gray, (u32)(u8)red, (u32)(u8)green, (u32)(u8)blue);
			convert_viaFormat(&color, ECF_A8R8G8B8, 1, (u8*)target + bpp * (y * w2 + x), format); 
		}
	}
}

void CBlit::resizeNearest( const void* src, u32 w1, u32 h1, ECOLOR_FORMAT srcFormat, void* target, u32 w2, u32 h2, ECOLOR_FORMAT targetFormat )
{
	u32 bytesPerPixel = getBytesPerPixelFromFormat(srcFormat);
	const u32 bpp=getBytesPerPixelFromFormat(targetFormat);

	u32 srcPitch = bytesPerPixel * w1;
	u32 targetPitch = bpp * w2;

	const f32 sourceXStep = (f32)w1 / (f32)w2;
	const f32 sourceYStep = (f32)h1 / (f32)h2;
	s32 yval=0, syval=0;
	f32 sy = 0.0f;
	for (u32 y=0; y<h2; ++y)
	{
		f32 sx = 0.0f;
		for (u32 x=0; x<w2; ++x)
		{
			convert_viaFormat((u8*)src+ syval + (s32)sx*bytesPerPixel, srcFormat, 1, ((u8*)target)+ yval + (x*bpp), targetFormat);
			sx+=sourceXStep;
		}
		sy+=sourceYStep;
		syval=(s32)sy*srcPitch;
		yval+=targetPitch;
	}
}

void CBlit::resizeNearest( const void* src, u32 w1, u32 h1, u32 srcPitch, ECOLOR_FORMAT srcFormat, void* target, u32 w2, u32 h2, u32 targetPitch, ECOLOR_FORMAT targetFormat )
{
	u32 bytesPerPixel = getBytesPerPixelFromFormat(srcFormat);
	const u32 bpp=getBytesPerPixelFromFormat(targetFormat);

	const f32 sourceXStep = (f32)w1 / (f32)w2;
	const f32 sourceYStep = (f32)h1 / (f32)h2;
	s32 yval=0, syval=0;
	f32 sy = 0.0f;
	for (u32 y=0; y<h2; ++y)
	{
		f32 sx = 0.0f;
		for (u32 x=0; x<w2; ++x)
		{
			convert_viaFormat((u8*)src+ syval + (s32)sx*bytesPerPixel, srcFormat, 1, ((u8*)target)+ yval + (x*bpp), targetFormat);
			sx+=sourceXStep;
		}
		sy+=sourceYStep;
		syval=(s32)sy*srcPitch;
		yval+=targetPitch;
	}
}

void CBlit::copyA8L8MipMap( const u8* src, u8* tgt, u32 width, u32 height, u32 pitchsrc, u32 pitchtgt )
{
	for (u32 y=0; y<height; ++y)
	{
		for (u32 x=0; x<width; ++x)
		{
			u32 a=0, l = 0;

			for (s32 dy=0; dy<2; ++dy)
			{
				const s32 tgy = (y*2)+dy;
				for (s32 dx=0; dx<2; ++dx)
				{
					const s32 tgx = (x*2)+dx;

					u16 c = *(u16*)(&src[(tgx*2)+(tgy*pitchsrc)]);

					a += (c & 0xff00) >> 8;
					l += (c & 0xff);
				}
			}

			a /= 4;
			l /= 4;

			u16 c = a<<8 | l; 
			*(u16*)(&tgt[(x*2)+(y*pitchtgt)]) = c;
		}
	}
}

void CBlit::copy16BitMipMap( const u8* src, u8* tgt, u32 width, u32 height, u32 pitchsrc, u32 pitchtgt, ECOLOR_FORMAT format )
{
	ASSERT(format == ECF_A1R5G5B5 || format == ECF_R5G6B5);

	for (u32 y=0; y<height; ++y)
	{
		for (u32 x=0; x<width; ++x)
		{
			u32 a=0, r=0, g=0, b=0;

			for (s32 dy=0; dy<2; ++dy)
			{
				const s32 tgy = (y*2)+dy;
				for (s32 dx=0; dx<2; ++dx)
				{
					const s32 tgx = (x*2)+dx;

					SColor c;
					if (format == ECF_A1R5G5B5)
						c = SColor::A1R5G5B5toA8R8G8B8(*(u16*)(&src[(tgx*2)+(tgy*pitchsrc)]));
					else
						c = SColor::R5G6B5toA8R8G8B8(*(u16*)(&src[(tgx*2)+(tgy*pitchsrc)]));

					a += c.getAlpha();
					r += c.getRed();
					g += c.getGreen();
					b += c.getBlue();
				}
			}

			a /= 4;
			r /= 4;
			g /= 4;
			b /= 4;

			u16 c;
			if (format == ECF_A1R5G5B5)
				c = SColor::RGBA16(r,g,b,a);
			else
				c = SColor::A8R8G8B8toR5G6B5(SColor(a,r,g,b).color);
			*(u16*)(&tgt[(x*2)+(y*pitchtgt)]) = c;
		}
	}
}

void CBlit::copy32BitMipMap( const u8* src, u8* tgt, u32 width, u32 height, u32 pitchsrc, u32 pitchtgt, ECOLOR_FORMAT format )
{
	ASSERT(format == ECF_A8R8G8B8);

	for (u32 y=0; y<height; ++y)
	{
		for (u32 x=0; x<width; ++x)
		{
			u32 a=0, r=0, g=0, b=0;
			SColor c;

			for (s32 dy=0; dy<2; ++dy)
			{
				const s32 tgy = (y*2)+dy;
				for (s32 dx=0; dx<2; ++dx)
				{
					const s32 tgx = (x*2)+dx;

					c = *(u32*)(&src[(tgx*4)+(tgy*pitchsrc)]);

					a += c.getAlpha();
					r += c.getRed();
					g += c.getGreen();
					b += c.getBlue();
				}
			}

			a /= 4;
			r /= 4;
			g /= 4;
			b /= 4;

			if (format == ECF_A8R8G8B8)
				c.set(a, r, g, b);
			else
				c.set(a, b, g, r);

			*(u32*)(&tgt[(x*4)+(y*pitchtgt)]) = c.color;
		}
	}
}

void CBlit::copyR8G8B8BitMipMap( const u8* src, u8* tgt, u32 width, u32 height, u32 pitchsrc, u32 pitchtgt )
{
	for (u32 y=0; y<height; ++y)
	{
		for (u32 x=0; x<width; ++x)
		{
			u32 r=0, g=0, b=0;

			for (s32 dy=0; dy<2; ++dy)
			{
				const s32 tgy = (y*2)+dy;
				for (s32 dx=0; dx<2; ++dx)
				{
					const s32 tgx = (x*2)+dx;

					const u8* s = &src[(tgx*3)+(tgy*pitchsrc)];

					r += s[0];
					g += s[1];
					b += s[2];
				}
			}

			r /= 4;
			g /= 4;
			b /= 4;

			u8* t = &tgt[(x*3)+(y*pitchtgt)];
			t[0] = r;
			t[1] = g;
			t[2] = b;
		}
	}
}