#include "hls_define.h"

enum FILETYPE{
	NONE=-1,
	EXE_FILE=0,
	ZIP_FILE=1,
	REG_FILE=2,
	FIFO_FILE=3,
	CHR_FILE=4,
	DIR_FILE=5,
	BLK_FILE=6,
	LNK_FILE=7,
	SOCK_FILE=8,
	WHT_FILE=9
};

char pwd[FILE_BUFF];
DIR* dir=NULL;
struct dirent* ent=NULL;
FILETYPE fileType=NONE;
int color=-1;
void Init()
{
	color=-1;
	fileType=NONE;
	dir=NULL;
	ent=NULL;
	memset(pwd,0,sizeof(pwd));
}

void setColor(int color)
{
	printf("\033[1;%dm",color);
}
void setColorInit()
{
	printf("\033[1;0m");
}
mode_t getFileMode(char* name)
{
	struct stat buf;
	string path=pwd;
	path+="/";
	path+=name;


	int ret=stat(path.c_str(),&buf);
	if(ret!=0)
	{
		perror("stat");
		exit(1);
	}

	return buf.st_mode;

}
bool IsExcute(mode_t md)
{
	if(md&S_IXUSR || md&S_IXGRP || md&S_IXOTH)
	return true;

}
bool IsCompressed(char *name)
{
	if(strstr(name,".tar"))
		return true;
	if(strstr(name,".tgz"))
		return true;
	if(strstr(name,".arc"))
		return true;
	if(strstr(name,".arj"))
		return true;
	if(strstr(name,".taz"))
		return true;
	if(strstr(name,".lha"))
		return true;
	if(strstr(name,".lz4"))
		return true;
	if(strstr(name,".lzh"))
		return true;
	if(strstr(name,".lzma"))
		return true;	
	if(strstr(name,".tlz"))
		return true;
	if(strstr(name,".txz"))
		return true;
	if(strstr(name,".tzo"))
		return true;
	if(strstr(name,".t7z"))
		return true;
	if(strstr(name,".zip"))
		return true;
	if(strstr(name,".z"))
		return true;
	if(strstr(name,".Z"))
		return true;
	if(strstr(name,".dz"))
		return true;
	if(strstr(name,".gz"))
		return true;
	if(strstr(name,".lrz"))
		return true;
	if(strstr(name,".lz"))
		return true;
	if(strstr(name,".lzo"))
		return true;
	if(strstr(name,".xz"))
		return true;
	if(strstr(name,".bz2"))
		return true;
	if(strstr(name,".bz"))
		return true;
	if(strstr(name,".tbz"))
		return true;
	if(strstr(name,".tbz2"))
		return true;
	if(strstr(name,".tz"))
		return true;
	if(strstr(name,".deb"))
		return true;
	if(strstr(name,".rpm"))
		return true;
	if(strstr(name,".jar"))
		return true;
	if(strstr(name,".war"))
		return true;
	if(strstr(name,".ear"))
		return true;
}

bool IsImage(char* name)
{

	if(strstr(name,".jpg"))
		return true;
	if(strstr(name,".jpeg"))
		return true;
	if(strstr(name,".gif"))
		return true;
	if(strstr(name,".bmp"))
		return true;
	if(strstr(name,".pbm"))
		return true;
	if(strstr(name,".pgm"))
		return true;
	if(strstr(name,".ppm"))
		return true;
	if(strstr(name,".tga"))
		return true;
	if(strstr(name,".xbm"))
		return true;
}
void checkFileType(int type,char* name)
{
	//1단계 : 파일타입에 따른 구분
	//2단계 : 일반 파일일 경우 실행파일, 일반파일,압축파일 구분
	
	mode_t md=0;
	switch(type)
	{
		case DT_UNKNOWN:

			if(strstr(name,".git"))
			{
				//깃디렉터리예외처리
				fileType=DIR_FILE;
				color=34;
				break;
			}
			//일반파일잠시보류
			md=getFileMode(name);
			if(IsExcute(md))
				color=32;
			else if(IsCompressed(name))
				color=31;
			else if(IsImage(name))
				color=35;
			else
				color=0;
			break;
		case DT_FIFO:
			fileType=FIFO_FILE;
			color=33;//초록
			break;
		case DT_DIR:
			fileType=DIR_FILE;
			color=34;//파랑
			break;
		case DT_BLK:
			fileType=BLK_FILE;
			color=33;//노랑
			break;
		case DT_LNK:
			fileType=LNK_FILE;
			color=36;//연두
			break;
		case DT_WHT:
			fileType=WHT_FILE;
			break;
		case DT_SOCK:
			fileType=SOCK_FILE;
			color=35;//빨강
			break;
		case DT_CHR:
			fileType=CHR_FILE;
			color=33;//노랑
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
		

		setColor(color);
		printf("%s\n",ent->d_name);
		setColorInit();
	}

	//디렉터리스트림해제
	closedir(dir);
	return 0;

}

