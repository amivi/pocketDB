//
// Created by Ajay on 12-12-2017.
//

#ifndef POCKETDB_PATH_H
#define POCKETDB_PATH_H

#if defined( __linux__ )
    #define POCKETFILES_PATH "../../POCKETFILES/"
    #define POCKETFILES_PATH_BACKUP "../../POCKETFILES_BACKUP/"
    #define MIN_DIR_LEN 5
    #define START_INDEX 3
#elif defined( __APPLE__ )
    #define POCKETFILES_PATH "../../POCKETFILES/"
    #define POCKETFILES_PATH_BACKUP "../../POCKETFILES_BACKUP/"
    #define MIN_DIR_LEN 5
    #define START_INDEX 3
#elif defined( _WIN64 ) || defined(_WINDOWS_)
    #define POCKETFILES_PATH "..\\..\\POCKETFILES\\"
    #define POCKETFILES_PATH_BACKUP "..\\..\\POCKETFILES_BACKUP\\"
    #define MIN_DIR_LEN 4
    #define START_INDEX 2
#else
    #define POCKETFILES_PATH "..\\..\\POCKETFILES\\"
    #define POCKETFILES_PATH_BACKUP "..\\..\\POCKETFILES_BACKUP\\"
    #define MIN_DIR_LEN 4
    #define START_INDEX 2
#endif

#endif //POCKETDB_PATH_H
