
/*
COPYRIGHT JAD 2026
Licensed under the GNU General Public License v3.0

branding file, don't wanna modify 300000+ files because of a name changing or smt.

*/

#include "../drivers/VGAf/vgaf.h"

int build = 280;

// build counter
// + one for each build, or maybe each day?

int version = 1;

// ascci art logo

void logok() {
    printv("    __________                             .__        \n");    
    printv("\______   \ _____    ________ ________ |  |     ____  \n");
    printv(" |       _/ \__  \   \___   / \___   / |  |   _/ __ \ \n");
    printv(" |    |   \  / __ \_  /    /   /    /  |  |__ \  ___/ \n");
    printv(" |____|_  / (____  / /_____ \ /_____ \ |____/  \___  >\n");
    printv("        \/       \/        \/       \/             \/ \n");
}