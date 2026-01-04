/* run shell command */
void sh(char *cmd) {
    if (fork() == 0) {
        execvp(cmd, (char *[]){cmd, NULL});
    }
}

/* run shell command in the background */
void spawnDaemon(char *command) {
    popen(command, "r");
}
