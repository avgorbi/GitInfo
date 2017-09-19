# GitInfo
Add git information into your projects 

Программа генерирует информацию о git репозитории.

Результат записывается в заголовочный файл который можно включать с Си/С++ проект.
Имя выходного файла задается параметром outfilename.

Пример создаваемого файла:

#ifndef __GIT_INFO_H__
#define __GIT_INFO_H__

#ifdef __cplusplus
extern "C" {
#endif

    // Git info
    #define GIT_HASH 8302421
    #define GIT_BRANCH VTOL
    #define GIT_COMMIT_TIME "2017-09-19 21:05:10 +0300"

    // Git info in one line
    #define GIT_INFO_LINE "8302421 VTOL 2017-09-19 21:05:10 +0300"

#ifdef __cplusplus
}
#endif

#endif

Исполняемый файл запускать из директории репозитория настроив правило PREBUILD вашего проекта.
Для работы программы требуется проинсталированный git.

Компилируется компилятором GCC из под оболочки Codeblocks. Файлы проекта прилогаются.

