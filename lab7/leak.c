#include <stdio.h>
#include <stdlib.h>

struct Node {
   struct Node *l;
   struct Node *r;
};

struct Node* mk()
{
   struct Node *x = malloc(sizeof(struct Node));
   x->l = NULL;
   x->r = NULL;
   return x;
}

struct Node* t;

void createTree()
{
   t       = mk();   // A (still reachable)
   t->l    = mk();   // B (still reachable)
   t->r    = mk();   // C (set to null)
   t->l->l = mk();   // D (set to null)
   t->l->r = mk();   // E (still reachable)
   t->r->l = mk();   // F (indirectly lost)
   t->r->r = mk();   // G (indirectly lost)

   /* To fix, comment these out */
   //t->l->l = NULL;
   //t->r = NULL;
}

void deleteTree()
{
   /* To fix, fill in with free() calls. Order of freeing is important.
    * Be careful not to access memory that has already been freed. */
   free(t->r->r);
   free(t->r->l);
   free(t->l->r);
   free(t->l->l);
   free(t->r);
   free(t->l);
   free(t);
}

int main()
{
   createTree();
   deleteTree();
   return 0;
}

