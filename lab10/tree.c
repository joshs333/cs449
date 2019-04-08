#include "tree.h"
#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

void freetree(struct tree *root)
{
        if(root->left != NULL)
                freetree(root->left);
        if(root->right != NULL)
                freetree(root->right);
        FREE(root);
}

void randominsert(struct tree *head, struct tree *new)
{
        int way;
       	struct tree *curr, *prev;
       	prev = NULL;
        curr = head;

        while(curr != NULL)
        {
               	prev = curr;
               	way = rand()%2;
               	if(way == 0)
               	{
                       	curr = curr->left;
               	}
                else
               	{
                       	curr = curr->right;
                }
       	}
       	if(way == 0)
                prev->left = new;
       	else
               	prev->right = new;
}

void printtree(struct tree *head)
{
        struct tree *curr = head;
        if(head == NULL)
                return;

        printtree(curr->left);
        printf("%d\n", curr->data);
        printtree(curr->right);
}
