#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 128

bool checkLastInfo(char * hash, char * branch, char * time)
{
   	FILE * iFile;
    char buf_hash[128];
    char buf_branch[128];
    char buf_time[128];

	iFile = fopen ("gitinfo.cash","r");    // Открываем файл кэша
	if (iFile)
    {
        fgets(buf_hash, 128, iFile);
        fgets(buf_branch, 128, iFile);
        fgets(buf_time, 128, iFile);
        fclose(iFile);

        // remove for LF, CR, CRLF, LFCR, ...
        buf_hash[strcspn(buf_hash, "\r\n")] = 0;
        buf_branch[strcspn(buf_branch, "\r\n")] = 0;
        buf_time[strcspn(buf_time, "\r\n")] = 0;

        if (strcmp(hash, buf_hash) == 0 &&
            strcmp(branch, buf_branch) == 0 &&
            strcmp(time, buf_time) == 0)
        {
            return true;   // Данные не изменились
        }
    }

    iFile = fopen ("gitinfo.cash","w");    // Открываем файл кэша на запись
	fputs(hash, iFile);
	fputc('\n', iFile);
	fputs(branch, iFile);
	fputc('\n', iFile);
	fputs(time, iFile);
	fputc('\n', iFile);
	fclose(iFile);
	return false;        // данные обновлены
}

void execGit(const char * cmd, char * buf)
{
    FILE *fp;

     if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        exit(-1);
    }

    fgets(buf, BUFSIZE, fp);

    if (pclose(fp))
    {
        printf("Command not found or exited with error status\n");
        exit(-1);
    }

    buf[strcspn(buf, "\r\n")] = 0; // works for LF, CR, CRLF, LFCR, ...
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
		printf("GitInfo v2.1\n");
		printf("Use: %s outfilename\n", argv[0]);
		return 1;
	}

    char *cmd1 = "git rev-parse --short HEAD 2>NUL";        // Хэш последнего коммита
    char *cmd2 = "git rev-parse --abbrev-ref HEAD 2>NUL";   // Текущая ветка
    char *cmd3 = "git log -1 --format=%ci 2>NUL";           // Дата последнего коммита

    char buf_hash[BUFSIZE];
    char buf_branch[BUFSIZE];
    char buf_time[BUFSIZE];

    execGit(cmd1, buf_hash);

    execGit(cmd2, buf_branch);

    execGit(cmd3, buf_time);

    if (checkLastInfo(buf_hash, buf_branch, buf_time))
    {
        printf(" No change\n");
        return 0;
    }

    FILE * oFile;

    oFile = fopen(argv[1], "wb");
	if (oFile == NULL) {
		printf("Can't open file for writing\n");
		return 5;
	}

    fprintf(oFile, "#ifndef __GIT_INFO_H__\n");
    fprintf(oFile, "#define __GIT_INFO_H__\n");
    fprintf(oFile, "\n");
    fprintf(oFile, "#ifdef __cplusplus\n");
    fprintf(oFile, "extern \"C\" {\n");
    fprintf(oFile, "#endif\n");
    fprintf(oFile, "\n");
    fprintf(oFile, "    // Git info\n");
    fprintf(oFile, "    #define GIT_HASH %s\n", buf_hash);
    fprintf(oFile, "    #define GIT_BRANCH %s\n", buf_branch);
    fprintf(oFile, "    #define GIT_COMMIT_TIME \"%s\"\n", buf_time);
    fprintf(oFile, "\n");
    fprintf(oFile, "    // Git info in one line\n");
    fprintf(oFile, "    #define GIT_INFO_LINE \"%s %s %s\"\n", buf_hash, buf_branch, buf_time);
    fprintf(oFile, "\n");
    fprintf(oFile, "#ifdef __cplusplus\n");
    fprintf(oFile, "}\n");
    fprintf(oFile, "#endif\n");
    fprintf(oFile, "\n");
    fprintf(oFile, "#endif\n");
    fclose(oFile);
    printf(" %s %s %s\n", buf_hash, buf_branch, buf_time);
    return 0;
}
