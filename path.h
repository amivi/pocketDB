//
// Created by Ajay on 12-12-2017.
//

#ifndef POCKETDB_PATH_H
#define POCKETDB_PATH_H

#if defined( __linux__ )
    #define POCKETFILES_PATH "../../POCKETFILES/"
    #define POCKETFILES_PATH_BACKUP "../../POCKETFILES_BACKUP/"
#elif defined( __APPLE__ )
    #define POCKETFILES_PATH "../../POCKETFILES/"
    #define POCKETFILES_PATH_BACKUP "../../POCKETFILES_BACKUP/"
#elif defined( _WIN64 ) || defined(_WINDOWS_)
    #define POCKETFILES_PATH "..\\..\\POCKETFILES\\"
    #define POCKETFILES_PATH_BACKUP "..\\..\\POCKETFILES_BACKUP\\"
#else
    #define POCKETFILES_PATH "..\\..\\POCKETFILES\\"
    #define POCKETFILES_PATH_BACKUP "..\\..\\POCKETFILES_BACKUP\\"
#endif

#endif //POCKETDB_PATH_H
