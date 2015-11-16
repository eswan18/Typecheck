#ifndef SCOPE_H
#define SCOPE_H

void scope_enter();
void scope_exit()
void scope_bind(const char *name, struct symbol *s);
struct symbol *scope_lookup(const char *name);

#endif
