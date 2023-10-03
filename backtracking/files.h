#pragma once

char **file_readlines(char *path);
char *file_to_str(char *path);
void file_write(char *path, char *data);
void file_writelines(char *path, char **data);
