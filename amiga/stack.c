/* A Shell gives 4 KB of stack; libnix's startup sets this much up instead.
 * The AmiCradle Team. GPL 3 or later, like Flare. */
unsigned long __stack = 512 * 1024;
static const char version[] __attribute__((used)) = "$VER: Flare 1.15 Amiga (05.11.2026)";
