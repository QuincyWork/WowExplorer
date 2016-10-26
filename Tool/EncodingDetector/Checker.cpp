#include "Checker.h"
#include "mywow.h"

extern IFileSystem* g_fs;

extern char g_leftDir[QMAX_PATH];
extern char g_rightDir[QMAX_PATH];

extern bool g_SkipMissingFile;

void checkEncodingScript()
{
	string512 leftdir = g_leftDir;
	leftdir.normalizeDir();
	leftdir.append("script");

	string512 rightDir = g_rightDir;
	rightDir.normalizeDir();
	rightDir.append("script");

	Q_iterateFiles(leftdir.c_str(), "*.*", funcCheckEncodingScript, NULL);

	printf("Script����Ƚ����!\n");
	printf("\n");
}

void funcCheckEncodingScript( const c8* filename, void* args )
{
	c8 ext[10];
	getFileExtensionA(filename, ext, 10);
	if (Q_stricmp(ext, "lua") != 0)
		return;

	c8 rightfilename[QMAX_PATH];
	const c8* c = strstr(filename, g_leftDir);
	_ASSERT(c);
	c += strlen(g_leftDir);
	Q_strcpy(rightfilename, QMAX_PATH, g_rightDir);
	Q_strcat(rightfilename, QMAX_PATH, c);

	E_UniMode leftMode = (E_UniMode)-1;
	E_UniMode rightMode = (E_UniMode)-1;

	{
		IReadFile* rfile = g_fs->createAndOpenFile(filename, false);

		u8* buffer = (u8*)Z_AllocateTempMemory(rfile->getSize());

		u32 read = rfile->read(buffer, rfile->getSize());
		_ASSERT(read <= rfile->getSize());

		leftMode = determineEncoding(buffer, rfile->getSize());

		Z_FreeTempMemory(buffer);

		delete rfile;
	}

	{
		IReadFile* rfile = g_fs->createAndOpenFile(rightfilename, false);

		if (!rfile)
		{
			if (!g_SkipMissingFile)
			{
				printf("�ļ��޷��� %s\n", rightfilename);
				getchar();
			}
			return;
		}

		u8* buffer = (u8*)Z_AllocateTempMemory(rfile->getSize());

		u32 read = rfile->read(buffer, rfile->getSize());
		_ASSERT(read <= rfile->getSize());

		rightMode = determineEncoding(buffer, rfile->getSize());

		Z_FreeTempMemory(buffer);

		delete rfile;
	}

	if (leftMode != rightMode)
	{
		printf("�ļ������ʽ��ͬ��ת�����ļ� %s\n", rightfilename);
		getchar();
	}
	else
	{
		printf("���ɹ� %s\n", rightfilename);
	}
}

void checkEncodingInterface()
{
	string512 leftdir = g_leftDir;
	leftdir.normalizeDir();
	leftdir.append("interfaces");

	string512 rightDir = g_rightDir;
	rightDir.normalizeDir();
	rightDir.append("interfaces");

	Q_iterateFiles(leftdir.c_str(), "*.*", funcCheckEncodingInterface, NULL);

	printf("Interfaces����Ƚ����!\n");
	printf("\n");
}

void funcCheckEncodingInterface( const c8* filename, void* args )
{
	c8 ext[10];
	getFileExtensionA(filename, ext, 10);
	if (Q_stricmp(ext, "xml") != 0)
		return;

	c8 rightfilename[QMAX_PATH];
	const c8* c = strstr(filename, g_leftDir);
	_ASSERT(c);
	c += strlen(g_leftDir);
	Q_strcpy(rightfilename, QMAX_PATH, g_rightDir);
	Q_strcat(rightfilename, QMAX_PATH, c);

	E_UniMode leftMode = (E_UniMode)-1;
	E_UniMode rightMode = (E_UniMode)-1;

	{
		IReadFile* rfile = g_fs->createAndOpenFile(filename, false);

		u8* buffer = (u8*)Z_AllocateTempMemory(rfile->getSize());

		u32 read = rfile->read(buffer, rfile->getSize());
		_ASSERT(read <= rfile->getSize());

		leftMode = determineEncoding(buffer, rfile->getSize());

		Z_FreeTempMemory(buffer);

		delete rfile;
	}

	{
		IReadFile* rfile = g_fs->createAndOpenFile(rightfilename, false);

		if (!rfile)
		{
			if (!g_SkipMissingFile)
			{
				printf("�ļ��޷��� %s\n", rightfilename);
				getchar();
			}
			return;
		}

		u8* buffer = (u8*)Z_AllocateTempMemory(rfile->getSize());

		u32 read = rfile->read(buffer, rfile->getSize());
		_ASSERT(read <= rfile->getSize());

		rightMode = determineEncoding(buffer, rfile->getSize());

		Z_FreeTempMemory(buffer);

		delete rfile;
	}

	if (leftMode != rightMode)
	{
		printf("�ļ������ʽ��ͬ��ת�����ļ� %s\n", rightfilename);
		getchar();
	}
	else
	{
		printf("���ɹ� %s\n", rightfilename);
	}
}

