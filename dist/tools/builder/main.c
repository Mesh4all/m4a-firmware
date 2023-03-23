// /*
//  * Copyright (c) 2022 Mesh4all <mesh4all.org>
//  *
//  * Licensed under the Apache License, Version 2.0 (the "License");
//  * you may not use this file except in compliance with the License.
//  * You may obtain a copy of the License at
//  *
//  *     http://www.apache.org/licenses/LICENSE-2.0
//  *
//  * Unless required by applicable law or agreed to in writing, software
//  * distributed under the License is distributed on an "AS IS" BASIS,
//  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  * See the License for the specific language governing permissions and
//  * limitations under the License.
//  */

// /**
//  * @brief       udp client test
//  *
//  * @author      xkevin190 <kevinvelasco193@gmail.com>
//  */

// #include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
// #include <pthread.h>

// // Let us create a global variable to change it in threads
// int g = 0;

// // The function to be executed by all threads
// void *myThreadFun(void *vargp)
// {
// 	// Store the value argument passed to this thread
// 	int *myid = (int *)vargp;

// 	// Let us create a static variable to observe its changes
// 	static int s = 0;
// 	// Change static and global variables
// 	++s; ++g;

// 	// Print the argument, static and global variables
// 	printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g);
// }

// int main()
// {
// 	int i;
// 	pthread_t tid;

// 	// Let us create three threads
// 	for (i = 0; i < 3; i++)
// 		pthread_create(&tid, NULL, myThreadFun, (void *)&i);

// 	pthread_exit(NULL);
// 	return 0;
// }

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>

int main(void) {
    struct dirent *de; // Pointer for directory entry
    char *titles[2] = {"examples", "tests"};
    char named[100][50];
    char b_dirs[2][20] = {{"../../../examples/"}, {"../../../tests/"}};

    for (uint8_t i = 0; i < sizeof(b_dirs) / sizeof(b_dirs[0]); i++) {

        uint16_t num_elem = 0;
        // opendir() returns a pointer of DIR type.
        DIR *dr = opendir(b_dirs[i]);

        if (dr == NULL) // opendir returns NULL if couldn't open directory
        {
            printf("Could not open current directory");
            return 0;
        }

        // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
        // for readdir()
        printf("\n%s\n\n", titles[i]);
        while ((de = readdir(dr)) != NULL) {
            if (!strstr(de->d_name, ".")) {
                strcpy(named[num_elem], de->d_name);
                printf("%s\n", de->d_name);
                num_elem++;
            }
        }
        // dr = opendir(b_dirs[i]);
        // while ((de = readdir(dr)) != NULL) {
        //     if (!strstr(de->d_name, ".")) {
        //         // strcpy(named[num_elem], de->d_name);
        //         printf("%s\n", de->d_name);
        //         num_elem++;
        //     }
        // }
        for (uint16_t j = 0; j < num_elem; j++) {
            char cmd[70] ={"make -C "};
            strcat(cmd, b_dirs[i]);
            strcat(cmd, named[j]);
            strcat(cmd, "> /dev/null 2>&1");
            if (system(cmd)) {
                printf("\033[0;31m Error -> folder:%s/%s\n",titles[i], named[j]);
                printf("\033[0;0m");
            }
        }
        closedir(dr);
    }

    return 0;
}
