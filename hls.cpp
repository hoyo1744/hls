#include "hls_define.h"

char pwd[FILE_BUFF];
DIR* dir=NULL;
struct dirent* ent=NULL;
FILETYPE fileType=NONE;
enum FILETYPE
{
	NONE=-1;
	EXE_FILE=0;
	ZIP_FILE=1;
	REG_FILE=2;
	FIFO_FILE=3;
	DIR_FILE=4;
	BLK_FILE=5;
	LNK_FILE=6;
	SOCK_FILE=7;
	WHT_FILE=8;
};

void Init()
{
	fileType=NONE;
	dir=NULL;
	ent=NULL;
	memset(pwd,0,sizeof(pwd));
}
void checkFileType(int type,char* name)
{
	//1단계 : 파일타입에 따른 구분
	//2단계 : 일반 파일일 경우 실행파일, 일반파일,압축파일 구분
	
	switch(type)
	{
		case DT_UNKNOWN:
			
			break;
		case DT_FIFO:
			fileType=FIFO_FILE
			break;
		case DT_DIR:
			fileType=DIR_FILE;
			break;
		case DT_BLK:
			fileType=BLK_FILE;
			break;
		case DT_LNK:
			fileType=LNK_FILE;
			break;
		case DT_WHT:
			fileType=WHT_FILE;
			break;


	}

	

}
int main(void)
{
	//변수 초기화
	Init();	
	
	//현재 파일디렉터리의 위치
	getcwd(pwd,sizeof(pwd));
	
	//getcwd 에러처리
	if(strlen(pwd)==0)
	{
		perror("getcwd");
		exit(1);
	}

	//디렉터리스트림 생성
	dir=opendir(pwd);

	if(dir==NULL)
	{
		perror("opendir");
		exit(1);
	}


	while(1)
	{
		errno=0;
		ent=readdir(dir);
		if(ent==NULL)
		{
			if(errno=='\0')
			{
				//스트림값을 모두 읽은 경우
				break;	
			}
			else
			{
				//readir이 error인 경우
				perror("readdir");
				exit(1);
			}
		}
		checkFileType(ent->d_type,ent->d_name);
		printf("%s\n",ent->d_name);

	}

	//디렉터리스트림해제
	closedir(dir);
	return 0;

}