void checkEncodingSurfaces()
{
	string512 leftdir = g_leftDir;
	leftdir.normalizeDir();
	leftdir.append("surfaces");

	string512 rightDir = g_rightDir;
	rightDir.normalizeDir();
	rightDir.append("surfaces");

	Q_iterateFiles(leftdir.c_str(), "*.*", funcCheckEncodingSurfaces, NULL);

	printf("Surfaces����Ƚ����!\n");
	printf("\n");
}

void funcCheckEncodingSurfaces( const c8* filename, void* args )
{
	c8 ext[10];
	getFileExtensionA(filename, ext, 10);
	if (Q_stricmp(ext, "txt") != 0)
		return;

	c8 rightfilename[QMAX_PATH];
	const c8* c = strstr(filename, g_leftDir);
	_ASSERT(c);
	c += strlen(g_leftDir);
	Q_strcpy(rightfilename, QMAX_PATH, g_rightDir);
	Q_strcat(rightfilename, QMAX_PATH, c);

	E_UniMode leftMode = (E_UniMode)-1;
	E_UniMode rightMode = (E_UniMode)-1;

	{
		IReadFile* rfile = g_fs->createAndOpenFile(filename, false);

		u8* buffer = (u8*)Z_AllocateTempMemory(rfile->getSize());

		u32 read = rfile->read(buffer, rfile->getSize());
		_ASSERT(read <= rfile->getSize());

		leftMode = determineEncoding(buffer, rfile->getSize());

		Z_FreeTempMemory(buffer);

		delete rfile;
	}

	{
		IReadFile* rfile = g_fs->createAndOpenFile(rightfilename, false);

		if (!rfile)
		{
			if (!g_SkipMissingFile)
			{
				printf("�ļ��޷��� %s\n", rightfilename);
				getchar();
			}
			return;
		}

		u8* buffer = (u8*)Z_AllocateTempMemory(rfile->getSize());

		u32 read = rfile->read(buffer, rfile->getSize());
		_ASSERT(read <= rfile->getSize());

		rightMode = determineEncoding(buffer, rfile->getSize());

		Z_FreeTempMemory(buffer);

		delete rfile;
	}

	if (leftMode != rightMode)
	{
		printf("�ļ������ʽ��ͬ��ת�����ļ� %s\n", rightfilename);
		getchar();
	}
	else
	{
		printf("���ɹ� %s\n", rightfilename);
	}
}

void checkImageFileSize()
{
	string512 leftdir = g_leftDir;
	leftdir.normalizeDir();

	string512 rightDir = g_rightDir;
	rightDir.normalizeDir();

	Q_iterateFiles(leftdir.c_str(), "*.*", funcCheckImageSize, NULL);

	printf("ͼƬ��С�Ƚ����!\n");
	printf("\n");
}

