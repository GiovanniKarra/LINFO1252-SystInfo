#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>

#include "lib_tar.h"

bool allzeros(tar_header_t *header) {
    char *ptr = (char*)header;
    for (int i = 0; i < sizeof(tar_header_t)*2; i++)
        if (ptr[i] != 0) return false;

    return true;
}

/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd) {
    int err = 0;
    int count = 0; // nombre de headers non-nuls

    struct stat *stats = (struct stat*)malloc(sizeof(struct stat));
    if (stats == NULL) { err = -4; goto stat_error; }

    fstat(tar_fd, stats);
    
    tar_header_t *header = (tar_header_t*)malloc(stats->st_size);
    if (header == NULL) { err = -4; goto header_error; }

    while (lseek(tar_fd, 0, SEEK_CUR) < stats->st_size) {
        err = read(tar_fd, header, sizeof(tar_header_t));
        if (err == -1) { err = -5; goto error; }

        if (allzeros(header)) break;
        
        // vérifie que la magic value vaut ustar\0
        int cmp = strcmp(header->magic, TMAGIC);
        if (cmp != 0) { err = -1; goto error; }

        // vérifie char par char car strcmp prend en compte les \0, or ici il ne
        // devrait pas y en avoir
        if (header->version[0] != TVERSION[0] || header->version[1] != TVERSION[1])
            { err = -2; goto error; }

        // calcul du checksum en additionnant tous les bytes
        unsigned long chksum = 0;
        for (int j = 0; j < 148; j++) chksum += *(((char*)header)+j);
        for (int j = 156; j < 512; j++) chksum += *(((char*)header)+j);
        chksum += 256; // les espaces du champ chksum vide
        if (chksum != TAR_INT(header->chksum)) { err = -3; goto error; }
        
        // printf("%s : %c\n", header->name, header->typeflag);
        int offset = TAR_INT(header->size);
        if (offset != 0) offset += 512-(offset%512); // car blocs de 512 bytes
        lseek(tar_fd, offset, SEEK_CUR);
        count++;
    }

    err = count; // on veut return le nombre de headers
    lseek(tar_fd, 0, SEEK_SET);

    error:
        free(header);
    header_error:
        free(stats);
    stat_error:
        if (err == -4) { perror("malloc error\n");    exit(1); }
        if (err == -5) { perror("file read error\n"); exit(1); }

    return err;
}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {
    int err = 0;
    int toret = 0;

    struct stat *stats = (struct stat*)malloc(sizeof(struct stat));
    if (stats == NULL) { err = -1; goto stat_error; }

    fstat(tar_fd, stats);
    
    tar_header_t *header = (tar_header_t*)malloc(stats->st_size);
    if (header == NULL) { err = -1; goto header_error; }

    while (lseek(tar_fd, 0, SEEK_CUR) < stats->st_size) {
        err = read(tar_fd, header, sizeof(tar_header_t));
        if (err == -1) { err = -1; break; }
        if (allzeros(header)) break;
        
        if (strcmp(path, header->name) == 0) {
            toret = lseek(tar_fd, 0, SEEK_CUR);
            break;
        }

        int offset = TAR_INT(header->size);
        if (offset != 0) offset += 512-(offset%512); // car blocs de 512 bytes
        lseek(tar_fd, offset, SEEK_CUR);
    }

    lseek(tar_fd, 0, SEEK_SET);

    stat_error:
        free(stats);
    header_error:
        free(header);
    if (err == -1) { perror("ERROR\n"); exit(1); }

    return toret;
}

// returns 0 if ok, -1 if error, 1 if doesn't exist
int get_header(int tar_fd, char *path, tar_header_t *header, bool seek_back) {
    int offset = exists(tar_fd, path);
    if (offset == 0) return 1;

    lseek(tar_fd, offset-sizeof(tar_header_t), SEEK_SET);
    
    int err = read(tar_fd, header, sizeof(tar_header_t));
    if (err == -1) return -1;

    if (seek_back) lseek(tar_fd, 0, SEEK_SET);
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {
    int toret = false;

    tar_header_t *header = (tar_header_t*)malloc(sizeof(tar_header_t));
    if (header == NULL) { perror("malloc error\n"); exit(1); }

    int err = get_header(tar_fd, path, header, true);
    if (err == -1) { free(header); perror("read error\n"); exit(1); }
    if (err == 1) return 0;

    if (header->typeflag == DIRTYPE) toret = true;

    free(header);

    return toret;
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
    int toret = false;

    tar_header_t *header = (tar_header_t*)malloc(sizeof(tar_header_t));
    if (header == NULL) { perror("malloc error\n"); exit(1); }

    int err = get_header(tar_fd, path, header, true);
    if (err == -1) { free(header); perror("read error\n"); exit(1); }
    if (err == 1) return 0;

    if (header->typeflag == REGTYPE) toret = true;

    free(header);

    return toret;
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    int toret = false;

    tar_header_t *header = (tar_header_t*)malloc(sizeof(tar_header_t));
    if (header == NULL) { perror("malloc error\n"); exit(1); }

    int err = get_header(tar_fd, path, header, true);
    if (err == -1) { free(header); perror("read error\n"); exit(1); }
    if (err == 1) return 0;

    if (header->typeflag == LNKTYPE || header->typeflag == SYMTYPE) toret = true;

    free(header);

    return toret;
}


/**
 * Lists the entries at a given path in the archive.
 * list() does not recurse into the directories listed at the given path.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         any other value otherwise.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {

    return 0;
}

/**
 * Reads a file at a given path in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive to read from.  If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param offset An offset in the file from which to start reading from, zero indicates the start of the file.
 * @param dest A destination buffer to read the given file into.
 * @param len An in-out argument.
 *            The caller set it to the size of dest.
 *            The callee set it to the number of bytes written to dest.
 *
 * @return -1 if no entry at the given path exists in the archive or the entry is not a file,
 *         -2 if the offset is outside the file total length,
 *         zero if the file was read in its entirety into the destination buffer,
 *         a positive value if the file was partially read, representing the remaining bytes left to be read to reach
 *         the end of the file.
 *
 */
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {
    bool sym = is_symlink(tar_fd, path);
    if (!is_file(tar_fd, path) && !sym) return -1;
    
    tar_header_t *header = (tar_header_t*)malloc(sizeof(tar_header_t));
    if (header == NULL) { perror("malloc error\n"); exit(1); }

    int err = get_header(tar_fd, path, header, false);
    if (err == -1) { perror("read error\n"); exit(1); }

    if (sym) {
        int ret = read_file(tar_fd, header->linkname, offset, dest, len);
        free(header);
        return ret;
    }

    if (offset > TAR_INT(header->size)) return -2;
    
    size_t toread = TAR_INT(header->size)-offset;
    if (*len < toread) toread = *len;
    lseek(tar_fd, offset, SEEK_CUR);
    err = read(tar_fd, dest, toread);
    if (err == -1) { perror("read error\n"); exit(1); }

    *len = err;
    int toret = TAR_INT(header->size)-offset-err;

    free(header);
    lseek(tar_fd, 0, SEEK_SET);

    return toret;
}