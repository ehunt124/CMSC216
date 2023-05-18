/* Name: Ethan Hunt  UID: 117171051  DirID: ehunt124 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo_album.h"

Photo *create_photo(int id, const char *description) {
    Photo *return_p;
    int length;
    return_p = malloc(sizeof(Photo));
    if(return_p == NULL) {
        return NULL;
    }
    if(description != NULL) {
        length = strlen(description);
        return_p->description = malloc(length + 1);
        if(return_p->description == NULL) {
            return NULL;
        }
        strcpy(return_p->description, description);
    } else {
        return_p->description = NULL;
    }
    return_p->id = id;
    return return_p;
}

void print_photo(Photo *photo) {
    if(photo != NULL) {
        if(photo->description == NULL) {
            printf("Photo Id: %d, Description: None\n", photo->id);
        } else {
            printf("Photo Id: %d, Description: %s\n", photo->id, 
                photo->description);
        }
    }
}

void destroy_photo(Photo *photo) {
    if(photo != NULL) {
        free(photo->description);
        free(photo);
    }
}

void initialize_album(Album *album) {
    if(album != NULL) {
        album->size = 0;
    }
}

void print_album(const Album *album) {
    int i;
    if(album != NULL) {
        for(i = 0; i < album->size; i++) {
            print_photo(album->all_photos[i]);
        }
        if(i == 0) {
            printf("Album has no photos.\n");
        }
    }
}

void destroy_album(Album *album) {
    int i;
    if(album != NULL) {
        for(i = 0; i < album->size; i++) {
            destroy_photo(album->all_photos[i]);
        }
        album->size = 0;
    }
}

void add_photo_to_album(Album *album, int id, const char *description) {
    if(album != NULL) {
        if(album->size < MAX_ALBUM_SIZE) {
            album->all_photos[album->size] = create_photo(id, description);
            album->size++;
        }
    }
}