void funcCheckImageSize( const c8* filename, void* args )
{
	c8 ext[10];
	getFileExtensionA(filename, ext, 10);
	if (Q_stricmp(ext, "ktx") != 0)
		return;

	c8 rightfilename[QMAX_PATH];
	const c8* c = strstr(filename, g_leftDir);
	_ASSERT(c);
	c += strlen(g_leftDir);
	Q_strcpy(rightfilename, QMAX_PATH, g_rightDir);
	Q_strcat(rightfilename, QMAX_PATH, c);

	u32 leftSize = 0;
	u32 rightSize = 0;

	{
		IReadFile* rfile = g_fs->createAndOpenFile(filename, true);

		leftSize = rfile->getSize();

		delete rfile;
	}

	{
		IReadFile* rfile = g_fs->createAndOpenFile(rightfilename, true);

		if (!rfile)
		{
			if (!g_SkipMissingFile)
			{
				printf("�ļ��޷��� %s\n", rightfilename);
				getchar();
			}
			return;
		}

		rightSize = rfile->getSize();

		delete rfile;
	}

	if (leftSize != rightSize)
	{
		printf("�ļ���С��ͬ��ת�����ļ� %s\n", rightfilename);
		printf("����С: %u, �Ҳ��С: %u\n", leftSize, rightSize);
		getchar();
	}
	else
	{
		printf("���ɹ� %s\n", rightfilename);
	}
}

void checkScript()
{
	string512 leftdir = g_leftDir;
	leftdir.normalizeDir();
	leftdir.append("script");

	string512 rightDir = g_rightDir;
	rightDir.normalizeDir();
	rightDir.append("script");

	Q_iterateFiles(leftdir.c_str(), "*.*", funcCheckScript, NULL);

	printf("Script���������!\n");
	printf("\n");
}

void funcCheckScript( const c8* filename, void* args )
{
	c8 ext[10];
	getFileExtensionA(filename, ext, 10);
	if (Q_stricmp(ext, "lua") != 0)
		return;

	c8 rightfilename[QMAX_PATH];
	const c8* c = strstr(filename, g_leftDir);
	_ASSERT(c);
	c += strlen(g_leftDir);
	Q_strcpy(rightfilename, QMAX_PATH, g_rightDir);
	Q_strcat(rightfilename, QMAX_PATH, c);

	{
		c8 buffer[4096];

		IReadFile* rfile = g_fs->createAndOpenFile(rightfilename, false);

		printf("file: %s\n", rightfilename);

		int line = 1;
		while(!rfile->isEof())
		{
			memset(buffer, 0, sizeof(buffer));

			u32 len = rfile->readLine(buffer, 4096);
			_ASSERT(len < 4095);

			//ecm
			if (strstr(buffer, ".ecm\""))
			{
				c16 utf16[4096];
				utf8to16(buffer, utf16, 4096);
				u32 len = utf16len(utf16);
				for (u32 i=0; i<len; ++i)
				{
					c16 c = utf16[i];
					if (c == L'-' &&
						(i+1<len) && utf16[i+1] == c)
					{
						break;
					}

					if (((c & 0xff00) != 0) &&
						(c < 0x4E00 || c > 0x9FbF))
					{
						printf("����ķ���! �ļ� %s �� %d ��\n", filename, line);
						getchar();
						break;
					}
				}
			}

			if (strstr(buffer, ".gfx\""))
			{
				c16 utf16[4096];
				utf8to16(buffer, utf16, 4096);
				u32 len = utf16len(utf16);
				for (u32 i=0; i<len; ++i)
				{
					c16 c = utf16[i];

					if (c == L'-' &&
						(i+1<len) && utf16[i+1] == c)
					{
						break;
					}

					if (((c & 0xff00) != 0) &&
						(c < 0x4E00 || c > 0x9FbF))
					{
						printf("����ķ���! �ļ� %s �� %d ��\n", filename, line);
						getchar();
						break;
					}
				}
			}

			if (strstr(buffer, ".ski\""))
			{
				c16 utf16[4096];
				utf8to16(buffer, utf16, 4096);
				u32 len = utf16len(utf16);
				for (u32 i=0; i<len; ++i)
				{
					c16 c = utf16[i];
					
					if (c == L'-' &&
						(i+1<len) && utf16[i+1] == c)
					{
						break;
					}

					if (((c & 0xff00) != 0) &&
						(c < 0x4E00 || c > 0x9FbF))
					{
						printf("����ķ���! �ļ� %s �� %d ��\n", filename, line);
						getchar();
						break;
					}
				}
			}

			++line;
		}

		delete rfile;
	}
}

