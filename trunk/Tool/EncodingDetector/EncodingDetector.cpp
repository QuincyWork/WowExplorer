#include "Checker.h"
#include "mywow.h"
#include "CFileSystem.h"

#pragma comment(lib, "mywow.lib")

IFileSystem* g_fs = NULL;

char g_leftDir[QMAX_PATH];
char g_rightDir[QMAX_PATH];

bool g_SkipMissingFile = false;


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("��������ȷ��ʹ�ø�ʽ: EncodingDetector.exe <������Դ��Ŀ¼> <�������Դ��Ŀ¼>\n");
		getchar();
		return 0;
	}

	Q_strcpy(g_leftDir, QMAX_PATH, argv[1]);
	normalizeDirName(g_leftDir);
	printf("������Դ��Ŀ¼: %s\n", g_leftDir);

	Q_strcpy(g_rightDir, QMAX_PATH, argv[2]);
	normalizeDirName(g_rightDir);
	printf("�������Դ��Ŀ¼: %s\n", g_rightDir);

	if (argc > 3)
	{
		const c8* param = argv[3];
		if (Q_stricmp(param, "/k") == 0)
		{
			g_SkipMissingFile = true;
		}
	}

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	initGlobal();

	QMem_Init(0, 0, 40);

	g_fs = new CFileSystem("", "", false);

	//checkEncodingScript();

	//checkEncodingInterface();

	//checkEncodingSurfaces();

	checkImageFileSize();

	//checkScript();

	delete g_fs;

	QMem_End();

	deleteGlobal();

	getchar();
	return 0;
}
